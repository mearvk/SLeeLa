#include "sleela_terminal.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#include <processthreadsapi.h>
#include <consoleapi2.h>

typedef struct {
    HPCON console;
    HANDLE input_write;
    HANDLE output_read;
    HANDLE process;
} SLWinTerminal;

static SLWinTerminal* as_terminal(SLTerminalHandle h) {
    return (SLWinTerminal*)(intptr_t)h;
}

SLTerminalPlatform slterminal_platform(void) { return SL_TERMINAL_WINDOWS; }
const char* slterminal_platform_name(void) { return "windows-conpty"; }
int slterminal_platform_is_available(SLTerminalPlatform platform) {
    return platform == SL_TERMINAL_AUTO || platform == SL_TERMINAL_WINDOWS;
}

static wchar_t* utf8_to_wide(const char* text) {
    int n;
    wchar_t* out;
    if (!text) return NULL;
    n = MultiByteToWideChar(CP_UTF8, 0, text, -1, NULL, 0);
    if (n <= 0) return NULL;
    out = (wchar_t*)calloc((size_t)n, sizeof(*out));
    if (!out) return NULL;
    if (!MultiByteToWideChar(CP_UTF8, 0, text, -1, out, n)) {
        free(out);
        return NULL;
    }
    return out;
}

int slterminal_spawn(SLTerminalHandle* terminal, const char* command,
                     unsigned cols, unsigned rows) {
    HANDLE child_in = NULL, parent_in = NULL;
    HANDLE parent_out = NULL, child_out = NULL;
    SECURITY_ATTRIBUTES sa;
    COORD size;
    HPCON hpc = NULL;
    STARTUPINFOEXW si;
    PROCESS_INFORMATION pi;
    SIZE_T attr_size = 0;
    wchar_t* cmd = NULL;
    SLWinTerminal* t = NULL;
    HRESULT hr;
    DWORD error_code;

    if (!terminal || cols == 0 || rows == 0) return EINVAL;
    *terminal = (SLTerminalHandle)0;
    memset(&sa, 0, sizeof(sa));
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;

    if (!CreatePipe(&child_in, &parent_in, &sa, 0) ||
        !CreatePipe(&parent_out, &child_out, &sa, 0)) {
        error_code = GetLastError();
        if (child_in) CloseHandle(child_in);
        if (parent_in) CloseHandle(parent_in);
        if (parent_out) CloseHandle(parent_out);
        if (child_out) CloseHandle(child_out);
        return (int)error_code;
    }
    if (!SetHandleInformation(parent_in, HANDLE_FLAG_INHERIT, 0) ||
        !SetHandleInformation(parent_out, HANDLE_FLAG_INHERIT, 0)) {
        error_code = GetLastError();
        CloseHandle(child_in); CloseHandle(parent_in);
        CloseHandle(parent_out); CloseHandle(child_out);
        return (int)error_code;
    }

    size.X = (SHORT)(cols > 32767 ? 32767 : cols);
    size.Y = (SHORT)(rows > 32767 ? 32767 : rows);
    hr = CreatePseudoConsole(size, child_in, child_out, 0, &hpc);
    CloseHandle(child_in);
    CloseHandle(child_out);
    if (FAILED(hr)) {
        CloseHandle(parent_in); CloseHandle(parent_out);
        return (int)hr;
    }

    memset(&si, 0, sizeof(si));
    si.StartupInfo.cb = sizeof(si);
    InitializeProcThreadAttributeList(NULL, 1, 0, &attr_size);
    si.lpAttributeList = (LPPROC_THREAD_ATTRIBUTE_LIST)HeapAlloc(
        GetProcessHeap(), 0, attr_size);
    if (!si.lpAttributeList ||
        !InitializeProcThreadAttributeList(si.lpAttributeList, 1, 0, &attr_size) ||
        !UpdateProcThreadAttribute(si.lpAttributeList, 0,
            PROC_THREAD_ATTRIBUTE_PSEUDOCONSOLE, hpc, sizeof(hpc), NULL, NULL)) {
        error_code = GetLastError();
        if (si.lpAttributeList) HeapFree(GetProcessHeap(), 0, si.lpAttributeList);
        CloseHandle(parent_in); CloseHandle(parent_out); ClosePseudoConsole(hpc);
        return (int)error_code;
    }

    cmd = utf8_to_wide(command ? command : "cmd.exe");
    if (!cmd) {
        DeleteProcThreadAttributeList(si.lpAttributeList);
        HeapFree(GetProcessHeap(), 0, si.lpAttributeList);
        CloseHandle(parent_in); CloseHandle(parent_out); ClosePseudoConsole(hpc);
        return ERROR_OUTOFMEMORY;
    }
    memset(&pi, 0, sizeof(pi));
    if (!CreateProcessW(NULL, cmd, NULL, NULL, FALSE,
                        EXTENDED_STARTUPINFO_PRESENT | CREATE_UNICODE_ENVIRONMENT,
                        NULL, NULL, &si.StartupInfo, &pi)) {
        error_code = GetLastError();
        free(cmd);
        DeleteProcThreadAttributeList(si.lpAttributeList);
        HeapFree(GetProcessHeap(), 0, si.lpAttributeList);
        CloseHandle(parent_in); CloseHandle(parent_out); ClosePseudoConsole(hpc);
        return (int)error_code;
    }
    CloseHandle(pi.hThread);
    free(cmd);
    DeleteProcThreadAttributeList(si.lpAttributeList);
    HeapFree(GetProcessHeap(), 0, si.lpAttributeList);

    t = (SLWinTerminal*)calloc(1, sizeof(*t));
    if (!t) {
        TerminateProcess(pi.hProcess, 1);
        CloseHandle(pi.hProcess);
        CloseHandle(parent_in); CloseHandle(parent_out); ClosePseudoConsole(hpc);
        return ERROR_OUTOFMEMORY;
    }
    t->console = hpc;
    t->input_write = parent_in;
    t->output_read = parent_out;
    t->process = pi.hProcess;
    *terminal = (SLTerminalHandle)(intptr_t)t;
    return 0;
}

