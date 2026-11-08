// acquire.h - Idempotent, content-addressed acquisition facade.
//
// One entry point for reaching remote resources over the supported secure
// protocols (https, http, sftp, ssh, file) and bringing their bytes into a
// local content-addressed store (CAS).
//
// Idempotency model:
//   * Downloaded bytes are stored under objects/<sha256>. The same content
//     always lands at the same path, so re-fetching is a no-op copy.
//   * Writes are atomic (temp file + rename), so an interrupted fetch never
//     corrupts the store.
//   * review() fetches only metadata (HTTP HEAD) without downloading a body.
//   * A fetch that has already been satisfied (object present) short-circuits
//     and is reported as `fromCache = true`.
//
// Security model:
//   * HTTPS verifies the certificate chain and hostname by default.
//   * SSH/SFTP enforce host-key checking via OpenSSH.
//   * Optional expectedSha256 lets a caller pin content; a mismatch fails
//     the fetch and discards the bytes.
#ifndef ADVANTAGE_ACQUIRE_H
#define ADVANTAGE_ACQUIRE_H

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "http_client.h"
#include "sftp_client.h"

namespace advantage {

struct AcquireOptions {
    HttpOptions http;
    SftpOptions sftp;
    std::string expectedSha256;   // optional content pin (lowercase hex)
};

struct AcquireResult {
    bool        ok = false;
    bool        fromCache = false;   // object already present, no transfer
    std::string sha256;              // content id of the stored object
    std::string objectPath;          // absolute path in the CAS
    size_t      size = 0;
    int         httpStatus = 0;      // for http(s); 0 otherwise
    std::map<std::string, std::string> metadata;  // review() headers, etc.
    std::string error;               // populated when !ok
};

// A content-addressed store rooted at a directory. Thread-compatible (not
// internally synchronized); construct one per logical operation or guard
// externally.
class AcquisitionStore {
public:
    // Root directory for the CAS. Created (with an objects/ subdir) on demand.
    explicit AcquisitionStore(std::string root) : root_(std::move(root)) {}

    // Metadata-only "review": HTTP(S) HEAD. For non-HTTP schemes, reports
    // what can be known without transferring the body.
    AcquireResult review(const std::string &url, const AcquireOptions &opts = {});

    // "Obtain / download / store": fetch the resource and place it in the CAS.
    // Idempotent: returns fromCache=true if the content (by expectedSha256)
    // or a prior fetch of the same URL already satisfied this.
    AcquireResult fetch(const std::string &url, const AcquireOptions &opts = {});

    // Path where an object with the given content id lives (may not exist).
    std::string objectPathFor(const std::string &sha256) const;

    // True if an object with this content id is already stored.
    bool has(const std::string &sha256) const;

    const std::string &root() const { return root_; }

private:
    bool ensureLayout(std::string &err);
    // Store raw bytes into the CAS atomically; returns the object's sha256.
    AcquireResult commitBytes(const std::vector<uint8_t> &bytes,
                              const std::string &expectedSha256);
    AcquireResult commitFile(const std::string &tmpPath,
                             const std::string &expectedSha256);

    std::string root_;
};

}  // namespace advantage

#endif  // ADVANTAGE_ACQUIRE_H
