/*
 * rrs_signal_monitor_demo.c — watch the RRS ternary 1/0/2 signal stream.
 *
 * Runs RRS feed-forwards through the monitor and prints the signal analysis:
 * per-signal tallies, eliminations by reason (incl. faith subtracted), stage
 * power, and the potestas culmination. Self-checks the tallies.
 *
 * Build:
 *   cc -std=c11 -O2 -Wall -Wextra -pedantic \
 *      ../rrs.c ../../large_language_models.c ../../large_language_models_reasoning.c \
 *      rrs_signal_monitor.c rrs_signal_monitor_demo.c -o rrs_signal_monitor_demo
 */
#include "rrs_signal_monitor.h"
#include <stdio.h>

static int fails=0;
static void ok(int c,const char*w){ printf("  %s %s\n", c?"ok:":"FAIL:", w); if(!c) fails++; }

static void run(const char* title,
                const sllmrrs_premise* s1,size_t n1,
                const sllmrrs_premise* s2,size_t n2,
                const sllmrrs_premise* s3,size_t n3,
                sllmrm_monitor* m){
 sllmrm_observe(m, s1,n1, s2,n2, s3,n3, NULL);
 char report[2048];
 sllmrm_format_report(m, report, sizeof report);
 printf("== %s ==\n%s\n", title, report);
}

int main(void){
 sllmrm_event ev[32];
 sllmrm_monitor mon;
 sllmrm_monitor_init(&mon, ev, 32);

 /* A: solid + responsible -> warranted; signals 2,1 then 2 */
 sllmrrs_premise a1[]={ {"the ground is firm",SLLMRRS_POWER_ADD,0,1},
                        {"the load is bounded",SLLMRRS_AFFIRM,0,1} };
 sllmrrs_premise a2[]={ {"the actor accepts responsibility",SLLMRRS_POWER_ADD,0,1} };
 run("A: solid + responsible", a1,2, a2,1, NULL,0, &mon);
 ok(mon.tally.power_add==2 && mon.tally.affirm==1 && mon.tally.eliminate==0
    && mon.tally.potestas==SLLMRRS_POTESTAS_WARRANTED, "A: 2 power-add, 1 affirm, warranted");

 /* B: a faith premise is subtracted -> eliminate by reason FAITH */
 sllmrrs_premise b1[]={ {"the ground is firm",SLLMRRS_AFFIRM,0,1},
                        {"it holds because we trust on faith",SLLMRRS_POWER_ADD,1,1} };
 sllmrrs_premise b2[]={ {"responsibility accepted",SLLMRRS_AFFIRM,0,1} };
 run("B: faith premise subtracted", b1,2, b2,1, NULL,0, &mon);
 ok(mon.tally.by_reason[SLLMRM_REASON_FAITH]==1 && mon.tally.eliminate==1
    && mon.tally.potestas==SLLMRRS_POTESTAS_NONE, "B: 1 faith-subtracted elimination, none");

 /* C: responsibility unmet -> eliminate by reason RESPONSIBILITY */
 sllmrrs_premise c1[]={ {"ground firm",SLLMRRS_AFFIRM,0,1} };
 sllmrrs_premise c2[]={ {"actor declines responsibility",SLLMRRS_AFFIRM,0,0} };
 run("C: responsibility unmet", c1,1, c2,1, NULL,0, &mon);
 ok(mon.tally.by_reason[SLLMRM_REASON_RESPONSIBILITY]==1
    && mon.tally.potestas==SLLMRRS_POTESTAS_NONE, "C: responsibility-unmet elimination, none");

 /* tally-only mode (no event buffer) still produces correct counts */
 sllmrm_monitor t; sllmrm_monitor_init(&t, NULL, 0);
 sllmrm_observe(&t, a1,2, a2,1, NULL,0, NULL);
 ok(t.event_count==0 && t.tally.power_add==2 && t.tally.total_power==5,
    "tally-only mode: no events, correct power");

 if(fails){ fprintf(stderr,"rrs_signal_monitor_demo: %d FAIL\n",fails); return 1; }
 printf("rrs_signal_monitor_demo: OK\n");
 return 0;
}
