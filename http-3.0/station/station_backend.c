/* ==========================================================================
 * station_backend.c -- userland Station backend: simulation + device.
 * ========================================================================== */
#include "station_backend.h"

#include <string.h>
#include <stddef.h>
#include <fcntl.h>
#include <unistd.h>

int sl_station_init(sl_station_t *st, sl_backend_kind_t kind, const char *device_path)
{
    if (st == NULL) {
        return -1;
    }
    memset(st, 0, sizeof(*st));
    st->kind = kind;
    st->fd = -1;
    st->next_session_id = 1u;
    st->clock_ns = 1u; /* simulated monotonic clock starts at 1ns */

    if (kind == SL_BACKEND_DEVICE) {
        const char *path = device_path ? device_path : SLEELA_STATION_DEVPATH;
        st->fd = open(path, O_RDWR);
        if (st->fd < 0) {
            return -1;
        }
    }
    return 0;
}

static sl_session_t *sim_find(sl_station_t *st, uint64_t id)
{
    size_t i;
    for (i = 0; i < st->session_count; ++i) {
        if (st->sessions[i].session_id == id) {
            return &st->sessions[i];
        }
    }
    return NULL;
}

int sl_station_open(sl_station_t *st, uint64_t *session_id)
{
    if (st == NULL || session_id == NULL) {
        return -1;
    }
    if (st->kind == SL_BACKEND_DEVICE) {
        sl_session_view_t view;
        memset(&view, 0, sizeof(view));
        if (ioctl(st->fd, SL_IOC_OPEN, &view) != 0) {
            return -1;
        }
        *session_id = view.session_id;
        return 0;
    }
    /* simulation */
    if (st->session_count >= SL_BACKEND_MAX_SESSIONS) {
        return -1;
    }
    {
        sl_session_t *s = &st->sessions[st->session_count++];
        uint64_t id = st->next_session_id++;
        sl_session_open(s, id, st->clock_ns);
        *session_id = id;
    }
    return 0;
}

int sl_station_signal(sl_station_t *st, const sl_signal_t *sig, sl_record_t *out)
{
    if (st == NULL || sig == NULL) {
        return -1;
    }
    if (st->kind == SL_BACKEND_DEVICE) {
        sl_signal_t local = *sig;
        if (ioctl(st->fd, SL_IOC_SIGNAL, &local) != 0) {
            return -1;
        }
        /* Record is read back via query in the device path; out is best-effort. */
        if (out != NULL) {
            memset(out, 0, sizeof(*out));
        }
        return 0;
    }
    /* simulation */
    {
        sl_session_t *s = sim_find(st, sig->session_id);
        if (s == NULL) {
            return -1;
        }
        return sl_session_signal(s, sig, out);
    }
}

int sl_station_query(sl_station_t *st, uint64_t session_id, sl_session_view_t *view)
{
    if (st == NULL || view == NULL) {
        return -1;
    }
    if (st->kind == SL_BACKEND_DEVICE) {
        memset(view, 0, sizeof(*view));
        view->session_id = session_id;
        return ioctl(st->fd, SL_IOC_QUERY, view) == 0 ? 0 : -1;
    }
    {
        sl_session_t *s = sim_find(st, session_id);
        if (s == NULL) {
            return -1;
        }
        sl_session_view(s, view);
        return 0;
    }
}

int sl_station_close_session(sl_station_t *st, uint64_t session_id)
{
    if (st == NULL) {
        return -1;
    }
    if (st->kind == SL_BACKEND_DEVICE) {
        uint64_t id = session_id;
        return ioctl(st->fd, SL_IOC_CLOSE, &id) == 0 ? 0 : -1;
    }
    {
        sl_session_t *s = sim_find(st, session_id);
        if (s == NULL) {
            return -1;
        }
        sl_session_close(s, st->clock_ns);
        return 0;
    }
}

const sl_session_t *sl_station_peek(const sl_station_t *st, uint64_t session_id)
{
    size_t i;
    if (st == NULL || st->kind != SL_BACKEND_SIMULATION) {
        return NULL;
    }
    for (i = 0; i < st->session_count; ++i) {
        if (st->sessions[i].session_id == session_id) {
            return &st->sessions[i];
        }
    }
    return NULL;
}

void sl_station_shutdown(sl_station_t *st)
{
    if (st == NULL) {
        return;
    }
    if (st->kind == SL_BACKEND_DEVICE && st->fd >= 0) {
        close(st->fd);
        st->fd = -1;
    }
}

sl_signal_t sl_make_signal(uint64_t session_id, sl_party_t party,
                           uint8_t urg_units, int hop_delta,
                           uint8_t flags_extra, uint64_t timestamp_ns)
{
    sl_signal_t sig;
    memset(&sig, 0, sizeof(sig));
    sig.abi_version = SLEELA_STATION_ABI_VERSION;
    sig.session_id = session_id;
    sig.party = (uint8_t)party;
    sig.urg_units = urg_units;
    sig.hop_delta = (int8_t)hop_delta;
    sig.tcp_flags = flags_extra;
    if (urg_units > 0u) {
        sig.tcp_flags |= SL_TCP_URG; /* a premise-bearing signal carries URG */
    }
    sig.timestamp_ns = timestamp_ns;
    return sig;
}
