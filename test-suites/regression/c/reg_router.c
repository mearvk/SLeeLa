/*
 * reg_router.c -- regression suite for preferred-routers/preferred_router.c
 *
 * Contract (preferred-routers/preferred_router.h):
 *   - select(config, protocol, out) fills every field of `out` and returns 0, or
 *     returns -1 for a NULL `out`; a missing or unreadable config file selects the
 *     defaults (US, national_backbone, preference 5);
 *   - the protocol "HTTP/<digit>" selects a VM generation 1..9 with its route
 *     name and path; anything else is generation 0 ("Core", "/impl");
 *   - a configured role that scores below min_score is replaced by one that
 *     meets it; min_score is clamped to 1..5; text fields are truncated, never
 *     overflowed;
 *   - packet_policy(protocol, minimum) is true when the selected preference meets
 *     the minimum; the minimum is clamped to 1..5.
 *
 * Config files are written to a per-process scratch directory.
 */
#define _POSIX_C_SOURCE 200809L
#include "../../../preferred-routers/preferred_router.h"
#include "regression.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static char g_dir[512];

static const char *write_config(const char *name, const char *text) {
    static char path[640];
    FILE *f;
    snprintf(path, sizeof path, "%s/%s", g_dir, name);
    f = fopen(path, "w");
    if (f) { fputs(text, f); fclose(f); }
    return path;
}

static void t_defaults_without_a_config(void) {
    sleela_preferred_router r;
    unsetenv("SLEELA_PREFERRED_ROUTER_CONFIG");
    REG_EQ_I(0, sleela_preferred_router_select(NULL, "HTTP/6.0", &r));
    REG_EQ_S("US", r.country);
    REG_EQ_S("national_backbone", r.role);
    REG_EQ_I(5, r.routing_preference);
    REG_EQ_S("HTTP/6.0", r.protocol);
    REG_EQ_S("HTTP/6.0", r.server_surface);
    REG_CHECK(strstr(r.candidate, "US") != NULL);          /* a default candidate is composed */
    REG_EQ_I(0, sleela_preferred_router_select("/no/such/file", "HTTP/6.0", &r));
    REG_EQ_S("US", r.country);                             /* unreadable config: defaults, not an error */
}

static void t_null_output_is_refused(void) {
    REG_EQ_I(-1, sleela_preferred_router_select(NULL, "HTTP/6.0", NULL));
}

static void t_protocol_selects_generation(void) {
    sleela_preferred_router r;
    REG_EQ_I(0, sleela_preferred_router_select(NULL, "HTTP/6.0", &r));
    REG_EQ_I(6, r.vm_generation);
    REG_EQ_S("Director", r.vm_name);
    REG_EQ_S("/6", r.vm_path);
    REG_EQ_I(0, sleela_preferred_router_select(NULL, "HTTP/1.1", &r));
    REG_EQ_I(1, r.vm_generation);
    REG_EQ_S("Foundation", r.vm_name);
    REG_EQ_I(0, sleela_preferred_router_select(NULL, "HTTP/9.0", &r));
    REG_EQ_I(9, r.vm_generation);
    REG_EQ_S("/9", r.vm_path);
    REG_EQ_I(0, sleela_preferred_router_select(NULL, "HTTP/0.9", &r));   /* digit 0 is not a generation */
    REG_EQ_I(0, r.vm_generation);
    REG_EQ_S("Core", r.vm_name);
    REG_EQ_S("/impl", r.vm_path);
    REG_EQ_I(0, sleela_preferred_router_select(NULL, "FTP", &r));
    REG_EQ_I(0, r.vm_generation);
    REG_EQ_I(0, sleela_preferred_router_select(NULL, "HTTP/", &r));
    REG_EQ_I(0, r.vm_generation);
    REG_EQ_I(0, sleela_preferred_router_select(NULL, NULL, &r));         /* no protocol at all */
    REG_EQ_I(0, r.vm_generation);
    REG_EQ_S("HTTP", r.protocol);
    REG_EQ_S("server-edition", r.server_surface);
}

static void t_config_values_are_applied(void) {
    sleela_preferred_router r;
    const char *cfg = write_config("a.conf",
        "# a comment\n\ncountry=DE\npreferred_role=regional_carrier\nmin_score=4\ncandidate=Frankfurt anchor\n");
    REG_EQ_I(0, sleela_preferred_router_select(cfg, "HTTP/6.0", &r));
    REG_EQ_S("DE", r.country);
    REG_EQ_S("regional_carrier", r.role);
    REG_EQ_I(4, r.routing_preference);
    REG_EQ_S("Frankfurt anchor", r.candidate);
}

/* The config path may also come from the environment; an explicit path wins. */
static void t_environment_config(void) {
    sleela_preferred_router r;
    const char *cfg = write_config("env.conf", "country=FR\n");
    setenv("SLEELA_PREFERRED_ROUTER_CONFIG", cfg, 1);
    REG_EQ_I(0, sleela_preferred_router_select(NULL, "HTTP/6.0", &r));
    REG_EQ_S("FR", r.country);
    cfg = write_config("explicit.conf", "country=IT\n");
    REG_EQ_I(0, sleela_preferred_router_select(cfg, "HTTP/6.0", &r));
    REG_EQ_S("IT", r.country);
    unsetenv("SLEELA_PREFERRED_ROUTER_CONFIG");
}

