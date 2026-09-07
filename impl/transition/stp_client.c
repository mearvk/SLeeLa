// ===========================================================================
// stp_client.c -- Sleela's STP-0001 client + safe-trim fallback recorder.
//
// The transition path (connect + handshake + request + ack) is compiled only
// when STP_HAVE_OPENSSL is set (link -lcrypto). The failure recorder is always
// compiled so safe-trim capture works even in a no-crypto build.
// ===========================================================================
#define _GNU_SOURCE 1
#include "stp_client.h"
#include "stp_wire.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <unistd.h>

// ---------- default pipe resolution ----------

const char* stp_default_pipe(void) {
    static char buf[512];
    const char* env = getenv("SLEELA_STP_PIPE");
    if (env && *env) { snprintf(buf, sizeof buf, "%s", env); return buf; }
    // Prefer /run/sleela if its parent is writable, else /tmp.
    if (access("/run/sleela", W_OK) == 0 || access("/run", W_OK) == 0) {
        snprintf(buf, sizeof buf, "/run/sleela/stp.sock");
    } else {
        snprintf(buf, sizeof buf, "/tmp/sleela-stp.sock");
    }
    return buf;
}

// ---------- failure recorder (always available) ----------

static void esc_json(const char* s, char* out, size_t out_sz) {
    size_t j = 0;
    for (size_t i = 0; s && s[i] && j + 2 < out_sz; i++) {
        char c = s[i];
        if (c == '"' || c == '\\') { out[j++] = '\\'; out[j++] = c; }
        else if (c == '\n') { out[j++] = ' '; }
        else out[j++] = c;
    }
    out[j] = '\0';
}

int stp_record_failure(const stp_config* cfg,
                       const char* program_id, const char* source_name,
                       const uint8_t parse_digest[32], const stp_mem_model* mm,
                       const char* reason, const char* detail, const char* transport) {
    const char* path = (cfg && cfg->fallback_path) ? cfg->fallback_path
                     : (getenv("SLEELA_STP_FALLBACK") ? getenv("SLEELA_STP_FALLBACK")
                                                       : "/tmp/sleela-stp-failures.jsonl");
    FILE* f = fopen(path, "a");
    if (!f) {
        fprintf(stderr, "[stp] WARN: cannot open fallback store %s (failure not persisted)\n", path);
        return -1;   // best effort; caller (safe-trim) continues regardless
    }
    char digest_hex[65] = {0};
    if (parse_digest) stp_hex_local(parse_digest, 32, digest_hex);

    char src_e[512], reason_e[128], detail_e[512], pid_e[128];
    esc_json(source_name ? source_name : "?", src_e, sizeof src_e);
    esc_json(reason ? reason : "OTHER", reason_e, sizeof reason_e);
    esc_json(detail ? detail : "", detail_e, sizeof detail_e);
    esc_json(program_id ? program_id : "", pid_e, sizeof pid_e);

    long long ts = (long long) time(NULL);
    fprintf(f,
        "{\"id\":%lld,\"ts\":%lld,\"program_id\":\"%s\",\"source_name\":\"%s\","
        "\"parse_digest\":\"%s\",\"reason\":\"%s\",\"detail\":\"%s\","
        "\"mm_globals\":%u,\"mm_functions\":%u,\"mm_code_len\":%u,"
        "\"mm_max_threads\":%u,\"mm_locks\":%u,\"mm_mailboxes\":%u,\"mm_est_heap\":%llu,"
        "\"transport\":\"%s\",\"status\":\"NEW\"}\n",
        ts, ts, pid_e, src_e, digest_hex, reason_e, detail_e,
        mm ? mm->globals : 0, mm ? mm->functions : 0, mm ? mm->code_len : 0,
        mm ? mm->max_threads : 0, mm ? mm->locks : 0, mm ? mm->mailboxes : 0,
        (unsigned long long)(mm ? mm->est_heap : 0),
        transport ? transport : "unknown");
    fclose(f);
    fprintf(stderr, "[stp] recorded failed transition to %s (reason=%s) for Admin review\n",
            path, reason ? reason : "OTHER");
    return 0;
}

