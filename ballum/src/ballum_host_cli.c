/*
 * ballum-host -- put a program inside the Ballum environment.
 *
 *   ballum-host --list
 *   ballum-host --profile sleela-terminal
 *   ballum-host --profile xterm --enter disintegrate --leave fade
 *   ballum-host [options] -- /usr/bin/xterm -e top
 *
 * The screen turns white, the program enters with the chosen effect, and every
 * key and mouse event goes to it 1:1. Leave with Ctrl+Alt+Esc (or end the
 * program). Exit status: 0 ran and ended, 2 usage, 3 cannot host here.
 */
#include "sleela_ballum/ballum_host.h"
#include "sleela_ballum/ballum_host_profiles.h"
#include "sleela_ballum/ballum_sandbox.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void usage(FILE *f) {
    fprintf(f,
        "usage: ballum-host [options] [-- command [args...]]\n"
        "  --list                 show profiles and transitions\n"
        "  --profile NAME         start a profile (see --list)\n"
        "  --enter NAME           entry transition   (default: profile or disintegrate)\n"
        "  --leave NAME           exit transition    (default: profile or fade)\n"
        "  --enter-ms N           entry duration, ms (default 700)\n"
        "  --display ID           display id         (default: the primary display)\n"
        "  --fps N                frame rate         (default 60)\n"
        "  --max-seconds N        end the session after N seconds\n"
        "  --header N             header height in px (0 = none, default 32)\n"
        "  --no-grab              do not grab keyboard and pointer\n"
        "  --no-sandbox           run the program without the seccomp/no_new_privs layer\n"
        "Leave the session with Ctrl+Alt+Esc.\n");
}

static void list_all(void) {
    puts("profiles:");
    for (size_t i = 0; i < ballum_profile_count(); ++i) {
        const ballum_profile_t *p = ballum_profile_at(i);
        printf("  %-16s %s  [enter %s, leave %s]\n", p->name, p->description, p->enter, p->leave);
    }
    puts("transitions:");
    printf(" ");
    ballum_transition_t t;
    static const char *host_kinds[] = { "none", "slide-left", "slide-right", "slide-up",
                                        "slide-down", "pixelize", "disintegrate" };
    for (size_t i = 0; i < sizeof(host_kinds) / sizeof(host_kinds[0]); ++i) printf(" %s", host_kinds[i]);
    for (int i = 0; i < ballum_fx_count(); ++i) {
        t.kind = BALLUM_TRANSITION_FX; t.fx = (ballum_fx_t)i;
        const char *n = ballum_transition_name(&t);
        int shadowed = 0;
        for (size_t k = 0; k < sizeof(host_kinds) / sizeof(host_kinds[0]); ++k)
            if (strcmp(n, host_kinds[k]) == 0) shadowed = 1;
        if (!shadowed) printf(" %s", n);
    }
    putchar('\n');
}

static int parse_uint(const char *s, unsigned long max, unsigned long *out) {
    char *end = NULL;
    errno = 0;
    unsigned long v = strtoul(s, &end, 10);
    if (errno || end == s || *end || v > max) return 0;
    *out = v;
    return 1;
}

