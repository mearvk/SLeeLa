#include "osls.h"
#include <assert.h>
#include <stdint.h>

int main(void) {
    osls_system_info info;
    int64_t next = 0;
    assert(osls_get_system_info(&info) == OSLS_OK);
    assert(info.process_id != 0);
    assert(info.hostname[0] != '\0');
    assert(osls_cron_validate("*/15 9-17 * * 1-5") == OSLS_OK);
    assert(osls_cron_validate("60 * * * *") == OSLS_INVALID_ARGUMENT);
    assert(osls_cron_next_after("0 0 * * *", 1704067200, &next) == OSLS_OK);
    assert(next > 1704067200);
    return 0;
}
