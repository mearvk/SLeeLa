#ifndef SLEELA_AI_LLM_BASE_VOCABULARY_H
#define SLEELA_AI_LLM_BASE_VOCABULARY_H
/*
 * SLeeLa LLM base (seed) vocabulary loader.
 *
 * Populates an sllmv_vocabulary with a careful, provenance-tracked seed word
 * set so a model has stable, known premises to index into before any corpus
 * tokens are added. Special tokens occupy fixed low IDs (pad=0, unk=1, ...),
 * after which ordinary tokens follow in declaration order. Because the
 * underlying store assigns stable insertion-order IDs, loading the base set
 * first guarantees the reserved IDs are exactly as declared.
 *
 * This loader builds the token-ID table only. It never imports dictionary
 * definitions; the lexical corpus is a separate store (see the corpus->
 * tokenizer builder). The bundled seed data (tokenizers/base-vocabulary.json)
 * is original, CC0-licensed SLeeLa authorship and carries no third-party
 * licensing obligation.
 */
#include "large_language_models_vocabulary.h"
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#if defined(_WIN32) && defined(SLEELA_AI_SHARED)
# if defined(SLEELA_AI_BUILD)
#  define SLLMBV_API __declspec(dllexport)
# else
#  define SLLMBV_API __declspec(dllimport)
# endif
#elif defined(__GNUC__) && defined(SLEELA_AI_SHARED)
# define SLLMBV_API __attribute__((visibility("default")))
#else
# define SLLMBV_API
#endif

#define SLLMBV_ABI_VERSION 1u

/* Special-token roles, mirroring the "role" enum of base-vocabulary.schema.json.
   SLLMBV_ROLE_COUNT is not a role; it bounds the array. */
typedef enum sllmbv_role {
 SLLMBV_ROLE_PAD=0, SLLMBV_ROLE_UNK=1, SLLMBV_ROLE_BOS=2, SLLMBV_ROLE_EOS=3,
 SLLMBV_ROLE_SEP=4, SLLMBV_ROLE_MASK=5, SLLMBV_ROLE_SYSTEM=6, SLLMBV_ROLE_USER=7,
 SLLMBV_ROLE_ASSISTANT=8, SLLMBV_ROLE_TOOL=9, SLLMBV_ROLE_RESERVED=10,
 SLLMBV_ROLE_COUNT=11
} sllmbv_role;

typedef enum sllmbv_status {
 SLLMBV_OK=0, SLLMBV_INVALID_ARGUMENT=1, SLLMBV_OUT_OF_MEMORY=2,
 SLLMBV_PARSE_ERROR=3, SLLMBV_ID_MISMATCH=4, SLLMBV_DUPLICATE_TOKEN=5,
 SLLMBV_ROLE_UNSET=6
} sllmbv_status;

/* The resolved special-token IDs after a base vocabulary is loaded.
   An entry is SLLMBV_NO_ID when that role was not declared in the data. */
#define SLLMBV_NO_ID 0xFFFFFFFFu
typedef struct sllmbv_specials {
 uint32_t id[SLLMBV_ROLE_COUNT];
} sllmbv_specials;

/* Load the compiled-in default seed set (sleela-base-en-1.0) into an empty
   vocabulary. The vocabulary MUST be empty; the loader verifies that each
   declared special/token lands at its declared ID and fails closed on any
   mismatch or duplicate. On success, *specials (if non-NULL) receives the
   resolved IDs for every declared role. */
SLLMBV_API sllmbv_status sllmbv_load_default(sllmv_vocabulary *vocabulary, sllmbv_specials *specials);

/* Load a base set from a parsed, validated descriptor. Prefer sllmbv_load_json
   when reading base-vocabulary.json; this entry is for callers that already
   hold token arrays. specials_tokens[i]/specials_roles[i] describe the reserved
   tokens (in ascending id order from 0); tokens[] are the ordinary tokens that
   follow. */
SLLMBV_API sllmbv_status sllmbv_load_arrays(
 sllmv_vocabulary *vocabulary,
 const char *const *special_tokens, const sllmbv_role *special_roles, size_t special_count,
 const char *const *tokens, size_t token_count,
 sllmbv_specials *specials);

/* Convenience: look up a role's token ID from a resolved specials struct.
   Returns SLLMBV_NO_ID if the role was not declared. */
SLLMBV_API uint32_t sllmbv_special_id(const sllmbv_specials *specials, sllmbv_role role);

/* Human-readable, static string for a status code (never NULL). */
SLLMBV_API const char *sllmbv_status_string(sllmbv_status status);

/* The identifier of the compiled-in default seed set, e.g. "sleela-base-en-1.0". */
SLLMBV_API const char *sllmbv_default_vocabulary_id(void);

#ifdef __cplusplus
}
#endif
#endif
