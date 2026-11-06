/* Self-test for the model-capacity profile (2..16 GB envelope, user-owned).
 * Build:
 *   cc -std=c11 -O2 -Wall -Wextra -pedantic \
 *      ../large_language_models_capacity.c capacity_selftest.c -o capacity_selftest
 */
#include "../large_language_models_capacity.h"
#include <stdio.h>
#include <string.h>

#define GIB (1024ULL*1024ULL*1024ULL)
static int fails=0;
static void expect(int cond, const char* what){ if(!cond){ fprintf(stderr,"FAIL: %s\n",what); fails++; } }

int main(void){
 /* Tier boundaries across the envelope. */
 expect(sllmc_tier_for_bytes(0)==SLLMC_TIER_UNKNOWN, "0 -> UNKNOWN");
 expect(sllmc_tier_for_bytes(2*GIB)==SLLMC_TIER_COMPACT, "2 GB -> COMPACT");
 expect(sllmc_tier_for_bytes(4*GIB)==SLLMC_TIER_STANDARD, "4 GB -> STANDARD");
 expect(sllmc_tier_for_bytes(8*GIB)==SLLMC_TIER_CAPABLE, "8 GB -> CAPABLE");
 expect(sllmc_tier_for_bytes(16*GIB)==SLLMC_TIER_LARGE, "16 GB -> LARGE");
 /* Out-of-envelope saturates, does not crash. */
 expect(sllmc_tier_for_bytes(1*GIB)==SLLMC_TIER_COMPACT, "1 GB -> COMPACT (saturate)");
 expect(sllmc_tier_for_bytes(32*GIB)==SLLMC_TIER_LARGE, "32 GB -> LARGE (saturate)");

 /* Advisory envelope check. */
 sllmc_profile p; sllmc_profile_init(&p);
 p.weight_bytes=1*GIB; expect(sllmc_profile_check(&p)==SLLMC_BELOW_MIN, "1 GB below min");
 p.weight_bytes=32*GIB; expect(sllmc_profile_check(&p)==SLLMC_ABOVE_MAX, "32 GB above max");
 p.weight_bytes=8*GIB; expect(sllmc_profile_check(&p)==SLLMC_OK, "8 GB in envelope");
 p.weight_bytes=0; expect(sllmc_profile_check(&p)==SLLMC_OK, "0 unspecified ok");

 /* Budgets scale with capacity. */
 sllmc_budget b;
 p.weight_bytes=2*GIB; p.context_tokens=0; sllmc_derive_budget(&p,&b);
 expect(b.context_tokens==2048 && b.max_steps==8, "compact budget");
 p.weight_bytes=16*GIB; sllmc_derive_budget(&p,&b);
 expect(b.context_tokens==16384 && b.max_steps==24 && b.max_backtracks==6, "large budget");

 /* User override of context wins over the tier default. */
 p.weight_bytes=2*GIB; p.context_tokens=12000; sllmc_derive_budget(&p,&b);
 expect(b.context_tokens==12000, "user context override");

 /* Unknown size falls back to STANDARD defaults. */
 p.weight_bytes=0; p.context_tokens=0; sllmc_derive_budget(&p,&b);
 expect(b.context_tokens==4096 && b.max_steps==12, "unknown -> standard default");

 if(fails){ fprintf(stderr,"capacity_selftest: %d FAIL\n",fails); return 1; }
 printf("capacity_selftest: OK — tiers, envelope (2..16 GB), budgets, overrides\n");
 return 0;
}
