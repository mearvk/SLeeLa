/* Self-test for parametry resolution (known vs local; either/or; fail-closed).
 * Build:
 *   cc -std=c11 -O2 -Wall -Wextra -pedantic parametry.c parametry_selftest.c -o parametry_selftest
 */
#include "parametry.h"
#include <stdio.h>
#include <string.h>

#define GIB (1024ULL*1024ULL*1024ULL)
static int fails=0;
static void expect(int c,const char*w){ if(!c){ fprintf(stderr,"FAIL: %s\n",w); fails++; } }

int main(void){
 /* The registry has entries and lookups work. */
 expect(sllmp_known_model_count()>=4, "registry non-trivial");
 expect(sllmp_find_known_model("sleela-code-standard")!=NULL, "find known LLM");
 expect(sllmp_find_known_model("sleela-nn-xor")!=NULL, "find known NN");
 expect(sllmp_find_known_model("nope")==NULL, "unknown -> NULL");

 sllmp_parameters p;

 /* Known LLM resolves with registry params. */
 sllmp_binding kl={0}; kl.abi_version=SLLMP_ABI_VERSION; kl.kind=SLLMP_KIND_LLM; kl.name="sleela-code-large";
 expect(sllmp_resolve(&kl,&p)==SLLMP_OK && p.source==SLLMP_SOURCE_KNOWN && p.weight_bytes==16ULL*GIB,
        "known LLM -> 16 GB from registry");

 /* Caller override of a known entry's context wins. */
 kl.context_tokens=1234;
 expect(sllmp_resolve(&kl,&p)==SLLMP_OK && p.context_tokens==1234, "override context on known");

 /* Local LLM accepts caller params. */
 sllmp_binding ll={0}; ll.abi_version=SLLMP_ABI_VERSION; ll.kind=SLLMP_KIND_LLM;
 ll.local_path="/m/x.gguf"; ll.weight_bytes=5ULL*GIB;
 expect(sllmp_resolve(&ll,&p)==SLLMP_OK && p.source==SLLMP_SOURCE_LOCAL && p.weight_bytes==5ULL*GIB,
        "local LLM -> caller params");

 /* Known NN resolves with a shape. */
 sllmp_binding kn={0}; kn.abi_version=SLLMP_ABI_VERSION; kn.kind=SLLMP_KIND_NEURAL_NET; kn.name="sleela-nn-xor";
 expect(sllmp_resolve(&kn,&p)==SLLMP_OK && p.layer_count==2 && p.layer_sizes && p.layer_sizes[0]==2,
        "known NN -> 2-4-1 shape");

 /* Local NN requires a shape. */
 static const size_t shp[]={3,5,1};
 sllmp_binding ln={0}; ln.abi_version=SLLMP_ABI_VERSION; ln.kind=SLLMP_KIND_NEURAL_NET;
 ln.local_path="/m/n.slnn"; ln.layer_count=2; ln.layer_sizes=shp;
 expect(sllmp_resolve(&ln,&p)==SLLMP_OK && p.source==SLLMP_SOURCE_LOCAL && p.layer_sizes[1]==5,
        "local NN -> caller shape");
 sllmp_binding ln_bad={0}; ln_bad.abi_version=SLLMP_ABI_VERSION; ln_bad.kind=SLLMP_KIND_NEURAL_NET; ln_bad.local_path="/m/n.slnn";
 expect(sllmp_resolve(&ln_bad,&p)==SLLMP_INVALID_ARGUMENT, "local NN without shape -> INVALID");

 /* Either/or fail-closed. */
 sllmp_binding unk={0}; unk.abi_version=SLLMP_ABI_VERSION; unk.kind=SLLMP_KIND_LLM; unk.name="ghost";
 expect(sllmp_resolve(&unk,&p)==SLLMP_UNKNOWN_MODEL, "unknown name no local -> UNKNOWN_MODEL");
 /* Unknown name WITH local fallback -> local. */
 unk.local_path="/m/fallback.gguf"; unk.weight_bytes=3ULL*GIB;
 expect(sllmp_resolve(&unk,&p)==SLLMP_OK && p.source==SLLMP_SOURCE_LOCAL, "unknown name + local -> local fallback");
 sllmp_binding none={0}; none.abi_version=SLLMP_ABI_VERSION; none.kind=SLLMP_KIND_LLM;
 expect(sllmp_resolve(&none,&p)==SLLMP_NO_SOURCE, "no source -> NO_SOURCE");
 sllmp_binding mis={0}; mis.abi_version=SLLMP_ABI_VERSION; mis.kind=SLLMP_KIND_NEURAL_NET; mis.name="sleela-code-standard";
 expect(sllmp_resolve(&mis,&p)==SLLMP_KIND_MISMATCH, "kind mismatch -> KIND_MISMATCH");

 if(fails){ fprintf(stderr,"parametry_selftest: %d FAIL\n",fails); return 1; }
 printf("parametry_selftest: OK — registry, known/local resolution, overrides, fail-closed\n");
 return 0;
}
