#ifndef SLEELA_AI_LLM_VOCABULARY_H
#define SLEELA_AI_LLM_VOCABULARY_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#if defined(_WIN32) && defined(SLEELA_AI_SHARED)
# if defined(SLEELA_AI_BUILD)
#  define SLLMV_API __declspec(dllexport)
# else
#  define SLLMV_API __declspec(dllimport)
# endif
#elif defined(__GNUC__) && defined(SLEELA_AI_SHARED)
# define SLLMV_API __attribute__((visibility("default")))
#else
# define SLLMV_API
#endif
#define SLLMV_ABI_VERSION 1u
typedef struct sllmv_vocabulary sllmv_vocabulary;
typedef enum sllmv_status {
 SLLMV_OK=0, SLLMV_INVALID_ARGUMENT=1, SLLMV_OUT_OF_MEMORY=2,
 SLLMV_NOT_FOUND=3, SLLMV_BUFFER_TOO_SMALL=4
} sllmv_status;
SLLMV_API sllmv_status sllmv_vocabulary_create(sllmv_vocabulary **out);
SLLMV_API void sllmv_vocabulary_destroy(sllmv_vocabulary *vocabulary);
SLLMV_API size_t sllmv_vocabulary_size(const sllmv_vocabulary *vocabulary);
/* IDs are stable insertion-order indices. Tokens are exact, non-empty UTF-8 byte strings. */
SLLMV_API sllmv_status sllmv_vocabulary_add(sllmv_vocabulary *vocabulary,const char *token,uint32_t *out_id);
SLLMV_API sllmv_status sllmv_vocabulary_find(const sllmv_vocabulary *vocabulary,const char *token,uint32_t *out_id);
SLLMV_API sllmv_status sllmv_vocabulary_token(const sllmv_vocabulary *vocabulary,uint32_t id,const char **out_token);
/* Encodes whitespace-separated tokens; punctuation is preserved as part of each token.
   Caller owns *out_ids and releases it with sllmv_vocabulary_free_ids. */
SLLMV_API sllmv_status sllmv_vocabulary_encode(const sllmv_vocabulary *vocabulary,const char *text,uint32_t **out_ids,size_t *out_count);
SLLMV_API void sllmv_vocabulary_free_ids(uint32_t *ids);
#ifdef __cplusplus
}
#endif
#endif
