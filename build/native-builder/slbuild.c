/* ==========================================================================
 * slbuild — SLeeLa OS-specific native builder/installer.
 * Max Rupplin · MEARVK LLC · 2026
 *
 * WHAT IT IS
 *   A single native binary, compiled per-OS by build/native-builder/
 *   make-slbuild.sh, that orchestrates the SLeeLa main-package build and
 *   installation with the calm, beautiful presentation of the shell
 *   clean-install. It is the compiled front door to the same real process:
 *     1) build the integrity-gated core products (sleela, sleelvac, nordshrift)
 *     2) stage the main package (standard library minus the excluded trees)
 *     3) install user-local (or to --prefix), link the binaries
 *     4) print a formal "Record of Build" attestation
 *
 * WHY A BINARY
 *   The build/install logic lives in a real, OS-aware executable so it can be
 *   shipped and run directly on each platform, rather than requiring a shell
 *   script interpreter. It detects the host OS at runtime (operating-system/
 *   identity model) and adapts paths and the build command accordingly.
 *
 * USAGE
 *   slbuild [--prefix DIR] [--plain] [--no-color] [--build-only] [--dry-run]
 *           [--repo DIR] [-h|--help]
 *
 *   --prefix DIR   install root (default: $HOME/.local/sleela)
 *   --build-only   build the core products, do not stage/install
 *   --dry-run      print what would happen; do not build or install
 *   --plain        ASCII only, no color (also honors NO_COLOR / non-TTY)
 *   --repo DIR     repository root (default: auto-detected from argv[0])
 *
 * The EXACT, verbatim compiler/linker output is captured to a timestamped
 * log; the screen shows a calm, summarized progress report.
 * ========================================================================== */

/* Enable POSIX/GNU library surface (realpath, setenv, popen, fileno,
 * localtime_r, PATH_MAX) under -std=c11, which otherwise hides them. */
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 700
#endif
#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <libgen.h>

/* Some platforms do not expose PATH_MAX in <limits.h>; provide a safe default. */
#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#define SL_PRODUCT  "SLeeLa"
#define SL_RELEASE  "0.3.29-dev"
#define SL_SYNTAX   "1.0 - 2.5"
#define SL_TAGLINE  "THE NOW. THE INGREDIENT."

/* ---- host OS identity (operating-system/ model: National -> ... ) -------- */
static const char *host_os(void) {
#if defined(_WIN32) || defined(_WIN64)
    return "Windows";
#elif defined(__APPLE__)
    return "macOS";
#elif defined(__linux__)
    return "Linux";
#elif defined(__FreeBSD__)
    return "FreeBSD";
#else
    return "POSIX";
#endif
}

/* ---- presentation layer: color, glyphs, rules, boxes --------------------- */
static int USE_STYLE = 1;
static const char *RESET="", *BOLD="", *DIM="", *ITAL="",
                  *GREEN="", *GREENB="", *WHITE="", *GREY="", *RED="", *AMBER="";
static const char *G_OK="[ok]", *G_RUN="[..]", *G_WARN="[!]", *G_ERR="[x]", *G_DOT="-";

static void style_init(void) {
    if (getenv("NO_COLOR") != NULL) USE_STYLE = 0;
    if (!isatty(fileno(stdout))) USE_STYLE = 0;
    if (USE_STYLE) {
        RESET="\033[0m"; BOLD="\033[1m"; DIM="\033[2m"; ITAL="\033[3m";
        GREEN="\033[38;5;43m"; GREENB="\033[1;38;5;43m"; WHITE="\033[38;5;231m";
        GREY="\033[38;5;245m"; RED="\033[38;5;203m"; AMBER="\033[38;5;215m";
        G_OK="[\u2713]"; G_RUN="[\u00bb]"; G_WARN="[!]"; G_ERR="[x]"; G_DOT="\u00b7";
    } else {
        RESET=""; BOLD=""; DIM=""; ITAL=""; GREEN=""; GREENB=""; WHITE="";
        GREY=""; RED=""; AMBER="";
        G_OK="[ok]"; G_RUN="[..]"; G_WARN="[!]"; G_ERR="[x]"; G_DOT="-";
    }
}

