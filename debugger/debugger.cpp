#include "debugger.hpp"
#include <algorithm>
namespace sleela::debugger {
static const char* name(EventType t) {
 switch(t) {
  case EventType::Info:return "info"; case EventType::Breakpoint:return "breakpoint"; case EventType::Watchpoint:return "watchpoint";
  case EventType::Exception:return "exception"; case EventType::Assertion:return "assertion"; case EventType::Thread:return "thread";
  case EventType::Log:return "log"; case EventType::Coverage:return "coverage"; case EventType::Sanitizer:return "sanitizer";
  case EventType::Test:return "test"; case EventType::Regression:return "regression"; case EventType::Crash:return "crash";
 }
 return "unknown";
}
std::uint64_t DebugSession::addBreakpoint(Breakpoint b){ b.id=next_id_++; breakpoints_.push_back(std::move(b)); return breakpoints_.back().id; }
std::uint64_t DebugSession::addWatchpoint(Watchpoint w){ w.id=next_id_++; watchpoints_.push_back(std::move(w)); return watchpoints_.back().id; }
bool DebugSession::removeBreakpoint(std::uint64_t id){ auto n=breakpoints_.size(); breakpoints_.erase(std::remove_if(breakpoints_.begin(),breakpoints_.end(),[&](const Breakpoint& b){return b.id==id;}),breakpoints_.end()); return n!=breakpoints_.size(); }
bool DebugSession::removeWatchpoint(std::uint64_t id){ auto n=watchpoints_.size(); watchpoints_.erase(std::remove_if(watchpoints_.begin(),watchpoints_.end(),[&](const Watchpoint& w){return w.id==id;}),watchpoints_.end()); return n!=watchpoints_.size(); }
void DebugSession::emit(DebugEvent e){ events_.push_back(std::move(e)); }
void DiagnosticReport::write(std::ostream& out,const std::vector<DebugEvent>& es){
 out<<"SLeeLa Debugger Diagnostic Report\n"<<"events="<<es.size()<<"\n";
 for(const auto& e:es){ out<<"["<<e.severity<<"] "<<name(e.type)<<": "<<e.message; if(!e.thread.empty())out<<" thread="<<e.thread; if(!e.location.file.empty())out<<" "<<e.location.file<<":"<<e.location.line<<":"<<e.location.column; if(!e.stack.empty()){out<<" stack=";for(size_t i=0;i<e.stack.size();++i){if(i)out<<" <- ";out<<e.stack[i].function;}} if(!e.fields.empty()){out<<" fields=";bool first=true;for(const auto& p:e.fields){if(!first)out<<",";first=false;out<<p.first<<"="<<p.second;}} out<<"\n"; }
}
void DebugSession::report(std::ostream& out) const { DiagnosticReport::write(out,events_); }
}
