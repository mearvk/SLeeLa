#include "large_language_models_reasoning.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* ---------------------------------------------------------------------------
 * Reasoning controller.
 *
 * The LLM is a proposer: for each step we ask the backend to produce the next
 * piece of the chain of thought, prefixed with a kind tag it chooses from a
 * fixed menu. We parse that tag, optionally REFLECT, then VERIFY through the
 * supplied checker. A REFUTED step triggers BACKTRACK: the current path is
 * abandoned (minus its failing tail) and a fresh attempt begins at greater
 * depth, with the refutation fed back to the backend so it does not repeat it.
 *
 * No weights live here. All "intelligence" is the backend's; all "soundness" is
 * the verifier's. This file is the control loop + honest bookkeeping only.
 * ------------------------------------------------------------------------- */

struct sllmr_engine {
 sllm_engine *llm;              /* borrowed */
 const sllmr_verifier *verifier;/* borrowed, may be NULL */
};

/* ---- small growable string ---- */
typedef struct { char *buf; size_t len, cap; } str;
static int str_reserve(str *s, size_t need){
 if(s->cap>=need) return 1;
 size_t nc = s->cap? s->cap*2 : 256;
 while(nc<need) nc*=2;
 char *p=(char*)realloc(s->buf,nc); if(!p) return 0;
 s->buf=p; s->cap=nc; return 1;
}
static int str_append(str *s, const char *data, size_t n){
 if(!str_reserve(s, s->len+n+1)) return 0;
 memcpy(s->buf+s->len, data, n); s->len+=n; s->buf[s->len]=0; return 1;
}
static int str_puts(str *s, const char *z){ return str_append(s, z, strlen(z)); }
static char *str_take(str *s){ char *p=s->buf; s->buf=NULL; s->len=s->cap=0; return p? p : (char*)calloc(1,1); }
static void str_free(str *s){ free(s->buf); s->buf=NULL; s->len=s->cap=0; }

/* chunk sink: accumulate the backend's streamed UTF-8 into a str */
static int collect_chunk(const char *utf8, size_t n, void *ud){
 return str_append((str*)ud, utf8, n) ? 1 : 0;
}

const char *sllmr_status_string(sllmr_status s){
 switch(s){
  case SLLMR_OK: return "ok";
  case SLLMR_INVALID_ARGUMENT: return "invalid argument";
  case SLLMR_OUT_OF_MEMORY: return "out of memory";
  case SLLMR_BACKEND_ERROR: return "backend error";
  case SLLMR_NOT_LOADED: return "llm not loaded";
  case SLLMR_CANCELLED: return "cancelled";
  case SLLMR_NO_SOUND_PATH: return "no sound path within backtrack budget";
  case SLLMR_UNSUPPORTED: return "unsupported";
 }
 return "unknown";
}
const char *sllmr_subject_name(sllmr_subject s){
 switch(s){case SLLMR_SUBJECT_MATH:return "math";case SLLMR_SUBJECT_PHILOSOPHY:return "philosophy";
  case SLLMR_SUBJECT_RELIGION:return "religion";default:return "general";}
}
const char *sllmr_step_kind_name(sllmr_step_kind k){
 switch(k){case SLLMR_STEP_DECOMPOSE:return "decompose";case SLLMR_STEP_ASSUMPTION:return "assumption";
  case SLLMR_STEP_DEDUCTION:return "deduction";case SLLMR_STEP_INDUCTION:return "induction";
  case SLLMR_STEP_OBJECTION:return "objection";case SLLMR_STEP_REFLECTION:return "reflection";
  case SLLMR_STEP_REVISION:return "revision";case SLLMR_STEP_CONCLUSION:return "conclusion";}
 return "step";
}
const char *sllmr_verdict_name(sllmr_verdict v){
 switch(v){case SLLMR_VERDICT_SUPPORTED:return "supported";case SLLMR_VERDICT_REFUTED:return "refuted";
  default:return "unknown";}
}

sllmr_status sllmr_options_init(sllmr_options *o){
 if(!o) return SLLMR_INVALID_ARGUMENT;
 o->abi_version=SLLMR_ABI_VERSION;
 o->subject=SLLMR_SUBJECT_GENERAL;
 o->max_steps=12;
 o->max_backtracks=3;
 o->reflection_every=3;
 o->min_confidence=0.55;
 o->require_verified=0;
 o->seed=0;
 return SLLMR_OK;
}

