#include "debugger.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
using namespace sleela::debugger;
static int self_test(){
 DebugSession s; Breakpoint b; b.function="annotation::ForwardingPolicy::valid"; b.location={"impl/annotation/ForwardingAnnotation.cpp",20,1}; auto id=s.addBreakpoint(b);
 Watchpoint w; w.expression="nexter_colony"; s.addWatchpoint(w);
 DebugEvent e; e.type=EventType::Test; e.message="annotation runtime self-test"; e.thread="self-test"; e.location=b.location; e.fields["test"]="annotation.runtime"; s.emit(e);
 std::ostringstream out; s.report(out); if(out.str().find("annotation.runtime")==std::string::npos)return 1; return s.removeBreakpoint(id)?0:2;
}
int main(int argc,char** argv){
 if(argc==2 && std::string(argv[1])=="--self-test") return self_test();
 if(argc==3 && std::string(argv[1])=="--report"){std::ofstream out(argv[2]);if(!out)return 2;DebugSession s;DebugEvent e;e.message="empty diagnostic session";s.emit(e);s.report(out);return 0;}
 std::cerr<<"Usage: sleela-debugger --self-test | --report FILE\n"; return 2;
}
