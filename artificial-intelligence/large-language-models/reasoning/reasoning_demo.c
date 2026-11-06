/*
 * reasoning_demo.c — end-to-end demonstration of the SLeeLa RLM layer.
 *
 * It wires a DETERMINISTIC STUB backend (no real model, no network, no weights)
 * and a TINY SYMBOLIC VERIFIER to the reasoning engine, then runs a math
 * question (which must verify and backtrack past a wrong path) and a philosophy
 * question (which stays honestly tentative). This proves the control loop:
 * decompose -> reflect -> verify -> backtrack -> conclude, with honest
 * confidence. It is illustrative, not a trained model.
 *
 * Build:
 *   cc -std=c11 -O2 -Wall -Wextra -pedantic \
 *      ../large_language_models.c ../large_language_models_reasoning.c \
 *      reasoning_demo.c -o reasoning_demo
 */
#include "../large_language_models_reasoning.h"
#include <stdio.h>
#include <string.h>

/* ---- Deterministic stub LLM. It inspects the prompt and emits the next
   tagged step. For the math prompt it deliberately proposes a WRONG deduction
   first so the verifier can refute it and force a backtrack. ---- */
typedef struct { int math_wrong_emitted; } stub_state;

static sllm_status stub_load(void* ud, const char* id){ (void)ud;(void)id; return SLLM_OK; }
static void stub_unload(void* ud){ (void)ud; }

static int has(const char* hay, const char* needle){ return strstr(hay,needle)!=NULL; }

static sllm_status stub_generate(void* ud, const char* prompt,
                                 const sllm_generation_options* o,
                                 sllm_chunk_callback cb, void* cbd){
 (void)o;
 stub_state* st=(stub_state*)ud;
 const char* line;

 int reflect = has(prompt,"Make this step a REFLECTION");
 int math = has(prompt,"sum of the interior angles of a triangle");
 int phil = has(prompt,"Is it ever moral to lie");

 if(reflect){
  line="REFLECTION: Re-examining the path; each stated step should follow from the assumptions.";
 } else if(math){
  if(!has(prompt,"Reasoning so far"))
   line="ASSUMPTION: Work in standard Euclidean plane geometry.";
  else if(has(prompt,"A previous path was refuted")){
   /* After a refutation: prove 180 once, then conclude. */
   if(has(prompt,"180 degrees"))
    line="CONCLUSION: The interior angles of a Euclidean triangle sum to 180 degrees.";
   else
    line="DEDUCTION: By the Euclidean parallel postulate the interior angles sum to 180 degrees.";
  } else if(!st->math_wrong_emitted){ st->math_wrong_emitted=1;
   line="DEDUCTION: The interior angles of a triangle sum to 360 degrees."; }
  else if(has(prompt,"180 degrees"))
   line="CONCLUSION: The interior angles of a Euclidean triangle sum to 180 degrees.";
  else
   line="DEDUCTION: By the Euclidean parallel postulate the interior angles sum to 180 degrees.";
 } else if(phil){
  if(!has(prompt,"Reasoning so far"))
   line="DECOMPOSE: Separate the duty not to deceive from the duty to prevent serious harm.";
  else if(!has(prompt,"OBJECTION"))
   line="OBJECTION: A strict Kantian holds lying is always wrong, even to a murderer at the door.";
  else
   line="CONCLUSION: On balance, lying can be morally permissible to prevent grave harm, though the duty of truthfulness remains weighty and the judgement is contestable.";
 } else {
  line="CONCLUSION: Insufficient information to decide.";
 }
 return cb(line, strlen(line), cbd)? SLLM_OK : SLLM_ERROR_CANCELLED;
}

/* ---- Tiny symbolic verifier. It knows a couple of ground facts and checks the
   candidate step against them. Everything else is UNKNOWN (honest). ---- */
