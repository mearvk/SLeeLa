/* ==========================================================================
 * sleela_bestof.c -- Best-of selection engine (transparent, honest).
 *
 * Scoring is deliberately simple and inspectable. For each candidate we keep a
 * running measured summary (mean RTT, jitter, loss) and derive a carrier
 * certainty in [0..1000] permille the same spirit as QOS.md: a clean, on-time,
 * low-jitter reply raises certainty; a loss or a large deviation lowers it.
 *
 * The final score blends four weighted, normalized terms:
 *   data      : lower mean RTT + lower loss + lower jitter is better
 *   decisions : higher carrier certainty is better
 *   costs     : lower cost is better (subtracted)
 *   versions  : higher version fitness (>= min_version) is better
 * A candidate with no measurements is "unproven": it scores from its static
 * fitness only, with a certainty of 0, so a measured-good route beats an
 * unmeasured one. Nothing is assumed good.
 * ========================================================================== */
#include "sleela_bestof.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SL_BESTOF_NAME 64
#define SL_BESTOF_ROUTE 192
/* Reference ceilings used only to normalize measured values into [0..1000].
 * They are scaling constants, not guarantees or limits on the transport. */
#define SL_BESTOF_RTT_CEIL_US   1000000L  /* 1s: RTT at/above this scores ~0   */
#define SL_BESTOF_JITTER_CEIL_US 500000L  /* 0.5s jitter reference             */
#define SL_BESTOF_SCORE_MIN  (-1000000000LL)

typedef struct {
    int used;
    char name[SL_BESTOF_NAME];
    char route[SL_BESTOF_ROUTE];
    int timeout_ms, payload_len, min_gap_ms;
    int flags, version, cost, replays;
    /* internet QoS architecture (SL_BESTOF_ARCH_*) + its parameter + state */
    int arch, arch_param, arch_state;
    /* measured accumulators */
    int64_t sent, received;
    int64_t sum_us;          /* sum of matched RTTs (us)          */
    int64_t last_us;         /* previous matched RTT for jitter    */
    int64_t sum_jitter_us;   /* sum of |rtt - last| across matches */
    int64_t jitter_count;
} SLBestCandidate;

struct SLBestOf {
    int w_data, w_decisions, w_costs, w_versions, w_architecture;
    int min_version;
    int cost_budget;   /* 0 == unbounded */
    SLBestCandidate c[SL_BESTOF_CANDIDATES];
    int ncandidates;
};

SLBestOf* slbestof_new(void) {
    SLBestOf* b = (SLBestOf*)calloc(1, sizeof(SLBestOf));
    if (!b) return NULL;
    b->w_data = 45;
    b->w_decisions = 25;
    b->w_costs = 10;
    b->w_versions = 5;
    b->w_architecture = 15;
    b->min_version = 0;
    b->cost_budget = 0;
    return b;
}

int slbestof_weight(SLBestOf* b, int axis, int weight) {
    if (!b) return -1;
    if (weight < 0) weight = 0;
    if (weight > 100) weight = 100;
    switch (axis) {
        case SL_BESTOF_AXIS_DATA:         b->w_data = weight; return 0;
        case SL_BESTOF_AXIS_DECISIONS:    b->w_decisions = weight; return 0;
        case SL_BESTOF_AXIS_COSTS:        b->w_costs = weight; return 0;
        case SL_BESTOF_AXIS_VERSIONS:     b->w_versions = weight; return 0;
        case SL_BESTOF_AXIS_ARCHITECTURE: b->w_architecture = weight; return 0;
        default: return -1;
    }
}

void slbestof_min_version(SLBestOf* b, int min_version) {
    if (b) b->min_version = min_version;
}
void slbestof_cost_budget(SLBestOf* b, int cost_budget) {
    if (b) b->cost_budget = cost_budget < 0 ? 0 : cost_budget;
}

int slbestof_add_candidate(SLBestOf* b, const char* name, const char* route,
                           int timeout_ms, int payload_len, int min_gap_ms,
                           int flags, int version, int cost, int replays) {
    if (!b || b->ncandidates >= SL_BESTOF_CANDIDATES) return -1;
    SLBestCandidate* cn = &b->c[b->ncandidates];
    memset(cn, 0, sizeof(*cn));
    cn->used = 1;
    snprintf(cn->name, sizeof(cn->name), "%s", name ? name : "candidate");
    snprintf(cn->route, sizeof(cn->route), "%s", route ? route : "");
    cn->timeout_ms = timeout_ms;
    cn->payload_len = payload_len;
    cn->min_gap_ms = min_gap_ms;
    cn->flags = flags;
    cn->version = version;
    cn->cost = cost < 0 ? 0 : cost;
    cn->replays = replays < 1 ? 1 : replays;
    cn->last_us = -1;
    /* Default architecture: best-effort, requested (memset already zeroed). */
    cn->arch = SL_BESTOF_ARCH_BESTEFFORT;
    cn->arch_param = 0;
    cn->arch_state = SL_BESTOF_ARCH_REQUESTED;
    return b->ncandidates++;
}