SLTerminalCount slterminal_read(SLTerminalHandle terminal, void* buffer, size_t size) {
    SLWinTerminal* t = as_terminal(terminal);
    DWORD got = 0;
    if (!t || !buffer || size == 0 || size > 0xffffffffu) return -1;
    if (!ReadFile(t->output_read, buffer, (DWORD)size, &got, NULL)) return -1;
    return (SLTerminalCount)got;
}

SLTerminalCount slterminal_write(SLTerminalHandle terminal, const void* buffer, size_t size) {
    SLWinTerminal* t = as_terminal(terminal);
    DWORD sent = 0;
    if (!t || !buffer || size == 0 || size > 0xffffffffu) return -1;
    if (!WriteFile(t->input_write, buffer, (DWORD)size, &sent, NULL)) return -1;
    return (SLTerminalCount)sent;
}

int slterminal_resize(SLTerminalHandle terminal, unsigned cols, unsigned rows) {
    SLWinTerminal* t = as_terminal(terminal);
    COORD size;
    HRESULT hr;
    if (!t || cols == 0 || rows == 0) return EINVAL;
    size.X = (SHORT)(cols > 32767 ? 32767 : cols);
    size.Y = (SHORT)(rows > 32767 ? 32767 : rows);
    hr = ResizePseudoConsole(t->console, size);
    return SUCCEEDED(hr) ? 0 : (int)hr;
}

int slterminal_close(SLTerminalHandle terminal) {
    SLWinTerminal* t = as_terminal(terminal);
    if (!t) return EINVAL;
    CloseHandle(t->input_write);
    CloseHandle(t->output_read);
    WaitForSingleObject(t->process, INFINITE);
    CloseHandle(t->process);
    ClosePseudoConsole(t->console);
    free(t);
    return 0;
}

int slterminal_valid(SLTerminalHandle terminal) { return as_terminal(terminal) != NULL; }
int slterminal_last_error(void) { return (int)GetLastError(); }

#else

#include <fcntl.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#ifndef TIOCSWINSZ
#error "A POSIX PTY implementation requires TIOCSWINSZ"
#endif

typedef struct {
    int master;
    pid_t child;
} SLLinuxTerminal;

