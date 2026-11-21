/*
 * ballum/src/ballum_sandbox.c -- process protection (see ballum_sandbox.h).
 */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif
#include "sleela_ballum/ballum_sandbox.h"

#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ballum_sandbox_policy_default(ballum_sandbox_policy_t *p) {
    if (!p) return;
    memset(p, 0, sizeof(*p));
    p->no_new_privs = 1;
    p->seccomp_denylist = 1;
    p->disable_core_dumps = 1;
    p->close_inherited_fds = 1;
    p->new_session = 1;
    p->die_with_parent = 1;
    p->max_open_files = 4096;
}

#if defined(__unix__) || defined(__APPLE__)

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#if defined(__linux__)
#  include <linux/audit.h>
#  include <linux/filter.h>
#  include <linux/seccomp.h>
#  include <sys/prctl.h>
#  include <sys/syscall.h>
#endif

extern char **environ;

/* ---- seccomp denylist (Linux) -------------------------------------------- */

#if defined(__linux__) && (defined(__x86_64__) || defined(__aarch64__))
#  define BALLUM_HAVE_SECCOMP 1
#  if defined(__x86_64__)
#    define BALLUM_AUDIT_ARCH AUDIT_ARCH_X86_64
#  else
#    define BALLUM_AUDIT_ARCH AUDIT_ARCH_AARCH64
#  endif

#define DENY(nr) nr,
static const int k_denied[] = {
#ifdef __NR_ptrace
    __NR_ptrace,
#endif
#ifdef __NR_process_vm_readv
    __NR_process_vm_readv,
#endif
#ifdef __NR_process_vm_writev
    __NR_process_vm_writev,
#endif
#ifdef __NR_kcmp
    __NR_kcmp,
#endif
#ifdef __NR_mount
    __NR_mount,
#endif
#ifdef __NR_umount2
    __NR_umount2,
#endif
#ifdef __NR_pivot_root
    __NR_pivot_root,
#endif
#ifdef __NR_chroot
    __NR_chroot,
#endif
#ifdef __NR_move_mount
    __NR_move_mount,
#endif
#ifdef __NR_open_tree
    __NR_open_tree,
#endif
#ifdef __NR_fsopen
    __NR_fsopen,
#endif
#ifdef __NR_fsconfig
    __NR_fsconfig,
#endif
#ifdef __NR_fsmount
    __NR_fsmount,
#endif
#ifdef __NR_fspick
    __NR_fspick,
#endif
#ifdef __NR_mount_setattr
    __NR_mount_setattr,
#endif
#ifdef __NR_swapon
    __NR_swapon,
#endif
#ifdef __NR_swapoff
    __NR_swapoff,
#endif
#ifdef __NR_reboot
    __NR_reboot,
#endif
#ifdef __NR_kexec_load
    __NR_kexec_load,
#endif
#ifdef __NR_kexec_file_load
    __NR_kexec_file_load,
#endif
#ifdef __NR_init_module
    __NR_init_module,
#endif
#ifdef __NR_finit_module
    __NR_finit_module,
#endif
#ifdef __NR_delete_module
    __NR_delete_module,
#endif
#ifdef __NR_bpf
    __NR_bpf,
#endif
#ifdef __NR_perf_event_open
    __NR_perf_event_open,
#endif
#ifdef __NR_userfaultfd
    __NR_userfaultfd,
#endif
#ifdef __NR_open_by_handle_at
    __NR_open_by_handle_at,
#endif
#ifdef __NR_add_key
    __NR_add_key,
#endif
#ifdef __NR_request_key
    __NR_request_key,
#endif
#ifdef __NR_keyctl
    __NR_keyctl,
#endif
#ifdef __NR_acct
    __NR_acct,
#endif
#ifdef __NR_quotactl
    __NR_quotactl,
#endif
#ifdef __NR_settimeofday
    __NR_settimeofday,
#endif
#ifdef __NR_clock_settime
    __NR_clock_settime,
#endif
#ifdef __NR_clock_adjtime
    __NR_clock_adjtime,
#endif
#ifdef __NR_adjtimex
    __NR_adjtimex,
#endif
#ifdef __NR_sethostname
    __NR_sethostname,
#endif
#ifdef __NR_setdomainname
    __NR_setdomainname,
#endif
#ifdef __NR_iopl
    __NR_iopl,
#endif
#ifdef __NR_ioperm
    __NR_ioperm,
#endif
#ifdef __NR_syslog
    __NR_syslog,
#endif
#ifdef __NR_setns
    __NR_setns,
#endif
#ifdef __NR_io_uring_setup
    __NR_io_uring_setup,
#endif
#ifdef __NR_io_uring_enter
    __NR_io_uring_enter,
#endif
#ifdef __NR_io_uring_register
    __NR_io_uring_register,
#endif
};
#define K_DENIED_N (sizeof(k_denied) / sizeof(k_denied[0]))