static void t_weak_role_is_replaced_to_meet_min_score(void) {
    sleela_preferred_router r;
    const char *cfg = write_config("weak.conf", "preferred_role=some_hobby_network\nmin_score=4\n");
    REG_EQ_I(0, sleela_preferred_router_select(cfg, "HTTP/6.0", &r));
    REG_EQ_S("regional_carrier", r.role);                  /* score 1 < 4: upgraded to the lowest role that meets it */
    REG_EQ_I(4, r.routing_preference);
    cfg = write_config("weak5.conf", "preferred_role=regional_carrier\nmin_score=5\n");
    REG_EQ_I(0, sleela_preferred_router_select(cfg, "HTTP/6.0", &r));
    REG_EQ_S("national_backbone", r.role);
    REG_EQ_I(5, r.routing_preference);
    cfg = write_config("keep.conf", "preferred_role=some_hobby_network\nmin_score=1\n");
    REG_EQ_I(0, sleela_preferred_router_select(cfg, "HTTP/6.0", &r));
    REG_EQ_S("some_hobby_network", r.role);                /* it meets a minimum of 1: kept */
    REG_EQ_I(1, r.routing_preference);
}

static void t_min_score_is_clamped(void) {
    sleela_preferred_router r;
    const char *cfg = write_config("hi.conf", "preferred_role=national_ixp\nmin_score=99\n");
    REG_EQ_I(0, sleela_preferred_router_select(cfg, "HTTP/6.0", &r));
    REG_EQ_I(5, r.routing_preference);                     /* 99 -> 5; national_ixp scores 5 */
    REG_EQ_S("national_ixp", r.role);
    cfg = write_config("lo.conf", "preferred_role=some_hobby_network\nmin_score=-7\n");
    REG_EQ_I(0, sleela_preferred_router_select(cfg, "HTTP/6.0", &r));
    REG_EQ_S("some_hobby_network", r.role);                /* -7 -> 1: nothing to upgrade */
}

static void t_long_text_is_truncated_not_overflowed(void) {
    sleela_preferred_router r;
    char text[700];
    const char *cfg;
    memset(text, 0, sizeof text);
    strcpy(text, "country=");
    memset(text + strlen(text), 'C', 300);
    strcat(text, "\npreferred_role=");
    memset(text + strlen(text), 'R', 300);
    strcat(text, "\n");
    cfg = write_config("long.conf", text);
    REG_EQ_I(0, sleela_preferred_router_select(cfg, "HTTP/6.0", &r));
    REG_EQ_U(sizeof r.country - 1, strlen(r.country));    /* cut to the field, still terminated */
    REG_CHECK(strlen(r.role) < sizeof r.role);
    REG_CHECK(strlen(r.candidate) < sizeof r.candidate);
}

static void t_packet_policy(void) {
    unsetenv("SLEELA_PREFERRED_ROUTER_CONFIG");
    REG_EQ_I(1, sleela_preferred_router_packet_policy("HTTP/6.0", 1));
    REG_EQ_I(1, sleela_preferred_router_packet_policy("HTTP/6.0", 4));
    REG_EQ_I(1, sleela_preferred_router_packet_policy("HTTP/6.0", 5));   /* the default role scores 5 */
    REG_EQ_I(1, sleela_preferred_router_packet_policy("HTTP/6.0", 0));   /* clamped up to 1 */
    REG_EQ_I(1, sleela_preferred_router_packet_policy("HTTP/6.0", 99));  /* clamped down to 5 */
    REG_EQ_I(1, sleela_preferred_router_packet_policy("HTTP/6.0", -3));
    REG_EQ_I(1, sleela_preferred_router_packet_policy(NULL, 4));
}

static void t_policy_follows_the_configured_role(void) {
    const char *cfg = write_config("p.conf", "preferred_role=some_hobby_network\nmin_score=1\n");
    setenv("SLEELA_PREFERRED_ROUTER_CONFIG", cfg, 1);
    REG_EQ_I(1, sleela_preferred_router_packet_policy("HTTP/6.0", 1));   /* a score-1 role meets 1 ... */
    REG_EQ_I(0, sleela_preferred_router_packet_policy("HTTP/6.0", 2));   /* ... and not 2 */
    REG_EQ_I(0, sleela_preferred_router_packet_policy("HTTP/6.0", 5));
    unsetenv("SLEELA_PREFERRED_ROUTER_CONFIG");
}

int main(void) {
    char tmpl[] = "/tmp/reg_router_XXXXXX";
    const char *base = getenv("REG_TMPDIR");
    if (base && *base) {
        snprintf(g_dir, sizeof g_dir, "%s/router-%ld", base, (long)getpid());
        mkdir(g_dir, 0700);
    } else if (mkdtemp(tmpl)) {
        snprintf(g_dir, sizeof g_dir, "%s", tmpl);
    }
    printf("preferred router regression suite\n");
    REG_RUN(t_defaults_without_a_config);
    REG_RUN(t_null_output_is_refused);
    REG_RUN(t_protocol_selects_generation);
    REG_RUN(t_config_values_are_applied);
    REG_RUN(t_environment_config);
    REG_RUN(t_weak_role_is_replaced_to_meet_min_score);
    REG_RUN(t_min_score_is_clamped);
    REG_RUN(t_long_text_is_truncated_not_overflowed);
    REG_RUN(t_packet_policy);
    REG_RUN(t_policy_follows_the_configured_role);
    return reg_finish("router");
}
