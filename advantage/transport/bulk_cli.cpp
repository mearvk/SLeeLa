// bulk_cli.cpp - Command-line front end for the bulk parallel fetcher.
//
// Usage:
//   bulkfetch <list-file> [--store DIR] [--jobs N] [--rate MS]
//                         [--retries N] [--insecure] [--accept-new]
//
// <list-file> has one URL per line (optionally "URL SHA256"); blank lines and
// lines starting with '#' are ignored.
#include <cstdlib>
#include <iostream>
#include <string>

#include "bulk.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr <<
            "usage: bulkfetch <list-file> [--store DIR] [--jobs N] [--rate MS]\n"
            "                 [--retries N] [--insecure] [--accept-new]\n";
        return 2;
    }

    std::string listFile = argv[1];
    advantage::BulkOptions opts;

    for (int i = 2; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "--store" && i + 1 < argc) opts.storeDir = argv[++i];
        else if (a == "--jobs" && i + 1 < argc) opts.concurrency = (size_t)std::strtoul(argv[++i], nullptr, 10);
        else if (a == "--rate" && i + 1 < argc) opts.minIntervalMs = std::strtol(argv[++i], nullptr, 10);
        else if (a == "--retries" && i + 1 < argc) opts.maxRetries = (int)std::strtol(argv[++i], nullptr, 10);
        else if (a == "--insecure") { opts.acquire.http.verifyPeer = false; opts.acquire.http.verifyHost = false; }
        else if (a == "--accept-new") opts.acquire.sftp.hostKey = advantage::HostKeyPolicy::AcceptNew;
        else { std::cerr << "unknown option: " << a << "\n"; return 2; }
    }

    std::string err;
    auto items = advantage::loadItemsFromFile(listFile, err);
    if (!err.empty()) { std::cerr << err << "\n"; return 1; }
    if (items.empty()) { std::cerr << "no URLs in list\n"; return 1; }

    std::cout << "bulk fetch: " << items.size() << " item(s), jobs="
              << opts.concurrency << ", rate=" << opts.minIntervalMs
              << "ms, retries=" << opts.maxRetries << "\n";

    advantage::BulkSummary s = advantage::bulkFetch(items, opts);

    for (const auto& r : s.results) {
        if (r.ok) {
            std::cout << (r.fromCache ? "  [cache] " : "  [ ok  ] ")
                      << r.sha256.substr(0, 12) << "  " << r.size << "B  "
                      << r.url << (r.attempts > 1 ? "  (" + std::to_string(r.attempts) + " attempts)" : "")
                      << "\n";
        } else {
            std::cout << "  [fail ] " << r.url << "  (" << r.attempts
                      << " attempts): " << r.error << "\n";
        }
    }

    std::cout << "----\n"
              << "total " << s.total << ", succeeded " << s.succeeded
              << " (cached " << s.fromCache << "), failed " << s.failed << "\n";

    return s.failed == 0 ? 0 : 1;
}
