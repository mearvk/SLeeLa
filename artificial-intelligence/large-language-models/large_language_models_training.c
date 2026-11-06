#include "large_language_models_training.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
struct sllmt_trainer { sllmt_backend backend; int ready; };
static int valid_backend(const sllmt_backend *b){return b&&b->abi_version==SLLMT_ABI_VERSION&&b->name&&*b->name&&b->initialize&&b->shutdown&&b->train_batch&&b->evaluate_batch&&b->save_checkpoint;}
static int valid_options(const sllmt_options *o){return o&&o->abi_version==SLLMT_ABI_VERSION&&isfinite(o->learning_rate)&&o->learning_rate>0&&isfinite(o->gradient_clip_norm)&&o->gradient_clip_norm>=0&&o->micro_batch_size>0&&o->accumulation_steps>0;}
sllmt_status sllmt_options_init(sllmt_options *o){if(!o)return SLLMT_INVALID_ARGUMENT;o->abi_version=SLLMT_ABI_VERSION;o->learning_rate=0.0001;o->gradient_clip_norm=1.0;o->micro_batch_size=1;o->accumulation_steps=1;o->seed=0;return SLLMT_OK;}
sllmt_status sllmt_trainer_create(const sllmt_backend *b,sllmt_trainer **out){if(!out||!valid_backend(b))return SLLMT_INVALID_ARGUMENT;*out=NULL;sllmt_trainer *t=(sllmt_trainer*)calloc(1,sizeof(*t));if(!t)return SLLMT_OUT_OF_MEMORY;t->backend=*b;*out=t;return SLLMT_OK;}
void sllmt_trainer_shutdown(sllmt_trainer *t){if(t&&t->ready){t->backend.shutdown(t->backend.user_data);t->ready=0;}}
void sllmt_trainer_destroy(sllmt_trainer *t){if(t){sllmt_trainer_shutdown(t);free(t);}}
sllmt_status sllmt_trainer_initialize(sllmt_trainer *t,const char *id,const sllmt_options *o){if(!t||!id||!*id||!valid_options(o))return SLLMT_INVALID_ARGUMENT;sllmt_trainer_shutdown(t);sllmt_status s=t->backend.initialize(t->backend.user_data,id,o);if(s==SLLMT_OK)t->ready=1;return s;}
int sllmt_trainer_is_ready(const sllmt_trainer *t){return t?t->ready:0;}
sllmt_status sllmt_trainer_train_batch(sllmt_trainer *t,const uint32_t *in,const uint32_t *target,size_t n,const sllmt_options *o,sllmt_metrics *m){if(!t||!in||!target||!n||!valid_options(o)||!m)return SLLMT_INVALID_ARGUMENT;if(!t->ready)return SLLMT_NOT_READY;memset(m,0,sizeof(*m));return t->backend.train_batch(t->backend.user_data,in,target,n,o,m);}
sllmt_status sllmt_trainer_evaluate_batch(sllmt_trainer *t,const uint32_t *in,const uint32_t *target,size_t n,sllmt_metrics *m){if(!t||!in||!target||!n||!m)return SLLMT_INVALID_ARGUMENT;if(!t->ready)return SLLMT_NOT_READY;memset(m,0,sizeof(*m));return t->backend.evaluate_batch(t->backend.user_data,in,target,n,m);}
sllmt_status sllmt_trainer_save_checkpoint(sllmt_trainer *t,const char *path){if(!t||!path||!*path)return SLLMT_INVALID_ARGUMENT;if(!t->ready)return SLLMT_NOT_READY;return t->backend.save_checkpoint(t->backend.user_data,path);}