// A tiny local hex (so the recorder needs no OpenSSL in a no-crypto build).
void stp_hex_local(const uint8_t* in, unsigned long n, char* out) {
    static const char* h = "0123456789abcdef";
    for (size_t i = 0; i < n; i++) { out[2*i] = h[in[i] >> 4]; out[2*i+1] = h[in[i] & 0xf]; }
    out[2*n] = '\0';
}

#ifndef STP_HAVE_OPENSSL
// ---------- no-crypto build: transition always falls back ----------
int stp_transition(const stp_config* cfg, const char* program_id, const char* source_name,
                   const uint8_t parse_digest[32], const stp_mem_model* mm, stp_outcome* out) {
    (void) cfg; (void) program_id; (void) source_name; (void) parse_digest; (void) mm;
    if (out) {
        memset(out, 0, sizeof *out);
        out->kind = STP_FALLBACK;
        snprintf(out->reason, sizeof out->reason, "UNREACHABLE");
        snprintf(out->detail, sizeof out->detail, "client built without crypto (STP_HAVE_OPENSSL off)");
    }
    return 0;
}
#else
// ============================================================================
//  Full crypto client
// ============================================================================
#include "stp_crypto.h"

#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <poll.h>

// ---- I/O helpers with timeout ----
static int io_write_all(int fd, const uint8_t* p, size_t n) {
    size_t off = 0;
    while (off < n) {
        ssize_t w = write(fd, p + off, n - off);
        if (w < 0) { if (errno == EINTR) continue; return -1; }
        off += (size_t) w;
    }
    return 0;
}
static int io_read_all(int fd, uint8_t* p, size_t n, int timeout_ms) {
    size_t off = 0;
    while (off < n) {
        struct pollfd pfd = { fd, POLLIN, 0 };
        int pr = poll(&pfd, 1, timeout_ms);
        if (pr == 0) return -2;             // timeout
        if (pr < 0) { if (errno == EINTR) continue; return -1; }
        ssize_t r = read(fd, p + off, n - off);
        if (r == 0) return -1;              // closed
        if (r < 0) { if (errno == EINTR) continue; return -1; }
        off += (size_t) r;
    }
    return 0;
}

// ---- connect (local UNIX or remote TCP) ----
static int stp_connect(const stp_config* cfg, char* transport_out, size_t tsz) {
    if (cfg && cfg->remote_host) {
        struct addrinfo hints, *res = NULL;
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_UNSPEC; hints.ai_socktype = SOCK_STREAM;
        char port[16]; snprintf(port, sizeof port, "%d", cfg->remote_port ? cfg->remote_port : 8443);
        if (getaddrinfo(cfg->remote_host, port, &hints, &res) != 0) return -1;
        int fd = -1;
        for (struct addrinfo* ai = res; ai; ai = ai->ai_next) {
            fd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
            if (fd < 0) continue;
            if (connect(fd, ai->ai_addr, ai->ai_addrlen) == 0) break;
            close(fd); fd = -1;
        }
        freeaddrinfo(res);
        snprintf(transport_out, tsz, "remote-tls");
        return fd;
    }
    const char* path = (cfg && cfg->pipe_path) ? cfg->pipe_path : stp_default_pipe();
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) return -1;
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof addr);
    addr.sun_family = AF_UNIX;
    snprintf(addr.sun_path, sizeof addr.sun_path, "%s", path);
    if (connect(fd, (struct sockaddr*) &addr, sizeof addr) != 0) { close(fd); return -1; }
    snprintf(transport_out, tsz, "local-pipe");
    return fd;
}

// ---- send/recv a raw frame ----
static int send_clear(int fd, int type, int flags, const uint8_t* body, uint32_t blen) {
    uint8_t hdr[STP_HEADER_LEN];
    stp_header(hdr, type, flags, blen);
    if (io_write_all(fd, hdr, STP_HEADER_LEN) < 0) return -1;
    if (blen && io_write_all(fd, body, blen) < 0) return -1;
    return 0;
}

// Session key state.
typedef struct {
    uint8_t k_send[32], k_recv[32], k_region[32];
    uint8_t salt_send[4], salt_recv[4];
    uint64_t ctr_send, ctr_recv;
} stp_session;

static void make_nonce(const uint8_t salt4[4], uint64_t counter, uint8_t nonce12[12]) {
    memcpy(nonce12, salt4, 4);
    for (int i = 0; i < 8; i++) nonce12[4 + i] = (uint8_t)(counter >> (56 - 8 * i)); // big-endian
}

