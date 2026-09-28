/* ==========================================================================
 * station_abi.h -- shared ABI between the SLeeLa HTTP 3.0 Station kernel driver
 * (http-3.0/kernel/sleela_station.ko) and userland (http-3.0/station/).
 *
 * The Station is a Router function that WITNESSES a two-party negotiation over
 * TCP/IP control semantics. Clients do not (yet) exchange payload; they signal
 * INTENT using TCP control flags (URG/ACK/SYN/...) plus a HOP distance encoding
 * (DIST+0 / DIST+1). The Station classifies each signal into a CONDITION,
 * advances a three-phase premise (start / intermediary / cancelling), and
 * records the negotiation, including the terminal states UNCLEAR, HUNG-UP, and
 * DISASTROUS.
 *
 * This header is the single source of truth for the numbers on the wire and in
 * the ioctl interface, so the kernel module and userland agree exactly.
 * ========================================================================== */
#ifndef SLEELA_STATION_ABI_H
#define SLEELA_STATION_ABI_H

#ifdef __KERNEL__
#include <linux/types.h>
#include <linux/ioctl.h>
#else
#include <stdint.h>
#include <sys/ioctl.h>
#endif

#define SLEELA_STATION_ABI_VERSION 1u

/* Character device the driver exposes; userland opens /dev/<name>. */
#define SLEELA_STATION_DEVICE "sleela_station"
#define SLEELA_STATION_DEVPATH "/dev/" SLEELA_STATION_DEVICE

/* ----------------------------------------------------------------------------
 * TCP control-flag vocabulary (RFC 793 bit positions). The Station reads these
 * from the TCP header of signaling segments; combinations carry meaning.
 * -------------------------------------------------------------------------- */
#define SL_TCP_FIN 0x01u
#define SL_TCP_SYN 0x02u
#define SL_TCP_RST 0x04u
#define SL_TCP_PSH 0x08u
#define SL_TCP_ACK 0x10u
#define SL_TCP_URG 0x20u
#define SL_TCP_ECE 0x40u
#define SL_TCP_CWR 0x80u

/* The negotiation uses the URGENT pointer count as a small signal integer.
 * "8 URG" (SL_STATION_URG_MAX) is the full-scale signal that selects a premise
 * bank; fewer URG units select within a bank. */
#define SL_STATION_URG_MAX 8u

/* ----------------------------------------------------------------------------
 * HOP distance encoding. DIST is the Station's own hop index on the path.
 * A signaling segment carries a hop field; its relation to DIST is the verb:
 *   DIST + 0  -> HOLD  : witness/record at the current station (no advance)
 *   DIST + 1  -> ADVANCE: move the premise forward one phase
 *   DIST - 1  -> CANCEL : withdraw/roll back the current premise
 * -------------------------------------------------------------------------- */
typedef enum {
    SL_HOP_CANCEL  = -1, /* DIST-1 */
    SL_HOP_HOLD    =  0, /* DIST+0 */
    SL_HOP_ADVANCE =  1  /* DIST+1 */
} sl_hop_verb_t;

/* ----------------------------------------------------------------------------
 * Conditions the Station classifies a signal into (the "Intentional Condition,
 * Legal Condition, Warning Condition, etc." of the design).
 * -------------------------------------------------------------------------- */
typedef enum {
    SL_COND_NONE        = 0,
    SL_COND_INTENTIONAL = 1, /* a deliberate premise offer/step            */
    SL_COND_LEGAL       = 2, /* a legally-framed assertion (carries dressing)*/
    SL_COND_WARNING     = 3, /* a caution; proceed only with acknowledgement */
    SL_COND_EMERGENCY   = 4  /* time-critical; expedited witnessing         */
} sl_condition_t;

/* ----------------------------------------------------------------------------
 * The three-phase premise of a negotiation turn.
 * -------------------------------------------------------------------------- */
