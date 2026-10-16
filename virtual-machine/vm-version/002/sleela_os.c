/* ==========================================================================
 * sleela_os.c -- Operating-system system-call bridge (Windows/Linux/macOS).
 *
 * Three real backends selected at compile time. The POSIX branch serves both
 * Linux and macOS (with a small Apple-specific include for _NSGetExecutablePath
 * style needs handled elsewhere); the Windows branch uses the Win32 API. Each
 * entry point makes the genuine host System API call.
 * ========================================================================== */
/* Request the POSIX.1-2008 surface (setenv/gethostname/getcwd/posix_spawn/...)
 * under -std=c11, which otherwise hides them. Mirrors sleela_core.c. */
#if !defined(_WIN32)
#  ifndef _POSIX_C_SOURCE
#    define _POSIX_C_SOURCE 200809L
#  endif
#  if defined(__APPLE__) && !defined(_DARWIN_C_SOURCE)
#    define _DARWIN_C_SOURCE
#  endif
#endif
#include "sleela_os.h"
#include "sleela_platform.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* A small helper to copy a C string into a bounded output buffer, returning the
 * number of bytes written (excluding the NUL) or -1 on bad arguments. */
static int slos_copy_out(const char *src, char *buf, size_t cap) {
    if (!buf || cap == 0) return -1;
    if (!src) src = "";
    size_t n = strlen(src);
    if (n >= cap) n = cap - 1;
    memcpy(buf, src, n);
    buf[n] = 0;
    return (int)n;
}

int slos_platform_name(char *buf, size_t cap) {
    return slos_copy_out(slplatform_name(slplatform_current()), buf, cap);
}
int slos_capability(int cap) {
    if (cap < 0 || cap >= SL_CAP_MAX) return 0;
    return slplatform_capability_available((SLPlatformCapability)cap);
}

/* ========================================================================== */
#if defined(_WIN32)
/* -------------------------------- Windows --------------------------------- */
#include <windows.h>
#include <direct.h>
#include <lmcons.h>

int slos_getenv(const char *name, char *buf, size_t cap) {
    if (!name || !buf || cap == 0) return -1;
    DWORD n = GetEnvironmentVariableA(name, buf, (DWORD)cap);
    if (n == 0) { buf[0] = 0; return 0; }        /* unset -> empty */
    if (n >= cap) { buf[cap - 1] = 0; return (int)(cap - 1); }
    return (int)n;
}
int slos_setenv(const char *name, const char *value) {
    if (!name) return -1;
    return SetEnvironmentVariableA(name, value ? value : "") ? 0 : -1;
}
int slos_cwd(char *buf, size_t cap) {
    if (!buf || cap == 0) return -1;
    DWORD n = GetCurrentDirectoryA((DWORD)cap, buf);
    if (n == 0 || n >= cap) return -1;
    return (int)n;
}
int slos_chdir(const char *path) {
    if (!path) return -1;
    return SetCurrentDirectoryA(path) ? 0 : -1;
}
int slos_hostname(char *buf, size_t cap) {
    if (!buf || cap == 0) return -1;
    DWORD n = (DWORD)cap;
    if (!GetComputerNameA(buf, &n)) return -1;
    return (int)n;
}
int slos_username(char *buf, size_t cap) {
    if (!buf || cap == 0) return -1;
    DWORD n = (DWORD)cap;
    if (!GetUserNameA(buf, &n)) return -1;
    return (n > 0) ? (int)(n - 1) : 0;           /* GetUserName counts the NUL */
}
int slos_tempdir(char *buf, size_t cap) {
    if (!buf || cap == 0) return -1;
    DWORD n = GetTempPathA((DWORD)cap, buf);
    if (n == 0 || n >= cap) return -1;
    return (int)n;
}
int64_t slos_process_id(void) { return (int64_t)GetCurrentProcessId(); }