/* Layout: [0] load arch  [1] arch ok? skip kill  [2] kill
 *         [3] load nr    (x86_64: [4] x32 bit set? kill)
 *         one JEQ+RET pair per denied syscall, then RET ALLOW, RET KILL. */
static int install_seccomp(void) {
    enum { MAXF = 4 + 2 * 96 + 3 };
    struct sock_filter f[MAXF];
    unsigned n = 0;
    if (K_DENIED_N > 96) return -1;

    f[n++] = (struct sock_filter)BPF_STMT(BPF_LD | BPF_W | BPF_ABS,
                 (unsigned)offsetof(struct seccomp_data, arch));
    f[n++] = (struct sock_filter)BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, BALLUM_AUDIT_ARCH, 1, 0);
    f[n++] = (struct sock_filter)BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_KILL_PROCESS);
    f[n++] = (struct sock_filter)BPF_STMT(BPF_LD | BPF_W | BPF_ABS,
                 (unsigned)offsetof(struct seccomp_data, nr));
#if defined(__x86_64__)
    /* x32 ABI calls carry bit 30; they would bypass the numbers below. */
    f[n++] = (struct sock_filter)BPF_JUMP(BPF_JMP | BPF_JSET | BPF_K, 0x40000000u, 0, 1);
    f[n++] = (struct sock_filter)BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_KILL_PROCESS);
#endif
    for (size_t i = 0; i < K_DENIED_N; ++i) {
        f[n++] = (struct sock_filter)BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, (unsigned)k_denied[i], 0, 1);
        f[n++] = (struct sock_filter)BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_ERRNO | (EPERM & SECCOMP_RET_DATA));
    }
    f[n++] = (struct sock_filter)BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_ALLOW);

    struct sock_fprog prog = { (unsigned short)n, f };
    return prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &prog, 0, 0);
}
#endif /* seccomp arch */

static int set_limit(int resource, uint64_t value) {
    struct rlimit rl;
    if (getrlimit(resource, &rl) != 0) return -1;
    rlim_t want = (rlim_t)value;
    if (rl.rlim_max != RLIM_INFINITY && want > rl.rlim_max) want = rl.rlim_max;
    /* lower the hard limit too: the program must not be able to raise it again */
    rl.rlim_cur = want;
    rl.rlim_max = want;
    return setrlimit(resource, &rl);
}

ballum_status_t ballum_sandbox_apply(const ballum_sandbox_policy_t *p) {
    if (!p) return BALLUM_STATUS_INVALID_ARGUMENT;
#if !defined(BALLUM_HAVE_SECCOMP)
    if (p->seccomp_denylist) return BALLUM_STATUS_UNSUPPORTED;      /* fail closed */
#endif
    if (p->new_session && setsid() < 0) return BALLUM_STATUS_PERMISSION_DENIED;
    if (p->disable_core_dumps && set_limit(RLIMIT_CORE, 0) != 0)
        return BALLUM_STATUS_PERMISSION_DENIED;
    if (p->max_open_files && set_limit(RLIMIT_NOFILE, p->max_open_files) != 0)
        return BALLUM_STATUS_PERMISSION_DENIED;
    if (p->max_cpu_seconds && set_limit(RLIMIT_CPU, p->max_cpu_seconds) != 0)
        return BALLUM_STATUS_PERMISSION_DENIED;
    if (p->max_address_space && set_limit(RLIMIT_AS, p->max_address_space) != 0)
        return BALLUM_STATUS_PERMISSION_DENIED;
#if defined(__linux__)
    if (p->die_with_parent) {
        pid_t parent = getppid();
        if (prctl(PR_SET_PDEATHSIG, SIGKILL, 0, 0, 0) != 0) return BALLUM_STATUS_PERMISSION_DENIED;
        if (getppid() != parent) return BALLUM_STATUS_PERMISSION_DENIED;   /* parent already gone */
    }
    if (p->no_new_privs && prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) != 0)
        return BALLUM_STATUS_PERMISSION_DENIED;
