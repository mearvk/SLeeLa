/*
 * parametry_demo.c — bind a SLeeLa LLM and a SLeeLa neural net to EITHER a
 * known/supported model (by name) OR a user's local model, via parametry.
 *
 * It then uses each resolved arrangement: the LLM params feed the capacity
 * layer (2..16 GB envelope); the neural-net params build a real slnn_* net and
 * run one inference. No weights are bundled; a "local" model is just a path the
 * user declares.
 *
 * Build:
 *   cc -std=c11 -O2 -Wall -Wextra -pedantic \
 *      parametry.c ../large_language_models_capacity.c \
 *      ../../neural-nets/neural_nets.c parametry_demo.c -o parametry_demo
 */
#include "parametry.h"
#include "../large_language_models_capacity.h"
#include "../../neural-nets/neural_nets.h"
#include <stdio.h>

#define GIB (1024ULL*1024ULL*1024ULL)
static int fails=0;
static void ok(int c,const char*w){ printf("  %s %s\n", c?"ok:":"FAIL:", w); if(!c) fails++; }

/* Bind an LLM arrangement and report the capacity tier/budget it implies. */
static void bind_llm(const char* title, sllmp_binding* b){
 sllmp_parameters p;
 sllmp_status st=sllmp_resolve(b,&p);
 printf("== %s ==\n status=%s source=%s name/path=%s\n",
        title, sllmp_status_string(st), sllmp_source_name(p.source),
        p.name_or_path? p.name_or_path : "(none)");
 if(st!=SLLMP_OK){ fails++; return; }
 /* Feed the capacity layer. */
 sllmc_profile cp; sllmc_profile_init(&cp);
 cp.weight_bytes=p.weight_bytes; cp.context_tokens=p.context_tokens;
 sllmc_budget bud; sllmc_derive_budget(&cp,&bud);
 printf(" LLM: %llu GB -> tier=%s ctx=%u steps=%u backtracks=%u\n\n",
        (unsigned long long)(p.weight_bytes/GIB),
        sllmc_tier_name(sllmc_tier_for_bytes(p.weight_bytes)),
        bud.context_tokens, bud.max_steps, bud.max_backtracks);
}

/* Bind a neural-net arrangement and actually build + run the slnn_* net. */
static void bind_nn(const char* title, sllmp_binding* b){
 sllmp_parameters p;
 sllmp_status st=sllmp_resolve(b,&p);
 printf("== %s ==\n status=%s source=%s name/path=%s\n",
        title, sllmp_status_string(st), sllmp_source_name(p.source),
        p.name_or_path? p.name_or_path : "(none)");
 if(st!=SLLMP_OK){ fails++; return; }
 printf(" NN shape:");
 for(size_t i=0;i<=p.layer_count;i++) printf(" %zu", p.layer_sizes[i]);
 printf("\n");
 slnn_network_config cfg; 
 cfg.abi_version=SLNN_ABI_VERSION; cfg.layer_count=p.layer_count; cfg.layer_sizes=p.layer_sizes;
 cfg.hidden_activation=SLNN_ACTIVATION_TANH; cfg.output_activation=SLNN_ACTIVATION_SIGMOID; cfg.seed=1;
 slnn_network* net=NULL;
 if(slnn_network_create(&cfg,&net)!=SLNN_OK){ printf(" FAIL: slnn_network_create\n"); fails++; return; }
 double in[64]={0}, out[64]={0};
 size_t ni=slnn_network_input_count(net), no=slnn_network_output_count(net);
 for(size_t i=0;i<ni && i<64;i++) in[i]=0.5;
 slnn_status rs=slnn_network_predict(net,in,ni,out,no);
 printf(" built real slnn_* net (in=%zu out=%zu) predict=%s; first out=%.4f\n\n",
        ni,no, slnn_status_string(rs), out[0]);
 slnn_network_destroy(net);
}

int main(void){
 /* ---- SLeeLa LLM: known-by-name vs local ---- */
 sllmp_binding known_llm={0};
 known_llm.abi_version=SLLMP_ABI_VERSION; known_llm.kind=SLLMP_KIND_LLM;
 known_llm.name="sleela-code-capable";                 /* a supported model name */
 bind_llm("SLeeLa LLM bound to a KNOWN model (sleela-code-capable)", &known_llm);

 sllmp_binding local_llm={0};
 local_llm.abi_version=SLLMP_ABI_VERSION; local_llm.kind=SLLMP_KIND_LLM;
 local_llm.local_path="/models/my-sleela-coder.gguf";  /* a user's local model */
 local_llm.weight_bytes=6ULL*GIB; local_llm.weight_format="gguf-q4_k_m";
 bind_llm("SLeeLa LLM bound to a LOCAL model (/models/my-sleela-coder.gguf)", &local_llm);

 /* ---- SLeeLa neural net: known-by-name vs local ---- */
 sllmp_binding known_nn={0};
 known_nn.abi_version=SLLMP_ABI_VERSION; known_nn.kind=SLLMP_KIND_NEURAL_NET;
 known_nn.name="sleela-nn-xor";                         /* a supported net shape */
 bind_nn("SLeeLa neural net bound to a KNOWN shape (sleela-nn-xor)", &known_nn);

 static const size_t myshape[]={8,12,3};
 sllmp_binding local_nn={0};
 local_nn.abi_version=SLLMP_ABI_VERSION; local_nn.kind=SLLMP_KIND_NEURAL_NET;
 local_nn.local_path="/models/my-net.slnn";             /* a user's local net */
 local_nn.layer_count=2; local_nn.layer_sizes=myshape;
 bind_nn("SLeeLa neural net bound to a LOCAL shape (8-12-3)", &local_nn);

 /* ---- the either/or fails closed ---- */
 printf("== fail-closed checks ==\n");
 sllmp_binding unknown={0}; unknown.abi_version=SLLMP_ABI_VERSION; unknown.kind=SLLMP_KIND_LLM; unknown.name="no-such-model";
 sllmp_parameters tmp;
 ok(sllmp_resolve(&unknown,&tmp)==SLLMP_UNKNOWN_MODEL, "unknown name, no local -> UNKNOWN_MODEL");
 sllmp_binding none={0}; none.abi_version=SLLMP_ABI_VERSION; none.kind=SLLMP_KIND_LLM;
 ok(sllmp_resolve(&none,&tmp)==SLLMP_NO_SOURCE, "no name and no path -> NO_SOURCE");
 sllmp_binding mism={0}; mism.abi_version=SLLMP_ABI_VERSION; mism.kind=SLLMP_KIND_NEURAL_NET; mism.name="sleela-code-standard";
 ok(sllmp_resolve(&mism,&tmp)==SLLMP_KIND_MISMATCH, "LLM name bound as neural-net -> KIND_MISMATCH");

 if(fails){ fprintf(stderr,"parametry_demo: %d FAIL\n",fails); return 1; }
 printf("\nparametry_demo: OK\n");
 return 0;
}
