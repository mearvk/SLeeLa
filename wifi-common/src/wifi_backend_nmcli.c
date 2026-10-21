#define _POSIX_C_SOURCE 200809L
#include "sleela_wifi/wifi_manager.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int contains_ci(const char *s, const char *needle) {
    size_t i, n;
    if (!s || !needle) return 0;
    n = strlen(needle);
    for (; *s; ++s) {
        for (i = 0; i < n && s[i] &&
             tolower((unsigned char)s[i]) == tolower((unsigned char)needle[i]); ++i) {}
        if (i == n) return 1;
    }
    return 0;
}
static wifi_security_t classify_security(const char *s) {
    if (!s || !*s || strcmp(s, "--") == 0) return WIFI_SECURITY_OPEN;
    if (contains_ci(s, "WEP")) return WIFI_SECURITY_WEP;
    if (contains_ci(s, "OWE")) return WIFI_SECURITY_OWE;
    if (contains_ci(s, "WPA3") && contains_ci(s, "EAP")) return WIFI_SECURITY_WPA3_ENTERPRISE;
    if (contains_ci(s, "WPA3") || contains_ci(s, "SAE")) return WIFI_SECURITY_WPA3_PERSONAL;
    if (contains_ci(s, "802.1X") || contains_ci(s, "EAP")) return WIFI_SECURITY_WPA2_ENTERPRISE;
    if (contains_ci(s, "WPA2")) return WIFI_SECURITY_WPA2_PERSONAL;
    if (contains_ci(s, "WPA")) return WIFI_SECURITY_WPA_PERSONAL;
    return WIFI_SECURITY_UNKNOWN;
}
/* Decode nmcli's escaped colon-delimited format without shell interpolation. */
static int split_fields(char *line, char fields[][256], int expected) {
    int f = 0;
    size_t n = 0u;
    char *p;
    memset(fields, 0, (size_t)expected * 256u);
    for (p = line; *p && f < expected; ++p) {
        if (*p == '\\' && p[1]) {
            ++p;
            fields[f][n++] = (*p == 'n' || *p == 'r' || *p == 't') ? ' ' : *p;
        } else if (*p == ':' && f + 1 < expected) {
            fields[f][n] = '\0'; ++f; n = 0u;
        } else if (*p != '\n' && *p != '\r' && n + 1u < 256u) {
            fields[f][n++] = *p;
        }
    }
    fields[f][n] = '\0';
    return f + 1;
}
static unsigned parse_unsigned(const char *s, unsigned max) {
    char *end = NULL;
    unsigned long v;
    if (!s || !*s) return 0u;
    v = strtoul(s, &end, 10);
    return (!end || *end || v > max) ? 0u : (unsigned)v;
}
static wifi_status_t nmcli_scan(void *context, wifi_network_t *out, size_t capacity, size_t *out_count) {
    const char *cmd = "nmcli --terse --escape yes --fields SSID,BSSID,CHAN,SIGNAL,SECURITY,IN-USE device wifi list --rescan yes";
    FILE *pipe;
    char *line = NULL;
    size_t line_cap = 0u, count = 0u;
    ssize_t len;
    int rc;
    (void)context;
    if (!out_count || (!out && capacity)) return WIFI_STATUS_INVALID_ARGUMENT;
    *out_count = 0u;
    pipe = popen(cmd, "r"); /* constant command; no SSID or credential enters a shell */
    if (!pipe) return WIFI_STATUS_BACKEND_UNAVAILABLE;
    while ((len = getline(&line, &line_cap, pipe)) >= 0) {
        char fields[6][256];
        wifi_network_t n;
        (void)len;
        if (split_fields(line, fields, 6) != 6 || !fields[0][0] || !fields[1][0]) continue;
        if (strlen(fields[0]) > WIFI_SSID_MAX_BYTES || strlen(fields[1]) >= WIFI_BSSID_TEXT_SIZE) continue;
        memset(&n, 0, sizeof(n));
        (void)snprintf(n.ssid, sizeof(n.ssid), "%s", fields[0]);
        (void)snprintf(n.bssid, sizeof(n.bssid), "%s", fields[1]);
        n.channel = parse_unsigned(fields[2], 233u);
        n.signal_percent = parse_unsigned(fields[3], 100u);
        n.security = classify_security(fields[4]);
        n.in_use = strcmp(fields[5], "*") == 0;
        n.protected_management_frames = -1;
        if (count < capacity) out[count] = n;
        ++count;
    }
    free(line);
    rc = pclose(pipe);
    if (rc == -1 || !WIFEXITED(rc) || WEXITSTATUS(rc) != 0) return WIFI_STATUS_BACKEND_UNAVAILABLE;
    *out_count = count;
    return count > capacity ? WIFI_STATUS_BUFFER_TOO_SMALL : WIFI_STATUS_OK;
}
static int run_argv(char *const argv[]) {
    pid_t pid = fork();
    int status = 0;
    if (pid < 0) return -1;
    if (pid == 0) { execvp(argv[0], argv); _exit(127); }
    if (waitpid(pid, &status, 0) < 0) return -1;
    return WIFEXITED(status) && WEXITSTATUS(status) == 0 ? 0 : -1;
}
static wifi_status_t nmcli_connect(void *context, const wifi_network_t *network, const wifi_auth_t *auth) {
    char *argv[12];
    int i = 0;
    (void)context;
    if (!network || !auth) return WIFI_STATUS_INVALID_ARGUMENT;
    /* Never put a PSK/EAP password in argv. Use the trusted secret agent or a saved profile. */
    if (auth->mode == WIFI_AUTH_PASSPHRASE || auth->mode == WIFI_AUTH_ENTERPRISE_PASSWORD)
        return WIFI_STATUS_AUTH_REQUIRED;
    argv[i++] = (char *)"nmcli"; argv[i++] = (char *)"--wait"; argv[i++] = (char *)"30";
    if (auth->mode == WIFI_AUTH_SAVED_PROFILE) {
        argv[i++] = (char *)"connection"; argv[i++] = (char *)"up";
        argv[i++] = (char *)"id"; argv[i++] = (char *)auth->profile_name;
    } else {
        if (isatty(STDIN_FILENO)) argv[i++] = (char *)"--ask";
        argv[i++] = (char *)"device"; argv[i++] = (char *)"wifi"; argv[i++] = (char *)"connect";
        argv[i++] = (char *)network->ssid; argv[i++] = (char *)"bssid"; argv[i++] = (char *)network->bssid;
    }
    argv[i] = NULL;
    return run_argv(argv) == 0 ? WIFI_STATUS_OK : WIFI_STATUS_BACKEND_ERROR;
}
static wifi_status_t nmcli_disconnect(void *context) {
    FILE *pipe;
    char *line = NULL, device[128] = {0};
    size_t cap = 0u;
    ssize_t n;
    (void)context;
    pipe = popen("nmcli --terse --fields DEVICE,TYPE,STATE device status", "r");
    if (!pipe) return WIFI_STATUS_BACKEND_UNAVAILABLE;
    while ((n = getline(&line, &cap, pipe)) >= 0) {
        char *first = line, *second, *third;
        (void)n;
        second = strchr(first, ':'); if (!second) continue; *second++ = '\0';
        third = strchr(second, ':'); if (!third) continue; *third++ = '\0';
        if (strcmp(second, "wifi") == 0 && strncmp(third, "connected", 9u) == 0) {
            (void)snprintf(device, sizeof(device), "%s", first); break;
        }
    }
    free(line); (void)pclose(pipe);
    if (!device[0]) return WIFI_STATUS_NOT_CONNECTED;
    {
        char *argv[] = { (char *)"nmcli", (char *)"--wait", (char *)"15",
                         (char *)"device", (char *)"disconnect", (char *)"ifname", device, NULL };
        return run_argv(argv) == 0 ? WIFI_STATUS_OK : WIFI_STATUS_BACKEND_ERROR;
    }
}
static const wifi_backend_t NMCLI_BACKEND = {
    "NetworkManager/nmcli (OS-managed secrets)", nmcli_scan, nmcli_connect, nmcli_disconnect
};
const wifi_backend_t *wifi_backend_default(void) { return &NMCLI_BACKEND; }