#  if defined(BALLUM_HAVE_SECCOMP)
    if (p->seccomp_denylist) {
        /* an unprivileged filter requires no_new_privs */
        if (!p->no_new_privs) return BALLUM_STATUS_INVALID_ARGUMENT;
        if (install_seccomp() != 0) return BALLUM_STATUS_PERMISSION_DENIED;
    }
#  endif
#else
    if (p->no_new_privs) return BALLUM_STATUS_UNSUPPORTED;           /* fail closed */
#endif
    return BALLUM_STATUS_OK;
}

ballum_status_t ballum_sandbox_harden_self(void) {
    struct rlimit rl = { 0, 0 };
    if (setrlimit(RLIMIT_CORE, &rl) != 0) return BALLUM_STATUS_PERMISSION_DENIED;
#if defined(__linux__)
    if (prctl(PR_SET_DUMPABLE, 0, 0, 0, 0) != 0) return BALLUM_STATUS_PERMISSION_DENIED;
    return BALLUM_STATUS_OK;
#else
    return BALLUM_STATUS_UNSUPPORTED;
#endif
}

/* ---- trusted program resolution ------------------------------------------ */

static int unsafe_mode(const struct stat *st) {
    return (st->st_mode & (S_IWGRP | S_IWOTH)) != 0;
}

static int trusted_executable(const char *path) {
    struct stat st;
    if (stat(path, &st) != 0) return 0;
    if (!S_ISREG(st.st_mode) || unsafe_mode(&st)) return 0;
    if (access(path, X_OK) != 0) return 0;
    /* the containing directory must not be writable by group/others either */
    char dir[4096];
    size_t len = strlen(path);
    if (len >= sizeof(dir)) return 0;
    memcpy(dir, path, len + 1);
    char *slash = strrchr(dir, '/');
    if (!slash) return 0;
    if (slash == dir) dir[1] = '\0'; else *slash = '\0';
    if (stat(dir, &st) != 0 || !S_ISDIR(st.st_mode)) return 0;
    if (unsafe_mode(&st)) return 0;       /* group/world-writable, sticky or not */
    return 1;
}

ballum_status_t ballum_sandbox_resolve(const char *name, char *out, size_t cap) {
    if (!name || !*name || !out || cap == 0) return BALLUM_STATUS_INVALID_ARGUMENT;
    if (strlen(name) >= 4096u) return BALLUM_STATUS_INVALID_ARGUMENT;
    if (strchr(name, '/')) {
        if (name[0] != '/') return BALLUM_STATUS_INVALID_ARGUMENT;   /* relative path */
        if (strlen(name) >= cap) return BALLUM_STATUS_BUFFER_TOO_SMALL;
        if (!trusted_executable(name)) return BALLUM_STATUS_PERMISSION_DENIED;
        strcpy(out, name);
        return BALLUM_STATUS_OK;
    }
    static const char *const dirs[] = { "/usr/local/bin", "/usr/bin", "/bin",
                                        "/usr/local/sbin", "/usr/sbin", "/sbin" };
    for (size_t i = 0; i < sizeof(dirs) / sizeof(dirs[0]); ++i) {
        char cand[4096];
        int n = snprintf(cand, sizeof(cand), "%s/%s", dirs[i], name);
        if (n < 0 || (size_t)n >= sizeof(cand)) continue;
        if (!trusted_executable(cand)) continue;
        if ((size_t)n >= cap) return BALLUM_STATUS_BUFFER_TOO_SMALL;
        strcpy(out, cand);
        return BALLUM_STATUS_OK;
    }
    return BALLUM_STATUS_BACKEND_UNAVAILABLE;
}

/* ---- environment --------------------------------------------------------- */

static int valid_env_name(const char *s, size_t len) {
    if (len == 0 || !(isalpha((unsigned char)s[0]) || s[0] == '_')) return 0;
    for (size_t i = 1; i < len; ++i)
        if (!(isalnum((unsigned char)s[i]) || s[i] == '_')) return 0;
    return 1;
}

void ballum_sandbox_env_free(char **env) {
    if (!env) return;
    for (char **e = env; *e; ++e) free(*e);
    free(env);
}

