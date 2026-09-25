#pragma once
#include <cstdint>
#include <stdexcept>
#include <string>

namespace sleela::decompiler {

enum class ErrorCode {
    InvalidArtifact,
    TruncatedArtifact,
    UnsupportedFormat,
    UnsupportedArchitecture,
    MalformedHeader,
    InvalidSection,
    InvalidAddress,
    DecodeFailure,
    AmbiguousDecode,
    AnalysisFailure,
    UnsupportedInstruction,
    OutputFailure,
    ResourceLimit
};

class DecompilerError : public std::runtime_error {
public:
    DecompilerError(ErrorCode code, std::string message,
                    std::uint64_t file_offset = 0,
                    std::uint64_t virtual_address = 0,
                    std::string stage = {})
        : std::runtime_error(std::move(message)), code_(code),
          file_offset_(file_offset), virtual_address_(virtual_address),
          stage_(std::move(stage)) {}

    ErrorCode code() const noexcept { return code_; }
    std::uint64_t file_offset() const noexcept { return file_offset_; }
    std::uint64_t virtual_address() const noexcept { return virtual_address_; }
    const std::string& stage() const noexcept { return stage_; }

private:
    ErrorCode code_;
    std::uint64_t file_offset_;
    std::uint64_t virtual_address_;
    std::string stage_;
};

inline const char* error_code_name(ErrorCode code) noexcept {
    switch (code) {
        case ErrorCode::InvalidArtifact: return "InvalidArtifact";
        case ErrorCode::TruncatedArtifact: return "TruncatedArtifact";
        case ErrorCode::UnsupportedFormat: return "UnsupportedFormat";
        case ErrorCode::UnsupportedArchitecture: return "UnsupportedArchitecture";
        case ErrorCode::MalformedHeader: return "MalformedHeader";
        case ErrorCode::InvalidSection: return "InvalidSection";
        case ErrorCode::InvalidAddress: return "InvalidAddress";
        case ErrorCode::DecodeFailure: return "DecodeFailure";
        case ErrorCode::AmbiguousDecode: return "AmbiguousDecode";
        case ErrorCode::AnalysisFailure: return "AnalysisFailure";
        case ErrorCode::UnsupportedInstruction: return "UnsupportedInstruction";
        case ErrorCode::OutputFailure: return "OutputFailure";
        case ErrorCode::ResourceLimit: return "ResourceLimit";
    }
    return "UnknownError";
}

} // namespace sleela::decompiler