int slos_exists(const char *path) {
    if (!path) return -1;
    return (GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES) ? 1 : 0;
}
int slos_is_dir(const char *path) {
    if (!path) return -1;
    DWORD a = GetFileAttributesA(path);
    if (a == INVALID_FILE_ATTRIBUTES) return 0;
    return (a & FILE_ATTRIBUTE_DIRECTORY) ? 1 : 0;
}
int64_t slos_file_size(const char *path) {
    if (!path) return -1;
    WIN32_FILE_ATTRIBUTE_DATA d;
    if (!GetFileAttributesExA(path, GetFileExInfoStandard, &d)) return -1;
    return ((int64_t)d.nFileSizeHigh << 32) | (int64_t)d.nFileSizeLow;
}
int slos_mkdir(const char *path) {
    if (!path) return -1;
    if (CreateDirectoryA(path, NULL)) return 0;
    return (GetLastError() == ERROR_ALREADY_EXISTS) ? 0 : -1;
}
int slos_remove(const char *path) {
    if (!path) return -1;
    if (slos_is_dir(path) == 1) return RemoveDirectoryA(path) ? 0 : -1;
    return DeleteFileA(path) ? 0 : -1;
}
int slos_rename(const char *from, const char *to) {
    if (!from || !to) return -1;
    return MoveFileExA(from, to, MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED) ? 0 : -1;
}

int slos_run(const char *command) {
    if (!command) return -1;
    int rc = system(command);                    /* routes through cmd.exe */
    return (rc == -1) ? -1 : rc;
}

