#include "http4_session.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static int accept(http4_session_state_t *s, uint8_t type, uint64_t sid,
                  uint64_t rid, uint64_t seq, uint32_t len) {
    http4_frame_view_t f;
    memset(&f, 0, sizeof(f));
    f.header.version = HTTP4_VERSION;
    f.header.type = type;
    f.header.stream_id = sid;
    f.header.request_id = rid;
    f.header.sequence = seq;
    f.header.payload_length = len;
    return http4_session_accept(s, &f);
}
int main(void) {
    http4_session_state_t s;
    http4_session_init(&s, 5, 8);
    assert(accept(&s, HTTP4_FRAME_OPEN, 10, 100, 0, 0) == 0);
    assert(accept(&s, HTTP4_FRAME_OPEN, 11, 101, 0, 0) == 0);
    assert(accept(&s, HTTP4_FRAME_DATA, 10, 100, 1, 4) == 0);
    assert(accept(&s, HTTP4_FRAME_DATA, 11, 101, 1, 4) == 0);
    assert(accept(&s, HTTP4_FRAME_DATA, 10, 100, 2, 2) != 0); /* per-stream limit */
    assert(accept(&s, HTTP4_FRAME_DATA, 10, 100, 1, 1) != 0); /* duplicate seq */
    assert(accept(&s, HTTP4_FRAME_END, 10, 100, 2, 0) == 0);
    assert(accept(&s, HTTP4_FRAME_DATA, 10, 100, 3, 1) != 0); /* after END */
    assert(http4_session_find(&s, 10) == NULL);
    assert(s.active_streams == 1);
    assert(accept(&s, HTTP4_FRAME_OPEN, 10, 200, 0, 0) != 0); /* IDs never reused */
    assert(accept(&s, HTTP4_FRAME_OPEN, 12, 102, 0, 0) == 0); /* closed slot reused */
    puts("SLeeLa HTTP 4.0 session-state self-test: PASS");
    return 0;
}
