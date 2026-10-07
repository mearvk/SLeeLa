#ifndef SLEELA_OS_H
#define SLEELA_OS_H
/* ==========================================================================
 * sleela_os.h -- Operating-system system-call bridge for the SLeeLa VM.
 *
 * This is the OS-abstraction backend behind the `os*` built-ins the VM
 * exposes (OP_OS_* in sleela_core.h). Every function has three real
 * implementations selected at compile time -- Win32 on Windows, POSIX on
 * Linux, and POSIX + Apple specifics on macOS -- so a single Sleela program
 * that calls osRun()/osGetEnv()/osExists()/... makes the genuine host System
 * API call on whichever OS it is compiled for.
 *
 * Bounded-handle discipline (identical to sockets/files/threads): a spawned
 * child process is tracked as a VM-local integer handle in a bounded table;
 * Sleela source never sees a raw PID, HANDLE, or pointer. The backend owns the
 * platform resource and reaps it on wait/close.
 * ========================================================================== */
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

/* ---- Platform identity (thin wrappers over sleela_platform) -------------- */
/* Writes the host OS name ("Linux"/"Windows"/"macOS") into buf; returns length
 * written (excluding NUL) or -1. */
int slos_platform_name(char *buf, size_t cap);
/* 1 if the host supports a capability class (process/ipc/paths/...), else 0.
 * `cap` matches SLPlatformCapability ordinals. */
int slos_capability(int cap);

/* ---- Environment --------------------------------------------------------- */
/* Reads environment variable `name` into buf (empty string if unset); returns
 * the length written (>=0) or -1 on bad arguments. */
int slos_getenv(const char *name, char *buf, size_t cap);
/* Sets (overwrites) environment variable `name`=`value`; 0 on success, -1 on
 * failure. */
int slos_setenv(const char *name, const char *value);

/* ---- Working directory, identity ----------------------------------------- */
int slos_cwd(char *buf, size_t cap);        /* current working dir; len or -1 */
int slos_chdir(const char *path);           /* 0 ok, -1 fail                  */
int slos_hostname(char *buf, size_t cap);   /* host name; len or -1           */
int slos_username(char *buf, size_t cap);   /* login/user name; len or -1     */
int slos_tempdir(char *buf, size_t cap);    /* system temp dir; len or -1     */
int64_t slos_process_id(void);              /* this process' id               */

/* ---- Filesystem metadata / operations ------------------------------------ */
int slos_exists(const char *path);          /* 1 exists, 0 no, -1 bad arg     */
int slos_is_dir(const char *path);          /* 1 dir, 0 not, -1 bad arg       */
int64_t slos_file_size(const char *path);   /* size in bytes, or -1           */
int slos_mkdir(const char *path);           /* 0 ok (or exists), -1 fail      */
int slos_remove(const char *path);          /* unlink file or empty dir; 0/-1 */
int slos_rename(const char *from, const char *to); /* 0 ok, -1 fail           */

/* ---- Process execution --------------------------------------------------- */
/* Run `command` through the host shell synchronously and return its exit code
 * (or -1 if it could not be started). Blocks until the child exits. */
int slos_run(const char *command);

/* Asynchronous process control over an opaque backend handle. slos_spawn starts
 * `command` in the background and returns a backend handle (>=0) or -1; the VM
 * wraps it in a bounded VM-local handle. slos_wait blocks for the child and
 * returns its exit code (release the handle after). slos_kill terminates it.
 * slos_release frees the backend resource without waiting. */
typedef intptr_t SLOSProcess;
#define SLOS_INVALID_PROCESS ((SLOSProcess)-1)
SLOSProcess slos_spawn(const char *command);
int slos_wait(SLOSProcess process);         /* exit code, or -1               */
int slos_kill(SLOSProcess process);         /* 0 ok, -1 fail                  */
void slos_release(SLOSProcess process);     /* free without waiting           */

#ifdef __cplusplus
}
#endif
#endif /* SLEELA_OS_H */
