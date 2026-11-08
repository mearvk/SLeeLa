// bulk.h - Rate-limited, parallel bulk acquisition over the secure facade.
//
// Fetches a list of URLs into a single content-addressed store, with:
//   * a bounded worker pool (parallelism)
//   * a global rate limiter (minimum interval between request starts)
//   * bounded retries with exponential backoff for transient failures
//   * the same security posture as AcquisitionStore (TLS verify-on, SSH
//     host-key checking, size caps, idempotent content addressing)
//
// This performs no network access on its own beyond what the supplied URLs
// request; callers remain responsible for pointing it only at sources they
// are authorized to fetch.
#ifndef ADVANTAGE_BULK_H
#define ADVANTAGE_BULK_H

#include <cstddef>
#include <string>
#include <vector>

#include "acquire.h"

namespace advantage {

struct BulkItem {
    std::string url;
    std::string expectedSha256;  // optional per-item content pin
};

struct BulkOptions {
    std::string storeDir = "./advantage-store";
    size_t      concurrency = 4;        // worker threads
    long        minIntervalMs = 0;      // min gap between request *starts* (rate limit)
    int         maxRetries = 2;         // retries after the first attempt
    long        backoffBaseMs = 250;    // exponential backoff base
    AcquireOptions acquire;             // per-fetch transport options
};

struct BulkItemResult {
    std::string url;
    bool        ok = false;
    bool        fromCache = false;
    std::string sha256;
    std::string objectPath;
    size_t      size = 0;
    int         attempts = 0;
    std::string error;
};

struct BulkSummary {
    size_t total = 0;
    size_t succeeded = 0;
    size_t failed = 0;
    size_t fromCache = 0;
    std::vector<BulkItemResult> results;  // in input order
};

// Fetch all items. Thread-safe internally; returns a per-item summary.
BulkSummary bulkFetch(const std::vector<BulkItem>& items, const BulkOptions& opts);

// Convenience: load items from a text file, one URL per line. A line may be
// "URL" or "URL<whitespace>SHA256". Lines beginning with '#' and blank lines
// are ignored.
std::vector<BulkItem> loadItemsFromFile(const std::string& path, std::string& error);

}  // namespace advantage

#endif  // ADVANTAGE_BULK_H
