/*
 * ballum/include/sleela_ballum/ballum_sandbox.h
 * Process protection for the Ballum program host.
 *
 * Two directions of protection:
 *
 *   hosted program -> system : the program Ballum launches is confined with
 *     least privilege (NIST SP 800-53 Rev. 5 AC-6, SC-39, CM-7):
 *       - no_new_privs: execve can never grant privilege (setuid, file caps)
 *       - a seccomp-BPF DENYLIST of kernel interfaces a terminal or admin UI
 *         never needs: ptrace, process_vm_*, mount family, module loading,
 *         kexec, reboot, bpf, perf_event_open, userfaultfd, keyrings, clock
 *         and hostname setters, io_uring, namespace entry. Denied calls return
 *         EPERM; a call made through a foreign syscall ABI kills the process.
 *       - resource limits: no core dumps, bounded open files / CPU / memory
 *       - a clean environment (allowlist, never inherited wholesale)
 *       - no inherited descriptors; stdin from /dev/null
 *       - its own session and process group, so the whole tree can be ended
 *       - PR_SET_PDEATHSIG: the program dies if Ballum dies
 *
 *   other processes -> Ballum : ballum_sandbox_harden_self() marks the Ballum
 *     process non-dumpable (blocks ptrace and /proc/<pid>/mem reads by other
 *     processes of the same user) and disables core files, so the input it
 *     monitors cannot be scraped from its memory.
 *
 * Everything FAILS CLOSED: a protection that was requested but cannot be
 * established makes the call fail; the program is then NOT started. A policy
 * never silently degrades.
 *
 * This is a denylist hardening layer, not a complete container. It limits what
 * a misbehaving program can do to the host; it does not replace namespaces or a
 * dedicated unprivileged account for hostile code. Aligned with, not certified
 * against, the controls named above.
 *
 * Linux implements all of it. Other POSIX systems provide the fork/exec,
 * environment, descriptor, limit and session parts; requesting seccomp there
 * (or any call on Windows) returns BALLUM_STATUS_UNSUPPORTED.
 */
#ifndef SLEELA_BALLUM_SANDBOX_H
#define SLEELA_BALLUM_SANDBOX_H

#include "ballum_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int      no_new_privs;        /* default 1 */
    int      seccomp_denylist;    /* default 1 (Linux x86_64/aarch64) */
    int      disable_core_dumps;  /* default 1 */
    int      close_inherited_fds; /* default 1: close every fd above stderr */
    int      new_session;         /* default 1: setsid() */
    int      die_with_parent;     /* default 1: SIGKILL when Ballum exits (Linux) */
    uint64_t max_open_files;      /* default 4096; 0 = leave unchanged */
    uint64_t max_cpu_seconds;     /* default 0 = unlimited */
    uint64_t max_address_space;   /* bytes; default 0 = unlimited */
} ballum_sandbox_policy_t;

/* Fill `p` with the secure defaults above. */
void ballum_sandbox_policy_default(ballum_sandbox_policy_t *p);

/* Apply the policy to the CALLING process. Irreversible. Intended for use in a
 * freshly forked child before exec; ballum_sandbox_spawn does exactly that. */
ballum_status_t ballum_sandbox_apply(const ballum_sandbox_policy_t *p);

/* Protect the Ballum process itself (non-dumpable, no core files). */
ballum_status_t ballum_sandbox_harden_self(void);

/* Resolve `name` to an absolute path of an executable regular file. A name with
 * a '/' is checked as given and must be absolute. Otherwise the fixed trusted
 * search path ("/usr/local/bin:/usr/bin:/bin:/usr/local/sbin:/usr/sbin:/sbin")
 * is used, never $PATH. The file and every directory entry are rejected if
 * writable by group or others (CWE-426 untrusted search path). Returns
 * BALLUM_STATUS_OK and writes `out` (NUL-terminated), or a failure status. */
ballum_status_t ballum_sandbox_resolve(const char *name, char *out, size_t cap);

/* Build a heap-allocated, NULL-terminated environment holding only the
 * variables in `allow` (copied from the current environment when set) plus the
 * "NAME=value" strings in `extra`. NAME must match [A-Za-z_][A-Za-z0-9_]*.
 * Returns NULL on invalid input or allocation failure. Free with
 * ballum_sandbox_env_free. Either list may be NULL (NULL-terminated otherwise). */
char **ballum_sandbox_env_new(const char *const *allow, const char *const *extra);
void   ballum_sandbox_env_free(char **env);

/* Fork, apply the policy in the child, then execve(path, argv, envp) with `path`
 * absolute. The child's stdout/stderr go to `log_fd` (or /dev/null when < 0).
 * On success *pid_out is the child (also its session and group leader). When
 * the sandbox cannot be established or exec fails, no program runs and a
 * failure status is returned:
 *   INVALID_ARGUMENT     bad path/argv
 *   PERMISSION_DENIED    the policy could not be applied (fail closed)
 *   BACKEND_UNAVAILABLE  exec failed (missing/unreadable program)
 *   BACKEND_ERROR        fork/pipe failure */
ballum_status_t ballum_sandbox_spawn(const ballum_sandbox_policy_t *policy,
                                     const char *path, char *const argv[],
                                     char *const envp[], int log_fd, int *pid_out);

/* Reap `pid` if it has ended. Returns 1 and sets *exit_code (128 + signal when
 * killed by a signal) if it ended, 0 if it is still running, -1 on error.
 * With block != 0 it waits for the child. */
int ballum_sandbox_wait(int pid, int block, int *exit_code);

/* End the program's whole process group: SIGTERM, wait up to grace_ms, then
 * SIGKILL, then reap. Safe to call on an already-ended child. */
void ballum_sandbox_terminate(int pid, unsigned grace_ms);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_BALLUM_SANDBOX_H */
