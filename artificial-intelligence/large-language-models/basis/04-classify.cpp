/* Text classification — C++17 terminal example. */
#include "basis_cli.h"
#include "basis_backend.h"
#include "../large_language_models.hpp"
#include <exception>
#include <iostream>
#include <string>
int main(int argc,char **argv) {
    sllm_basis_cli cli; char prompt[SLLM_BASIS_PROMPT_CAPACITY];
    const int parsed=sllm_basis_parse_cli(argc,argv,&cli);
    if(parsed==1){sllm_basis_usage(argv[0],"Text classification");return 0;}
    if(parsed!=0||!sllm_basis_format_prompt(prompt,sizeof(prompt),"Classify the supplied text into the most appropriate concise category. Return the category first, then a brief evidence-based explanation. Do not infer sensitive personal attributes.",&cli)){
        sllm_basis_usage(argv[0],"Text classification");return 2;
    }
    const sllm_backend *backend=sllm_basis_backend();
    if(!backend){std::cerr<<"No LLM provider adapter configured; link an implementation of sllm_basis_backend().\n";return 3;}
    try {
        sleela::ai::LanguageModel model(*backend);
        model.load(cli.model);
        sleela::ai::GenerationOptions options;
        options.max_new_tokens=cli.max_new_tokens; options.temperature=cli.temperature;
        options.top_p=cli.top_p; options.seed=cli.seed;
        model.generate(prompt,options,[](std::string_view chunk){
            std::cout.write(chunk.data(),static_cast<std::streamsize>(chunk.size()));
            std::cout.flush(); return static_cast<bool>(std::cout);
        });
        std::cout<<'\n'; return 0;
    } catch(const std::exception& error) {
        std::cerr<<"\nLLM operation failed: "<<error.what()<<'\n'; return 5;
    }
}
