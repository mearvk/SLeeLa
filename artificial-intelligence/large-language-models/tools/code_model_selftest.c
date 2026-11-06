/* Self-test for the Code LLM profile.
 * Build:
 *   cc -std=c11 -O2 -Wall -Wextra -pedantic \
 *      ../large_language_models_code.c code_model_selftest.c -o code_model_selftest
 */
#include "../large_language_models_code.h"
#include <stdio.h>
#include <string.h>

static int fails=0;
static void expect(int c,const char*w){ if(!c){ fprintf(stderr,"FAIL: %s\n",w); fails++; } }

int main(void){
 sllmcx_profile p; sllmcx_profile_init(&p);
 expect(p.primary_language==SLLMCX_LANG_C, "default language C");
 expect(p.check_mode==SLLMCX_CHECK_TRACE, "default check trace");
 expect(sllmcx_profile_has_task(&p,SLLMCX_TASK_SUMMARIZE), "default has summarize");
 expect(sllmcx_profile_has_task(&p,SLLMCX_TASK_DRY_RUN), "default has dry-run");
 expect(!sllmcx_profile_has_task(&p,SLLMCX_TASK_DEBUG), "default lacks debug");

 expect(strcmp(sllmcx_language_name(SLLMCX_LANG_FORTRAN),"fortran")==0, "fortran name");
 expect(strcmp(sllmcx_task_name(SLLMCX_TASK_TRANSLATE),"translate")==0, "translate name");
 expect(strcmp(sllmcx_check_mode_name(SLLMCX_CHECK_COMPILE),"compile")==0, "compile name");

 /* OTHER language label falls through to the supplied name. */
 p.primary_language=SLLMCX_LANG_OTHER; p.other_language="cobol";
 expect(strcmp(sllmcx_profile_language_label(&p),"cobol")==0, "other label -> cobol");
 p.other_language=NULL;
 expect(strcmp(sllmcx_profile_language_label(&p),"other")==0, "other label fallback");

 /* Task flags compose. */
 p.tasks=SLLMCX_TASK_DEBUG|SLLMCX_TASK_COMPLETE;
 expect(sllmcx_profile_has_task(&p,SLLMCX_TASK_DEBUG) &&
        sllmcx_profile_has_task(&p,SLLMCX_TASK_COMPLETE) &&
        !sllmcx_profile_has_task(&p,SLLMCX_TASK_SUMMARIZE), "task flag composition");

 if(fails){ fprintf(stderr,"code_model_selftest: %d FAIL\n",fails); return 1; }
 printf("code_model_selftest: OK — languages, tasks, check modes, labels\n");
 return 0;
}
