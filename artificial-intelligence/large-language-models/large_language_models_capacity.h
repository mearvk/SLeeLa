#ifndef SLEELA_AI_LLM_CAPACITY_H
#define SLEELA_AI_LLM_CAPACITY_H
/*
 * SLeeLa LLM model-capacity profile.
 *
 * A declarative, USER-OWNED description of the model the caller intends to run,
 * within a supported envelope of roughly 2 GB to 16 GB of model weights. The
 * SLeeLa layer ships NO weights and picks NO model: it reads the caller's
 * profile and adapts its own behavior (context budget, reasoning depth,
 * backtracks) to the declared capacity. Which model, which size, and where the
 * weights come from are entirely the user's choice.
 *
 * The envelope bounds are advisory guardrails, not hard refusals: a caller may
 * declare a size outside 2..16 GB, and the clamp helpers report it so a host
 * can warn, but nothing here loads or validates actual weights.
 */
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#if defined(_WIN32) && defined(SLEELA_AI_SHARED)
# if defined(SLEELA_AI_BUILD)
#  define SLLMC_API __declspec(dllexport)
# else
#  define SLLMC_API __declspec(dllimport)
# endif
#elif defined(__GNUC__) && defined(SLEELA_AI_SHARED)
# define SLLMC_API __attribute__((visibility("default")))
#else
# define SLLMC_API
#endif

#define SLLMC_ABI_VERSION 1u

/* Supported model-weight envelope. Bytes, so hosts can compare precisely. */
#define SLLMC_MIN_BYTES  (2ULL  * 1024ULL * 1024ULL * 1024ULL)   /*  2 GiB */
#define SLLMC_MAX_BYTES  (16ULL * 1024ULL * 1024ULL * 1024ULL)   /* 16 GiB */

/* Coarse tiers across the 2..16 GB envelope. A tier is derived from the
   declared weight size; it steers default context + reasoning budgets. The
   user may always override the derived defaults. */
typedef enum sllmc_tier {
 SLLMC_TIER_UNKNOWN=0,
 SLLMC_TIER_COMPACT=1,   /* ~2 GB  : small/quantized; modest context + depth */
 SLLMC_TIER_STANDARD=2,  /* ~4 GB  : balanced default */
 SLLMC_TIER_CAPABLE=3,   /* ~8 GB  : larger context + deeper reasoning */
 SLLMC_TIER_LARGE=4      /* ~16 GB : widest context + deepest reasoning */
} sllmc_tier;

typedef enum sllmc_status {
 SLLMC_OK=0, SLLMC_INVALID_ARGUMENT=1, SLLMC_BELOW_MIN=2, SLLMC_ABOVE_MAX=3
} sllmc_status;

/* The user-owned capacity profile. model_id/weight_format are borrowed strings
   (may be NULL). Sizes are in bytes; zero means "unspecified". */
typedef struct sllmc_profile {
 uint32_t abi_version;
 const char *model_id;        /* caller's identifier for THEIR model (not shipped) */
 const char *weight_format;   /* e.g. "gguf-q4_k_m", "safetensors-fp16" (informational) */
 uint64_t weight_bytes;       /* on-disk/in-memory weight size the user declares */
 uint32_t context_tokens;     /* 0 => derive a tier default */
 uint64_t ram_budget_bytes;   /* host memory the user allots; 0 => unspecified */
 int      accelerated;        /* nonzero if a GPU/accelerator backend is used */
} sllmc_profile;

/* Reasoning budget the engine should use for a given profile. These are the
   knobs the RLM layer (sllmr_options) consumes; the user may override any. */
typedef struct sllmc_budget {
 uint32_t context_tokens;   /* effective prompt+generation window */
 uint32_t max_steps;        /* chain-of-thought length cap */
 uint32_t max_backtracks;   /* abandon-and-retry budget */
 uint32_t reflection_every; /* forced self-critique cadence */
 uint32_t max_new_tokens;   /* per-step generation cap */
} sllmc_budget;

SLLMC_API const char *sllmc_status_string(sllmc_status status);
SLLMC_API const char *sllmc_tier_name(sllmc_tier tier);

/* Initialize a profile to safe defaults (STANDARD tier, 4 GB, unspecified RAM). */
SLLMC_API sllmc_status sllmc_profile_init(sllmc_profile *profile);

/* Classify a declared weight size into a tier. A size of 0 => UNKNOWN. Sizes
   outside the envelope still classify to the nearest tier (COMPACT/LARGE). */
SLLMC_API sllmc_tier sllmc_tier_for_bytes(uint64_t weight_bytes);

/* Report whether weight_bytes sits inside the 2..16 GB envelope. Advisory:
   returns SLLMC_BELOW_MIN / SLLMC_ABOVE_MAX without modifying anything. */
SLLMC_API sllmc_status sllmc_profile_check(const sllmc_profile *profile);

/* Derive a reasoning budget from the profile. Honors an explicit
   profile->context_tokens when nonzero; otherwise uses the tier default. Always
   succeeds for a non-NULL profile (uses STANDARD defaults for UNKNOWN). */
SLLMC_API sllmc_status sllmc_derive_budget(const sllmc_profile *profile, sllmc_budget *out_budget);

#ifdef __cplusplus
}
#endif
#endif
