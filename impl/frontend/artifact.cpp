// ===========================================================================
// artifact.cpp -- Sleelvac compiler -> persistent .sleela artifact.
// ===========================================================================
#include "artifact.h"
#include "compiler.h"
#include "native_api.h"
#include "chemistry_api.h"
#include "financial_api.h"
#include "../core/sleela_core.h"
#include <algorithm>
#include <stdexcept>
#include <string>
#include <utility>
#include <fstream>
namespace sleela {
static void lowerNativeModules(Program&prog){chemistry::lowerProgram(prog);financial::lowerProgram(prog);auto saved=prog.imports;prog.imports.erase(std::remove(prog.imports.begin(),prog.imports.end(),"chemistry"),prog.imports.end());prog.imports.erase(std::remove(prog.imports.begin(),prog.imports.end(),"financial"),prog.imports.end());native::lowerProgram(prog);prog.imports=std::move(saved);}
int compileToArtifact(Program&prog,const std::string&outputPath,const catalog::Catalog*cat,const SyntaxVersion&syntax){
    if(outputPath.empty()) throw std::runtime_error("artifact output path must not be empty");
    lowerNativeModules(prog);
    SLVM*vm=slvm_new();
    if(!vm) throw std::runtime_error("unable to allocate Sleela VM");
    try{
        compile(prog,vm,cat,syntax);
        int rc=slvm_save_file(vm,outputPath.c_str());
        if(rc!=0) throw std::runtime_error("unable to write runnable .sleela artifact '"+outputPath+"' (error "+std::to_string(rc)+")");
        std::ofstream meta(outputPath+".annotations",std::ios::trunc);
        if(!meta) throw std::runtime_error("unable to write annotation metadata '"+outputPath+".annotations'");
        for(const auto& a:prog.annotations.all()) meta<<a.name<<"="<<a.value<<"\n";
        if(!meta) throw std::runtime_error("failed while writing annotation metadata '"+outputPath+".annotations'");
        slvm_free(vm);
        return 0;
    }catch(...){
        slvm_free(vm);
        throw;
    }
}
} // namespace sleela