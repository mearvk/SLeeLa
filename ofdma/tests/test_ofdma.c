#include "ofdma.h"
#include <assert.h>
#include <stdio.h>
int main(void) {
    const ofdma_numerology n = {2048, 1200, 15000, 14, 144, 12};
    ofdma_metrics m = {0};
    assert(ofdma_validate_numerology(&n) == OFDMA_OK);
    assert(ofdma_calculate_metrics(&n, 14, &m) == OFDMA_OK);
    assert(m.resource_blocks == 100 && m.grid_resource_elements == 16800);
    ofdma_user_request u[] = {{1,3,0},{2,2,0},{3,4,0}};
    uint32_t allocated = 0;
    assert(ofdma_schedule_round_robin(u, 3, 6, &allocated) == OFDMA_OK);
    assert(allocated == 6 && u[0].allocated_rbs == 2 && u[1].allocated_rbs == 2 && u[2].allocated_rbs == 2);
    puts("OFDMA tests: PASS");
    return 0;
}
