#ifndef SLEELA_AI_LLM_TRAINING_H
#define SLEELA_AI_LLM_TRAINING_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#if defined(_WIN32) && defined(SLEELA_AI_SHARED)
# if defined(SLEELA_AI_BUILD)
#  define SLLMT_API __declspec(dllexport)
# else
#  define SLLMT_API __declspec(dllimport)
# endif
#elif defined(__GNUC__) && defined(SLEELA_AI_SHARED)
# define SLLMT_API __attribute__((visibility("default")))
#else
# define SLLMT_API
#endif
#define SLLMT_ABI_VERSION 1u
typedef struct sllmt_trainer sllmt_trainer;
typedef enum sllmt_status { SLLMT_OK=0,SLLMT_INVALID_ARGUMENT=1,SLLMT_OUT_OF_MEMORY=2,SLLMT_BACKEND_ERROR=3,SLLMT_NOT_READY=4,SLLMT_UNSUPPORTED=5 } sllmt_status;
typedef struct sllmt_options { uint32_t abi_version; double learning_rate; double gradient_clip_norm; uint32_t micro_batch_size; uint32_t accumulation_steps; uint64_t seed; } sllmt_options;
typedef struct sllmt_metrics { double loss; uint64_t tokens_processed; uint64_t optimizer_steps; } sllmt_metrics;
/* Backend and user_data are borrowed and must outlive the trainer. */
typedef struct sllmt_backend {
 uint32_t abi_version; const char *name; void *user_data;
 sllmt_status (*initialize)(void*,const char *model_id,const sllmt_options*);
 void (*shutdown)(void*);
 sllmt_status (*train_batch)(void*,const uint32_t *input_ids,const uint32_t *target_ids,size_t token_count,const sllmt_options*,sllmt_metrics*);
 sllmt_status (*evaluate_batch)(void*,const uint32_t *input_ids,const uint32_t *target_ids,size_t token_count,sllmt_metrics*);
 sllmt_status (*save_checkpoint)(void*,const char *path);
} sllmt_backend;
SLLMT_API sllmt_status sllmt_options_init(sllmt_options *options);
SLLMT_API sllmt_status sllmt_trainer_create(const sllmt_backend *backend,sllmt_trainer **out);
SLLMT_API void sllmt_trainer_destroy(sllmt_trainer *trainer);
SLLMT_API sllmt_status sllmt_trainer_initialize(sllmt_trainer *trainer,const char *model_id,const sllmt_options *options);
SLLMT_API void sllmt_trainer_shutdown(sllmt_trainer *trainer);
SLLMT_API int sllmt_trainer_is_ready(const sllmt_trainer *trainer);
SLLMT_API sllmt_status sllmt_trainer_train_batch(sllmt_trainer *trainer,const uint32_t *input_ids,const uint32_t *target_ids,size_t token_count,const sllmt_options *options,sllmt_metrics *metrics);
SLLMT_API sllmt_status sllmt_trainer_evaluate_batch(sllmt_trainer *trainer,const uint32_t *input_ids,const uint32_t *target_ids,size_t token_count,sllmt_metrics *metrics);
SLLMT_API sllmt_status sllmt_trainer_save_checkpoint(sllmt_trainer *trainer,const char *path);
#ifdef __cplusplus
}
#endif
#endif