static int valid_idx(const SLBestOf* b, int idx) {
    return b && idx >= 0 && idx < b->ncandidates && b->c[idx].used;
}

int slbestof_candidate_arch(SLBestOf* b, int idx, int architecture,
                            int arch_param, int realized) {
    if (!valid_idx(b, idx)) return -1;
    if (architecture < SL_BESTOF_ARCH_BESTEFFORT || architecture > SL_BESTOF_ARCH_MPLS)
        return -1;
    SLBestCandidate* cn = &b->c[idx];
    cn->arch = architecture;
    cn->arch_param = arch_param;
    cn->arch_state = (realized < SL_BESTOF_ARCH_REQUESTED || realized > SL_BESTOF_ARCH_DENIED)
                     ? SL_BESTOF_ARCH_REQUESTED : realized;
    return 0;
}

int slbestof_arch_realized(SLBestOf* b, int idx, int realized) {
    if (!valid_idx(b, idx)) return -1;
    if (realized < SL_BESTOF_ARCH_REQUESTED || realized > SL_BESTOF_ARCH_DENIED) return -1;
    b->c[idx].arch_state = realized;
    return 0;
}

int slbestof_arch(const SLBestOf* b, int idx) {
    return valid_idx(b, idx) ? b->c[idx].arch : -1;
}
int slbestof_arch_param(const SLBestOf* b, int idx) {
    return valid_idx(b, idx) ? b->c[idx].arch_param : -1;
}
int slbestof_arch_state(const SLBestOf* b, int idx) {
    return valid_idx(b, idx) ? b->c[idx].arch_state : -1;
}

/* The architecture-fitness term in [0..1000]: how much the candidate's QoS
 * architecture, as actually realized, should raise its score. Best-effort is a
 * neutral baseline; a REALIZED DiffServ/IntServ/MPLS path earns a bonus scaled
 * by how strong that architecture's guarantee is; a merely REQUESTED path earns
 * a small credit; a DENIED reservation is penalized below best-effort. Honest:
 * the bonus is gated on realization, never on the mere request. */
static int64_t arch_fitness(const SLBestCandidate* cn) {
    int64_t base;
    switch (cn->arch) {
        case SL_BESTOF_ARCH_INTSERV: base = 1000; break; /* end-to-end reservation */
        case SL_BESTOF_ARCH_MPLS:    base = 800;  break; /* traffic-engineered path */
        case SL_BESTOF_ARCH_DIFFSERV:base = 600;  break; /* per-hop class marking   */
        default:                     base = 400;  break; /* best-effort baseline    */
    }
    if (cn->arch == SL_BESTOF_ARCH_BESTEFFORT) return base; /* state N/A */
    switch (cn->arch_state) {
        case SL_BESTOF_ARCH_REALIZED:  return base;              /* full credit */
        case SL_BESTOF_ARCH_DENIED:    return 200;               /* below best-effort */
        default: /* REQUESTED */       return 400 + (base - 400) / 4; /* partial */
    }
}

int slbestof_record(SLBestOf* b, int idx, int64_t rtt_us) {
    if (!valid_idx(b, idx)) return -1;
    SLBestCandidate* cn = &b->c[idx];
    cn->sent++;
    if (rtt_us < 0) return 0;               /* honest loss: counted, no RTT   */
    cn->received++;
    cn->sum_us += rtt_us;
    if (cn->last_us >= 0) {
        int64_t d = rtt_us - cn->last_us;
        if (d < 0) d = -d;
        cn->sum_jitter_us += d;
        cn->jitter_count++;
    }
    cn->last_us = rtt_us;
    return 0;
}

int64_t slbestof_mean_us(const SLBestOf* b, int idx) {
    if (!valid_idx(b, idx)) return -1;
    const SLBestCandidate* cn = &b->c[idx];
    return cn->received > 0 ? cn->sum_us / cn->received : -1;
}

int64_t slbestof_loss_permille(const SLBestOf* b, int idx) {
    if (!valid_idx(b, idx)) return -1;
    const SLBestCandidate* cn = &b->c[idx];
    if (cn->sent == 0) return 0;
    int64_t lost = cn->sent - cn->received;
    if (lost < 0) lost = 0;
    return (lost * 1000) / cn->sent;
}

