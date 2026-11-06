#include "parametry.h"
#include <string.h>

#define GIB (1024ULL*1024ULL*1024ULL)

/* ---- Registry of KNOWN, supported models (names + declared parameters only;
   NO weights are shipped). LLM sizes sit in the 2..16 GB envelope. The names
   are SLeeLa-side identifiers for model *families* a user may bind by name;
   the user still supplies the actual weights for that family. ---- */

/* Neural-net shapes (borrowed by the registry entries). */
static const size_t kXorShape[]   = {2, 4, 1};        /* classic XOR net */
static const size_t kSmallMlp[]   = {16, 32, 16, 4};  /* small general MLP */

static const sllmp_known_model kRegistry[] = {
 /* --- SLeeLa LLMs (code/language), by family name, within 2..16 GB --- */
 { "sleela-code-compact",  SLLMP_KIND_LLM, 2ULL*GIB,  2048,  "gguf-q4_k_m",     0, NULL,
   "Compact SLeeLa Code LLM (lower envelope). User supplies weights." },
 { "sleela-code-standard", SLLMP_KIND_LLM, 4ULL*GIB,  4096,  "gguf-q5_k_m",     0, NULL,
   "Balanced SLeeLa Code LLM." },
 { "sleela-code-capable",  SLLMP_KIND_LLM, 8ULL*GIB,  8192,  "safetensors-fp16",0, NULL,
   "Capable SLeeLa Code LLM; larger context." },
 { "sleela-code-large",    SLLMP_KIND_LLM, 16ULL*GIB, 16384, "safetensors-fp16",0, NULL,
   "Large SLeeLa Code LLM (upper envelope)." },
 /* --- SLeeLa neural nets, by shape name --- */
 { "sleela-nn-xor",        SLLMP_KIND_NEURAL_NET, 0, 0, NULL, 2, kXorShape,
   "Classic 2-4-1 XOR demonstration net." },
 { "sleela-nn-small-mlp",  SLLMP_KIND_NEURAL_NET, 0, 0, NULL, 3, kSmallMlp,
   "Small general-purpose MLP 16-32-16-4." },
};
static const size_t kRegistryCount = sizeof(kRegistry)/sizeof(kRegistry[0]);

const char *sllmp_status_string(sllmp_status s){
 switch(s){
  case SLLMP_OK: return "ok";
  case SLLMP_INVALID_ARGUMENT: return "invalid argument";
  case SLLMP_UNKNOWN_MODEL: return "unknown model name and no local fallback";
  case SLLMP_NO_SOURCE: return "no source: neither a known name nor a local path";
  case SLLMP_KIND_MISMATCH: return "named model is of a different kind than requested";
  case SLLMP_UNSUPPORTED: return "unsupported";
 }
 return "unknown";
}
const char *sllmp_kind_name(sllmp_kind k){
 return k==SLLMP_KIND_LLM? "llm" : k==SLLMP_KIND_NEURAL_NET? "neural-net" : "unknown";
}
const char *sllmp_source_name(sllmp_source s){
 return s==SLLMP_SOURCE_KNOWN? "known" : s==SLLMP_SOURCE_LOCAL? "local" : "unset";
}

size_t sllmp_known_model_count(void){ return kRegistryCount; }
const sllmp_known_model *sllmp_known_model_at(size_t i){ return i<kRegistryCount? &kRegistry[i] : NULL; }
const sllmp_known_model *sllmp_find_known_model(const char *name){
 if(!name) return NULL;
 for(size_t i=0;i<kRegistryCount;i++) if(strcmp(kRegistry[i].name,name)==0) return &kRegistry[i];
 return NULL;
}

sllmp_status sllmp_resolve(const sllmp_binding *b, sllmp_parameters *out){
 if(!b || !out) return SLLMP_INVALID_ARGUMENT;
 memset(out,0,sizeof(*out));
 out->abi_version=SLLMP_ABI_VERSION;
 out->kind=b->kind;

 int have_name  = (b->name && *b->name);
 int have_local = (b->local_path && *b->local_path);

 if(have_name){
  const sllmp_known_model *m=sllmp_find_known_model(b->name);
  if(!m){
   /* Unknown name: fall back to local if given, else fail closed. */
   if(!have_local) return SLLMP_UNKNOWN_MODEL;
  } else {
   if(m->kind != b->kind) return SLLMP_KIND_MISMATCH;
   out->source=SLLMP_SOURCE_KNOWN;
   out->name_or_path=m->name;
   out->weight_bytes   = b->weight_bytes   ? b->weight_bytes   : m->weight_bytes;
   out->context_tokens = b->context_tokens ? b->context_tokens : m->context_tokens;
   out->weight_format  = b->weight_format  ? b->weight_format  : m->weight_format;
   out->layer_count    = b->layer_count    ? b->layer_count    : m->layer_count;
   out->layer_sizes    = b->layer_sizes    ? b->layer_sizes    : m->layer_sizes;
   return SLLMP_OK;
  }
 }

 if(have_local){
  out->source=SLLMP_SOURCE_LOCAL;
  out->name_or_path=b->local_path;
  out->weight_bytes=b->weight_bytes;
  out->context_tokens=b->context_tokens;
  out->weight_format=b->weight_format;
  out->layer_count=b->layer_count;
  out->layer_sizes=b->layer_sizes;
  /* A local neural net must carry a shape. */
  if(b->kind==SLLMP_KIND_NEURAL_NET && (!out->layer_count || !out->layer_sizes))
   return SLLMP_INVALID_ARGUMENT;
  return SLLMP_OK;
 }

 return SLLMP_NO_SOURCE;
}