char **ballum_sandbox_env_new(const char *const *allow, const char *const *extra) {
    size_t cap = 1;
    for (const char *const *a = allow; a && *a; ++a) cap++;
    for (const char *const *x = extra; x && *x; ++x) cap++;
    char **env = (char **)calloc(cap, sizeof(char *));
    if (!env) return NULL;
    size_t n = 0;
    for (const char *const *a = allow; a && *a; ++a) {
        if (!valid_env_name(*a, strlen(*a))) goto fail;
        const char *v = getenv(*a);
        if (!v) continue;
        size_t len = strlen(*a) + strlen(v) + 2;
        char *s = (char *)malloc(len);
        if (!s) goto fail;
        snprintf(s, len, "%s=%s", *a, v);
        env[n++] = s;
    }
    for (const char *const *x = extra; x && *x; ++x) {
        const char *eq = strchr(*x, '=');
        if (!eq || !valid_env_name(*x, (size_t)(eq - *x))) goto fail;
        char *s = strdup(*x);
        if (!s) goto fail;
        env[n++] = s;
    }
    env[n] = NULL;
    return env;
fail:
    ballum_sandbox_env_free(env);
    return NULL;
}

/* ---- spawn / wait / terminate -------------------------------------------- */

struct child_report { int stage; int err; };   /* stage 1 = sandbox, 2 = exec */

static void close_fds_except(int keep) {
    if (keep < 3) keep = 2;                       /* stdio is rebuilt by the caller */
#if defined(__linux__) && defined(SYS_close_range)
    {
        int ok = 1;
        if (keep > 3) ok = syscall(SYS_close_range, 3u, (unsigned)(keep - 1), 0u) == 0;
        if (ok) ok = syscall(SYS_close_range, (unsigned)(keep + 1), ~0u, 0u) == 0;
        if (ok) return;
    }
#endif
    {
        struct rlimit rl;
        long max = 65536;
        if (getrlimit(RLIMIT_NOFILE, &rl) == 0 && rl.rlim_cur != RLIM_INFINITY &&
            (long)rl.rlim_cur < max)
            max = (long)rl.rlim_cur;
        for (int fd = 3; fd < max; ++fd) if (fd != keep) close(fd);
    }
}

ballum_status_t ballum_sandbox_spawn(const ballum_sandbox_policy_t *policy,
                                     const char *path, char *const argv[],
                                     char *const envp[], int log_fd, int *pid_out) {
    if (!policy || !path || path[0] != '/' || !argv || !argv[0] || !envp || !pid_out)
        return BALLUM_STATUS_INVALID_ARGUMENT;
    *pid_out = -1;
    int pipefd[2];
    if (pipe(pipefd) != 0) return BALLUM_STATUS_BACKEND_ERROR;
    if (pipefd[1] < 3) {                          /* keep it clear of stdio */
        int moved = fcntl(pipefd[1], F_DUPFD, 3);
        if (moved < 0) { close(pipefd[0]); close(pipefd[1]); return BALLUM_STATUS_BACKEND_ERROR; }
        close(pipefd[1]);
        pipefd[1] = moved;
    }
    if (fcntl(pipefd[1], F_SETFD, FD_CLOEXEC) != 0) {
        close(pipefd[0]); close(pipefd[1]);
        return BALLUM_STATUS_BACKEND_ERROR;
    }
    fflush(NULL);
    pid_t pid = fork();
    if (pid < 0) { close(pipefd[0]); close(pipefd[1]); return BALLUM_STATUS_BACKEND_ERROR; }

    if (pid == 0) {
        struct child_report rep = { 0, 0 };
        int devnull = open("/dev/null", O_RDWR);
        if (devnull < 0) { rep.stage = 1; rep.err = errno; goto report; }
        if (dup2(devnull, 0) < 0) { rep.stage = 1; rep.err = errno; goto report; }
        int out = log_fd >= 0 ? log_fd : devnull;
        if (dup2(out, 1) < 0 || dup2(out, 2) < 0) { rep.stage = 1; rep.err = errno; goto report; }
        if (policy->close_inherited_fds) close_fds_except(pipefd[1]);
        {
            struct sigaction sa;                      /* default dispositions for the program */
            memset(&sa, 0, sizeof(sa));
            sa.sa_handler = SIG_DFL;
            for (int s = 1; s < 32; ++s) if (s != SIGKILL && s != SIGSTOP) sigaction(s, &sa, NULL);
            sigset_t none; sigemptyset(&none); sigprocmask(SIG_SETMASK, &none, NULL);
        }
        {
            ballum_status_t ast = ballum_sandbox_apply(policy);
            if (ast != BALLUM_STATUS_OK) { rep.stage = 1; rep.err = (int)ast; goto report; }
        }
        execve(path, argv, envp);
        rep.stage = 2; rep.err = errno;
    report:
        if (write(pipefd[1], &rep, sizeof(rep)) < 0) { /* nothing more to do */ }
        _exit(127);
    }

    close(pipefd[1]);
    struct child_report rep;
    ssize_t got;
    do { got = read(pipefd[0], &rep, sizeof(rep)); } while (got < 0 && errno == EINTR);
    close(pipefd[0]);
    if (got == 0) { *pid_out = (int)pid; return BALLUM_STATUS_OK; }   /* exec succeeded: pipe closed */

    int st;
    waitpid(pid, &st, 0);                                             /* child failed: reap it */
    if (got == (ssize_t)sizeof(rep) && rep.stage == 2) return BALLUM_STATUS_BACKEND_UNAVAILABLE;
    if (got == (ssize_t)sizeof(rep) && rep.stage == 1 && rep.err > 0 &&
        rep.err <= (int)BALLUM_STATUS_NOT_INITIALIZED)
        return (ballum_status_t)rep.err;          /* the status apply() reported */
    return BALLUM_STATUS_PERMISSION_DENIED;
}