static int send_sealed(int fd, stp_session* s, int type, int flags,
                       const uint8_t* pt, size_t pt_len) {
    uint32_t sealed_len = (uint32_t) pt_len + 16;
    uint8_t hdr[STP_HEADER_LEN];
    stp_header(hdr, type, flags | STP_F_ENCRYPTED, sealed_len);
    uint8_t nonce[12]; make_nonce(s->salt_send, s->ctr_send++, nonce);
    uint8_t* sealed = (uint8_t*) malloc(sealed_len);
    if (!sealed) return -1;
    size_t ol = 0;
    if (stp_aead_seal(s->k_send, nonce, hdr, STP_HEADER_LEN, pt, pt_len, sealed, &ol) != 0) {
        free(sealed); return -1;
    }
    int rc = io_write_all(fd, hdr, STP_HEADER_LEN);
    if (rc == 0) rc = io_write_all(fd, sealed, ol);
    free(sealed);
    return rc;
}

// Receive a sealed frame into *pt (caller frees). Returns type, or <0 on error
// (-2 = timeout). ERROR frames may arrive in the clear.
static int recv_sealed(int fd, stp_session* s, int timeout_ms,
                       uint8_t** pt, size_t* pt_len, int* is_error_clear) {
    *is_error_clear = 0;
    uint8_t hdr[STP_HEADER_LEN];
    int rr = io_read_all(fd, hdr, STP_HEADER_LEN, timeout_ms);
    if (rr != 0) return rr;
    int ver, type, flags; uint32_t blen;
    stp_parse_header(hdr, &ver, &type, &flags, &blen);
    uint8_t* body = (uint8_t*) malloc(blen ? blen : 1);
    if (!body) return -1;
    if (blen && io_read_all(fd, body, blen, timeout_ms) != 0) { free(body); return -1; }
    if (!(flags & STP_F_ENCRYPTED)) {
        if (type == STP_T_ERROR) { *pt = body; *pt_len = blen; *is_error_clear = 1; return type; }
        free(body); return -1;
    }
    uint8_t* out = (uint8_t*) malloc(blen ? blen : 1);
    size_t ol = 0;
    uint8_t nonce[12]; make_nonce(s->salt_recv, s->ctr_recv++, nonce);
    if (stp_aead_open(s->k_recv, nonce, hdr, STP_HEADER_LEN, body, blen, out, &ol) != 0) {
        free(body); free(out); return -3;   // tag failure
    }
    free(body);
    *pt = out; *pt_len = ol;
    return type;
}

// Re-encode a hello body without its trailing "sig" entry (matches Java).
static size_t strip_last_entry(const uint8_t* body, size_t len, uint8_t* out) {
    uint16_t n = stp_get_u16le(body);
    size_t p = 2, last = 2;
    for (uint16_t i = 0; i < n && p < len; i++) {
        last = p;
        uint8_t kl = body[p++]; p += kl;
        int vt = body[p++];
        if (vt == STP_VT_BYTES || vt == STP_VT_TEXT) { uint32_t vl = stp_get_u32le(body + p); p += 4 + vl; }
        else if (vt == STP_VT_U64) p += 8;
        else p += 1;
    }
    stp_put_u16le(out, (uint16_t)(n - 1));
    memcpy(out + 2, body + 2, last - 2);
    return 2 + (last - 2);
}

static int hkdf_label(const uint8_t* ikm, size_t ikm_len, const uint8_t* salt, size_t salt_len,
                      const char* info, uint8_t* out, size_t out_len) {
    return stp_hkdf(ikm, ikm_len, salt, salt_len,
                    (const uint8_t*) info, strlen(info), out, out_len);
}