typedef enum {
    SL_PREMISE_START        = 0, /* opening premise (what I propose to send) */
    SL_PREMISE_INTERMEDIARY = 1, /* mediated/counter premise                 */
    SL_PREMISE_CANCELLING   = 2  /* withdrawal of a premise                  */
} sl_premise_t;

/* ----------------------------------------------------------------------------
 * Negotiation outcome / terminal state as witnessed by the Station.
 * -------------------------------------------------------------------------- */
typedef enum {
    SL_STATE_IDLE       = 0,
    SL_STATE_OPEN       = 1, /* negotiation in progress                     */
    SL_STATE_CLEAR      = 2, /* both parties agreed on what to send         */
    SL_STATE_UNCLEAR    = 3, /* ended without a definite agreement          */
    SL_STATE_HUNG_UP    = 4, /* a party stopped responding                  */
    SL_STATE_DISASTROUS = 5  /* RST/abort or contradictory/illegal premises */
} sl_state_t;

/* Which side of the two-party negotiation a signal came from. */
typedef enum { SL_PARTY_A = 0, SL_PARTY_B = 1 } sl_party_t;

/* ----------------------------------------------------------------------------
 * A single signaling event as seen on the wire (what the Netfilter hook feeds
 * the classifier). This is the raw evidence before classification.
 * -------------------------------------------------------------------------- */
typedef struct {
    uint32_t abi_version;   /* = SLEELA_STATION_ABI_VERSION                  */
    uint64_t session_id;    /* negotiation this signal belongs to            */
    uint8_t  party;         /* sl_party_t                                    */
    uint8_t  tcp_flags;     /* OR of SL_TCP_* bits                           */
    uint8_t  urg_units;     /* URGENT signal magnitude, 0..SL_STATION_URG_MAX*/
    int8_t   hop_delta;     /* hop - DIST, i.e. sl_hop_verb_t                */
    uint64_t timestamp_ns;  /* Station clock at observation                  */
} sl_signal_t;

/* ----------------------------------------------------------------------------
 * The classifier's verdict for one signal (condition + premise + verb), and a
 * witnessed record entry the Station stores.
 * -------------------------------------------------------------------------- */
typedef struct {
    uint64_t       session_id;
    uint8_t        party;      /* sl_party_t                                 */
    uint8_t        condition;  /* sl_condition_t                             */
    uint8_t        premise;    /* sl_premise_t                               */
    int8_t         hop_verb;   /* sl_hop_verb_t                              */
    uint64_t       timestamp_ns;
    uint32_t       sequence;   /* monotonic index within the session         */
} sl_record_t;

/* Snapshot of a session's witnessed state, returned by the query ioctl. */
typedef struct {
    uint64_t session_id;
    uint8_t  state;            /* sl_state_t                                 */
    uint8_t  last_premise_a;   /* sl_premise_t last seen from A              */
    uint8_t  last_premise_b;   /* sl_premise_t last seen from B              */
    uint8_t  reserved;
    uint32_t record_count;     /* number of witnessed records                */
    uint64_t opened_ns;
    uint64_t updated_ns;
} sl_session_view_t;

/* ----------------------------------------------------------------------------
 * ioctl interface (the "known API" for direct calls into the driver).
 * -------------------------------------------------------------------------- */
#define SL_IOC_MAGIC 0xB7

/* Open/register a new witnessed session; kernel fills session_id. */
#define SL_IOC_OPEN     _IOWR(SL_IOC_MAGIC, 1, sl_session_view_t)
/* Feed one signaling event; kernel returns the classified record. */
#define SL_IOC_SIGNAL   _IOWR(SL_IOC_MAGIC, 2, sl_signal_t)
/* Query a session's current witnessed view. */
#define SL_IOC_QUERY    _IOWR(SL_IOC_MAGIC, 3, sl_session_view_t)
/* Close a session, sealing its stored record. */
#define SL_IOC_CLOSE    _IOW (SL_IOC_MAGIC, 4, uint64_t)

#endif /* SLEELA_STATION_ABI_H */
