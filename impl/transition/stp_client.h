// ===========================================================================
// stp_client.h -- Sleela's client for the Secure Transition Protocol (STP-0001).
//
// Opens the secured pipe to the SecureJDK 28 Transition Supervisor (local UNIX
// socket by default, or a remote host:port), performs the crypto handshake,
// submits the program's memory model + parse digest for a regioned, secured
// acknowledgement, and reports the outcome. On any failure the caller continues
// LOCALLY as a safe trim and records the failure for Admin review.
// ===========================================================================
#ifndef SLEELA_STP_CLIENT_H
#define SLEELA_STP_CLIENT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// The Sleela memory-model summary submitted for supervision (STP-0001 §5).
typedef struct {
    uint32_t globals;
    uint32_t functions;
    uint32_t code_len;
    uint32_t max_threads;
    uint32_t locks;
    uint32_t mailboxes;
    uint64_t est_heap;
    int64_t  sysdepth;      // Constitution
    int64_t  degreemax;
} stp_mem_model;

// Where to connect.
typedef struct {
    const char* pipe_path;   // local UNIX socket; NULL -> default resolution
    const char* remote_host; // if non-NULL, use remote host:port (plain TCP here;
    int         remote_port; //   TLS wrapping handled by the Java side / stunnel)
    int         timeout_ms;  // 0 -> STP default (3000)
    const char* sup_pubkey_hex;  // pinned supervisor Ed25519 (hex, 64 chars); NULL -> unpinned
    const char* fallback_path;   // JSONL fallback for failures; NULL -> default
} stp_config;

// The outcome of a transition attempt.
typedef enum {
    STP_ADMITTED = 0,   // supervised: proceed under the granted region
    STP_FALLBACK        // failed: continue as local safe trim (reason set below)
} stp_outcome_kind;

typedef struct {
    stp_outcome_kind kind;
    char   region_name[128];    // memorable region (when ADMITTED)
    char   region_class[64];    // SecureJDK grade
    uint64_t region_id;
    uint32_t granted_threads;   // supervised thread budget (when ADMITTED)
    char   observer_endpoint[128];
    int    ack_verified;        // region ACK tag verified against k_region
    char   reason[64];          // failure reason (when FALLBACK): DENY code / TIMEOUT / ...
    char   detail[256];
} stp_outcome;

// Attempt the transition. Never blocks longer than timeout_ms on the ack.
// Returns 0 always (the outcome distinguishes ADMITTED vs FALLBACK); a negative
// return means the client could not even be initialized (e.g. no crypto).
int stp_transition(const stp_config* cfg,
                   const char* program_id,
                   const char* source_name,
                   const uint8_t parse_digest[32],
                   const stp_mem_model* mm,
                   stp_outcome* out);

// Record a failed transition to the private store for later Admin review.
// Best-effort: writes a JSONL line the Java Admin/importer can ingest into the
// secured MySQL. Never fails the caller (STP-0001 §6.3). Returns 0 on write.
int stp_record_failure(const stp_config* cfg,
                       const char* program_id,
                       const char* source_name,
                       const uint8_t parse_digest[32],
                       const stp_mem_model* mm,
                       const char* reason,
                       const char* detail,
                       const char* transport);

// Resolve the default local pipe path (honors $SLEELA_STP_PIPE, then
// /run/sleela/stp.sock, then /tmp/sleela-stp.sock). Returns a static buffer.
const char* stp_default_pipe(void);

// The safe-trim thread cap applied when unsupervised (STP-0001 §6.2).
#define STP_SAFE_TRIM_MAX_THREADS 16

// Local hex encoder (no crypto dependency) used by the failure recorder.
void stp_hex_local(const uint8_t* in, unsigned long n, char* out /* 2n+1 */);

#ifdef __cplusplus
}
#endif
#endif // SLEELA_STP_CLIENT_H
