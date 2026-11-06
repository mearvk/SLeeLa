#include "rrs_signal_monitor.h"
#include <string.h>
#include <stdio.h>

const char *sllmrm_reason_name(sllmrm_reason r){
 switch(r){
  case SLLMRM_REASON_NONE: return "-";
  case SLLMRM_REASON_SIGNALLED_NO: return "signalled-no";
  case SLLMRM_REASON_FAITH: return "faith-subtracted";
  case SLLMRM_REASON_RESPONSIBILITY: return "responsibility-unmet";
 }
 return "unknown";
}

sllmrm_status sllmrm_monitor_init(sllmrm_monitor *m, sllmrm_event *buf, size_t cap){
 if(!m) return SLLMRM_INVALID_ARGUMENT;
 memset(m,0,sizeof(*m));
 m->abi_version=SLLMRM_ABI_VERSION;
 m->events=buf; m->event_cap=buf?cap:0; m->event_count=0;
 m->tally.potestas=SLLMRRS_POTESTAS_NONE;
 return SLLMRM_OK;
}

/* Re-derive the effective signal for one premise exactly as sllmrrs_fold_stage
   does, and classify the elimination reason. */
static sllmrrs_signal effective_signal(sllmrrs_stage stage,
                                       const sllmrrs_premise *pr,
                                       sllmrm_reason *reason_out){
 sllmrm_reason reason=SLLMRM_REASON_NONE;
 sllmrrs_signal sig=pr->signal;
 if(pr->faith_or_trial){ sig=SLLMRRS_ELIMINATE; reason=SLLMRM_REASON_FAITH; }
 else if(stage==SLLMRRS_STAGE_RESPONSIBILITY && !pr->responsibility_met){
  sig=SLLMRRS_ELIMINATE; reason=SLLMRM_REASON_RESPONSIBILITY;
 } else if(sig==SLLMRRS_ELIMINATE){
  reason=SLLMRM_REASON_SIGNALLED_NO;
 }
 if(reason_out) *reason_out=reason;
 return sig;
}

static sllmrm_status observe_stage(sllmrm_monitor *m, sllmrrs_stage stage,
                                   const sllmrrs_premise *ps, size_t n,
                                   uint32_t *running_power, int *overflow){
 for(size_t i=0;i<n;i++){
  sllmrm_reason reason;
  sllmrrs_signal sig=effective_signal(stage,&ps[i],&reason);
  uint32_t add = (sig==SLLMRRS_POWER_ADD)?2u : (sig==SLLMRRS_AFFIRM)?1u : 0u;
  *running_power += add;
  m->tally.stage_power[stage] += add;

  if(sig==SLLMRRS_AFFIRM) m->tally.affirm++;
  else if(sig==SLLMRRS_POWER_ADD) m->tally.power_add++;
  else m->tally.eliminate++;
  m->tally.by_reason[reason]++;

  if(m->events && m->event_count<m->event_cap){
   sllmrm_event *e=&m->events[m->event_count++];
   e->stage=stage; e->index_in_stage=(uint32_t)i; e->text=ps[i].text;
   e->effective_signal=sig; e->reason=reason; e->running_power=*running_power;
  } else if(m->events){
   *overflow=1; /* ran out of event slots; tallies still complete */
  }
 }
 return SLLMRM_OK;
}

sllmrm_status sllmrm_observe(sllmrm_monitor *m,
                             const sllmrrs_premise *s1, size_t n1,
                             const sllmrrs_premise *s2, size_t n2,
                             const sllmrrs_premise *s3, size_t n3,
                             sllmrrs_result *result_out){
 if(!m) return SLLMRM_INVALID_ARGUMENT;
 if((n1&&!s1)||(n2&&!s2)||(n3&&!s3)) return SLLMRM_INVALID_ARGUMENT;

 /* Run the real model so the monitor reflects the true outcome. */
 sllmrrs_result res;
 sllmrrs_status rc=sllmrrs_feed_forward(s1,n1,s2,n2,s3,n3,&res);
 if(rc!=SLLMRRS_OK) return SLLMRM_INVALID_ARGUMENT;
 if(result_out) *result_out=res;

 /* Reset tallies/events for this observation. */
 sllmrm_event *buf=m->events; size_t cap=m->event_cap;
 memset(&m->tally,0,sizeof(m->tally));
 m->event_count=0; m->events=buf; m->event_cap=cap;

 uint32_t power=0; int overflow=0;
 observe_stage(m,SLLMRRS_STAGE_CONJUNCTIVE,  s1,n1,&power,&overflow);
 observe_stage(m,SLLMRRS_STAGE_RESPONSIBILITY,s2,n2,&power,&overflow);
 observe_stage(m,SLLMRRS_STAGE_SOLIDITY,     s3,n3,&power,&overflow);

 m->tally.total_power=res.total_power;
 m->tally.potestas=res.potestas;
 m->tally.solid=res.solid;
 return overflow? SLLMRM_CAPACITY : SLLMRM_OK;
}

int sllmrm_format_report(const sllmrm_monitor *m, char *out, size_t cap){
 if(!m) return 0;
 /* Build into a local buffer, then copy (bounded) — keeps snprintf accounting simple. */
 char buf[4096]; size_t used=0; const size_t BUFSZ=sizeof buf;
 /* Append via snprintf. 'used' is always < BUFSZ (we never advance it past the
    buffer), so buf+used is valid and BUFSZ-used is the honest remaining size.
    snprintf's return (the would-be length) is only added when it fits, so the
    buffer never overflows and the warning about a size-1 region cannot arise. */
 #define EMIT(...) do{ int _n=snprintf(buf+used, BUFSZ-used, __VA_ARGS__); \
                       if(_n>0){ size_t _u=(size_t)_n; used += (_u < BUFSZ-used) ? _u : (BUFSZ-used-1); } }while(0)
 EMIT("RRS signal analysis (1=affirm 0=eliminate 2=power-add)\n");
 EMIT("  signals: affirm=%u eliminate=%u power-add=%u\n",
      m->tally.affirm, m->tally.eliminate, m->tally.power_add);
 EMIT("  eliminations by reason: signalled-no=%u faith-subtracted=%u responsibility-unmet=%u\n",
      m->tally.by_reason[SLLMRM_REASON_SIGNALLED_NO],
      m->tally.by_reason[SLLMRM_REASON_FAITH],
      m->tally.by_reason[SLLMRM_REASON_RESPONSIBILITY]);
 EMIT("  stage power: conjunctive=%u responsibility=%u solidity=%u\n",
      m->tally.stage_power[SLLMRRS_STAGE_CONJUNCTIVE],
      m->tally.stage_power[SLLMRRS_STAGE_RESPONSIBILITY],
      m->tally.stage_power[SLLMRRS_STAGE_SOLIDITY]);
 EMIT("  culmination: total_power=%u solid=%d potestas=%s\n",
      m->tally.total_power, m->tally.solid, sllmrrs_potestas_name(m->tally.potestas));
 if(m->events && m->event_count){
  EMIT("  stream:\n");
  for(size_t i=0;i<m->event_count;i++){
   const sllmrm_event *e=&m->events[i];
   EMIT("    [%-14s #%u] sig=%-11s reason=%-20s power=%u  %s\n",
        sllmrrs_stage_name(e->stage), e->index_in_stage,
        sllmrrs_signal_name(e->effective_signal), sllmrm_reason_name(e->reason),
        e->running_power, e->text?e->text:"");
  }
 }
 #undef EMIT
 if(out && cap){
  size_t n = used < cap-1 ? used : cap-1;
  memcpy(out,buf,n); out[n]=0;
 }
 return (int)used;
}
