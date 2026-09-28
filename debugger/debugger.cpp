#include "debugger.hpp"
#include <algorithm>
#include <utility>
#include <utility>

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

const char* stopReasonName(StopReason r) noexcept {
 switch(r) {
  case StopReason::None:return "none"; case StopReason::Breakpoint:return "breakpoint";
  case StopReason::Watchpoint:return "watchpoint"; case StopReason::Exception:return "exception";
  case StopReason::Signal:return "signal"; case StopReason::Assertion:return "assertion";
  case StopReason::Sanitizer:return "sanitizer"; case StopReason::Crash:return "crash";
  case StopReason::UserPause:return "user-pause"; case StopReason::TestFailure:return "test-failure";
 }
 return "unknown";
}

std::uint64_t DebugSession::addBreakpoint(Breakpoint b){ b.id=next_id_++; breakpoints_.push_back(std::move(b)); return breakpoints_.back().id; }
std::uint64_t DebugSession::addWatchpoint(Watchpoint w){ w.id=next_id_++; watchpoints_.push_back(std::move(w)); return watchpoints_.back().id; }
bool DebugSession::removeBreakpoint(std::uint64_t id){ auto n=breakpoints_.size(); breakpoints_.erase(std::remove_if(breakpoints_.begin(),breakpoints_.end(),[&](const Breakpoint& b){return b.id==id;}),breakpoints_.end()); return n!=breakpoints_.size(); }
bool DebugSession::removeWatchpoint(std::uint64_t id){ auto n=watchpoints_.size(); watchpoints_.erase(std::remove_if(watchpoints_.begin(),watchpoints_.end(),[&](const Watchpoint& w){return w.id==id;}),watchpoints_.end()); return n!=watchpoints_.size(); }

void DebugSession::emit(DebugEvent e) {
  if (e.type==EventType::Breakpoint || e.type==EventType::Watchpoint || e.type==EventType::Exception ||
      e.type==EventType::Assertion || e.type==EventType::Sanitizer || e.type==EventType::Crash) {
    StopRecord record;
    record.reason = e.type==EventType::Breakpoint ? StopReason::Breakpoint :
                    e.type==EventType::Watchpoint ? StopReason::Watchpoint :
                    e.type==EventType::Exception ? StopReason::Exception :
                    e.type==EventType::Assertion ? StopReason::Assertion :
                    e.type==EventType::Sanitizer ? StopReason::Sanitizer : StopReason::Crash;
    record.thread=e.thread; record.location=e.location; record.detail=e.message;
    record.event_index=events_.size();
    auto bp=e.fields.find("breakpoint_id"); if(bp!=e.fields.end()) try { record.breakpoint_id=std::stoull(bp->second); } catch(...) {}
    auto wp=e.fields.find("watchpoint_id"); if(wp!=e.fields.end()) try { record.watchpoint_id=std::stoull(wp->second); } catch(...) {}
    stop_=record;
  }
  events_.push_back(std::move(e));
}

std::uint64_t DebugSession::addEvidence(EvidenceRecord record) {
  record.sequence=next_evidence_++;
  evidence_.push_back(std::move(record));
  return evidence_.back().sequence;
}

void DiagnosticReport::write(std::ostream& out,const std::vector<DebugEvent>& es){
 out<<"SLeeLa Debugger Diagnostic Report\n"<<"events="<<es.size()<<"\n";
 for(const auto& e:es){ out<<"["<<e.severity<<"] "<<name(e.type)<<": "<<e.message; if(!e.thread.empty())out<<" thread="<<e.thread; if(!e.location.file.empty())out<<" "<<e.location.file<<":"<<e.location.line<<":"<<e.location.column; if(!e.stack.empty()){out<<" stack=";for(size_t i=0;i<e.stack.size();++i){if(i)out<<" <- ";out<<e.stack[i].function;}} if(!e.fields.empty()){out<<" fields=";bool first=true;for(const auto& p:e.fields){if(!first)out<<",";first=false;out<<p.first<<"="<<p.second;}} out<<"\n"; }
}
void DiagnosticReport::writeEvidence(std::ostream& out,const std::vector<EvidenceRecord>& es) {
 out<<"SLeeLa Debug Evidence\nrecords="<<es.size()<<"\n";
 for(const auto& e:es) {
   out<<"#"<<e.sequence<<" observation="<<e.observation<<" event="<<e.event;
   if(!e.source.file.empty()) out<<" source="<<e.source.file<<":"<<e.source.line<<":"<<e.source.column;
   if(!e.thread.empty()) out<<" thread="<<e.thread;
   if(!e.stack_summary.empty()) out<<" stack="<<e.stack_summary;
   if(!e.action.empty()) out<<" action="<<e.action;
   if(!e.backend_result.empty()) out<<" backend="<<e.backend_result;
   if(!e.diagnostic.empty()) out<<" diagnostic="<<e.diagnostic;
   out<<"\n";
 }
}
void DebugSession::report(std::ostream& out) const {
 DiagnosticReport::write(out,events_);
 if(hasStopRecord()) out<<"stop_reason="<<stopReasonName(stop_.reason)<<" detail="<<stop_.detail<<"\n";
 DiagnosticReport::writeEvidence(out,evidence_);
}
}
