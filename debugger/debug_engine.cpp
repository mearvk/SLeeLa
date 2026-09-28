#include "debug_engine.hpp"
#include <cctype>
#include <iomanip>
#include <sstream>
namespace sleela::debugger {
const char* capabilityStateName(CapabilityState s)noexcept{switch(s){case CapabilityState::Unavailable:return"unavailable";case CapabilityState::Declared:return"declared";case CapabilityState::Implemented:return"implemented";case CapabilityState::Tested:return"tested";}return"unknown";}
const char* exceptionKindName(ExceptionKind k)noexcept{switch(k){case ExceptionKind::Signal:return"signal";case ExceptionKind::AccessViolation:return"access-violation";case ExceptionKind::IllegalInstruction:return"illegal-instruction";case ExceptionKind::DivideByZero:return"divide-by-zero";case ExceptionKind::Assertion:return"assertion";case ExceptionKind::Sanitizer:return"sanitizer";default:return"unknown";}}
DebugEngine::DebugEngine(){const char*n[]={"breakpoints","watchpoints","threads","stack","symbols","memory","registers","exceptions","expressions","evidence"};for(auto s:n)capabilities_.push_back({s,CapabilityState::Implemented,"portable engine state","portable"});}
std::uint64_t DebugEngine::addBreakpoint(BreakpointSpec s){auto id=next_breakpoint_++;breakpoints_.push_back({id,std::move(s),true,0});return id;}
bool DebugEngine::removeBreakpoint(std::uint64_t id){for(auto i=breakpoints_.begin();i!=breakpoints_.end();++i)if(i->id==id){breakpoints_.erase(i);return true;}return false;}
bool DebugEngine::setBreakpointEnabled(std::uint64_t id,bool e){for(auto&r:breakpoints_)if(r.id==id){r.enabled=e;return true;}return false;}
bool DebugEngine::hitBreakpoint(std::uint64_t id){for(auto&r:breakpoints_)if(r.id==id&&r.enabled){++r.hit_count;if(r.spec.temporary)r.enabled=false;return r.hit_count>r.spec.ignore_count;}return false;}
std::uint64_t DebugEngine::addWatchpoint(WatchpointSpec s){auto id=next_watchpoint_++;watchpoints_.push_back({id,std::move(s),true,0,{},{}});return id;}
bool DebugEngine::removeWatchpoint(std::uint64_t id){for(auto i=watchpoints_.begin();i!=watchpoints_.end();++i)if(i->id==id){watchpoints_.erase(i);return true;}return false;}
bool DebugEngine::updateWatchpoint(std::uint64_t id,std::string oldv,std::string newv){for(auto&r:watchpoints_)if(r.id==id&&r.enabled){r.last_value=std::move(oldv);r.new_value=std::move(newv);++r.hit_count;return r.last_value!=r.new_value;}return false;}
void DebugEngine::setThreads(std::vector<ThreadRecord> t){threads_=std::move(t);current_thread_=0;for(auto&r:threads_)r.current=false;}
bool DebugEngine::selectThread(std::uint64_t id){for(auto&r:threads_)r.current=(r.id==id);for(const auto&r:threads_)if(r.current){current_thread_=id;return true;}return false;}
void DebugEngine::setStack(std::vector<StackFrameRecord>s){stack_=std::move(s);} void DebugEngine::setSymbols(std::vector<SymbolRecord>s){symbols_=std::move(s);} void DebugEngine::setMemoryMap(std::vector<MemoryRegion>s){memory_=std::move(s);} void DebugEngine::setRegisters(RegisterSnapshot s){registers_=std::move(s);} void DebugEngine::setException(ExceptionRecord e){exception_=std::move(e);} void DebugEngine::setEvidence(EvidenceBundle e){evidence_=std::move(e);}
bool DebugEngine::hasCapability(const std::string&n)const{for(const auto&c:capabilities_)if(c.name==n&&(c.state==CapabilityState::Implemented||c.state==CapabilityState::Tested))return true;return false;}
std::string DebugEngine::capabilityReport()const{std::ostringstream o;for(const auto&c:capabilities_)o<<c.name<<"="<<capabilityStateName(c.state)<<" mechanism="<<c.mechanism<<" platform="<<c.platform<<"\n";return o.str();}
void DebugEngine::setVariable(std::string n,std::uint64_t v){variables_[std::move(n)]=v;}
bool DebugEngine::evaluate(const std::string&e,std::uint64_t&out)const{if(e.empty()||e.size()>4096)return false;auto i=variables_.find(e);if(i!=variables_.end()){out=i->second;return true;}std::size_t p=0;while(p<e.size()&&std::isspace((unsigned char)e[p]))++p;std::uint64_t v=0;bool any=false;for(;p<e.size()&&std::isxdigit((unsigned char)e[p]);++p){any=true;v=v*16+(std::isdigit((unsigned char)e[p])?e[p]-'0':std::tolower((unsigned char)e[p])-'a'+10);}while(p<e.size()&&std::isspace((unsigned char)e[p]))++p;if(any&&p==e.size()){out=v;return true;}return false;}
std::string DebugEngine::memoryHex(const std::uint8_t*p,std::size_t n){std::ostringstream o;o<<std::hex<<std::setfill('0');for(std::size_t i=0;i<n;++i)o<<std::setw(2)<<(unsigned)p[i];return o.str();}
bool DebugEngine::validMemoryRange(std::uint64_t a,std::size_t n,std::uint64_t l)noexcept{return a<=l&&n<=l-a;}
std::string EvidenceBundle::text()const{std::ostringstream o;o<<"source_revision="<<source_revision<<"\nexecutable_hash="<<executable_hash<<"\ntoolchain="<<toolchain<<"\nplatform="<<platform<<"\ncommand_line="<<command_line<<"\nexception="<<exception<<"\nregisters="<<registers<<"\nstack="<<stack<<"\nmodules="<<modules<<"\nmemory_map="<<memory_map<<"\ntest_id="<<test_id<<"\ntimestamp="<<timestamp<<"\n";for(const auto&l:logs)o<<"log="<<l<<"\n";return o.str();}
}