static sllmr_verdict demo_check(void* ud, sllmr_subject subject, const char* q,
                                const sllmr_step* prior, size_t pn,
                                const sllmr_step* cand, char* note, size_t ncap){
 (void)ud;(void)q;(void)prior;(void)pn;
 const char* t=cand->text? cand->text : "";
 if(subject==SLLMR_SUBJECT_MATH){
  if(strstr(t,"360 degrees")){ if(note&&ncap) snprintf(note,ncap,"false: Euclidean triangle angle sum is 180, not 360"); return SLLMR_VERDICT_REFUTED; }
  if(strstr(t,"180 degrees")){ if(note&&ncap) snprintf(note,ncap,"matches the Euclidean theorem"); return SLLMR_VERDICT_SUPPORTED; }
 }
 return SLLMR_VERDICT_UNKNOWN; /* open / interpretive / unknown */
}

static int trace_step(void* ud, const sllmr_step* s){
 (void)ud;
 printf("   [d%u #%u] %-10s %-9s c=%.2f  %s\n",
        s->depth, s->index, sllmr_step_kind_name(s->kind),
        sllmr_verdict_name(s->verdict), s->confidence, s->text);
 return 1;
}
static void trace_bt(void* ud, uint32_t from, const char* why){
 (void)ud; printf("   -- backtrack from depth %u: %s\n", from, why);
}

static int run(const char* title, sllmr_engine* e, const char* q, sllmr_subject subj, int require_verified){
 sllmr_options o; sllmr_options_init(&o);
 o.subject=subj; o.require_verified=require_verified; o.max_steps=8; o.max_backtracks=2;
 sllmr_trace tr={0}; tr.on_step=trace_step; tr.on_backtrack=trace_bt;
 printf("== %s ==\n Q: %s\n", title, q);
 sllmr_result r;
 sllmr_status st=sllmr_reason(e,q,&o,&tr,&r);
 printf(" status=%s  verified=%d  tentative=%d  confidence=%.2f  backtracks=%u\n",
        sllmr_status_string(st), r.verified, r.tentative, r.confidence, r.backtracks);
 printf(" ANSWER: %s\n SUMMARY: %s\n\n", r.answer, r.summary);
 int ok = (st==SLLMR_OK);
 sllmr_result_free(&r);
 return ok;
}

int main(void){
 stub_state state={0};
 sllm_backend be={0};
 be.abi_version=SLLM_ABI_VERSION; be.name="demo-stub"; be.user_data=&state;
 be.load=stub_load; be.unload=stub_unload; be.generate=stub_generate;

 sllm_engine* llm=NULL;
 if(sllm_engine_create(&be,&llm)!=SLLM_OK || sllm_engine_load(llm,"demo")!=SLLM_OK){
  fprintf(stderr,"demo: failed to create/load stub llm\n"); return 1;
 }

 sllmr_verifier ver={0};
 ver.abi_version=SLLMR_ABI_VERSION; ver.name="demo-symbolic"; ver.check=demo_check;

 sllmr_engine* re=NULL;
 if(sllmr_engine_create(llm,&ver,&re)!=SLLMR_OK){ fprintf(stderr,"demo: reasoning create failed\n"); return 1; }

 int ok=1;
 /* Math: must refute the 360 path, backtrack, prove 180, and REQUIRE verification. */
 ok &= run("Mathematics (verified, with backtrack)", re,
           "What is the sum of the interior angles of a triangle?",
           SLLMR_SUBJECT_MATH, 1);
 /* Philosophy: stays honestly tentative, surfaces an objection, no proof-claim. */
 ok &= run("Philosophy (nuanced, tentative)", re,
           "Is it ever moral to lie?",
           SLLMR_SUBJECT_PHILOSOPHY, 0);

 sllmr_engine_destroy(re);
 sllm_engine_unload(llm); sllm_engine_destroy(llm);

 if(!ok){ fprintf(stderr,"demo: a run did not return OK\n"); return 1; }
 printf("reasoning_demo: OK\n");
 return 0;
}
