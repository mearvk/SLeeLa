#include "rrs.h"
#include <string.h>
#include <stdio.h>

const char *sllmrrs_status_string(sllmrrs_status s){
 switch(s){
  case SLLMRRS_OK: return "ok";
  case SLLMRRS_INVALID_ARGUMENT: return "invalid argument";
  case SLLMRRS_FAITH_PREMISE: return "faith/trial premise eliminated (subtracted from RRS)";
  case SLLMRRS_NOT_SOLID: return "argument did not stand: solidity/firmament broken";
  case SLLMRRS_RESPONSIBILITY_UNMET: return "reasonable-responsibility prelude unmet";
  case SLLMRRS_CAPACITY: return "capacity exceeded";
 }
 return "unknown";
}
const char *sllmrrs_signal_name(sllmrrs_signal s){
 switch(s){ case SLLMRRS_ELIMINATE: return "0:eliminate"; case SLLMRRS_AFFIRM: return "1:affirm";
  case SLLMRRS_POWER_ADD: return "2:power-add"; } return "signal";
}
const char *sllmrrs_stage_name(sllmrrs_stage s){
 switch(s){ case SLLMRRS_STAGE_CONJUNCTIVE: return "conjunctive";
  case SLLMRRS_STAGE_RESPONSIBILITY: return "responsibility";
  case SLLMRRS_STAGE_SOLIDITY: return "solidity-firmament"; default: return "stage"; }
}
const char *sllmrrs_potestas_name(sllmrrs_potestas p){
 switch(p){ case SLLMRRS_POTESTAS_NONE: return "none";
  case SLLMRRS_POTESTAS_PROVISIONAL: return "provisional";
  case SLLMRRS_POTESTAS_WARRANTED: return "warranted"; } return "unknown";
}

/* The congruent stage kernel — the SAME fold for all three stages.
   Signal semantics:
     ELIMINATE(0) -> hard zero: strict-AND conjunction becomes false for the stage.
     AFFIRM(1)    -> survives, +1 power.
     POWER_ADD(2) -> survives, +2 power (binary-simple power addition).
   Subtractions that force ELIMINATE regardless of the proposed signal:
     - a faith/trial premise (RRS subtracts faith);
     - at the RESPONSIBILITY stage, a premise whose responsibility is unmet. */
sllmrrs_status sllmrrs_fold_stage(sllmrrs_stage stage,
                                  const sllmrrs_premise *premises, size_t count,
                                  sllmrrs_accumulator *acc){
 if(!acc) return SLLMRRS_INVALID_ARGUMENT;
 acc->power=0; acc->affirmed=0; acc->eliminated=0; acc->conjunction=1;
 if(count && !premises) return SLLMRRS_INVALID_ARGUMENT;

 for(size_t i=0;i<count;i++){
  const sllmrrs_premise *pr=&premises[i];
  sllmrrs_signal sig=pr->signal;

  /* RRS subtracts faith: a faith/trial premise is always eliminated. */
  if(pr->faith_or_trial) sig=SLLMRRS_ELIMINATE;
  /* Responsibility prelude: unmet responsibility eliminates (prelude to continue). */
  if(stage==SLLMRRS_STAGE_RESPONSIBILITY && !pr->responsibility_met) sig=SLLMRRS_ELIMINATE;

  if(sig==SLLMRRS_ELIMINATE){
   acc->eliminated++;
   acc->conjunction=0;   /* strict AND: one elimination breaks the stage conjunction */
   continue;
  }
  /* AFFIRM or POWER_ADD: survives and contributes to the accumulated power. */
  acc->affirmed++;
  acc->power += (sig==SLLMRRS_POWER_ADD)? 2u : 1u;
 }
 return SLLMRRS_OK;
}

/* Count faith/trial premises across the stage inputs (for the report). */
static uint32_t count_faith(const sllmrrs_premise *p, size_t n){
 uint32_t c=0; for(size_t i=0;i<n;i++) if(p && p[i].faith_or_trial) c++; return c;
}