static SLLinuxTerminal* as_terminal(SLTerminalHandle h) {
    return (SLLinuxTerminal*)(intptr_t)h;
}

SLTerminalPlatform slterminal_platform(void) { return SL_TERMINAL_LINUX; }
const char* slterminal_platform_name(void) { return "linux-pty"; }
int slterminal_platform_is_available(SLTerminalPlatform platform) {
    return platform == SL_TERMINAL_AUTO || platform == SL_TERMINAL_LINUX;
}

int slterminal_spawn(SLTerminalHandle* terminal, const char* command,
                     unsigned cols, unsigned rows) {
    int master;
    pid_t child;
    struct winsize ws;
    SLLinuxTerminal* t;
    if (!terminal || cols == 0 || rows == 0) return EINVAL;
    *terminal = (SLTerminalHandle)0;
    master = posix_openpt(O_RDWR | O_NOCTTY);
    if (master < 0) return errno;
    if (grantpt(master) < 0 || unlockpt(master) < 0) {
        int e = errno; close(master); return e;
    }
    memset(&ws, 0, sizeof(ws));
    ws.ws_col = (unsigned short)(cols > 65535 ? 65535 : cols);
    ws.ws_row = (unsigned short)(rows > 65535 ? 65535 : rows);
    if (ioctl(master, TIOCSWINSZ, &ws) < 0) {
        int e = errno; close(master); return e;
    }
    child = fork();
    if (child < 0) { int e = errno; close(master); return e; }
    if (child == 0) {
        char* slave_name = ptsname(master);
        int slave;
        if (!slave_name) _exit(127);
        setsid();
        slave = open(slave_name, O_RDWR);
        if (slave < 0) _exit(127);
        ioctl(slave, TIOCSCTTY, 0);
        dup2(slave, STDIN_FILENO);
        dup2(slave, STDOUT_FILENO);
        dup2(slave, STDERR_FILENO);
        if (slave > STDERR_FILENO) close(slave);
        if (command) execl("/bin/sh", "sh", "-c", command, (char*)NULL);
        else execl("/bin/sh", "sh", (char*)NULL);
        _exit(127);
    }
    t = (SLLinuxTerminal*)calloc(1, sizeof(*t));
    if (!t) { kill(child, SIGTERM); close(master); waitpid(child, NULL, 0); return ENOMEM; }
    t->master = master;
    t->child = child;
    *terminal = (SLTerminalHandle)(intptr_t)t;
    return 0;
}

SLTerminalCount slterminal_read(SLTerminalHandle terminal, void* buffer, size_t size) {
    SLLinuxTerminal* t = as_terminal(terminal);
    ssize_t n;
    if (!t || !buffer || size == 0) return -1;
    n = read(t->master, buffer, size);
    return (SLTerminalCount)n;
}

SLTerminalCount slterminal_write(SLTerminalHandle terminal, const void* buffer, size_t size) {
    SLLinuxTerminal* t = as_terminal(terminal);
    ssize_t n;
    if (!t || !buffer || size == 0) return -1;
    n = write(t->master, buffer, size);
    return (SLTerminalCount)n;
}

int slterminal_resize(SLTerminalHandle terminal, unsigned cols, unsigned rows) {
    SLLinuxTerminal* t = as_terminal(terminal);
    struct winsize ws;
    if (!t || cols == 0 || rows == 0) return EINVAL;
    memset(&ws, 0, sizeof(ws));
    ws.ws_col = (unsigned short)(cols > 65535 ? 65535 : cols);
    ws.ws_row = (unsigned short)(rows > 65535 ? 65535 : rows);
    return ioctl(t->master, TIOCSWINSZ, &ws) == 0 ? 0 : errno;
}

int slterminal_close(SLTerminalHandle terminal) {
    SLLinuxTerminal* t = as_terminal(terminal);
    int status;
    if (!t) return EINVAL;
    close(t->master);
    if (waitpid(t->child, &status, 0) < 0) {
        int e = errno; free(t); return e;
    }
    free(t);
    return 0;
}

int slterminal_valid(SLTerminalHandle terminal) { return as_terminal(terminal) != NULL; }
int slterminal_last_error(void) { return errno; }

#endif
