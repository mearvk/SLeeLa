#include "../../debugger/debugger.hpp"
#include <sstream>
int main(){
 sleela::debugger::DebugSession s;
 sleela::debugger::Breakpoint b; b.function="test_target"; auto id=s.addBreakpoint(b); if(!id)return 1;
 sleela::debugger::DebugEvent e; e.type=sleela::debugger::EventType::Assertion; e.severity="error"; e.message="expected assertion"; e.fields["test"]="debugger.contract"; s.emit(e);
 std::ostringstream out; s.report(out); if(out.str().find("debugger.contract")==std::string::npos)return 2;
 return s.removeBreakpoint(id)?0:3;
}
