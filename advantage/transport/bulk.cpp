// bulk.cpp - Rate-limited parallel bulk acquisition (see bulk.h).
#include "bulk.h"

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <fstream>
#include <mutex>
#include <sstream>
#include <thread>

namespace advantage {

namespace {

// A simple token-paced gate: ensures request *starts* are separated by at
// least minIntervalMs across all workers.
class RateGate {
public:
    explicit RateGate(long minIntervalMs) : interval_(minIntervalMs) {}

    void wait() {
        if (interval_ <= 0) return;
        std::unique_lock<std::mutex> lk(m_);
        auto now = std::chrono::steady_clock::now();
        if (next_ > now) {
            cv_.wait_until(lk, next_);
            now = std::chrono::steady_clock::now();
        }
        auto base = (now > next_) ? now : next_;
        next_ = base + std::chrono::milliseconds(interval_);
    }

private:
    long interval_;
    std::mutex m_;
    std::condition_variable cv_;
    std::chrono::steady_clock::time_point next_ = std::chrono::steady_clock::now();
};

bool transientError(const std::string& e) {
    // Heuristic: network/timeout-ish failures are worth retrying; a hash
    // mismatch or an unsupported scheme is not.
    if (e.find("hash mismatch") != std::string::npos) return false;
    if (e.find("unsupported") != std::string::npos) return false;
    if (e.find("invalid URL") != std::string::npos) return false;
    return true;
}

}  // namespace

BulkSummary bulkFetch(const std::vector<BulkItem>& items, const BulkOptions& opts) {
    BulkSummary summary;
    summary.total = items.size();
    summary.results.resize(items.size());

    if (items.empty()) return summary;

    AcquisitionStore store(opts.storeDir);  // thread-compatible for distinct writes
    RateGate gate(opts.minIntervalMs);

    std::atomic<size_t> nextIndex{0};
    size_t workers = opts.concurrency == 0 ? 1 : opts.concurrency;
    if (workers > items.size()) workers = items.size();

    auto worker = [&]() {
        for (;;) {
            size_t i = nextIndex.fetch_add(1);
            if (i >= items.size()) break;

            const BulkItem& item = items[i];
            BulkItemResult res;
            res.url = item.url;

            AcquireOptions ao = opts.acquire;
            ao.expectedSha256 = item.expectedSha256;

            int attempt = 0;
            for (;;) {
                gate.wait();                 // rate limit request starts
                attempt++;
                AcquireResult r = store.fetch(item.url, ao);
                res.attempts = attempt;
                if (r.ok) {
                    res.ok = true;
                    res.fromCache = r.fromCache;
                    res.sha256 = r.sha256;
                    res.objectPath = r.objectPath;
                    res.size = r.size;
                    res.error.clear();
                    break;
                }
                res.error = r.error;
                if (attempt > opts.maxRetries || !transientError(r.error)) break;

                // Exponential backoff: base * 2^(attempt-1).
                long delay = opts.backoffBaseMs * (1L << (attempt - 1));
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            }

            summary.results[i] = res;
        }
    };

    std::vector<std::thread> pool;
    pool.reserve(workers);
    for (size_t w = 0; w < workers; ++w) pool.emplace_back(worker);
    for (auto& t : pool) t.join();

    for (const auto& r : summary.results) {
        if (r.ok) { summary.succeeded++; if (r.fromCache) summary.fromCache++; }
        else summary.failed++;
    }
    return summary;
}

std::vector<BulkItem> loadItemsFromFile(const std::string& path, std::string& error) {
    std::vector<BulkItem> items;
    std::ifstream in(path);
    if (!in) { error = "could not open list file: " + path; return items; }

    std::string line;
    while (std::getline(in, line)) {
        // Trim leading whitespace.
        size_t a = line.find_first_not_of(" \t\r\n");
        if (a == std::string::npos) continue;
        if (line[a] == '#') continue;  // comment
        std::string rest = line.substr(a);

        std::istringstream ss(rest);
        BulkItem it;
        ss >> it.url;
        if (it.url.empty()) continue;
        ss >> it.expectedSha256;  // optional; empty if absent
        items.push_back(it);
    }
    error.clear();
    return items;
}

}  // namespace advantage
