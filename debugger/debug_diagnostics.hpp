#pragma once
#include <cstdint>
#include <string>
#include <vector>
namespace sleela::debugger {
struct ReplayCheckpoint { std::uint64_t id{},instruction{}; std::string state_hash,label; };
struct ReplayRecord { bool recording{}; std::vector<ReplayCheckpoint> checkpoints; };
struct CrashDump { std::string format,path,fingerprint,signal,exception; std::uint64_t address{}; bool valid{}; };
struct SanitizerFinding { std::string tool,kind,message,file; std::uint32_t line{}; };
struct MemoryFinding { std::string kind,address,description; std::uint64_t size{}; };
struct ProfileSample { std::string thread,function,module; std::uint64_t samples{}; double cpu_percent{}; };
struct CoverageRecord { std::string file,function; std::uint64_t lines_total{},lines_hit{},branches_total{},branches_hit{}; };
struct LockRecord { std::uint64_t thread{},lock{}; std::string name; bool owned{},waiting{}; };
struct SessionArtifact { std::string session_id,version,target,source_revision,executable_hash; std::vector<std::string> files; std::string manifest()const; };
struct DebugSecurityPolicy { bool allow_attach{false},allow_memory_write{false},allow_expression_execution{false}; std::size_t expression_limit{4096},memory_write_limit{0}; };
class DiagnosticsEngine {
 ReplayRecord replay_; std::vector<CrashDump> crashes_; std::vector<SanitizerFinding> sanitizers_; std::vector<MemoryFinding> memory_; std::vector<ProfileSample> profile_; std::vector<CoverageRecord> coverage_; std::vector<LockRecord> locks_; SessionArtifact artifact_; DebugSecurityPolicy policy_;
 public:
 bool beginRecording(); bool endRecording(); bool recording()const noexcept; bool checkpoint(std::uint64_t,std::string,std::string);
 void addCrash(CrashDump); void addSanitizer(SanitizerFinding); void addMemoryFinding(MemoryFinding); void addProfile(ProfileSample); void addCoverage(CoverageRecord); void addLock(LockRecord);
 const ReplayRecord& replay()const noexcept{return replay_;} const std::vector<CrashDump>& crashes()const noexcept{return crashes_;} const std::vector<SanitizerFinding>& sanitizers()const noexcept{return sanitizers_;} const std::vector<MemoryFinding>& memoryFindings()const noexcept{return memory_;} const std::vector<ProfileSample>& profile()const noexcept{return profile_;} const std::vector<CoverageRecord>& coverage()const noexcept{return coverage_;} const std::vector<LockRecord>& locks()const noexcept{return locks_;}
 void setArtifact(SessionArtifact); const SessionArtifact& artifact()const noexcept{return artifact_;} void setSecurityPolicy(DebugSecurityPolicy); bool authorize(const std::string&,std::size_t=0)const;
}; }