#include "debug_diagnostics.hpp"
#include <sstream>
namespace sleela::debugger {
bool DiagnosticsEngine::beginRecording(){if(replay_.recording)return false;replay_.recording=true;return true;}
bool DiagnosticsEngine::endRecording(){if(!replay_.recording)return false;replay_.recording=false;return true;}
bool DiagnosticsEngine::recording()const noexcept{return replay_.recording;}
bool DiagnosticsEngine::checkpoint(std::uint64_t pc,std::string h,std::string l){if(!replay_.recording)return false;replay_.checkpoints.push_back({replay_.checkpoints.size()+1,pc,std::move(h),std::move(l)});return true;}
void DiagnosticsEngine::addCrash(CrashDump x){crashes_.push_back(std::move(x));} void DiagnosticsEngine::addSanitizer(SanitizerFinding x){sanitizers_.push_back(std::move(x));} void DiagnosticsEngine::addMemoryFinding(MemoryFinding x){memory_.push_back(std::move(x));} void DiagnosticsEngine::addProfile(ProfileSample x){profile_.push_back(std::move(x));} void DiagnosticsEngine::addCoverage(CoverageRecord x){coverage_.push_back(std::move(x));} void DiagnosticsEngine::addLock(LockRecord x){locks_.push_back(std::move(x));}
void DiagnosticsEngine::setArtifact(SessionArtifact x){artifact_=std::move(x);} void DiagnosticsEngine::setSecurityPolicy(DebugSecurityPolicy x){policy_=std::move(x);}
bool DiagnosticsEngine::authorize(const std::string&a,std::size_t n)const{if(a=="attach")return policy_.allow_attach;if(a=="memory-write")return policy_.allow_memory_write&&n<=policy_.memory_write_limit;if(a=="expression-execution")return policy_.allow_expression_execution&&n<=policy_.expression_limit;return a=="read"||a=="inspect"||a=="report";}
std::string SessionArtifact::manifest()const{std::ostringstream o;o<<"session_id="<<session_id<<"\nversion="<<version<<"\ntarget="<<target<<"\nsource_revision="<<source_revision<<"\nexecutable_hash="<<executable_hash<<"\n";for(const auto&f:files)o<<"file="<<f<<"\n";return o.str();}
}