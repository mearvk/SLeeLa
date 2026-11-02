#pragma once
#include <cstdint>
#include <string>
#include <vector>

// Output destination adapter for Slecompiler reconstructions.
//
// This is the governed network/destination layer. It delivers an already-produced
// reconstruction (a byte buffer) to a target. It NEVER reads or executes an input
// artifact -- it only transmits output the pipeline has already generated.
//
// Transports:
//   File       -- write the bytes to a local path.
//   Directory  -- write the bytes to <path>/<filename> (filename chosen by the caller).
//   FtpStore   -- RFC 959 FTP STOR over an active/passive data connection.
//   HttpPost   -- HTTP/1.1 POST with Content-Length.
//   HttpPut    -- HTTP/1.1 PUT with Content-Length.
//   Sftp       -- SSH File Transfer, delegated to a governed `sftp`/`scp` helper when
//                 present (no SSH stack is embedded); reported unavailable otherwise.
//
// Addressing is explicit: a DNS host name OR an IPv4/IPv6 literal, plus a port. Name
// resolution uses getaddrinfo, so IPv4 and IPv6 are handled uniformly. Nothing is sent
// unless the caller supplies a destination; local file/directory output is the default.
namespace sleela::decompiler {

enum class DestinationKind { File, Directory, FtpStore, HttpPost, HttpPut, Sftp };

// Which IP family to prefer when a host resolves to both A and AAAA records.
enum class IpPreference { Any, IPv4, IPv6 };

struct Destination {
    DestinationKind kind{DestinationKind::File};
    // Network addressing (unused for File/Directory).
    std::string host;              // DNS name, or IPv4/IPv6 literal (e.g. "ex.org", "192.0.2.10", "2001:db8::1")
    std::uint16_t port{0};         // 0 => transport default (FTP 21, HTTP 80, SFTP/SSH 22)
    IpPreference ip_preference{IpPreference::Any};
    // Resource path / local path. For File this is the file path; for Directory the dir;
    // for Ftp/Sftp the remote path; for Http the request path (e.g. "/upload/recovered.cpp").
    std::string path;
    std::string filename;          // used by Directory to compose <path>/<filename>
    // Optional credentials (FTP/SFTP). HTTP auth, when needed, is carried in `headers`.
    std::string username;
    std::string password;
    std::vector<std::string> headers;   // extra "Name: value" lines for HTTP
    std::string content_type{"text/plain"};
    bool passive_ftp{true};        // PASV (recommended) vs. active FTP
    int timeout_seconds{30};
};

struct DeliveryResult {
    bool ok{false};
    int status_code{0};            // HTTP status, FTP reply code, or 0 for local/sftp
    std::uint64_t bytes_sent{0};
    std::string resolved_address;  // the IP actually connected to (evidence/audit)
    std::string message;           // human-readable detail or error
};

// Parse a destination URI into a Destination. Supported schemes:
//   file:/abs/path  or  a bare /local/path (treated as File)
//   ftp://[user:pass@]host[:port]/remote/path
//   http://host[:port]/path   https-not-supported-here (plain HTTP/1.1 only)
//   http+post://... / http+put://...  (explicit method; or use method arg)
//   sftp://[user@]host[:port]/remote/path
// Returns true on success; on failure sets `error`.
bool parse_destination(const std::string& uri, Destination& out, std::string& error);

// Deliver `data` to the destination. Pure output; never touches an input artifact.
DeliveryResult deliver(const Destination& dest, const std::string& data);

const char* destination_kind_name(DestinationKind);

} // namespace sleela::decompiler
