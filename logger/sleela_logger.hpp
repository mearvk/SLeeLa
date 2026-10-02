#ifndef SLEELA_LOGGER_HPP
#define SLEELA_LOGGER_HPP
#include <cstdint>
#include <cstdio>
#include <mutex>
#include <string>
namespace sleela {
enum class LogDirection { Sent, Received };
enum class LogSeverity { Trace, Debug, Info, Notice, Warning, Error, Critical };
enum class LogAdmission { Accepted, Rejected, Error };
enum class LogSortKey { Timestamp, Direction, Severity, Protocol, Size, Sequence };
struct LoggerLogic {
    bool log_sent = true;
    bool log_received = true;
    LogSeverity minimum_severity = LogSeverity::Info;
    LogAdmission minimum_admission = LogAdmission::Accepted;
    std::string protocol_prefix;
    std::string source_prefix;
    std::string destination_prefix;
    std::uint64_t minimum_packet_bytes = 0;
    std::uint64_t maximum_packet_bytes = 0;
    double minimum_heuristic = 0.0;
    bool include_payload = false;
    bool move_completed_segments = false;
    std::string archive_directory;
};
struct PacketRecord {
    std::string timestamp_utc;
    LogDirection direction = LogDirection::Received;
    LogSeverity severity = LogSeverity::Info;
    LogAdmission admission = LogAdmission::Accepted;
    std::string protocol, service, operation, source, destination, stream_id, request_id;
    std::uint64_t sequence = 0;
    std::uint64_t packet_bytes = 0;
    double heuristic_score = 0.0;
    std::string reason;
    std::string payload_base64;
};
class Logger {
public:
    static constexpr std::uint64_t kMaxSegmentBytes = 240ULL * 1024ULL * 1024ULL;
    explicit Logger(std::string directory = "logs");
    ~Logger();
    bool open();
    void close();
    void set_logic(const LoggerLogic& logic);
    LoggerLogic logic() const;
    bool record(const PacketRecord& record);
    std::uint64_t current_segment_bytes() const;
    std::uint64_t segment_number() const;
    static const char* direction_name(LogDirection);
    static const char* severity_name(LogSeverity);
    static const char* admission_name(LogAdmission);
    static const char* sort_key_name(LogSortKey);
private:
    bool selected(const PacketRecord&) const;
    bool rotate_if_needed(std::size_t);
    bool write_json_line(const PacketRecord&);
    std::string escape_json(const std::string&) const;
    std::string now_utc() const;
    std::string segment_path() const;
    std::string directory_;
    LoggerLogic logic_;
    std::FILE* file_ = nullptr;
    std::uint64_t segment_bytes_ = 0;
    std::uint64_t segment_number_ = 0;
    mutable std::mutex mutex_;
};
}
#endif