int main(int argc, char **argv) {
    const char *profile_name = NULL, *enter = NULL, *leave = NULL;
    unsigned long display = 0, fps = 60, max_s = 0, header = 32, enter_ms = 700;
    int have_display = 0, grab = 1, sandbox = 1, have_header = 0, have_enter_ms = 0;
    char **cmd = NULL;

    for (int i = 1; i < argc; ++i) {
        const char *a = argv[i];
        const char *v = (i + 1 < argc) ? argv[i + 1] : NULL;
        if (strcmp(a, "--") == 0) { cmd = &argv[i + 1]; break; }
        else if (strcmp(a, "--help") == 0 || strcmp(a, "-h") == 0) { usage(stdout); return 0; }
        else if (strcmp(a, "--list") == 0) { list_all(); return 0; }
        else if (strcmp(a, "--no-grab") == 0) grab = 0;
        else if (strcmp(a, "--no-sandbox") == 0) sandbox = 0;
        else if (!v) { fprintf(stderr, "ballum-host: %s needs a value\n", a); usage(stderr); return 2; }
        else if (strcmp(a, "--profile") == 0) { profile_name = v; ++i; }
        else if (strcmp(a, "--enter") == 0) { enter = v; ++i; }
        else if (strcmp(a, "--leave") == 0) { leave = v; ++i; }
        else if (strcmp(a, "--display") == 0) { if (!parse_uint(v, 0xFFFFFFFFul, &display)) goto bad_value; have_display = 1; ++i; }
        else if (strcmp(a, "--fps") == 0) { if (!parse_uint(v, 240, &fps) || fps == 0) goto bad_value; ++i; }
        else if (strcmp(a, "--max-seconds") == 0) { if (!parse_uint(v, 86400ul * 7, &max_s)) goto bad_value; ++i; }
        else if (strcmp(a, "--header") == 0) { if (!parse_uint(v, 512, &header)) goto bad_value; have_header = 1; ++i; }
        else if (strcmp(a, "--enter-ms") == 0) { if (!parse_uint(v, 10000, &enter_ms)) goto bad_value; have_enter_ms = 1; ++i; }
        else { fprintf(stderr, "ballum-host: unknown option %s\n", a); usage(stderr); return 2; }
        continue;
    bad_value:
        fprintf(stderr, "ballum-host: invalid value for %s: %s\n", a, v);
        return 2;
    }

    const ballum_profile_t *profile = NULL;
    if (profile_name) {
        profile = ballum_profile_find(profile_name);
        if (!profile) { fprintf(stderr, "ballum-host: unknown profile '%s' (see --list)\n", profile_name); return 2; }
    }
    if ((cmd && cmd[0]) == (profile != NULL)) {
        fprintf(stderr, "ballum-host: give exactly one of --profile NAME or -- command...\n");
        usage(stderr);
        return 2;
    }

    ballum_host_config_t cfg;
    ballum_host_config_default(&cfg, 0);
    if (profile) {
        ballum_transition_by_name(profile->enter, &cfg.enter);
        ballum_transition_by_name(profile->leave, &cfg.leave);
    }
    if (enter && !ballum_transition_by_name(enter, &cfg.enter)) { fprintf(stderr, "ballum-host: unknown transition '%s' (see --list)\n", enter); return 2; }
    if (leave && !ballum_transition_by_name(leave, &cfg.leave)) { fprintf(stderr, "ballum-host: unknown transition '%s' (see --list)\n", leave); return 2; }
    if (have_header) cfg.header_height = (uint32_t)header;
    if (have_enter_ms) cfg.enter_ms = (uint32_t)enter_ms;
    cfg.max_runtime_ms = (uint64_t)max_s * 1000u;
    cfg.grab_input = grab;

    /* Protect Ballum itself before it starts monitoring input. A failure is
     * reported, not ignored: the user should know the protection is absent. */
    ballum_status_t st = ballum_sandbox_harden_self();
    if (st != BALLUM_STATUS_OK)
        fprintf(stderr, "ballum-host: warning: could not harden this process (%s)\n", ballum_status_name(st));

    ballum_manager_t mgr;
    st = ballum_manager_init(&mgr, NULL, NULL);
    ballum_display_t displays[BALLUM_MAX_DISPLAYS];
    size_t count = 0;
    if (st == BALLUM_STATUS_OK) st = ballum_manager_enumerate(&mgr, displays, BALLUM_MAX_DISPLAYS, &count);
    if (st != BALLUM_STATUS_OK || count == 0) {
        fprintf(stderr, "ballum-host: no usable display (%s). Run from a graphical X11 session.\n", ballum_status_name(st));
        return 3;
    }
    cfg.display_id = displays[0].id;
    for (size_t i = 0; i < count; ++i) {
        if (have_display ? displays[i].id == (uint32_t)display : displays[i].is_primary) { cfg.display_id = displays[i].id; break; }
    }
    if (have_display && cfg.display_id != (uint32_t)display) { fprintf(stderr, "ballum-host: display %lu not found\n", display); return 2; }

    ballum_program_t prog;
    memset(&prog, 0, sizeof(prog));
    prog.argv = profile ? profile->argv : (const char *const *)cmd;
    prog.title = profile ? profile->title : cmd[0];
    prog.sandbox = sandbox;
    prog.log_fd = -1;

    ballum_host_source_t source;
    void *sctx = NULL;
    st = ballum_host_source_x11_create(&prog, &source, &sctx);
    if (st != BALLUM_STATUS_OK) { fprintf(stderr, "ballum-host: cannot host programs on this build (%s)\n", ballum_status_name(st)); return 3; }

    ballum_host_t *host = NULL;
    st = ballum_host_create(&mgr, &cfg, &source, sctx, &host);
    if (st != BALLUM_STATUS_OK) {
        fprintf(stderr, "ballum-host: cannot create the host (%s)\n", ballum_status_name(st));
        ballum_host_source_x11_destroy(sctx);
        return 3;
    }
    st = ballum_host_run(host, (unsigned)fps);
    ballum_exit_reason_t why = ballum_host_exit_reason(host);
    ballum_host_destroy(host);
    ballum_host_source_x11_destroy(sctx);

    if (st != BALLUM_STATUS_OK) {
        fprintf(stderr, "ballum-host: %s (%s)\n",
                why == BALLUM_EXIT_STARTUP_FAILED
                    ? "the program could not be started: is Xvfb, libXtst and the program installed?"
                    : "the session ended with an error",
                ballum_status_name(st));
        return 3;
    }
    fprintf(stderr, "ballum-host: session ended (%s)\n", ballum_exit_reason_name(why));
    return why == BALLUM_EXIT_STARTUP_FAILED || why == BALLUM_EXIT_ERROR ? 3 : 0;
}
