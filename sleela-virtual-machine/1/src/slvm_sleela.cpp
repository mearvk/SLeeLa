#include "slvm_sleela.hpp"
#include "../../impl/frontend/lexer.h"
#include "../../impl/frontend/parser.h"
#include "../../impl/frontend/compiler.h"
#include "../../impl/core/sleela_core.h"
#include <fstream>
#include <sstream>
namespace sleela::vm {
static execution_result run_program(Program& program) {
    SLVM* core=slvm_new();
    if(!core)return{-1,"unable to allocate authoritative SLeeLa Core VM"};
    try {
        sleela::compile(program,core);
        SLResult r=slvm_run(core);
        execution_result out{static_cast<int>(r),slvm_error(core)?slvm_error(core):""};
        slvm_free(core); return out;
    } catch(const std::exception&e){slvm_free(core);return{-1,e.what()};}
}
execution_result execute_source(const std::string& source){
    try { Lexer lexer(source); Parser parser(lexer.tokenize()); Program program=parser.parseProgram(); return run_program(program); }
    catch(const std::exception&e){return{-1,e.what()};}
}
execution_result execute_source_file(const std::string& path){
    std::ifstream in(path); if(!in)return{-1,"unable to open SLeeLa source file: "+path};
    std::ostringstream s;s<<in.rdbuf();return execute_source(s.str());
}
execution_result execute_artifact(const std::string& path){
    char error[512]={};
    if(!slvm_validate_artifact_file(path.c_str(),error,sizeof(error)))return{-1,error[0]?error:"invalid SLeeLa artifact"};
    SLVM* core=slvm_load_file(path.c_str());if(!core)return{-1,"unable to load SLeeLa artifact: "+path};
    SLResult r=slvm_run(core);execution_result out{static_cast<int>(r),slvm_error(core)?slvm_error(core):""};slvm_free(core);return out;
}
}