static void rule(void) {
    int w = 72, i; const char *ch = USE_STYLE ? "\u2500" : "-";
    printf("%s", GREY);
    for (i = 0; i < w; i++) printf("%s", ch);
    printf("%s\n", RESET);
}
static void heading(const char *s) { printf("\n%s\u2590 %s%s\n", GREENB, s, RESET); rule(); }
static void step(const char *s)  { printf("  %s%s%s %s\n", GREEN, G_RUN, RESET, s); }
static void okln(const char *s)  { printf("  %s%s%s %s\n", GREEN, G_OK, RESET, s); }
static void warnln(const char *s){ printf("  %s%s%s %s\n", AMBER, G_WARN, RESET, s); }
static void item(const char *s)  { printf("    %s%s%s %s\n", GREEN, G_DOT, RESET, s); }
static void dieln(const char *s) { fprintf(stderr, "  %s%s %s%s\n", RED, G_ERR, s, RESET); exit(1); }

static void bline(const char *vis, const char *col) {
    int IW = 55, pad; const char *v = USE_STYLE ? "\u2502" : "|";
    pad = IW - (int)strlen(vis); if (pad < 0) pad = 0;
    printf("   %s%s%s %s%*s %s%s%s\n", GREEN, v, RESET, col, pad, "", GREEN, v, RESET);
}
static void banner(void) {
    int IW = 55, i;
    const char *tl = USE_STYLE?"\u256d":"+", *tr = USE_STYLE?"\u256e":"+";
    const char *bl = USE_STYLE?"\u2570":"+", *br = USE_STYLE?"\u256f":"+";
    const char *h  = USE_STYLE?"\u2500":"-";
    printf("\n   %s%s", GREEN, tl);
    for (i = 0; i < IW + 2; i++) printf("%s", h);
    printf("%s%s\n", tr, RESET);
    bline("", "");
    {
        char col[256];
        snprintf(col, sizeof col, "  %s%sS L e e L a%s    %sBuilder / Installer%s",
                 BOLD, WHITE, RESET, GREEN, RESET);
        bline("  S L e e L a    Builder / Installer", col);
        snprintf(col, sizeof col, "  %s%s%s%s", DIM, ITAL, SL_TAGLINE, RESET);
        bline("  " SL_TAGLINE, col);
    }
    bline("", "");
    {
        char vis[128], col[256];
        snprintf(vis, sizeof vis, "  release %s    syntax %s    os %s",
                 SL_RELEASE, SL_SYNTAX, host_os());
        snprintf(col, sizeof col,
                 "  %srelease%s %s%s%s    %ssyntax%s %s%s%s    %sos%s %s%s%s",
                 GREY, RESET, WHITE, SL_RELEASE, RESET,
                 GREY, RESET, WHITE, SL_SYNTAX, RESET,
                 GREY, RESET, WHITE, host_os(), RESET);
        bline(vis, col);
    }
    bline("", "");
    printf("   %s%s", GREEN, bl);
    for (i = 0; i < IW + 2; i++) printf("%s", h);
    printf("%s%s\n\n", br, RESET);
}

/* ---- library exclusions (identical policy to the shell clean-install) ---- */
static const char *EXCLUDE_LIB[] = {
    "http","http-1.0","http-2.0","http-3.0","http-4.0","http-5.0","http-6.0",
    "http-7.0","http-8.0","http-9.0","http-servers","server-edition",
    "web-server-technology","ide","ide-workbench","linux-editors","wifi",
    "moral-capacitor","philosophers","manifesto","politico",
    "international-criminal-court","psychiatry","citizen", NULL
};
static int is_excluded(const char *name) {
    for (int i = 0; EXCLUDE_LIB[i]; i++)
        if (strcmp(EXCLUDE_LIB[i], name) == 0) return 1;
    return 0;
}

/* ---- small helpers ------------------------------------------------------- */
static char REPO[PATH_MAX];
static char BUILD_LOG[PATH_MAX];
static char BUILD_STARTED[64], BUILD_ENDED[64];
static int  LIB_KEPT = 0, LIB_DROPPED = 0;

static void now_str(char *out, size_t n) {
    time_t t = time(NULL); struct tm tmv; localtime_r(&t, &tmv);
    strftime(out, n, "%Y-%m-%d %H:%M:%S", &tmv);
}

/* run a shell command line, appending stdout+stderr to BUILD_LOG. */
static int run_logged(const char *cmd) {
    char full[PATH_MAX * 2];
    snprintf(full, sizeof full, "%s >> '%s' 2>&1", cmd, BUILD_LOG);
    int rc = system(full);
    return (rc == -1) ? -1 : WEXITSTATUS(rc);
}
static int run_quiet(const char *cmd) {
    char full[PATH_MAX];
    snprintf(full, sizeof full, "%s >/dev/null 2>&1", cmd);
    int rc = system(full);
    return (rc == -1) ? -1 : WEXITSTATUS(rc);
}

