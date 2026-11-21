/* Tests for process protection: policy, resolution, environment, spawn, seccomp. */
#define _GNU_SOURCE 1
#include "sleela_ballum/ballum_sandbox.h"

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#if defined(__linux__)
#  include <sys/prctl.h>
#  include <sys/resource.h>
#  include <sys/syscall.h>
#  include <sys/wait.h>
#endif

#define CHECK(c) do { if (!(c)) { fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #c); exit(1); } } while (0)

/* Run in the sandboxed child: exit 0 only if the protections are really in force. */
static int child_probe(void) {
#if defined(__linux__) && defined(__NR_ptrace)
    if (prctl(PR_GET_NO_NEW_PRIVS, 0, 0, 0, 0) != 1) return 10;
    struct rlimit rl;
    if (getrlimit(RLIMIT_CORE, &rl) != 0 || rl.rlim_cur != 0 || rl.rlim_max != 0) return 11;
    errno = 0;
    if (syscall(__NR_ptrace, 0 /*PTRACE_TRACEME*/, 0, 0, 0) != -1 || errno != EPERM) return 12;
    errno = 0;
    if (syscall(__NR_mount, "none", "/", "none", 0, 0) != -1 || errno != EPERM) return 13;
    errno = 0;
    if (syscall(__NR_reboot, 0, 0, 0, 0) != -1 || errno != EPERM) return 14;
    if (syscall(__NR_getpid) <= 0) return 15;                 /* ordinary calls still work */
    int fd = open("/dev/null", O_RDONLY);
    if (fd < 0) return 16;
    close(fd);
    if (getsid(0) != getpid()) return 17;                      /* own session */
#endif
    return 0;
}

