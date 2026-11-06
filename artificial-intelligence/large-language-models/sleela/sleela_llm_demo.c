/*
 * sleela_llm_demo.c — a Code LLM specialized for the SLeeLa language.
 *
 * Wires a deterministic stub SLeeLa-coding backend to the reasoning engine with
 * the REAL SLeeLa static verifier (sllmsl_*). The backend first proposes code
 * that violates a SLeeLa rule (a method named `record`, which is reserved); the
 * verifier refutes it; the engine backtracks and proposes valid SLeeLa with a
 * renamed method and the Metadata Class metapattern. No model/weights bundled.
 *
 * Build:
 *   cc -std=c11 -O2 -Wall -Wextra -pedantic \
 *      ../large_language_models.c ../large_language_models_reasoning.c \
 *      sleela_code_verifier.c sleela_llm_demo.c -o sleela_llm_demo
 */
#include "../large_language_models_reasoning.h"
#include "sleela_code_verifier.h"
#include <stdio.h>
#include <string.h>

typedef struct { int wrong_emitted; } stub_state;
static int has(const char* h, const char* n){ return strstr(h,n)!=NULL; }

static sllm_status sl_load(void* ud, const char* id){ (void)ud;(void)id; return SLLM_OK; }
static void sl_unload(void* ud){ (void)ud; }

static sllm_status sl_generate(void* ud, const char* prompt,
                               const sllm_generation_options* o,
                               sllm_chunk_callback cb, void* cbd){
 (void)o; stub_state* st=(stub_state*)ud; const char* line;
 if(has(prompt,"Make this step a REFLECTION")){
  line="REFLECTION: Check the draft against SLeeLa rules: reserved words, comment form, Metadata header.";
 } else if(!has(prompt,"Reasoning so far")){
  line="DECOMPOSE: Define a logger class with one method that writes a line.";
 } else if(has(prompt,"A previous path was refuted")){
  /* After the refutation: once a valid draft is on the path, conclude; else
     emit the corrected valid draft (renamed method + Metadata + << >> comment). */
  if(has(prompt,"writeLine"))
   line="CONCLUSION: Final SLeeLa: << logger >> class Logger { "
        "// ---- Descriptive metadata (version 1.11 nested-friendly accessors) ---- "
        "String metaAuthor = \"SLeeLa project\"; void writeLine(String s) { print s; } }";
  else
   line="DEDUCTION: Draft: << logger >> class Logger { "
        "// ---- Descriptive metadata (version 1.11 nested-friendly accessors) ---- "
        "String metaAuthor = \"SLeeLa project\"; void writeLine(String s) { print s; } }";
 } else if(!st->wrong_emitted){ st->wrong_emitted=1;
  /* Invalid SLeeLa: 'record' is a reserved keyword used as a method name. */
  line="DEDUCTION: Draft: class Logger { void record(String s) { print s; } }";
 } else if(has(prompt,"writeLine")){
  line="CONCLUSION: Final SLeeLa: << logger >> class Logger { "
       "// ---- Descriptive metadata (version 1.11 nested-friendly accessors) ---- "
       "String metaAuthor = \"SLeeLa project\"; void writeLine(String s) { print s; } }";
 } else {
  line="DEDUCTION: Draft: << logger >> class Logger { "
       "// ---- Descriptive metadata (version 1.11 nested-friendly accessors) ---- "
       "String metaAuthor = \"SLeeLa project\"; void writeLine(String s) { print s; } }";
 }
 return cb(line, strlen(line), cbd)? SLLM_OK : SLLM_ERROR_CANCELLED;
}

static int on_step(void* ud, const sllmr_step* s){
 (void)ud;
 printf("   [d%u #%u] %-10s %-9s c=%.2f\n     %s\n",
        s->depth, s->index, sllmr_step_kind_name(s->kind),
        sllmr_verdict_name(s->verdict), s->confidence, s->text);
 return 1;
}
static void on_bt(void* ud, uint32_t from, const char* why){
 (void)ud; printf("   -- backtrack from depth %u: %s\n", from, why);
}

int main(void){
 stub_state state={0};
 sllm_backend be={0};
 be.abi_version=SLLM_ABI_VERSION; be.name="sleela-code-stub"; be.user_data=&state;
 be.load=sl_load; be.unload=sl_unload; be.generate=sl_generate;

 sllm_engine* llm=NULL;
 if(sllm_engine_create(&be,&llm)!=SLLM_OK || sllm_engine_load(llm,"sleela-code-llm")!=SLLM_OK){
  fprintf(stderr,"demo: stub llm setup failed\n"); return 1; }

 /* The soundness half: the REAL SLeeLa static verifier. */
 sllmsl_options vo; sllmsl_options_init(&vo);
 sllmr_verifier ver=sllmsl_as_reasoning_verifier(&vo);

 sllmr_engine* re=NULL;
 if(sllmr_engine_create(llm,&ver,&re)!=SLLMR_OK){ fprintf(stderr,"demo: reasoning create failed\n"); return 1; }

 sllmr_options o; sllmr_options_init(&o);
 o.subject=SLLMR_SUBJECT_CODE; o.require_verified=1; o.max_steps=8; o.max_backtracks=2;
 sllmr_trace tr={0}; tr.on_step=on_step; tr.on_backtrack=on_bt;

 printf("== SLeeLa Code LLM: write a logger class (verified against SLeeLa rules) ==\n");
 printf(" verifier: %s\n", sllmr_engine_verifier_name(re));
 sllmr_result r;
 sllmr_status st=sllmr_reason(re,"Write a small SLeeLa class that logs a line.",&o,&tr,&r);
 printf(" status=%s  verified=%d  confidence=%.2f  backtracks=%u\n",
        sllmr_status_string(st), r.verified, r.confidence, r.backtracks);
 printf(" ANSWER: %s\n", r.answer);
 int ok = (st==SLLMR_OK && r.verified);
 sllmr_result_free(&r);

 sllmr_engine_destroy(re);
 sllm_engine_unload(llm); sllm_engine_destroy(llm);
 if(!ok){ fprintf(stderr,"sleela_llm_demo: did not reach a verified SLeeLa answer\n"); return 1; }
 printf("sleela_llm_demo: OK\n");
 return 0;
}
