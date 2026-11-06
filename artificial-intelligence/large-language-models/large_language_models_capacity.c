#include "large_language_models_capacity.h"

/* Tier thresholds (bytes). A declared size maps to the nearest tier; sizes
   outside the envelope saturate to COMPACT or LARGE rather than being refused. */
#define GIB (1024ULL*1024ULL*1024ULL)

const char *sllmc_status_string(sllmc_status s){
 switch(s){
  case SLLMC_OK: return "ok";
  case SLLMC_INVALID_ARGUMENT: return "invalid argument";
  case SLLMC_BELOW_MIN: return "below 2 GB envelope minimum (advisory)";
  case SLLMC_ABOVE_MAX: return "above 16 GB envelope maximum (advisory)";
 }
 return "unknown";
}

const char *sllmc_tier_name(sllmc_tier t){
 switch(t){
  case SLLMC_TIER_COMPACT: return "compact";
  case SLLMC_TIER_STANDARD: return "standard";
  case SLLMC_TIER_CAPABLE: return "capable";
  case SLLMC_TIER_LARGE: return "large";
  default: return "unknown";
 }
}

sllmc_status sllmc_profile_init(sllmc_profile *p){
 if(!p) return SLLMC_INVALID_ARGUMENT;
 p->abi_version=SLLMC_ABI_VERSION;
 p->model_id=NULL;
 p->weight_format=NULL;
 p->weight_bytes=4ULL*GIB;   /* STANDARD default; user overrides */
 p->context_tokens=0;        /* derive from tier */
 p->ram_budget_bytes=0;
 p->accelerated=0;
 return SLLMC_OK;
}

sllmc_tier sllmc_tier_for_bytes(uint64_t b){
 if(b==0) return SLLMC_TIER_UNKNOWN;
 /* Boundaries at the geometric midpoints of 2/4/8/16 GB. */
 if(b < 3ULL*GIB)  return SLLMC_TIER_COMPACT;   /* ~2 GB and below */
 if(b < 6ULL*GIB)  return SLLMC_TIER_STANDARD;  /* ~4 GB */
 if(b < 12ULL*GIB) return SLLMC_TIER_CAPABLE;   /* ~8 GB */
 return SLLMC_TIER_LARGE;                        /* ~16 GB and above */
}

sllmc_status sllmc_profile_check(const sllmc_profile *p){
 if(!p) return SLLMC_INVALID_ARGUMENT;
 if(p->weight_bytes==0) return SLLMC_OK; /* unspecified: nothing to warn about */
 if(p->weight_bytes < SLLMC_MIN_BYTES) return SLLMC_BELOW_MIN;
 if(p->weight_bytes > SLLMC_MAX_BYTES) return SLLMC_ABOVE_MAX;
 return SLLMC_OK;
}

sllmc_status sllmc_derive_budget(const sllmc_profile *p, sllmc_budget *out){
 if(!p || !out) return SLLMC_INVALID_ARGUMENT;
 sllmc_tier tier = sllmc_tier_for_bytes(p->weight_bytes);
 if(tier==SLLMC_TIER_UNKNOWN) tier=SLLMC_TIER_STANDARD;

 /* Defaults scale with capacity. These are starting points; the user may
    override any of them (and context via profile->context_tokens). The
    envelope is 2..16 GB, so the four tiers span the whole supported range. */
 switch(tier){
  case SLLMC_TIER_COMPACT:
   out->context_tokens=2048;  out->max_steps=8;  out->max_backtracks=2;
   out->reflection_every=3;   out->max_new_tokens=128; break;
  case SLLMC_TIER_CAPABLE:
   out->context_tokens=8192;  out->max_steps=16; out->max_backtracks=4;
   out->reflection_every=3;   out->max_new_tokens=256; break;
  case SLLMC_TIER_LARGE:
   out->context_tokens=16384; out->max_steps=24; out->max_backtracks=6;
   out->reflection_every=2;   out->max_new_tokens=384; break;
  case SLLMC_TIER_STANDARD:
  default:
   out->context_tokens=4096;  out->max_steps=12; out->max_backtracks=3;
   out->reflection_every=3;   out->max_new_tokens=192; break;
 }
 /* An explicit context request from the user wins. */
 if(p->context_tokens) out->context_tokens=p->context_tokens;
 return SLLMC_OK;
}