int64_t slbestof_jitter_us(const SLBestOf* b, int idx) {
    if (!valid_idx(b, idx)) return -1;
    const SLBestCandidate* cn = &b->c[idx];
    return cn->jitter_count > 0 ? cn->sum_jitter_us / cn->jitter_count : 0;
}

/* Certainty in [0..1000]: driven by delivered fraction, dampened by jitter.
 * No measurement -> 0 (unproven), consistent with "never assumed good". */
int slbestof_certainty_permille(const SLBestOf* b, int idx) {
    if (!valid_idx(b, idx)) return 0;
    const SLBestCandidate* cn = &b->c[idx];
    if (cn->sent == 0) return 0;
    int64_t delivered = (cn->received * 1000) / cn->sent;   /* 0..1000 */
    int64_t jitter = slbestof_jitter_us(b, idx);
    /* Reduce certainty by up to ~300 permille as jitter approaches the ceiling. */
    int64_t jitter_penalty = (jitter * 300) / SL_BESTOF_JITTER_CEIL_US;
    if (jitter_penalty > 300) jitter_penalty = 300;
    int64_t c = delivered - jitter_penalty;
    if (c < 0) c = 0;
    if (c > 1000) c = 1000;
    return (int)c;
}

/* Normalize a measured RTT to a [0..1000] "goodness" (lower RTT -> higher). */
static int64_t rtt_goodness(int64_t mean_us) {
    if (mean_us < 0) return 0;              /* unmeasured                     */
    if (mean_us >= SL_BESTOF_RTT_CEIL_US) return 0;
    return 1000 - (mean_us * 1000) / SL_BESTOF_RTT_CEIL_US;
}

int64_t slbestof_score(SLBestOf* b, int idx) {
    if (!valid_idx(b, idx)) return SL_BESTOF_SCORE_MIN;
    const SLBestCandidate* cn = &b->c[idx];

    int64_t mean = slbestof_mean_us(b, idx);
    int64_t loss = slbestof_loss_permille(b, idx);
    int64_t data_term = 0;
    if (cn->received > 0) {
        int64_t good = rtt_goodness(mean);            /* 0..1000 */
        int64_t deliver = 1000 - loss;                /* 0..1000 */
        data_term = (good + deliver) / 2;             /* 0..1000 */
    } /* else unproven: data_term stays 0 */

    int64_t decisions_term = slbestof_certainty_permille(b, idx); /* 0..1000 */

    /* Cost term: 0 cost -> 1000, rising cost -> lower. Normalized against the
     * budget when set, else against a soft reference of 1000. */
    int64_t ref = b->cost_budget > 0 ? b->cost_budget : 1000;
    int64_t cost_norm = (cn->cost * 1000) / (ref > 0 ? ref : 1000);
    if (cost_norm > 1000) cost_norm = 1000;
    int64_t costs_term = 1000 - cost_norm;            /* 0..1000 (higher=cheaper) */

    /* Version fitness: at/above min_version scores by how far above; a small
     * bonus per version over the floor, capped. */
    int64_t versions_term = 0;
    if (cn->version >= b->min_version) {
        int64_t over = cn->version - b->min_version;
        versions_term = 500 + (over * 100);
        if (versions_term > 1000) versions_term = 1000;
    }

    /* Architecture fitness: how the candidate's realized QoS architecture
     * (DiffServ/IntServ/MPLS/best-effort) should raise its score. */
    int64_t architecture_term = arch_fitness(cn);

    return (int64_t)b->w_data * data_term
         + (int64_t)b->w_decisions * decisions_term
         + (int64_t)b->w_costs * costs_term
         + (int64_t)b->w_versions * versions_term
         + (int64_t)b->w_architecture * architecture_term;
}

int slbestof_best(SLBestOf* b) {
    if (!b) return -1;
    int best = -1;
    int64_t best_score = SL_BESTOF_SCORE_MIN;
    for (int i = 0; i < b->ncandidates; i++) {
        const SLBestCandidate* cn = &b->c[i];
        if (!cn->used) continue;
        if (cn->version < b->min_version) continue;              /* version gate */
        if (b->cost_budget > 0 && cn->cost > b->cost_budget) continue; /* budget  */
        int64_t s = slbestof_score(b, i);
        if (s > best_score) { best_score = s; best = i; }
    }
    return best;
}