static SLOSProcess win_spawn(const char *command) {
    char cmd[32768];
    STARTUPINFOA si; PROCESS_INFORMATION pi;
    if (!command) return SLOS_INVALID_PROCESS;
    /* Launch via the shell so the command string is interpreted like osRun
     * (slos_run uses system(), itself cmd.exe). This mirrors the POSIX branch,
     * which runs "/bin/sh -c command" in both slos_run and slos_spawn, so a
     * command authored for osRun behaves the same when handed to osSpawn. */
    (void)snprintf(cmd, sizeof(cmd), "cmd.exe /c %s", command);
    memset(&si, 0, sizeof(si)); si.cb = sizeof(si);
    memset(&pi, 0, sizeof(pi));
    if (!CreateProcessA(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
        return SLOS_INVALID_PROCESS;
    CloseHandle(pi.hThread);
    return (SLOSProcess)(intptr_t)pi.hProcess;   /* keep the process handle */
}
SLOSProcess slos_spawn(const char *command) { return win_spawn(command); }
int slos_wait(SLOSProcess process) {
    HANDLE h = (HANDLE)(intptr_t)process;
    DWORD code = 0;
    if (!h || process == SLOS_INVALID_PROCESS) return -1;
    if (WaitForSingleObject(h, INFINITE) != WAIT_OBJECT_0) { CloseHandle(h); return -1; }
    if (!GetExitCodeProcess(h, &code)) code = (DWORD)-1;
    CloseHandle(h);
    return (int)code;
}
int slos_kill(SLOSProcess process) {
    HANDLE h = (HANDLE)(intptr_t)process;
    if (!h || process == SLOS_INVALID_PROCESS) return -1;
    return TerminateProcess(h, 1) ? 0 : -1;
}
void slos_release(SLOSProcess process) {
    HANDLE h = (HANDLE)(intptr_t)process;
    if (h && process != SLOS_INVALID_PROCESS) CloseHandle(h);
}

#else
/* ------------------------------ POSIX (Linux + macOS) --------------------- */
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <spawn.h>
#include <signal.h>
#include <limits.h>
#include <pwd.h>

extern char **environ;

int slos_getenv(const char *name, char *buf, size_t cap) {
    if (!name || !buf || cap == 0) return -1;
    const char *v = getenv(name);
    if (!v) { buf[0] = 0; return 0; }
    return slos_copy_out(v, buf, cap);
}
int slos_setenv(const char *name, const char *value) {
    if (!name) return -1;
    return setenv(name, value ? value : "", 1) == 0 ? 0 : -1;
}
int slos_cwd(char *buf, size_t cap) {
    if (!buf || cap == 0) return -1;
    return getcwd(buf, cap) ? (int)strlen(buf) : -1;
}
int slos_chdir(const char *path) {
    if (!path) return -1;
    return chdir(path) == 0 ? 0 : -1;
}
int slos_hostname(char *buf, size_t cap) {
    if (!buf || cap == 0) return -1;
    if (gethostname(buf, cap) != 0) return -1;
    buf[cap - 1] = 0;
    return (int)strlen(buf);
}
int slos_username(char *buf, size_t cap) {
    if (!buf || cap == 0) return -1;
    const char *u = getenv("USER");
    if (!u || !*u) u = getenv("LOGNAME");
    if (!u || !*u) {
        struct passwd *pw = getpwuid(getuid());
        u = (pw && pw->pw_name) ? pw->pw_name : "";
    }
    return slos_copy_out(u, buf, cap);
}
int slos_tempdir(char *buf, size_t cap) {
    const char *t = getenv("TMPDIR");
    if (!t || !*t) t = "/tmp";
    return slos_copy_out(t, buf, cap);
}
int64_t slos_process_id(void) { return (int64_t)getpid(); }

int slos_exists(const char *path) {
    if (!path) return -1;
    struct stat st;
    return (stat(path, &st) == 0) ? 1 : 0;
}
int slos_is_dir(const char *path) {
    if (!path) return -1;
    struct stat st;
    if (stat(path, &st) != 0) return 0;
    return S_ISDIR(st.st_mode) ? 1 : 0;
}
int64_t slos_file_size(const char *path) {
    if (!path) return -1;
    struct stat st;
    if (stat(path, &st) != 0) return -1;
    return (int64_t)st.st_size;
}
int slos_mkdir(const char *path) {
    if (!path) return -1;
    if (mkdir(path, 0777) == 0) return 0;
    struct stat st;
    if (stat(path, &st) == 0 && S_ISDIR(st.st_mode)) return 0;  /* already exists */
    return -1;
}
int slos_remove(const char *path) {
    if (!path) return -1;
    if (slos_is_dir(path) == 1) return rmdir(path) == 0 ? 0 : -1;
    return unlink(path) == 0 ? 0 : -1;
}
int slos_rename(const char *from, const char *to) {
    if (!from || !to) return -1;
    return rename(from, to) == 0 ? 0 : -1;
}

int slos_run(const char *command) {
    if (!command) return -1;
    int rc = system(command);                    /* /bin/sh -c command */
    if (rc == -1) return -1;
    if (WIFEXITED(rc)) return WEXITSTATUS(rc);
    return -1;
}

SLOSProcess slos_spawn(const char *command) {
    if (!command) return SLOS_INVALID_PROCESS;
    pid_t pid;
    char *argv[4];
    argv[0] = (char *)"/bin/sh";
    argv[1] = (char *)"-c";
    argv[2] = (char *)command;
    argv[3] = NULL;
    if (posix_spawn(&pid, "/bin/sh", NULL, NULL, argv, environ) != 0)
        return SLOS_INVALID_PROCESS;
    return (SLOSProcess)pid;
}
int slos_wait(SLOSProcess process) {
    if (process == SLOS_INVALID_PROCESS) return -1;
    int status = 0;
    pid_t pid = (pid_t)process;
    if (waitpid(pid, &status, 0) != pid) return -1;
    if (WIFEXITED(status)) return WEXITSTATUS(status);
    return -1;
}
int slos_kill(SLOSProcess process) {
    if (process == SLOS_INVALID_PROCESS) return -1;
    return kill((pid_t)process, SIGTERM) == 0 ? 0 : -1;
}
void slos_release(SLOSProcess process) {
    /* Reap without blocking so a released child does not linger as a zombie. */
    if (process == SLOS_INVALID_PROCESS) return;
    int status = 0;
    (void)waitpid((pid_t)process, &status, WNOHANG);
}

#endif