sllmr_status sllmr_engine_create(sllm_engine *llm, const sllmr_verifier *verifier, sllmr_engine **out){
 if(!llm || !out) return SLLMR_INVALID_ARGUMENT;
 *out=NULL;
 if(!sllm_engine_is_loaded(llm)) return SLLMR_NOT_LOADED;
 if(verifier && verifier->abi_version!=SLLMR_ABI_VERSION) return SLLMR_UNSUPPORTED;
 sllmr_engine *e=(sllmr_engine*)calloc(1,sizeof(*e));
 if(!e) return SLLMR_OUT_OF_MEMORY;
 e->llm=llm; e->verifier=verifier; *out=e; return SLLMR_OK;
}
void sllmr_engine_destroy(sllmr_engine *e){ free(e); }
const char *sllmr_engine_verifier_name(const sllmr_engine *e){
 return (e && e->verifier && e->verifier->name)? e->verifier->name : "(none)";
}

/* Parse a leading "KIND:" tag from a backend line; default DEDUCTION. */
static sllmr_step_kind parse_kind(const char *text, const char **body_out){
 struct { const char *tag; sllmr_step_kind k; } map[] = {
  {"DECOMPOSE:",SLLMR_STEP_DECOMPOSE},{"ASSUMPTION:",SLLMR_STEP_ASSUMPTION},
  {"DEDUCTION:",SLLMR_STEP_DEDUCTION},{"INDUCTION:",SLLMR_STEP_INDUCTION},
  {"OBJECTION:",SLLMR_STEP_OBJECTION},{"REFLECTION:",SLLMR_STEP_REFLECTION},
  {"REVISION:",SLLMR_STEP_REVISION},{"CONCLUSION:",SLLMR_STEP_CONCLUSION},
 };
 while(*text==' '||*text=='\t') text++;
 for(size_t i=0;i<sizeof(map)/sizeof(map[0]);i++){
  size_t L=strlen(map[i].tag);
  if(strncmp(text,map[i].tag,L)==0){
   const char *b=text+L; while(*b==' ') b++;
   *body_out=b; return map[i].k;
  }
 }
 *body_out=text; return SLLMR_STEP_DEDUCTION;
}

/* Build the proposer prompt for the next step, given subject + history. */
static int build_prompt(str *p, sllmr_subject subject, const char *question,
                        const sllmr_step *steps, size_t n, const char *backtrack_hint){
 p->len=0; if(p->buf) p->buf[0]=0;
 const char *persona;
 switch(subject){
  case SLLMR_SUBJECT_MATH:
   persona="You are a careful mathematician. Prefer exact, checkable steps. "
           "Mark each line with one tag: ASSUMPTION, DEDUCTION, REFLECTION, REVISION, or CONCLUSION."; break;
  case SLLMR_SUBJECT_PHILOSOPHY:
   persona="You are a rigorous philosopher. Surface hidden assumptions, steelman the strongest "
           "objection, and prefer a nuanced position over a slogan. Mark each line with one tag: "
           "DECOMPOSE, ASSUMPTION, DEDUCTION, INDUCTION, OBJECTION, REFLECTION, REVISION, or CONCLUSION."; break;
  case SLLMR_SUBJECT_RELIGION:
   persona="You are a careful comparative-religion scholar. Attribute claims to specific traditions "
           "or texts, distinguish faith commitments from empirical claims, and make NO claim of proof. "
           "Mark each line with one tag: DECOMPOSE, ASSUMPTION, INDUCTION, OBJECTION, REFLECTION, REVISION, or CONCLUSION."; break;
  default:
   persona="You are a careful reasoner. Decompose the problem, reflect on your own steps, and revise "
           "when a step is weak. Mark each line with one tag: DECOMPOSE, ASSUMPTION, DEDUCTION, INDUCTION, "
           "OBJECTION, REFLECTION, REVISION, or CONCLUSION."; break;
 }
 if(!str_puts(p,persona)) return 0;
 if(!str_puts(p,"\nQuestion: ")||!str_puts(p,question)) return 0;
 if(n){
  if(!str_puts(p,"\nReasoning so far:\n")) return 0;
  for(size_t i=0;i<n;i++){
   char tag[32]; snprintf(tag,sizeof tag,"%s: ",sllmr_step_kind_name(steps[i].kind));
   if(!str_puts(p,tag)||!str_puts(p,steps[i].text?steps[i].text:"")||!str_puts(p,"\n")) return 0;
  }
 }
 if(backtrack_hint && *backtrack_hint){
  if(!str_puts(p,"\nA previous path was refuted: ")||!str_puts(p,backtrack_hint)
     ||!str_puts(p,"\nDo not repeat that mistake.")) return 0;
 }
 if(!str_puts(p,"\nWrite ONLY the next single step, tagged.")) return 0;
 return 1;
}