static void flags_string(int flags, char* out, size_t cap) {
    out[0] = 0;
    size_t n = 0;
    const struct { int bit; const char* name; } names[] = {
        {SL_BESTOF_FLAG_CRYPTO, "crypto"}, {SL_BESTOF_FLAG_RETRY, "retry"},
        {SL_BESTOF_FLAG_PACING, "pacing"}, {SL_BESTOF_FLAG_DSCP, "dscp"},
        {SL_BESTOF_FLAG_REPLAY, "replay"},
    };
    for (size_t i = 0; i < sizeof(names)/sizeof(names[0]); i++) {
        if (flags & names[i].bit) {
            int w = snprintf(out + n, cap - n, "%s%s", n ? "," : "", names[i].name);
            if (w > 0) n += (size_t)w;
            if (n >= cap) break;
        }
    }
    if (n == 0) snprintf(out, cap, "none");
}

/* Render a candidate's architecture as "arch(param):state", e.g.
 * "diffserv(46):realized", "intserv(2000):requested", "mpls(17):realized",
 * or "best-effort". */
static void arch_string(const SLBestCandidate* cn, char* out, size_t cap) {
    const char* a;
    const char* param_label;
    switch (cn->arch) {
        case SL_BESTOF_ARCH_DIFFSERV: a = "diffserv"; param_label = "dscp"; break;
        case SL_BESTOF_ARCH_INTSERV:  a = "intserv";  param_label = "kbps"; break;
        case SL_BESTOF_ARCH_MPLS:     a = "mpls";     param_label = "label"; break;
        default:                      snprintf(out, cap, "best-effort"); return;
    }
    const char* st = cn->arch_state == SL_BESTOF_ARCH_REALIZED ? "realized"
                   : cn->arch_state == SL_BESTOF_ARCH_DENIED   ? "denied"
                   : "requested";
    snprintf(out, cap, "%s(%s=%d):%s", a, param_label, cn->arch_param, st);
}

int slbestof_choice(SLBestOf* b, char* out, size_t cap) {
    if (!out || cap == 0) return 0;
    out[0] = 0;
    int idx = slbestof_best(b);
    if (idx < 0) { snprintf(out, cap, "(no qualifying candidate)"); return (int)strlen(out); }
    const SLBestCandidate* cn = &b->c[idx];
    char fl[64], ar[64];
    flags_string(cn->flags, fl, sizeof(fl));
    arch_string(cn, ar, sizeof(ar));
    int w = snprintf(out, cap,
        "%s %s arch=%s timeout=%dms payload=%dB gap=%dms flags=[%s] version=%d replays=%d "
        "mean=%lldus loss=%lldpermille certainty=%d score=%lld",
        cn->name, cn->route, ar, cn->timeout_ms, cn->payload_len, cn->min_gap_ms, fl,
        cn->version, cn->replays,
        (long long)slbestof_mean_us(b, idx),
        (long long)slbestof_loss_permille(b, idx),
        slbestof_certainty_permille(b, idx),
        (long long)slbestof_score(b, idx));
    if (w < 0) { out[0] = 0; return 0; }
    return ((size_t)w < cap) ? w : (int)(cap - 1);
}

int slbestof_report(SLBestOf* b, char* out, size_t cap) {
    if (!out || cap == 0) return 0;
    out[0] = 0;
    size_t n = 0;
    int w = snprintf(out, cap,
        "best-of weights[data=%d decisions=%d costs=%d versions=%d architecture=%d] "
        "min-version=%d cost-budget=%d candidates=%d\n",
        b ? b->w_data : 0, b ? b->w_decisions : 0, b ? b->w_costs : 0,
        b ? b->w_versions : 0, b ? b->w_architecture : 0,
        b ? b->min_version : 0, b ? b->cost_budget : 0,
        b ? b->ncandidates : 0);
    if (w > 0) n += (size_t)w;
    if (!b) return (int)n;
    int best = slbestof_best(b);
    for (int i = 0; i < b->ncandidates && n < cap; i++) {
        const SLBestCandidate* cn = &b->c[i];
        if (!cn->used) continue;
        char fl[64], ar[64];
        flags_string(cn->flags, fl, sizeof(fl));
        arch_string(cn, ar, sizeof(ar));
        w = snprintf(out + n, cap - n,
            "  %c %s %s arch=%s v%d cost=%d flags=[%s] mean=%lldus loss=%lldpermille "
            "certainty=%d score=%lld\n",
            i == best ? '*' : ' ', cn->name, cn->route, ar, cn->version, cn->cost, fl,
            (long long)slbestof_mean_us(b, i),
            (long long)slbestof_loss_permille(b, i),
            slbestof_certainty_permille(b, i),
            (long long)slbestof_score(b, i));
        if (w > 0) n += (size_t)w;
    }
    return ((size_t)n < cap) ? (int)n : (int)(cap - 1);
}

void slbestof_close(SLBestOf* b) {
    free(b);
}
