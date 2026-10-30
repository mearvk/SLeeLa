/* Context-grounded response — C11 terminal example. */
#include "basis_cli.h"
#include "basis_backend.h"
#include <stdio.h>
static int print_chunk(const char *utf8,size_t byte_count,void *user_data) {
    (void)user_data;
    if(!utf8&&byte_count)return 0;
    if(byte_count&&fwrite(utf8,1u,byte_count,stdout)!=byte_count)return 0;
    (void)fflush(stdout); return 1;
}
int main(int argc,char **argv) {
    sllm_basis_cli cli; char prompt[SLLM_BASIS_PROMPT_CAPACITY];
    sllm_generation_options options; const sllm_backend *backend;
    sllm_engine *engine=NULL; sllm_status status;
    int parsed=sllm_basis_parse_cli(argc,argv,&cli);
    if(parsed==1){sllm_basis_usage(argv[0],"Context-grounded response");return 0;}
    if(parsed!=0||!sllm_basis_format_prompt(prompt,sizeof(prompt),"Answer using only the context supplied below. Cite the relevant context section or wording in plain language. If the context does not support an answer, say so rather than guessing.",&cli)){
        sllm_basis_usage(argv[0],"Context-grounded response");return 2;
    }
    backend=sllm_basis_backend();
    if(!backend){fputs("No LLM provider adapter configured; link an implementation of sllm_basis_backend().\n",stderr);return 3;}
    status=sllm_generation_options_init(&options);
    if(status!=SLLM_OK)return 4;
    options.max_new_tokens=cli.max_new_tokens; options.temperature=cli.temperature;
    options.top_p=cli.top_p; options.seed=cli.seed;
    status=sllm_engine_create(backend,&engine);
    if(status==SLLM_OK)status=sllm_engine_load(engine,cli.model);
    if(status==SLLM_OK)status=sllm_engine_generate(engine,prompt,&options,print_chunk,NULL);
    if(status!=SLLM_OK)fprintf(stderr,"\nLLM operation failed: %s\n",sllm_status_string(status));
    sllm_engine_destroy(engine); putchar('\n');
    return status==SLLM_OK?0:5;
}