/* count lines in a file (for the build summary). */
static long count_lines(const char *path) {
    FILE *f = fopen(path, "r"); if (!f) return 0;
    long n = 0; int c, prev = '\n';
    while ((c = fgetc(f)) != EOF) { if (c == '\n') n++; prev = c; }
    if (prev != '\n') n++;
    fclose(f); return n;
}

static void usage(const char *argv0) {
    printf("usage: %s [--prefix DIR] [--plain] [--no-color] [--build-only]\n"
           "          [--dry-run] [--repo DIR] [-h|--help]\n", argv0);
}

int main(int argc, char **argv) {
    const char *prefix = NULL, *repo_arg = NULL;
    int build_only = 0, dry_run = 0;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--plain") || !strcmp(argv[i], "--no-color")) { setenv("NO_COLOR","1",1); }
        else if (!strcmp(argv[i], "--build-only")) build_only = 1;
        else if (!strcmp(argv[i], "--dry-run")) dry_run = 1;
        else if (!strcmp(argv[i], "--prefix") && i+1 < argc) prefix = argv[++i];
        else if (!strcmp(argv[i], "--repo") && i+1 < argc) repo_arg = argv[++i];
        else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) { usage(argv[0]); return 0; }
        else { fprintf(stderr, "unknown option: %s\n", argv[i]); usage(argv[0]); return 2; }
    }

    style_init();

    /* Resolve the repository root: --repo, else SLEELA_REPO, else derive from
     * argv[0] (build/native-builder/slbuild -> two levels up). */
    if (repo_arg) {
        realpath(repo_arg, REPO);
    } else if (getenv("SLEELA_REPO")) {
        realpath(getenv("SLEELA_REPO"), REPO);
    } else {
        /* Resolve the real path of this executable, then walk up parent
         * directories until one contains impl/Makefile. This is robust to the
         * binary's depth (bin/<os>/slbuild) and to being run from anywhere. */
        char here[PATH_MAX];
        if (realpath(argv[0], here) == NULL) {
            /* argv[0] may be a bare name found via PATH; fall back to CWD. */
            if (getcwd(here, sizeof here) == NULL) here[0] = 0;
        }
        /* start from the directory containing the executable */
        char dirbuf[PATH_MAX]; strncpy(dirbuf, here, sizeof dirbuf - 1); dirbuf[sizeof dirbuf - 1] = 0;
        char *d = dirname(dirbuf);
        REPO[0] = 0;
        for (int up = 0; up < 12 && d && d[0]; up++) {
            char chk[PATH_MAX];
            snprintf(chk, sizeof chk, "%s/impl/Makefile", d);
            if (access(chk, F_OK) == 0) { strncpy(REPO, d, sizeof REPO - 1); REPO[sizeof REPO - 1] = 0; break; }
            if (strcmp(d, "/") == 0) break;
            char parent[PATH_MAX]; snprintf(parent, sizeof parent, "%s/..", d);
            static char resolved[PATH_MAX];
            if (realpath(parent, resolved) == NULL) break;
            d = resolved;
        }
    }
    /* sanity: repo must contain impl/Makefile */
    {
        char chk[PATH_MAX]; snprintf(chk, sizeof chk, "%s/impl/Makefile", REPO);
        if (access(chk, F_OK) != 0)
            dieln("could not locate the SLeeLa repository (impl/Makefile not found); pass --repo DIR");
    }

    /* default prefix */
    char prefbuf[PATH_MAX];
    if (!prefix) {
        const char *home = getenv("HOME");
        snprintf(prefbuf, sizeof prefbuf, "%s/.local/sleela", home ? home : "/tmp");
        prefix = prefbuf;
    }

    banner();
    printf("  %sBuilds the SLeeLa core products and installs the main package,%s\n", GREY, RESET);
    printf("  %susing this host's native toolchain (%s).%s\n", GREY, host_os(), RESET);

    heading("Plan");
    { char b[PATH_MAX]; snprintf(b,sizeof b,"repository   %s", REPO); item(b);
      snprintf(b,sizeof b,"host OS      %s", host_os()); item(b);
      snprintf(b,sizeof b,"products     sleela, sleelvac, nordshrift"); item(b);
      snprintf(b,sizeof b,"install to   %s", build_only ? "(build only)" : prefix); item(b);
    }

    if (dry_run) {
        heading("Dry run");
        okln("no build or install performed (--dry-run).");
        printf("  %s%s%s\n\n", DIM, SL_TAGLINE, RESET);
        return 0;
    }

    /* -------- build the core products (integrity-gated impl build) -------- */
    heading("Building SLeeLa");
    {
        char logdir[PATH_MAX]; snprintf(logdir, sizeof logdir, "%s/build/native-builder/logs", REPO);
        char mk[PATH_MAX]; snprintf(mk, sizeof mk, "mkdir -p '%s'", logdir);
        run_quiet(mk);
        char stamp[32]; time_t t = time(NULL); struct tm tmv; localtime_r(&t,&tmv);
        strftime(stamp, sizeof stamp, "%Y%m%d-%H%M%S", &tmv);
        snprintf(BUILD_LOG, sizeof BUILD_LOG, "%s/build-%s.log", logdir, stamp);
    }
    now_str(BUILD_STARTED, sizeof BUILD_STARTED);

    char manifest[PATH_MAX];
    if (getenv("SLEELA_SHA256_MANIFEST"))
        strncpy(manifest, getenv("SLEELA_SHA256_MANIFEST"), sizeof manifest - 1);
    else
        snprintf(manifest, sizeof manifest, "%s/security/sha256-manifest.json", REPO);

    { FILE *f = fopen(BUILD_LOG, "w");
      if (f) {
        fprintf(f, "SLeeLa slbuild — verbatim build log\n");
        fprintf(f, "host OS: %s\n", host_os());
        fprintf(f, "started: %s\n", BUILD_STARTED);
        fprintf(f, "command: make -C %s/impl build/sleela build/sleelvac build/nordshrift\n", REPO);
        fprintf(f, "manifest: %s\n", manifest);
        fprintf(f, "----------------------------------------------------------------------\n");
        fclose(f);
      } }

    printf("  %sbuilding the core products; verbatim output captured to the build log%s\n", GREY, RESET);
    const char *products[] = { "sleela", "sleelvac", "nordshrift", NULL };
    int built_ok = 1;
    for (int i = 0; products[i]; i++) {
        char msg[128]; snprintf(msg, sizeof msg, "build %s%s%s", WHITE, products[i], RESET); step(msg);
        char hdr[PATH_MAX]; snprintf(hdr, sizeof hdr, "printf '\\n===== make build/%s =====\\n'", products[i]);
        run_logged(hdr);
        char cmd[PATH_MAX * 2];
        snprintf(cmd, sizeof cmd,
                 "env SLEELA_SHA256_MANIFEST='%s' make -C '%s/impl' build/%s",
                 manifest, REPO, products[i]);
        if (run_logged(cmd) == 0) {
            char o[128]; snprintf(o, sizeof o, "%s built", products[i]); okln(o);
        } else {
            built_ok = 0;
            char w[160]; snprintf(w, sizeof w, "build of %s%s%s failed — see the log tail below:", WHITE, products[i], RESET);
            warnln(w);
            char tail[PATH_MAX]; snprintf(tail, sizeof tail, "tail -n 14 '%s' | sed 's/^/        /'", BUILD_LOG);
            system(tail);
            break;
        }
    }
    now_str(BUILD_ENDED, sizeof BUILD_ENDED);
    { FILE *f = fopen(BUILD_LOG, "a");
      if (f) { fprintf(f, "\n----------------------------------------------------------------------\nended: %s\n", BUILD_ENDED); fclose(f); } }

    if (!built_ok) {
        char m[PATH_MAX]; snprintf(m, sizeof m, "full verbatim build log: %s", BUILD_LOG);
        printf("  %s%s%s\n", GREY, m, RESET);
        dieln("build failed — the exact messages above and the full log are retained.");
    }
    okln("build complete");
    { long total = count_lines(BUILD_LOG);
      char s[128]; snprintf(s, sizeof s, "%ssummary%s  log lines %ld", GREY, RESET, total);
      printf("    %s\n", s);
      char l[PATH_MAX]; snprintf(l, sizeof l, "%sverbatim log:%s %s%s%s", GREY, RESET, WHITE, BUILD_LOG, RESET);
      printf("  %s\n", l);
    }

    /* verify the products exist and run */
    for (int i = 0; products[i]; i++) {
        char bin[PATH_MAX]; snprintf(bin, sizeof bin, "%s/impl/build/%s", REPO, products[i]);
        if (access(bin, X_OK) != 0) { char e[PATH_MAX]; snprintf(e,sizeof e,"required product missing: %s", bin); dieln(e); }
    }

    if (build_only) {
        heading("Record of Build");
        printf("  %sCore products built (build-only); no install performed.%s\n", GREY, RESET);
        printf("    %s%-14s%s %s %s(syntax %s)\n", GREY, "Product", RESET, SL_PRODUCT " " SL_RELEASE, GREY, SL_SYNTAX);
        printf("    %s%-14s%s %s\n", GREY, "Host OS", RESET, host_os());
        printf("    %s%-14s%s sleela, sleelvac, nordshrift\n", GREY, "Binaries", RESET);
        printf("    %s%-14s%s %s -> %s\n", GREY, "Build window", RESET, BUILD_STARTED, BUILD_ENDED);
        printf("    %s%-14s%s %s%s%s\n", GREY, "Verbatim log", RESET, WHITE, BUILD_LOG, RESET);
        printf("\n  %s%sSo attested, this build complete and in order.%s\n\n", GREEN, ITAL, RESET);
        return 0;
    }

    /* -------- stage + install the main package ---------------------------- */
    heading("Staging the main package");
    char stage[PATH_MAX];
    snprintf(stage, sizeof stage, "%s/build/native-builder/.stage", REPO);
    { char rm[PATH_MAX]; snprintf(rm, sizeof rm, "rm -rf '%s'", stage); run_quiet(rm); }
    { char mk[PATH_MAX]; snprintf(mk, sizeof mk,
        "mkdir -p '%s/bin' '%s/lib' '%s/config' '%s/tools' '%s/security'",
        stage, stage, stage, stage, stage); run_quiet(mk); }

    step("copying binaries");
    for (int i = 0; products[i]; i++) {
        char cp[PATH_MAX * 2];
        snprintf(cp, sizeof cp, "install -m 755 '%s/impl/build/%s' '%s/bin/%s'",
                 REPO, products[i], stage, products[i]);
        if (run_quiet(cp) != 0) dieln("failed to stage a binary");
        item(products[i]);
    }

    step("staging the standard library (excluding servers, IDEs, Wi-Fi, religious/moral)");
    {
        char libdir[PATH_MAX]; snprintf(libdir, sizeof libdir, "%s/lib", REPO);
        /* enumerate top-level lib/ packages */
        char listcmd[PATH_MAX]; snprintf(listcmd, sizeof listcmd,
            "find '%s' -mindepth 1 -maxdepth 1 -type d -printf '%%f\\n' | sort", libdir);
        FILE *p = popen(listcmd, "r");
        if (!p) dieln("could not enumerate the standard library");
        char name[256];
        while (fgets(name, sizeof name, p)) {
            name[strcspn(name, "\n")] = 0;
            if (name[0] == 0) continue;
            if (is_excluded(name)) { LIB_DROPPED++; continue; }
            char cp[PATH_MAX * 2];
            snprintf(cp, sizeof cp, "cp -a '%s/%s' '%s/lib/'", libdir, name, stage);
            if (run_quiet(cp) == 0) LIB_KEPT++;
        }
        pclose(p);
        /* inventory markdown travels with the library */
        char cpmd[PATH_MAX * 2];
        snprintf(cpmd, sizeof cpmd, "cp -a '%s'/lib/*.md '%s/lib/' 2>/dev/null || true", REPO, stage);
        run_quiet(cpmd);
    }
    { char m[128]; snprintf(m, sizeof m, "library staged: %d packages kept, %d excluded", LIB_KEPT, LIB_DROPPED); okln(m); }

    step("staging the integrity manifest and verifier");
    {
        char c[PATH_MAX * 2];
        snprintf(c, sizeof c, "cp -a '%s/tools/verify-before-execution.py' '%s/tools/' 2>/dev/null || true", REPO, stage); run_quiet(c);
        snprintf(c, sizeof c, "cp -a '%s/security/sha256-manifest.json' '%s/security/'", REPO, stage); run_quiet(c);
        snprintf(c, sizeof c, "cp -a '%s/SHEET.sheet' '%s/' 2>/dev/null || true", REPO, stage); run_quiet(c);
    }
    okln("manifest + verifier staged");

    /* startup.conf */
    {
        char conf[PATH_MAX]; snprintf(conf, sizeof conf, "%s/config/startup.conf", stage);
        FILE *f = fopen(conf, "w");
        if (f) {
            fprintf(f, "# Generated by slbuild (%s native builder/installer)\n", host_os());
            fprintf(f, "install.mode=user\ninstall.root=%s\npackage=main\n", prefix);
            fprintf(f, "servers=excluded\nides=excluded\nwifi=excluded\nhost.os=%s\n", host_os());
            fclose(f);
        }
    }

    heading("Installing");
    { char mk[PATH_MAX]; snprintf(mk, sizeof mk, "mkdir -p '%s'", prefix); run_quiet(mk); }
    { char cp[PATH_MAX * 2]; snprintf(cp, sizeof cp, "cp -a '%s/.' '%s/'", stage, prefix);
      if (run_quiet(cp) != 0) dieln("failed to copy the staged package into the install root"); }
    /* Verify the install actually materialized — do not trust the copy's exit
     * code alone (a sandboxed/overlay filesystem can report success yet not
     * persist). The installed sleela binary must exist and be executable. */
    { char chk[PATH_MAX]; snprintf(chk, sizeof chk, "%s/bin/sleela", prefix);
      if (access(chk, X_OK) != 0) {
        char e[PATH_MAX]; snprintf(e, sizeof e,
            "install did not materialize at %s (bin/sleela missing); choose a writable --prefix", prefix);
        dieln(e);
      } }
    { char m[PATH_MAX]; snprintf(m, sizeof m, "installed to %s%s%s", WHITE, prefix, RESET); okln(m); }

    /* preserve the verbatim build log alongside the install */
    char installed_log[PATH_MAX] = {0};
    {
        char mk[PATH_MAX]; snprintf(mk, sizeof mk, "mkdir -p '%s/logs'", prefix); run_quiet(mk);
        char cp[PATH_MAX * 2]; snprintf(cp, sizeof cp, "cp -a '%s' '%s/logs/'", BUILD_LOG, prefix);
        if (run_quiet(cp) == 0) {
            const char *base = strrchr(BUILD_LOG, '/'); base = base ? base + 1 : BUILD_LOG;
            snprintf(installed_log, sizeof installed_log, "%s/logs/%s", prefix, base);
        }
    }
    /* clean the stage */
    { char rm[PATH_MAX]; snprintf(rm, sizeof rm, "rm -rf '%s'", stage); run_quiet(rm); }

    heading("Installed");
    { char m[160]; snprintf(m, sizeof m, "%s %s main package is installed.", SL_PRODUCT, SL_RELEASE); okln(m); }
    printf("  %sBinaries:%s %s%s/bin%s  %s(sleela · sleelvac · nordshrift)%s\n",
           GREY, RESET, WHITE, prefix, RESET, GREY, RESET);
    printf("  %sTry:%s      %s%s/bin/sleela version%s\n", GREY, RESET, WHITE, prefix, RESET);

    /* -------- Record of Build -------------------------------------------- */
    heading("Record of Build");
    printf("  %sBe it recorded that the SLeeLa main package was built and installed%s\n", GREY, RESET);
    printf("  %sin good order on this %s host, as attested below.%s\n\n", GREY, host_os(), RESET);
    printf("    %s%-16s%s %s %s(syntax %s)%s\n", GREY, "Product", RESET, SL_PRODUCT " " SL_RELEASE, GREY, SL_SYNTAX, RESET);
    printf("    %s%-16s%s %s\n", GREY, "Host OS", RESET, host_os());
    printf("    %s%-16s%s sleela, sleelvac, nordshrift\n", GREY, "Binaries", RESET);
    { char m[96]; snprintf(m, sizeof m, "%d packages included \u00b7 %d excluded", LIB_KEPT, LIB_DROPPED);
      printf("    %s%-16s%s %s\n", GREY, "Library", RESET, m); }
    printf("    %s%-16s%s servers \u00b7 IDEs/editors \u00b7 Wi-Fi \u00b7 religious/moral/political\n", GREY, "Excluded", RESET);
    printf("    %s%-16s%s %s\n", GREY, "Destination", RESET, prefix);
    printf("    %s%-16s%s %s -> %s\n", GREY, "Build window", RESET, BUILD_STARTED, BUILD_ENDED);
    printf("    %s%-16s%s SHA-256 manifest honored (fail-closed)\n", GREY, "Integrity", RESET);
    printf("    %s%-16s%s %s%s%s\n", GREY, "Verbatim log", RESET, WHITE,
           installed_log[0] ? installed_log : BUILD_LOG, RESET);
    printf("\n  %s%sSo attested, this build complete and in order.%s\n", GREEN, ITAL, RESET);
    printf("  %s%s%s\n\n", DIM, SL_TAGLINE, RESET);
    return 0;
}
