#include "debug_engine_c.h"
#include "debug_engine.hpp"
#include <new>
#include <string>
struct sleela_debug_engine { sleela::debugger::DebugEngine engine; };
extern "C" {
sleela_debug_engine_t* sleela_debug_engine_create(void){return new(std::nothrow)sleela_debug_engine_t{};}
void sleela_debug_engine_destroy(sleela_debug_engine_t*p){delete p;}
uint64_t sleela_debug_engine_add_breakpoint(sleela_debug_engine_t*p,int kind,const char*file,uint32_t line,uint32_t column,const char*function,int temporary){if(!p)return 0;sleela::debugger::BreakpointSpec s; s.kind=static_cast<sleela::debugger::BreakpointKind>(kind);s.location={file?file:"",line,column};s.function=function?function:"";s.temporary=temporary!=0;return p->engine.addBreakpoint(std::move(s));}
int sleela_debug_engine_remove_breakpoint(sleela_debug_engine_t*p,uint64_t id){return p&&p->engine.removeBreakpoint(id)?1:0;}
int sleela_debug_engine_hit_breakpoint(sleela_debug_engine_t*p,uint64_t id){return p&&p->engine.hitBreakpoint(id)?1:0;}
int sleela_debug_engine_capability(sleela_debug_engine_t*p,const char*n){return p&&n&&p->engine.hasCapability(n)?1:0;}
}