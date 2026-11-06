/*
 * rrs_demo.c — RRS (Reasonable Responsibility Solidity), a kind of LLM.
 *
 * Exercises the three-model congruent feed-forward on the ternary 1/0/2 signal,
 * shows faith/trial subtraction and the responsibility prelude, grades the
 * potestas culmination, and plugs the RRS conjunctive verifier into the
 * reasoning engine over a stub backend.
 *
 * Build:
 *   cc -std=c11 -O2 -Wall -Wextra -pedantic \
 *      ../large_language_models.c ../large_language_models_reasoning.c \
 *      rrs.c rrs_demo.c -o rrs_demo
 */
#include "rrs.h"
#include <stdio.h>
#include <string.h>

static int fails=0;
static void ok(int c,const char*w){ printf("  %s %s\n", c?"ok:":"FAIL:", w); if(!c) fails++; }

static void show(const char* title, const sllmrrs_result* r){
 printf("== %s ==\n", title);
 for(int s=0;s<SLLMRRS_STAGE_COUNT;s++)
  printf("   stage %-16s power=%u affirmed=%u eliminated=%u conj=%d\n",
         sllmrrs_stage_name((sllmrrs_stage)s), r->stage[s].power,
         r->stage[s].affirmed, r->stage[s].eliminated, r->stage[s].conjunction);
 printf("   -> total_power=%u solid=%d faith_eliminated=%u potestas=%s\n\n",
        r->total_power, r->solid, r->faith_eliminated, sllmrrs_potestas_name(r->potestas));
}

int main(void){
 /* ---- Case A: a solid, responsible argument -> WARRANTED potestas ----
    Stage 1: assumptions affirm and power-add (strict AND holds).
    Stage 2: responsibility met.  Stage 3: solidity culmination reads forward. */
 sllmrrs_premise s1a[]={
  {"the ground is firm",            SLLMRRS_POWER_ADD, 0, 1},
  {"the load is bounded",           SLLMRRS_AFFIRM,    0, 1},
 };
 sllmrrs_premise s2a[]={
  {"the actor accepts responsibility", SLLMRRS_POWER_ADD, 0, 1},
 };
 sllmrrs_result ra;
 sllmrrs_feed_forward(s1a,2, s2a,1, NULL,0, &ra);
 show("A: solid + responsible (expect WARRANTED)", &ra);
 ok(ra.solid && ra.potestas==SLLMRRS_POTESTAS_WARRANTED, "A -> solid, warranted");

 /* ---- Case B: a faith/trial premise is SUBTRACTED -> eliminated, not solid ---- */
 sllmrrs_premise s1b[]={
  {"the ground is firm",                   SLLMRRS_AFFIRM,    0, 1},
  {"it will hold because we trust on faith",SLLMRRS_POWER_ADD, 1, 1}, /* faith -> eliminated */
 };
 sllmrrs_premise s2b[]={ {"responsibility accepted", SLLMRRS_AFFIRM, 0, 1} };
 sllmrrs_result rb;
 sllmrrs_feed_forward(s1b,2, s2b,1, NULL,0, &rb);
 show("B: faith premise subtracted (expect NOT solid, faith_eliminated=1)", &rb);
 ok(!rb.solid && rb.faith_eliminated==1 && rb.potestas==SLLMRRS_POTESTAS_NONE,
    "B -> faith subtracted, not solid, none");

 /* ---- Case C: responsibility UNMET -> prelude to continue fails ---- */
 sllmrrs_premise s1c[]={ {"ground firm", SLLMRRS_AFFIRM, 0, 1} };
 sllmrrs_premise s2c[]={ {"actor declines responsibility", SLLMRRS_AFFIRM, 0, 0} }; /* unmet */
 sllmrrs_result rc;
 sllmrrs_feed_forward(s1c,1, s2c,1, NULL,0, &rc);
 show("C: responsibility unmet (expect NOT solid, none)", &rc);
 ok(!rc.solid && rc.potestas==SLLMRRS_POTESTAS_NONE, "C -> responsibility unmet, none");

 /* ---- Case D: thin but solid -> PROVISIONAL ---- */
 sllmrrs_premise s1d[]={ {"ground firm", SLLMRRS_AFFIRM, 0, 1} };
 sllmrrs_premise s2d[]={ {"responsibility met", SLLMRRS_AFFIRM, 0, 1} };
 sllmrrs_result rd;
 sllmrrs_feed_forward(s1d,1, s2d,1, NULL,0, &rd);
 show("D: thin solid (expect PROVISIONAL)", &rd);
 ok(rd.solid && rd.potestas==SLLMRRS_POTESTAS_PROVISIONAL, "D -> provisional");

 /* ---- ternary fold sanity: 0 kills conjunction; 1=+1; 2=+2 ---- */
 sllmrrs_premise fold[]={
  {"a", SLLMRRS_AFFIRM,0,1}, {"b", SLLMRRS_POWER_ADD,0,1}, {"c", SLLMRRS_ELIMINATE,0,1}
 };
 sllmrrs_accumulator acc;
 sllmrrs_fold_stage(SLLMRRS_STAGE_CONJUNCTIVE, fold,3,&acc);
 ok(acc.power==3 && acc.affirmed==2 && acc.eliminated==1 && acc.conjunction==0,
    "ternary fold: 1+2 power, one eliminate breaks AND");

 /* ---- RRS as a reasoning-engine verifier ---- */
 sllmr_verifier v=sllmrrs_as_reasoning_verifier();
 sllmr_step faith_step; memset(&faith_step,0,sizeof faith_step);
 faith_step.text="We accept this on faith."; char note[128];
 ok(v.check(v.user_data, SLLMR_SUBJECT_PHILOSOPHY, "q", NULL,0, &faith_step, note,sizeof note)==SLLMR_VERDICT_REFUTED,
    "verifier refutes a faith premise");
 sllmr_step solid_step; memset(&solid_step,0,sizeof solid_step);
 solid_step.text="This premise stands and conjoins; the ground is solid.";
 ok(v.check(v.user_data, SLLMR_SUBJECT_PHILOSOPHY, "q", NULL,0, &solid_step, note,sizeof note)==SLLMR_VERDICT_SUPPORTED,
    "verifier supports a solid affirming premise");

 if(fails){ fprintf(stderr,"rrs_demo: %d FAIL\n",fails); return 1; }
 printf("rrs_demo: OK — RRS 3-stage feed-forward, ternary 1/0/2, faith subtracted, potestas culmination\n");
 return 0;
}