int stp_transition(const stp_config* cfg, const char* program_id, const char* source_name,
                   const uint8_t parse_digest[32], const stp_mem_model* mm, stp_outcome* out) {
    memset(out, 0, sizeof *out);
    out->kind = STP_FALLBACK;
    int timeout = (cfg && cfg->timeout_ms) ? cfg->timeout_ms : 3000;
    char transport[32] = "unknown";

    int fd = stp_connect(cfg, transport, sizeof transport);
    if (fd < 0) {
        snprintf(out->reason, sizeof out->reason, "UNREACHABLE");
        snprintf(out->detail, sizeof out->detail, "no supervisor on the pipe");
        return 0;
    }

    stp_key* client_id = stp_ed25519_generate();
    stp_key* client_eph = stp_x25519_generate();
    stp_session s; memset(&s, 0, sizeof s);
    int result_set = 0;

    // ---- build + send HELLO_CLIENT (cleartext, signed) ----
    uint8_t cid_raw[32], ceph_raw[32], nonce_c[24];
    stp_key_raw_public(client_id, cid_raw);
    stp_key_raw_public(client_eph, ceph_raw);
    stp_random(nonce_c, sizeof nonce_c);

    stp_map ch; stp_map_init(&ch);
    stp_map_put_text (&ch, "proto", STP_PROTO);
    stp_map_put_bytes(&ch, "client_pub_ed25519", cid_raw, 32);
    stp_map_put_bytes(&ch, "client_eph_x25519", ceph_raw, 32);
    stp_map_put_bytes(&ch, "nonce_c", nonce_c, sizeof nonce_c);
    stp_map_put_text (&ch, "region_hint", "sleela");
    stp_buf ch_nosig; stp_buf_init(&ch_nosig);
    stp_map_encode(&ch, &ch_nosig);
    uint8_t sig[64];
    stp_ed25519_sign(client_id, ch_nosig.data, ch_nosig.len, sig);
    stp_map_put_bytes(&ch, "sig", sig, 64);
    stp_buf ch_body; stp_buf_init(&ch_body);
    stp_map_encode(&ch, &ch_body);

    if (send_clear(fd, STP_T_HELLO_CLIENT, 0, ch_body.data, (uint32_t) ch_body.len) < 0) {
        snprintf(out->reason, sizeof out->reason, "UNREACHABLE");
        snprintf(out->detail, sizeof out->detail, "hello send failed");
        goto cleanup;
    }

    // ---- receive HELLO_SERVER (cleartext, signed) ----
    uint8_t shdr[STP_HEADER_LEN];
    int rr = io_read_all(fd, shdr, STP_HEADER_LEN, timeout);
    if (rr != 0) {
        snprintf(out->reason, sizeof out->reason, rr == -2 ? "TIMEOUT" : "UNREACHABLE");
        snprintf(out->detail, sizeof out->detail, "no HELLO_SERVER");
        goto cleanup;
    }
    int sver, stype, sflags; uint32_t sblen;
    stp_parse_header(shdr, &sver, &stype, &sflags, &sblen);
    if (stype != STP_T_HELLO_SERVER) {
        snprintf(out->reason, sizeof out->reason, "CRYPTO_FAIL");
        snprintf(out->detail, sizeof out->detail, "expected HELLO_SERVER");
        goto cleanup;
    }
    uint8_t* sh = (uint8_t*) malloc(sblen);
    if (io_read_all(fd, sh, sblen, timeout) != 0) { free(sh); snprintf(out->reason, sizeof out->reason, "UNREACHABLE"); goto cleanup; }
    stp_view shv; stp_view_init(&shv, sh, sblen);

    const uint8_t* sid_raw; uint32_t sid_len;
    const uint8_t* seph_raw; uint32_t seph_len;
    const uint8_t* ssig; uint32_t ssig_len;
    if (!stp_view_bytes(&shv, "server_pub_ed25519", &sid_raw, &sid_len) || sid_len != 32 ||
        !stp_view_bytes(&shv, "server_eph_x25519", &seph_raw, &seph_len) || seph_len != 32 ||
        !stp_view_bytes(&shv, "sig", &ssig, &ssig_len) || ssig_len != 64) {
        free(sh); snprintf(out->reason, sizeof out->reason, "CRYPTO_FAIL");
        snprintf(out->detail, sizeof out->detail, "malformed HELLO_SERVER");
        goto cleanup;
    }

    // pin check
    if (cfg && cfg->sup_pubkey_hex && strlen(cfg->sup_pubkey_hex) == 64) {
        char got[65]; stp_hex(sid_raw, 32, got);
        if (strcasecmp(got, cfg->sup_pubkey_hex) != 0) {
            free(sh); snprintf(out->reason, sizeof out->reason, "CRYPTO_FAIL");
            snprintf(out->detail, sizeof out->detail, "supervisor key pin mismatch");
            goto cleanup;
        }
    }
    // verify server signature over (body minus sig)
    stp_key* server_id = stp_ed25519_from_raw_public(sid_raw);
    uint8_t* sh_nosig = (uint8_t*) malloc(sblen);
    size_t sh_nosig_len = strip_last_entry(sh, sblen, sh_nosig);
    if (!stp_ed25519_verify(server_id, sh_nosig, sh_nosig_len, ssig)) {
        free(sh); free(sh_nosig); stp_key_free(server_id);
        snprintf(out->reason, sizeof out->reason, "CRYPTO_FAIL");
        snprintf(out->detail, sizeof out->detail, "server signature invalid");
        goto cleanup;
    }
    free(sh_nosig);

    // ---- derive keys ----
    stp_key* server_eph = stp_x25519_from_raw_public(seph_raw);
    uint8_t ss[32];
    if (stp_x25519_agree(client_eph, server_eph, ss) != 0) {
        free(sh); stp_key_free(server_id); stp_key_free(server_eph);
        snprintf(out->reason, sizeof out->reason, "CRYPTO_FAIL");
        goto cleanup;
    }
    uint8_t th[32];
    { // transcript hash = SHA256(ch_body || sh)
      stp_buf t; stp_buf_init(&t);
      stp_buf_append(&t, ch_body.data, ch_body.len);
      stp_buf_append(&t, sh, sblen);
      stp_sha256(t.data, t.len, th);
      stp_buf_free(&t);
    }
    uint8_t c2s[32], s2c[32];
    hkdf_label(ss, 32, th, 32, "STP-0001|c2s", c2s, 32);
    hkdf_label(ss, 32, th, 32, "STP-0001|s2c", s2c, 32);
    hkdf_label(ss, 32, th, 32, "STP-0001|region", s.k_region, 32);
    hkdf_label(ss, 32, th, 32, "STP-0001|c2s-salt", s.salt_send, 4);
    hkdf_label(ss, 32, th, 32, "STP-0001|s2c-salt", s.salt_recv, 4);
    memcpy(s.k_send, c2s, 32);
    memcpy(s.k_recv, s2c, 32);
    free(sh); stp_key_free(server_id); stp_key_free(server_eph);

    // ---- send TRANSITION_REQ (sealed) ----
    stp_map rq; stp_map_init(&rq);
    stp_map_put_text (&rq, "program_id", program_id ? program_id : "");
    stp_map_put_text (&rq, "source_name", source_name ? source_name : "?");
    stp_map_put_bytes(&rq, "parse_digest", parse_digest, 32);
    stp_map_put_u64  (&rq, "mm_globals", mm->globals);
    stp_map_put_u64  (&rq, "mm_functions", mm->functions);
    stp_map_put_u64  (&rq, "mm_code_len", mm->code_len);
    stp_map_put_u64  (&rq, "mm_max_threads", mm->max_threads);
    stp_map_put_u64  (&rq, "mm_locks", mm->locks);
    stp_map_put_u64  (&rq, "mm_mailboxes", mm->mailboxes);
    stp_map_put_u64  (&rq, "mm_est_heap", mm->est_heap);
    stp_map_put_u64  (&rq, "sysdepth", (uint64_t) mm->sysdepth);
    stp_map_put_u64  (&rq, "degreemax", (uint64_t) mm->degreemax);
    stp_map_put_text (&rq, "want", "monitorability,admin,supervision");
    stp_buf rqb; stp_buf_init(&rqb);
    stp_map_encode(&rq, &rqb);
    int se = send_sealed(fd, &s, STP_T_TRANSITION_REQ, 0, rqb.data, rqb.len);
    stp_map_free(&rq); stp_buf_free(&rqb);
    if (se < 0) {
        snprintf(out->reason, sizeof out->reason, "CRYPTO_FAIL");
        snprintf(out->detail, sizeof out->detail, "request seal/send failed");
        goto cleanup;
    }

    // ---- receive ACK / DENY ----
    uint8_t* rpt = NULL; size_t rpt_len = 0; int is_clear = 0;
    int rtype = recv_sealed(fd, &s, timeout, &rpt, &rpt_len, &is_clear);
    if (rtype < 0) {
        snprintf(out->reason, sizeof out->reason,
                 rtype == -2 ? "TIMEOUT" : (rtype == -3 ? "CRYPTO_FAIL" : "UNREACHABLE"));
        snprintf(out->detail, sizeof out->detail, "no valid ACK/DENY");
        goto cleanup;
    }
    stp_view rv; stp_view_init(&rv, rpt, rpt_len);
    if (rtype == STP_T_ACK) {
        int parse_ok = 0; stp_view_bool(&rv, "parse_ok", &parse_ok);
        uint64_t region_id = 0; stp_view_u64(&rv, "region_id", &region_id);
        stp_view_text(&rv, "region_name", out->region_name, sizeof out->region_name);
        stp_view_text(&rv, "region_class", out->region_class, sizeof out->region_class);
        stp_view_text(&rv, "observer_endpoint", out->observer_endpoint, sizeof out->observer_endpoint);
        uint64_t th_g = 0; stp_view_u64(&rv, "threads", &th_g); out->granted_threads = (uint32_t) th_g;
        out->region_id = region_id;

        // verify the regioned, secured ack tag = HMAC(k_region, "ACK"||u64le(id)||parse_digest)
        const uint8_t* got_tag; uint32_t tag_len;
        int have_tag = stp_view_bytes(&rv, "ack_tag", &got_tag, &tag_len) && tag_len == 32;
        uint8_t msg[3 + 8 + 32]; size_t mlen = 0;
        memcpy(msg, "ACK", 3); mlen = 3;
        stp_put_u64le(msg + mlen, region_id); mlen += 8;
        memcpy(msg + mlen, parse_digest, 32); mlen += 32;
        uint8_t want_tag[32];
        stp_hmac_sha256(s.k_region, 32, msg, mlen, want_tag);
        out->ack_verified = have_tag && memcmp(got_tag, want_tag, 32) == 0;

        if (parse_ok && out->ack_verified) {
            out->kind = STP_ADMITTED;
            // heartbeat once, then close cleanly.
            uint8_t hb[16]; stp_map hbm; stp_map_init(&hbm);
            stp_map_put_u64(&hbm, "region_id", region_id);
            stp_buf hbb; stp_buf_init(&hbb); stp_map_encode(&hbm, &hbb);
            send_sealed(fd, &s, STP_T_HEARTBEAT, 0, hbb.data, hbb.len);
            stp_map_free(&hbm); stp_buf_free(&hbb);
            uint8_t* echo = NULL; size_t el = 0; int ec = 0;
            recv_sealed(fd, &s, timeout, &echo, &el, &ec); free(echo);
            stp_map clm; stp_map_init(&clm); stp_map_put_bool(&clm, "bye", 1);
            stp_buf clb; stp_buf_init(&clb); stp_map_encode(&clm, &clb);
            send_sealed(fd, &s, STP_T_CLOSE, STP_F_LAST, clb.data, clb.len);
            stp_map_free(&clm); stp_buf_free(&clb);
            (void) hb;
        } else {
            snprintf(out->reason, sizeof out->reason, "CRYPTO_FAIL");
            snprintf(out->detail, sizeof out->detail,
                     out->ack_verified ? "parse not ok" : "ack tag unverified");
        }
    } else if (rtype == STP_T_DENY) {
        stp_view_text(&rv, "reason", out->reason, sizeof out->reason);
        stp_view_text(&rv, "detail", out->detail, sizeof out->detail);
        if (!out->reason[0]) snprintf(out->reason, sizeof out->reason, "POLICY");
    } else if (rtype == STP_T_ERROR) {
        stp_view_text(&rv, "reason", out->reason, sizeof out->reason);
        if (!out->reason[0]) snprintf(out->reason, sizeof out->reason, "POLICY");
    }
    free(rpt);
    result_set = 1;

cleanup:
    (void) result_set;
    stp_map_free(&ch); stp_buf_free(&ch_nosig); stp_buf_free(&ch_body);
    stp_key_free(client_id); stp_key_free(client_eph);
    if (fd >= 0) close(fd);
    return 0;
}
#endif // STP_HAVE_OPENSSL
