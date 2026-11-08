// acquire.cpp - Idempotent content-addressed acquisition (see acquire.h).
#include "acquire.h"

#include "digest.h"
#include "uri.h"

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <fstream>

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

namespace advantage {

namespace {

bool mkdir_p(const std::string &path) {
    if (path.empty()) return false;
    std::string acc;
    for (size_t i = 0; i < path.size(); ++i) {
        acc.push_back(path[i]);
        if (path[i] == '/' || i + 1 == path.size()) {
            if (acc == "/" ) continue;
            std::string dir = acc;
            if (dir.back() == '/') dir.pop_back();
            if (dir.empty()) continue;
            if (mkdir(dir.c_str(), 0700) != 0 && errno != EEXIST) return false;
        }
    }
    return true;
}

bool path_exists(const std::string &p) {
    struct stat st;
    return stat(p.c_str(), &st) == 0;
}

size_t file_size(const std::string &p) {
    struct stat st;
    if (stat(p.c_str(), &st) != 0) return 0;
    return (size_t)st.st_size;
}

}  // namespace

std::string AcquisitionStore::objectPathFor(const std::string &sha256) const {
    // Fan out by first two hex chars to keep directories shallow.
    if (sha256.size() < 2) return root_ + "/objects/" + sha256;
    return root_ + "/objects/" + sha256.substr(0, 2) + "/" + sha256;
}

bool AcquisitionStore::has(const std::string &sha256) const {
    return !sha256.empty() && path_exists(objectPathFor(sha256));
}

bool AcquisitionStore::ensureLayout(std::string &err) {
    if (!mkdir_p(root_ + "/objects")) {
        err = "could not create store layout under " + root_;
        return false;
    }
    if (!mkdir_p(root_ + "/tmp")) {
        err = "could not create temp dir under " + root_;
        return false;
    }
    return true;
}

// Atomically move the finished temp file to its content-addressed location.
AcquireResult AcquisitionStore::commitFile(const std::string &tmpPath,
                                           const std::string &expectedSha256) {
    AcquireResult r;
    std::string sha = sha256_file(tmpPath);
    if (sha.empty()) {
        r.error = "could not hash downloaded data";
        ::remove(tmpPath.c_str());
        return r;
    }
    if (!expectedSha256.empty() && expectedSha256 != sha) {
        r.error = "content hash mismatch: expected " + expectedSha256 +
                  " got " + sha;
        ::remove(tmpPath.c_str());
        return r;
    }

    std::string dest = objectPathFor(sha);
    std::string destDir = dest.substr(0, dest.find_last_of('/'));
    if (!mkdir_p(destDir)) {
        r.error = "could not create object directory";
        ::remove(tmpPath.c_str());
        return r;
    }

    if (path_exists(dest)) {
        // Already have this exact content: idempotent hit.
        ::remove(tmpPath.c_str());
        r.ok = true;
        r.fromCache = true;
    } else if (rename(tmpPath.c_str(), dest.c_str()) == 0) {
        r.ok = true;
    } else {
        r.error = std::string("atomic rename into store failed: ") + std::strerror(errno);
        ::remove(tmpPath.c_str());
        return r;
    }

    r.sha256 = sha;
    r.objectPath = dest;
    r.size = file_size(dest);
    return r;
}

AcquireResult AcquisitionStore::commitBytes(const std::vector<uint8_t> &bytes,
                                            const std::string &expectedSha256) {
    AcquireResult r;
    std::string err;
    if (!ensureLayout(err)) { r.error = err; return r; }

    // Short-circuit if we already have this content.
    std::string sha = sha256_hex(bytes.data(), bytes.size());
    if (!expectedSha256.empty() && expectedSha256 != sha) {
        r.error = "content hash mismatch: expected " + expectedSha256 +
                  " got " + sha;
        return r;
    }
    if (has(sha)) {
        r.ok = true;
        r.fromCache = true;
        r.sha256 = sha;
        r.objectPath = objectPathFor(sha);
        r.size = file_size(r.objectPath);
        return r;
    }

    // Write to a unique temp file, then commit atomically.
    std::string tmp = root_ + "/tmp/dl." + std::to_string((long)getpid()) + "." +
                      std::to_string((long)time(nullptr));
    std::ofstream out(tmp, std::ios::binary | std::ios::trunc);
    if (!out) { r.error = "could not open temp file"; return r; }
    if (!bytes.empty())
        out.write(reinterpret_cast<const char *>(bytes.data()), (std::streamsize)bytes.size());
    out.flush();
    if (!out) { r.error = "write to temp file failed"; out.close(); ::remove(tmp.c_str()); return r; }
    out.close();

    return commitFile(tmp, expectedSha256);
}

AcquireResult AcquisitionStore::review(const std::string &url,
                                       const AcquireOptions &opts) {
    AcquireResult r;
    Uri u = parseUri(url);
    if (!u.valid) { r.error = "invalid URL: " + url; return r; }

    if (u.scheme == "https" || u.scheme == "http") {
        HttpClient client(opts.http);
        HttpResponse resp = client.head(url);
        r.httpStatus = resp.status;
        if (!resp.ok) { r.error = resp.error.empty() ? "HEAD failed" : resp.error; return r; }
        r.metadata = resp.headers;
        r.ok = (resp.status >= 200 && resp.status < 400);
        if (!r.ok) r.error = "server returned " + std::to_string(resp.status);
        return r;
    }
    if (u.scheme == "file") {
        if (!path_exists(u.path)) { r.error = "no such file: " + u.path; return r; }
        r.metadata["content-length"] = std::to_string(file_size(u.path));
        r.ok = true;
        return r;
    }
    if (u.scheme == "sftp" || u.scheme == "ssh") {
        // A true metadata-only stat would need an extra round trip; we report
        // the parsed target so callers can decide to fetch.
        r.metadata["host"] = u.host;
        r.metadata["path"] = u.path;
        r.metadata["note"] = "metadata-only review not supported for ssh/sftp; use fetch";
        r.ok = true;
        return r;
    }
    r.error = "unsupported scheme: " + u.scheme;
    return r;
}

AcquireResult AcquisitionStore::fetch(const std::string &url,
                                      const AcquireOptions &opts) {
    AcquireResult r;
    std::string err;
    if (!ensureLayout(err)) { r.error = err; return r; }

    Uri u = parseUri(url);
    if (!u.valid) { r.error = "invalid URL: " + url; return r; }

    // If the caller pinned content and we already have it, short-circuit.
    if (!opts.expectedSha256.empty() && has(opts.expectedSha256)) {
        r.ok = true;
        r.fromCache = true;
        r.sha256 = opts.expectedSha256;
        r.objectPath = objectPathFor(opts.expectedSha256);
        r.size = file_size(r.objectPath);
        return r;
    }

    if (u.scheme == "https" || u.scheme == "http") {
        HttpClient client(opts.http);
        HttpResponse resp = client.get(url);
        r.httpStatus = resp.status;
        if (!resp.ok) { r.error = resp.error.empty() ? "GET failed" : resp.error; return r; }
        if (resp.status < 200 || resp.status >= 300) {
            r.error = "server returned " + std::to_string(resp.status);
            return r;
        }
        r.metadata = resp.headers;
        AcquireResult c = commitBytes(resp.body, opts.expectedSha256);
        c.httpStatus = resp.status;
        c.metadata = resp.headers;
        return c;
    }

    if (u.scheme == "file") {
        if (!path_exists(u.path)) { r.error = "no such file: " + u.path; return r; }
        std::ifstream in(u.path, std::ios::binary);
        if (!in) { r.error = "could not read file: " + u.path; return r; }
        std::vector<uint8_t> bytes((std::istreambuf_iterator<char>(in)),
                                   std::istreambuf_iterator<char>());
        return commitBytes(bytes, opts.expectedSha256);
    }

    if (u.scheme == "sftp" || u.scheme == "ssh") {
        SftpClient client(opts.sftp);
        std::string tmp = root_ + "/tmp/sftp." + std::to_string((long)getpid()) + "." +
                          std::to_string((long)time(nullptr));
        SftpResult sr = client.download(u.user, u.host, u.effectivePort(), u.path, tmp);
        if (!sr.ok) {
            ::remove(tmp.c_str());
            r.error = sr.error.empty() ? "sftp/scp download failed" : sr.error;
            return r;
        }
        return commitFile(tmp, opts.expectedSha256);
    }

    r.error = "unsupported scheme: " + u.scheme;
    return r;
}

}  // namespace advantage