sllmrrs_status sllmrrs_feed_forward(
 const sllmrrs_premise *s1, size_t n1,
 const sllmrrs_premise *s2, size_t n2,
 const sllmrrs_premise *s3, size_t n3,
 sllmrrs_result *out){
 if(!out) return SLLMRRS_INVALID_ARGUMENT;
 memset(out,0,sizeof(*out));
 out->potestas=SLLMRRS_POTESTAS_NONE;

 const sllmrrs_premise *ins[SLLMRRS_STAGE_COUNT]={s1,s2,s3};
 size_t counts[SLLMRRS_STAGE_COUNT]={n1,n2,n3};

 out->faith_eliminated = count_faith(s1,n1)+count_faith(s2,n2)+count_faith(s3,n3);

 int all_conjoined=1;
 uint32_t power=0;
 for(int st=0; st<SLLMRRS_STAGE_COUNT; st++){
  sllmrrs_status rc=sllmrrs_fold_stage((sllmrrs_stage)st, ins[st], counts[st], &out->stage[st]);
  if(rc!=SLLMRRS_OK) return rc;
  power += out->stage[st].power;

  /* Feed-forward, fail closed: a broken conjunction stops the forward pass.
     Stage 3 (solidity) with zero premises inherits the prior conjunction and
     reads the accumulated power, so an empty stage 3 does not break solidity. */
  if(counts[st]==0 && st==SLLMRRS_STAGE_SOLIDITY){
   out->stage[st].conjunction = all_conjoined;  /* culmination reads forward */
  } else if(!out->stage[st].conjunction){
   all_conjoined=0;
   /* record power so far, then stop the feed-forward */
   out->total_power=power;
   out->solid=0;
   if(st==SLLMRRS_STAGE_RESPONSIBILITY) return SLLMRRS_OK; /* responsibility unmet is a valid, honest NONE */
   return SLLMRRS_OK;
  }
 }

 out->total_power=power;
 out->solid = all_conjoined ? 1 : 0;

 /* Culmination: potestas as power issuing from a completed responsible process.
    Warranted requires solidity AND responsibility-stage power; provisional if
    solid but thin; none otherwise. */
 if(out->solid && out->stage[SLLMRRS_STAGE_RESPONSIBILITY].affirmed>0 && power>=4){
  out->potestas=SLLMRRS_POTESTAS_WARRANTED;
 } else if(out->solid && power>=2){
  out->potestas=SLLMRRS_POTESTAS_PROVISIONAL;
 } else {
  out->potestas=SLLMRRS_POTESTAS_NONE;
 }
 return SLLMRRS_OK;
}

/* ---- reasoning-engine verifier adapter ----
   Classify a candidate reasoning step as an RRS signal from light cues in its
   text, then map to a verdict. This lets RRS gate a chain-of-thought: a step
   that rests on faith/trial, or that eliminates, is REFUTED (backtrack); a step
   that affirms/conjoins is SUPPORTED; otherwise UNKNOWN. */
static int contains(const char *h,const char *n){ return h&&n&&strstr(h,n)!=NULL; }

static sllmr_verdict rrs_check(void *ud, sllmr_subject subject, const char *question,
                               const sllmr_step *prior, size_t pn,
                               const sllmr_step *cand, char *note, size_t ncap){
 (void)ud;(void)subject;(void)question;(void)prior;(void)pn;
 const char *t = cand->text? cand->text : "";
 /* Faith/trial is subtracted: such a step is refuted out of RRS. */
 if(contains(t,"faith")||contains(t,"trial of faith")||contains(t,"on faith")){
  if(note&&ncap) snprintf(note,ncap,"RRS subtracts faith: premise rests on faith/trial -> eliminated");
  return SLLMR_VERDICT_REFUTED;
 }
 /* Explicit elimination cue. */
 if(contains(t,"eliminate")||contains(t,"does not hold")||contains(t,"unmet responsibility")){
  if(note&&ncap) snprintf(note,ncap,"RRS: premise reclaimed by prelude -> eliminated (0)");
  return SLLMR_VERDICT_REFUTED;
 }
 /* Affirmation / conjunction cue. */
 if(contains(t,"affirm")||contains(t,"conjoin")||contains(t,"responsibility met")||
    contains(t,"stands")||contains(t,"solid")||contains(t,"firmament")){
  if(note&&ncap) snprintf(note,ncap,"RRS: premise affirms and conjoins -> power added (1/2)");
  return SLLMR_VERDICT_SUPPORTED;
 }
 return SLLMR_VERDICT_UNKNOWN;
}

sllmr_verifier sllmrrs_as_reasoning_verifier(void){
 sllmr_verifier v;
 v.abi_version=SLLMR_ABI_VERSION;
 v.name="rrs-conjunctive-verifier";
 v.user_data=NULL;
 v.check=rrs_check;
 return v;
}
