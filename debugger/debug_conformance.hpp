#pragma once
#include <string>
#include <vector>
namespace sleela::debugger {
enum class ConformanceStage { Model, Implement, Integrate, Verify };
struct ConformanceRecord { std::string capability,platform,backend,executable_hash,source_revision; bool model{},implemented{},integrated{},verified{}; std::string evidence; };
class ConformanceSuite {
 std::vector<ConformanceRecord> records_;
 public:
 void add(ConformanceRecord);
 bool passes(const ConformanceRecord&) const noexcept;
 bool releaseReady() const noexcept;
 const std::vector<ConformanceRecord>& records() const noexcept{return records_;}
 std::string report() const;
};
}