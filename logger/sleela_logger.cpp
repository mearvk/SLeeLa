#include "sleela_logger.hpp"
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <iomanip>
#include <sstream>
namespace sleela {
namespace {
int severity_rank(LogSeverity v) {
    switch (v) {
        case LogSeverity::Trace: return 0; case LogSeverity::Debug: return 1;
        case LogSeverity::Info: return 2; case LogSeverity::Notice: return 3;
        case LogSeverity::Warning: return 4; case LogSeverity::Error: return 5;
        case LogSeverity::Critical: return 6;
    } return 2;
}
int admission_rank(LogAdmission v) {
    switch (v) {
        case LogAdmission::Accepted: return 0; case LogAdmission::Rejected: return 1;
        case LogAdmission::Error: return 2;
    } return 0;
}
}
Logger::Logger(std::string directory) : directory_(std::move(directory)) {}
Logger::~Logger() { close(); }
bool Logger::open() {
    std::lock_guard<std::mutex> guard(mutex_);
    if (file_) return true;
    std::error_code ec;
    std::filesystem::create_directories(directory_, ec);
    if (ec) return false;
    file_ = std::fopen(segment_path().c_str(), "ab");
    if (!file_) return false;
    std::error_code sec;
    segment_bytes_ = std::filesystem::file_size(segment_path(), sec);
    if (sec) segment_bytes_ = 0;
    if (segment_bytes_ >= kMaxSegmentBytes) {
        std::fclose(file_); file_ = nullptr; ++segment_number_;
        file_ = std::fopen(segment_path().c_str(), "ab"); segment_bytes_ = 0;
    }
    return file_ != nullptr;
}
void Logger::close() {
    std::lock_guard<std::mutex> guard(mutex_);
    if (file_) { std::fflush(file_); std::fclose(file_); file_ = nullptr; }
}
void Logger::set_logic(const LoggerLogic& logic) { std::lock_guard<std::mutex> g(mutex_); logic_ = logic; }
LoggerLogic Logger::logic() const { std::lock_guard<std::mutex> g(mutex_); return logic_; }
bool Logger::selected(const PacketRecord& r) const {
    if (r.direction == LogDirection::Sent && !logic_.log_sent) return false;
    if (r.direction == LogDirection::Received && !logic_.log_received) return false;
    if (severity_rank(r.severity) < severity_rank(logic_.minimum_severity)) return false;
    if (admission_rank(r.admission) < admission_rank(logic_.minimum_admission)) return false;
    if (!logic_.protocol_prefix.empty() && r.protocol.rfind(logic_.protocol_prefix, 0) != 0) return false;
    if (!logic_.source_prefix.empty() && r.source.rfind(logic_.source_prefix, 0) != 0) return false;
    if (!logic_.destination_prefix.empty() && r.destination.rfind(logic_.destination_prefix, 0) != 0) return false;
    if (r.packet_bytes < logic_.minimum_packet_bytes) return false;
    if (logic_.maximum_packet_bytes && r.packet_bytes > logic_.maximum_packet_bytes) return false;
    if (r.heuristic_score < logic_.minimum_heuristic) return false;
    return true;
}
bool Logger::rotate_if_needed(std::size_t n) {
    if (n > kMaxSegmentBytes) return false;
    if (segment_bytes_ && segment_bytes_ + n > kMaxSegmentBytes) {
        std::fflush(file_); std::fclose(file_); file_ = nullptr; ++segment_number_;
        file_ = std::fopen(segment_path().c_str(), "ab"); segment_bytes_ = 0;
        if (!file_) return false;
    }
    return true;
}
std::string Logger::escape_json(const std::string& value) const {
    std::ostringstream out;
    for (unsigned char c : value) {
        switch (c) {
            case '\\': out << "\\\\"; break; case '"': out << "\\""; break;
            case '\b': out << "\\b"; break; case '\f': out << "\\f"; break;
            case '\n': out << "\\n"; break; case '\r': out << "\\r"; break;
            case '\t': out << "\\t"; break;
            default:
                if (c < 0x20) out << "\\u" << std::hex << std::setw(4) << std::setfill('0') << (unsigned)c << std::dec;
                else out << (char)c;
        }
    }
    return out.str();
}
std::string Logger::now_utc() const {
    auto now = std::chrono::system_clock::now();
    auto t = std::chrono::system_clock::to_time_t(now);
    std::tm tm{};
#if defined(_WIN32)
    gmtime_s(&tm, &t);
#else
    gmtime_r(&t, &tm);
#endif
    std::ostringstream out; out << std::put_time(&tm, "%Y-%m-%dT%H:%M:%SZ"); return out.str();
}
std::string Logger::segment_path() const {
    std::ostringstream out;
    out << directory_ << "/packets-" << std::setfill('0') << std::setw(6) << segment_number_ << ".jsonl";
    return out.str();
}
bool Logger::write_json_line(const PacketRecord& r) {
    const std::string ts = r.timestamp_utc.empty() ? now_utc() : r.timestamp_utc;
    std::ostringstream out;
    out << "{"
        << "\"format\":\"SLeeLa-PacketLog-1\","
        << "\"timestamp\":\"" << escape_json(ts) << "\","
        << "\"direction\":\"" << direction_name(r.direction) << "\","
        << "\"severity\":\"" << severity_name(r.severity) << "\","
        << "\"admission\":\"" << admission_name(r.admission) << "\","
        << "\"protocol\":\"" << escape_json(r.protocol) << "\","
        << "\"service\":\"" << escape_json(r.service) << "\","
        << "\"operation\":\"" << escape_json(r.operation) << "\","
        << "\"source\":\"" << escape_json(r.source) << "\","
        << "\"destination\":\"" << escape_json(r.destination) << "\","
        << "\"stream_id\":\"" << escape_json(r.stream_id) << "\","
        << "\"request_id\":\"" << escape_json(r.request_id) << "\","
        << "\"sequence\":" << r.sequence << ","
        << "\"packet_bytes\":" << r.packet_bytes << ","
        << "\"heuristic_score\":" << std::fixed << std::setprecision(6) << std::clamp(r.heuristic_score, 0.0, 1.0) << ","
        << "\"reason\":\"" << escape_json(r.reason) << "\"";
    if (logic_.include_payload && !r.payload_base64.empty())
        out << ",\"payload_base64\":\"" << escape_json(r.payload_base64) << "\"";
    out << "}\n";
    const std::string line = out.str();
    if (!rotate_if_needed(line.size())) return false;
    if (std::fwrite(line.data(), 1, line.size(), file_) != line.size()) return false;
    segment_bytes_ += line.size();
    return std::fflush(file_) == 0;
}
bool Logger::record(const PacketRecord& record) {
    std::lock_guard<std::mutex> guard(mutex_);
    if (!selected(record)) return true;
    if (!file_) {
        std::error_code ec; std::filesystem::create_directories(directory_, ec);
        if (ec) return false;
        file_ = std::fopen(segment_path().c_str(), "ab");
        if (!file_) return false;
    }
    return write_json_line(record);
}
std::uint64_t Logger::current_segment_bytes() const { std::lock_guard<std::mutex> g(mutex_); return segment_bytes_; }
std::uint64_t Logger::segment_number() const { std::lock_guard<std::mutex> g(mutex_); return segment_number_; }
const char* Logger::direction_name(LogDirection v) { return v == LogDirection::Sent ? "sent" : "received"; }
const char* Logger::severity_name(LogSeverity v) {
    switch (v) {
        case LogSeverity::Trace: return "trace"; case LogSeverity::Debug: return "debug";
        case LogSeverity::Info: return "info"; case LogSeverity::Notice: return "notice";
        case LogSeverity::Warning: return "warning"; case LogSeverity::Error: return "error";
        case LogSeverity::Critical: return "critical";
    } return "info";
}
const char* Logger::admission_name(LogAdmission v) {
    switch (v) { case LogAdmission::Accepted: return "accepted"; case LogAdmission::Rejected: return "rejected"; case LogAdmission::Error: return "error"; }
    return "accepted";
}
const char* Logger::sort_key_name(LogSortKey v) {
    switch (v) {
        case LogSortKey::Timestamp: return "timestamp"; case LogSortKey::Direction: return "direction";
        case LogSortKey::Severity: return "severity"; case LogSortKey::Protocol: return "protocol";
        case LogSortKey::Size: return "size"; case LogSortKey::Sequence: return "sequence";
    } return "timestamp";
}
}
