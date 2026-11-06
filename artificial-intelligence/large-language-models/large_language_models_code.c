#include "large_language_models_code.h"

const char *sllmcx_status_string(sllmcx_status s){
 switch(s){
  case SLLMCX_OK: return "ok";
  case SLLMCX_INVALID_ARGUMENT: return "invalid argument";
  case SLLMCX_UNSUPPORTED: return "unsupported";
 }
 return "unknown";
}

const char *sllmcx_language_name(sllmcx_language l){
 switch(l){
  case SLLMCX_LANG_C: return "c";
  case SLLMCX_LANG_FORTRAN: return "fortran";
  case SLLMCX_LANG_PASCAL: return "pascal";
  case SLLMCX_LANG_PYTHON: return "python";
  case SLLMCX_LANG_SHELL: return "shell";
  case SLLMCX_LANG_SLEELA: return "sleela";
  case SLLMCX_LANG_OTHER: return "other";
  default: return "unknown";
 }
}

const char *sllmcx_task_name(sllmcx_task t){
 switch(t){
  case SLLMCX_TASK_SUMMARIZE: return "summarize";
  case SLLMCX_TASK_DRY_RUN: return "dry-run";
  case SLLMCX_TASK_REFACTOR: return "refactor";
  case SLLMCX_TASK_TRANSLATE: return "translate";
  case SLLMCX_TASK_DEBUG: return "debug";
  case SLLMCX_TASK_COMPLETE: return "complete";
 }
 return "task";
}

const char *sllmcx_check_mode_name(sllmcx_check_mode m){
 switch(m){
  case SLLMCX_CHECK_NONE: return "none";
  case SLLMCX_CHECK_COMPILE: return "compile";
  case SLLMCX_CHECK_TEST: return "test";
  case SLLMCX_CHECK_TRACE: return "trace";
 }
 return "unknown";
}

sllmcx_status sllmcx_profile_init(sllmcx_profile *p){
 if(!p) return SLLMCX_INVALID_ARGUMENT;
 p->abi_version=SLLMCX_ABI_VERSION;
 p->model_id=NULL;
 p->base_family=NULL;
 p->primary_language=SLLMCX_LANG_C;
 p->other_language=NULL;
 p->tasks=SLLMCX_TASK_SUMMARIZE|SLLMCX_TASK_DRY_RUN|SLLMCX_TASK_REFACTOR|SLLMCX_TASK_TRANSLATE;
 p->check_mode=SLLMCX_CHECK_TRACE;
 p->fill_in_middle=0;
 return SLLMCX_OK;
}

int sllmcx_profile_has_task(const sllmcx_profile *p, sllmcx_task t){
 return (p && (p->tasks & (uint32_t)t)) ? 1 : 0;
}

const char *sllmcx_profile_language_label(const sllmcx_profile *p){
 if(!p) return "unknown";
 if(p->primary_language==SLLMCX_LANG_OTHER)
  return (p->other_language && *p->other_language)? p->other_language : "other";
 return sllmcx_language_name(p->primary_language);
}
