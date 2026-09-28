/* ssh_server.c -- a model of the SSH transport/auth handshake, as a toolchain
 * input. It does NOT implement SSH cryptography (that needs a real crypto
 * stack); it models the RFC 4253/4252 message sequence -- version exchange,
 * KEXINIT/NEWKEYS, and a userauth loop that accepts "publickey" and rejects
 * "none"/"password" -- mirroring SshServer.sleela / SshServer.java here.
 *
 *   cc -std=c11 -O2 -Wall -Wextra ssh_server.c -o ssh_server && ./ssh_server
 */
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX_TRIES 3

static bool auth_method_accepted(const char *method) {
    return strcmp(method, "publickey") == 0;
}

/* Returns true when authentication succeeds within the attempt budget. */
static bool userauth(const char *method, int attempt) {
    if (attempt > MAX_TRIES) {
        printf("SSH_MSG_DISCONNECT: too many auth failures\n");
        return false;
    }
    if (auth_method_accepted(method)) {
        printf("SSH_MSG_USERAUTH_SUCCESS (method=%s)\n", method);
        return true;
    }
    printf("SSH_MSG_USERAUTH_FAILURE (method=%s, attempt=%d)\n", method, attempt);
    return false;
}

int main(void) {
    printf("== C SSH server model: transport + auth handshake ==\n");

    /* 1) Protocol version exchange (RFC 4253 sec 4.2) */
    printf("S: SSH-2.0-SleelaModel_0.1\n");

    /* 2) Key exchange banner (algorithms negotiated here in a real server) */
    printf("S: SSH_MSG_KEXINIT (kex=curve25519, host-key=ed25519)\n");
    printf("S: SSH_MSG_NEWKEYS\n");

    /* 3) Auth loop: client offers none, then password, then publickey. */
    const char *offered[] = {"none", "password", "publickey"};
    bool ok = false;
    int attempt = 1;
    for (size_t i = 0; i < sizeof(offered) / sizeof(offered[0]); i++) {
        ok = userauth(offered[i], attempt);
        attempt++;
        if (ok) break;
    }

    /* 4) Channel open only if authenticated. */
    if (ok) {
        printf("S: SSH_MSG_CHANNEL_OPEN_CONFIRMATION (session)\n");
        printf("S: shell channel ready\n");
    } else {
        printf("S: connection refused\n");
    }
    printf("== handshake complete ==\n");
    return 0;
}