/* Confidence heuristic: verdict dominates; reflections/objections are honest
   (they lower premature certainty); conclusions inherit the path's health. */
static double step_confidence(sllmr_subject subject, sllmr_step_kind kind, sllmr_verdict v){
 double base = (v==SLLMR_VERDICT_SUPPORTED)?0.92 : (v==SLLMR_VERDICT_REFUTED)?0.05 : 0.5;
 if(subject==SLLMR_SUBJECT_RELIGION || subject==SLLMR_SUBJECT_PHILOSOPHY){
  /* open subjects: cap confidence; proof is not on offer */
  if(base>0.8) base=0.75;
 }
 if(kind==SLLMR_STEP_OBJECTION || kind==SLLMR_STEP_REFLECTION) base=base*0.6+0.2;
 return base;
}

static sllmr_verdict verify_step(sllmr_engine *e, sllmr_subject subject, const char *q,
                                 const sllmr_step *prior, size_t pn, const sllmr_step *cand,
                                 char *note, size_t ncap){
 if(note&&ncap) note[0]=0;
 if(!e->verifier || !e->verifier->check) return SLLMR_VERDICT_UNKNOWN;
 return e->verifier->check(e->verifier->user_data, subject, q, prior, pn, cand, note, ncap);
}

sllmr_status sllmr_reason(sllmr_engine *e, const char *question,
                          const sllmr_options *opt, const sllmr_trace *trace,
                          sllmr_result *out){
 if(!e||!question||!opt||!out) return SLLMR_INVALID_ARGUMENT;
 if(opt->abi_version!=SLLMR_ABI_VERSION) return SLLMR_UNSUPPORTED;
 memset(out,0,sizeof(*out));

 sllm_generation_options gopt; sllm_generation_options_init(&gopt);
 gopt.seed=opt->seed; gopt.temperature=0.6; gopt.max_new_tokens=192;

 sllmr_step *best=NULL; size_t best_n=0; double best_conf=-1.0; int best_verified=0;
 uint32_t backtracks=0;
 char backtrack_hint[512]; backtrack_hint[0]=0;
 sllmr_status rc=SLLMR_OK;
 str prompt; memset(&prompt,0,sizeof prompt);
 str raw;    memset(&raw,0,sizeof raw);

 for(uint32_t depth=0; depth<=opt->max_backtracks; depth++){
  sllmr_step *steps=(sllmr_step*)calloc(opt->max_steps? opt->max_steps:1,sizeof(sllmr_step));
  if(!steps){ rc=SLLMR_OUT_OF_MEMORY; goto done; }
  size_t n=0; int path_refuted=0; int reached_conclusion=0;

  for(uint32_t s=0; s<opt->max_steps; s++){
   /* Periodic forced reflection keeps the model honest about its own path. */
   int force_reflect = (opt->reflection_every && n>0 && (n % opt->reflection_every)==0);

   if(!build_prompt(&prompt, opt->subject, question, steps, n,
                    depth? backtrack_hint : NULL)){ rc=SLLMR_OUT_OF_MEMORY; free_steps: {
     for(size_t i=0;i<n;i++) free((char*)steps[i].text);
     free(steps); goto done; } }
   if(force_reflect) str_puts(&prompt,"\n(Make this step a REFLECTION that critiques the path so far.)");

   raw.len=0; if(raw.buf) raw.buf[0]=0;
   sllm_status gs=sllm_engine_generate(e->llm, prompt.buf, &gopt, collect_chunk, &raw);
   if(gs==SLLM_ERROR_CANCELLED){ rc=SLLMR_CANCELLED; goto free_steps; }
   if(gs!=SLLM_OK){ rc=SLLMR_BACKEND_ERROR; goto free_steps; }

   const char *body=NULL;
   sllmr_step_kind kind=parse_kind(raw.buf?raw.buf:"", &body);
   if(force_reflect) kind=SLLMR_STEP_REFLECTION;

   sllmr_step cand; memset(&cand,0,sizeof cand);
   cand.index=(uint32_t)n; cand.depth=depth; cand.kind=kind;
   cand.text=body? body : "";

   char note[256];
   sllmr_verdict v=verify_step(e, opt->subject, question, steps, n, &cand, note, sizeof note);
   cand.verdict=v;
   cand.confidence=step_confidence(opt->subject, kind, v);

   /* Persist a heap copy of the step text. */
   char *owned=(char*)malloc(strlen(cand.text)+1);
   if(!owned){ rc=SLLMR_OUT_OF_MEMORY; goto free_steps; }
   strcpy(owned, cand.text); cand.text=owned;
   steps[n]=cand;

   if(trace && trace->on_step){
    if(trace->on_step(trace->user_data,&steps[n])==0){ n++; rc=SLLMR_CANCELLED; goto free_steps; }
   }
   n++;

   if(v==SLLMR_VERDICT_REFUTED){
    path_refuted=1;
    snprintf(backtrack_hint,sizeof backtrack_hint,"%s (%s)",
             steps[n-1].text, note[0]?note:"refuted by verifier");
    if(trace && trace->on_backtrack) trace->on_backtrack(trace->user_data, depth, backtrack_hint);
    break;
   }
   if(kind==SLLMR_STEP_CONCLUSION){ reached_conclusion=1; break; }
  }

  /* Score this path honestly. */
  double sum=0; size_t counted=0; int any_refuted=0; int math_unproven=0;
  for(size_t i=0;i<n;i++){
   if(steps[i].kind==SLLMR_STEP_REFLECTION||steps[i].kind==SLLMR_STEP_OBJECTION) continue;
   sum+=steps[i].confidence; counted++;
   if(steps[i].verdict==SLLMR_VERDICT_REFUTED) any_refuted=1;
   if(opt->subject==SLLMR_SUBJECT_MATH &&
      (steps[i].kind==SLLMR_STEP_DEDUCTION) &&
      steps[i].verdict!=SLLMR_VERDICT_SUPPORTED) math_unproven=1;
  }
  double conf = counted? sum/(double)counted : 0.0;
  int verified = !any_refuted && !(opt->subject==SLLMR_SUBJECT_MATH && math_unproven);

  int acceptable = reached_conclusion && !path_refuted &&
                   (!opt->require_verified || verified);

  if(conf>best_conf){
   /* keep the best path seen for inspection even if not acceptable */
   for(size_t i=0;i<best_n;i++) free((char*)best[i].text);
   free(best);
   best=steps; best_n=n; best_conf=conf; best_verified=verified;
  } else {
   for(size_t i=0;i<n;i++) free((char*)steps[i].text);
   free(steps);
  }

  if(acceptable) break;
  if(depth<opt->max_backtracks) backtracks++;
 }

 /* Assemble the result from the best path. */
 {
  str ans; memset(&ans,0,sizeof ans);
  str sum; memset(&sum,0,sizeof sum);
  const char *concl=NULL;
  for(size_t i=best_n; i>0; i--){ if(best[i-1].kind==SLLMR_STEP_CONCLUSION){ concl=best[i-1].text; break; } }
  if(!concl && best_n) concl=best[best_n-1].text;
  str_puts(&ans, concl? concl : "No conclusion could be reached.");

  str_puts(&sum, "Reached via ");
  char nbuf[64]; snprintf(nbuf,sizeof nbuf,"%zu step(s)", best_n); str_puts(&sum,nbuf);
  if(backtracks){ snprintf(nbuf,sizeof nbuf," after %u backtrack(s)", backtracks); str_puts(&sum,nbuf); }
  str_puts(&sum, " [");
  str_puts(&sum, sllmr_subject_name(opt->subject));
  str_puts(&sum, best_verified? ", verified]" : ", unverified]");

  out->answer=str_take(&ans);
  out->summary=str_take(&sum);
  out->steps=best; out->step_count=best_n;
  out->confidence=best_conf<0?0:best_conf;
  out->verified=best_verified;
  out->tentative = (out->confidence<opt->min_confidence) ||
                   opt->subject==SLLMR_SUBJECT_PHILOSOPHY ||
                   opt->subject==SLLMR_SUBJECT_RELIGION;
  out->backtracks=backtracks;
  best=NULL; best_n=0;
  str_free(&ans); str_free(&sum);
 }

 if(rc==SLLMR_OK && opt->require_verified && !out->verified) rc=SLLMR_NO_SOUND_PATH;

done:
 str_free(&prompt); str_free(&raw);
 if(best){ for(size_t i=0;i<best_n;i++) free((char*)best[i].text); free(best); }
 return rc;
}

void sllmr_result_free(sllmr_result *r){
 if(!r) return;
 free(r->answer); r->answer=NULL;
 free(r->summary); r->summary=NULL;
 if(r->steps){ for(size_t i=0;i<r->step_count;i++) free((char*)r->steps[i].text); free(r->steps); }
 r->steps=NULL; r->step_count=0;
}