int ballum_sandbox_wait(int pid, int block, int *exit_code) {
    if (pid <= 0) return -1;
    int st = 0;
    pid_t r;
    do { r = waitpid((pid_t)pid, &st, block ? 0 : WNOHANG); } while (r < 0 && errno == EINTR);
    if (r == 0) return 0;
    if (r < 0) return -1;
    if (exit_code) *exit_code = WIFEXITED(st) ? WEXITSTATUS(st)
                              : (WIFSIGNALED(st) ? 128 + WTERMSIG(st) : -1);
    return 1;
}

static void sleep_ms(unsigned ms) {
    struct timespec ts = { (time_t)(ms / 1000u), (long)(ms % 1000u) * 1000000L };
    nanosleep(&ts, NULL);
}

void ballum_sandbox_terminate(int pid, unsigned grace_ms) {
    if (pid <= 1) return;
    int code;
    if (ballum_sandbox_wait(pid, 0, &code) == 1) { kill(-(pid_t)pid, SIGKILL); return; }
    kill(-(pid_t)pid, SIGTERM);
    for (unsigned waited = 0; waited <= grace_ms; waited += 10u) {
        if (ballum_sandbox_wait(pid, 0, &code) != 0) { kill(-(pid_t)pid, SIGKILL); return; }
        sleep_ms(10u);
    }
    kill(-(pid_t)pid, SIGKILL);
    ballum_sandbox_wait(pid, 1, &code);
}

#else  /* not POSIX (Windows): every call fails closed */

ballum_status_t ballum_sandbox_apply(const ballum_sandbox_policy_t *p) { (void)p; return BALLUM_STATUS_UNSUPPORTED; }
ballum_status_t ballum_sandbox_harden_self(void) { return BALLUM_STATUS_UNSUPPORTED; }
ballum_status_t ballum_sandbox_resolve(const char *n, char *o, size_t c) { (void)n; (void)o; (void)c; return BALLUM_STATUS_UNSUPPORTED; }
char **ballum_sandbox_env_new(const char *const *a, const char *const *x) { (void)a; (void)x; return NULL; }
void ballum_sandbox_env_free(char **e) { (void)e; }
ballum_status_t ballum_sandbox_spawn(const ballum_sandbox_policy_t *p, const char *path, char *const argv[],
                                     char *const envp[], int log_fd, int *pid_out) {
    (void)p; (void)path; (void)argv; (void)envp; (void)log_fd; if (pid_out) *pid_out = -1;
    return BALLUM_STATUS_UNSUPPORTED;
}
int ballum_sandbox_wait(int pid, int block, int *code) { (void)pid; (void)block; (void)code; return -1; }
void ballum_sandbox_terminate(int pid, unsigned grace_ms) { (void)pid; (void)grace_ms; }

#endif