int main(int argc, char **argv) {
    if (argc > 1 && strcmp(argv[1], "--probe") == 0) return child_probe();

    /* ---- policy defaults --------------------------------------------------- */
    ballum_sandbox_policy_t pol;
    ballum_sandbox_policy_default(&pol);
    CHECK(pol.no_new_privs && pol.seccomp_denylist && pol.disable_core_dumps);
    CHECK(pol.close_inherited_fds && pol.new_session && pol.die_with_parent);
    CHECK(pol.max_open_files == 4096 && pol.max_cpu_seconds == 0 && pol.max_address_space == 0);
    CHECK(ballum_sandbox_apply(NULL) == BALLUM_STATUS_INVALID_ARGUMENT);

    /* ---- resolve ----------------------------------------------------------- */
    char buf[PATH_MAX];
    CHECK(ballum_sandbox_resolve("sh", buf, sizeof(buf)) == BALLUM_STATUS_OK);
    CHECK(buf[0] == '/' && access(buf, X_OK) == 0);
    CHECK(ballum_sandbox_resolve("./sh", buf, sizeof(buf)) == BALLUM_STATUS_INVALID_ARGUMENT);
    CHECK(ballum_sandbox_resolve("bin/sh", buf, sizeof(buf)) == BALLUM_STATUS_INVALID_ARGUMENT);
    CHECK(ballum_sandbox_resolve("", buf, sizeof(buf)) == BALLUM_STATUS_INVALID_ARGUMENT);
    CHECK(ballum_sandbox_resolve(NULL, buf, sizeof(buf)) == BALLUM_STATUS_INVALID_ARGUMENT);
    CHECK(ballum_sandbox_resolve("no-such-program-xyz", buf, sizeof(buf)) == BALLUM_STATUS_BACKEND_UNAVAILABLE);
    CHECK(ballum_sandbox_resolve("/no/such/dir/prog", buf, sizeof(buf)) == BALLUM_STATUS_PERMISSION_DENIED);
    CHECK(ballum_sandbox_resolve("sh", buf, 3) == BALLUM_STATUS_BUFFER_TOO_SMALL);
    {   /* a group/world-writable program is never trusted */
        char dir[] = "./ballum-sbx-XXXXXX";
        CHECK(mkdtemp(dir));
        char file[PATH_MAX], real[PATH_MAX];
        snprintf(file, sizeof(file), "%s/prog", dir);
        int fd = open(file, O_CREAT | O_WRONLY, 0700);
        CHECK(fd >= 0); close(fd);
        CHECK(chmod(file, 0777) == 0);
        CHECK(realpath(file, real));
        CHECK(ballum_sandbox_resolve(real, buf, sizeof(buf)) == BALLUM_STATUS_PERMISSION_DENIED);
        CHECK(chmod(file, 0755) == 0);
        CHECK(chmod(dir, 0755) == 0);
        CHECK(ballum_sandbox_resolve(real, buf, sizeof(buf)) == BALLUM_STATUS_OK);
        CHECK(chmod(dir, 0777) == 0);                                  /* writable directory */
        CHECK(ballum_sandbox_resolve(real, buf, sizeof(buf)) == BALLUM_STATUS_PERMISSION_DENIED);
        unlink(file); chmod(dir, 0700); rmdir(dir);
    }

    /* ---- environment allowlist -------------------------------------------- */
    setenv("BALLUM_T_KEEP", "kept", 1);
    setenv("BALLUM_T_SECRET", "leak", 1);
    const char *allow[] = { "BALLUM_T_KEEP", "BALLUM_T_UNSET", NULL };
    const char *extra[] = { "DISPLAY=:77", "EMPTY=", NULL };
    char **env = ballum_sandbox_env_new(allow, extra);
    CHECK(env);
    int n = 0, kept = 0, disp = 0, leak = 0;
    for (char **e = env; *e; ++e) {
        ++n;
        if (strcmp(*e, "BALLUM_T_KEEP=kept") == 0) kept = 1;
        if (strcmp(*e, "DISPLAY=:77") == 0) disp = 1;
        if (strncmp(*e, "BALLUM_T_SECRET", 15) == 0) leak = 1;
    }
    CHECK(n == 3 && kept && disp && !leak);
    ballum_sandbox_env_free(env);
    const char *bad1[] = { "1BAD=x", NULL }, *bad2[] = { "NOEQUALS", NULL }, *bad3[] = { "=v", NULL };
    const char *badname[] = { "BAD NAME", NULL };
    CHECK(ballum_sandbox_env_new(NULL, bad1) == NULL);
    CHECK(ballum_sandbox_env_new(NULL, bad2) == NULL);
    CHECK(ballum_sandbox_env_new(NULL, bad3) == NULL);
    CHECK(ballum_sandbox_env_new(badname, NULL) == NULL);
    env = ballum_sandbox_env_new(NULL, NULL);
    CHECK(env && env[0] == NULL);
    ballum_sandbox_env_free(env);
    ballum_sandbox_env_free(NULL);

#if defined(__linux__)
    /* ---- apply() really installs the protections -------------------------- */
    {
        pid_t pid = fork();
        CHECK(pid >= 0);
        if (pid == 0) _exit(ballum_sandbox_apply(&pol) == BALLUM_STATUS_OK ? child_probe()
                                                                           : 100);
        int st = 0;
        CHECK(waitpid(pid, &st, 0) == pid);
        CHECK(WIFEXITED(st));
        int code = WEXITSTATUS(st);
        if (code == 100) {
            fprintf(stderr, "test_ballum_sandbox: seccomp unavailable here, apply failed closed (ok)\n");
        } else {
            if (code != 0) fprintf(stderr, "child probe failed with code %d\n", code);
            CHECK(code == 0);
        }
    }
    /* seccomp requested without no_new_privs is a programming error, not a downgrade */
    {
        pid_t pid = fork();
        CHECK(pid >= 0);
        if (pid == 0) {
            ballum_sandbox_policy_t q = pol;
            q.no_new_privs = 0; q.new_session = 0;
            _exit(ballum_sandbox_apply(&q) == BALLUM_STATUS_INVALID_ARGUMENT ? 0 : 1);
        }
        int st = 0;
        CHECK(waitpid(pid, &st, 0) == pid && WIFEXITED(st) && WEXITSTATUS(st) == 0);
    }
#endif

    /* ---- spawn ------------------------------------------------------------- */
    char *envp0[] = { "PATH=/usr/bin:/bin", NULL };
    int pid = -1, code = -1;
    {   char *av[] = { "sh", "-c", "exit 7", NULL };
        CHECK(ballum_sandbox_spawn(&pol, "/bin/sh", av, envp0, -1, &pid) == BALLUM_STATUS_OK);
        CHECK(pid > 1);
        CHECK(ballum_sandbox_wait(pid, 1, &code) == 1 && code == 7);
        CHECK(ballum_sandbox_wait(pid, 0, &code) == -1);               /* already reaped */
    }
    {   char *av[] = { "x", NULL };
        CHECK(ballum_sandbox_spawn(&pol, "/no/such/program", av, envp0, -1, &pid) == BALLUM_STATUS_BACKEND_UNAVAILABLE);
        CHECK(pid == -1);
        CHECK(ballum_sandbox_spawn(&pol, "relative/prog", av, envp0, -1, &pid) == BALLUM_STATUS_INVALID_ARGUMENT);
        CHECK(ballum_sandbox_spawn(NULL, "/bin/sh", av, envp0, -1, &pid) == BALLUM_STATUS_INVALID_ARGUMENT);
        CHECK(ballum_sandbox_spawn(&pol, "/bin/sh", NULL, envp0, -1, &pid) == BALLUM_STATUS_INVALID_ARGUMENT);
    }
    {   /* output goes to the supplied descriptor, never to our stdout */
        int p[2];
        CHECK(pipe(p) == 0);
        char *av[] = { "sh", "-c", "echo hello-ballum", NULL };
        CHECK(ballum_sandbox_spawn(&pol, "/bin/sh", av, envp0, p[1], &pid) == BALLUM_STATUS_OK);
        close(p[1]);
        char line[64] = { 0 };
        CHECK(read(p[0], line, sizeof(line) - 1) > 0);
        CHECK(strncmp(line, "hello-ballum", 12) == 0);
        close(p[0]);
        CHECK(ballum_sandbox_wait(pid, 1, &code) == 1 && code == 0);
    }
#if defined(__linux__)
    {   /* environment is the allowlist only, and inherited descriptors are closed */
        int leak_fd = open("/dev/null", O_RDONLY);                      /* not CLOEXEC */
        CHECK(leak_fd >= 3);
        char script[160];
        snprintf(script, sizeof(script),
                 "[ -z \"$BALLUM_T_SECRET\" ] && [ \"$MARK\" = yes ] && [ ! -e /proc/self/fd/%d ]", leak_fd);
        char *av[] = { "sh", "-c", script, NULL };
        char *envp1[] = { "PATH=/usr/bin:/bin", "MARK=yes", NULL };
        CHECK(ballum_sandbox_spawn(&pol, "/bin/sh", av, envp1, -1, &pid) == BALLUM_STATUS_OK);
        CHECK(ballum_sandbox_wait(pid, 1, &code) == 1 && code == 0);
        /* and with the policy off, the descriptor does leak (the test proves it measures) */
        ballum_sandbox_policy_t open_pol = pol;
        open_pol.close_inherited_fds = 0; open_pol.seccomp_denylist = 0;
        CHECK(ballum_sandbox_spawn(&open_pol, "/bin/sh", av, envp1, -1, &pid) == BALLUM_STATUS_OK);
        CHECK(ballum_sandbox_wait(pid, 1, &code) == 1 && code != 0);
        close(leak_fd);
    }
    {   /* the protections hold inside a program started by spawn (survive exec) */
        char self[PATH_MAX];
        ssize_t len = readlink("/proc/self/exe", self, sizeof(self) - 1);
        CHECK(len > 0);
        self[len] = '\0';
        char *av[] = { "probe", "--probe", NULL };
        CHECK(ballum_sandbox_spawn(&pol, self, av, envp0, -1, &pid) == BALLUM_STATUS_OK);
        CHECK(ballum_sandbox_wait(pid, 1, &code) == 1);
        if (code == 127 || code == 12) { /* 12 would mean ptrace was NOT denied */ }
        CHECK(code == 0);
    }
#endif
    {   /* terminate ends the whole group promptly */
        char *av[] = { "sh", "-c", "sleep 60 & wait", NULL };
        CHECK(ballum_sandbox_spawn(&pol, "/bin/sh", av, envp0, -1, &pid) == BALLUM_STATUS_OK);
        struct timespec a, b;
        clock_gettime(CLOCK_MONOTONIC, &a);
        ballum_sandbox_terminate(pid, 1000);
        clock_gettime(CLOCK_MONOTONIC, &b);
        CHECK((b.tv_sec - a.tv_sec) < 3);
        CHECK(ballum_sandbox_wait(pid, 0, &code) == -1);               /* reaped by terminate */
        ballum_sandbox_terminate(pid, 10);                              /* idempotent */
        ballum_sandbox_terminate(-5, 10);
    }

    /* ---- the Ballum process protects itself ------------------------------- */
#if defined(__linux__)
    CHECK(ballum_sandbox_harden_self() == BALLUM_STATUS_OK);
    CHECK(prctl(PR_GET_DUMPABLE, 0, 0, 0, 0) == 0);
#endif
    puts("test_ballum_sandbox: ok");
    return 0;
}
