// sftp_client.cpp - SSH/SFTP retrieval via OpenSSH scp (see sftp_client.h).
//
// We spawn `scp` with an explicit argv vector through posix_spawn, so none
// of the user-controlled strings (host, remote path) are ever interpreted
// by a shell. Security-relevant ssh options are passed with -o:
//   StrictHostKeyChecking   yes | accept-new   (never "no")
//   BatchMode               yes                (no interactive prompts)
//   ConnectTimeout          <seconds>
// Host-key verification is thus always enforced.
#include "sftp_client.h"

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <vector>

#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>

extern char **environ;

namespace advantage {

namespace {

const char *policyString(HostKeyPolicy p) {
    switch (p) {
        case HostKeyPolicy::Strict:    return "StrictHostKeyChecking=yes";
        case HostKeyPolicy::AcceptNew: return "StrictHostKeyChecking=accept-new";
    }
    return "StrictHostKeyChecking=yes";
}

// Spawn argv (NULL-terminated), wait, return exit status or -1 on spawn error.
int run(const std::vector<std::string> &args, std::string &err) {
    std::vector<char *> argv;
    argv.reserve(args.size() + 1);
    for (const auto &a : args) argv.push_back(const_cast<char *>(a.c_str()));
    argv.push_back(nullptr);

    pid_t pid = 0;
    int rc = posix_spawnp(&pid, argv[0], nullptr, nullptr, argv.data(), environ);
    if (rc != 0) {
        err = std::string("failed to spawn ") + args[0] + ": " + std::strerror(rc);
        return -1;
    }
    int status = 0;
    while (waitpid(pid, &status, 0) < 0) {
        if (errno != EINTR) { err = "waitpid failed"; return -1; }
    }
    if (WIFEXITED(status)) return WEXITSTATUS(status);
    err = "child terminated abnormally";
    return -1;
}

}  // namespace

SftpResult SftpClient::download(const std::string &user, const std::string &host,
                                uint16_t port, const std::string &remotePath,
                                const std::string &localPath) {
    SftpResult res;
    if (host.empty() || remotePath.empty() || localPath.empty()) {
        res.error = "host, remote path and local path are required";
        return res;
    }

    const uint16_t p = (port != 0) ? port : opts_.port;

    std::vector<std::string> args;
    args.emplace_back("scp");
    args.emplace_back("-p");                      // preserve mtime
    args.emplace_back("-o"); args.emplace_back(policyString(opts_.hostKey));
    if (opts_.batchMode) { args.emplace_back("-o"); args.emplace_back("BatchMode=yes"); }
    args.emplace_back("-o");
    args.emplace_back("ConnectTimeout=" + std::to_string(opts_.connectTimeoutSec));
    if (!opts_.knownHostsFile.empty()) {
        args.emplace_back("-o");
        args.emplace_back("UserKnownHostsFile=" + opts_.knownHostsFile);
    }
    if (!opts_.identityFile.empty()) {
        args.emplace_back("-i");
        args.emplace_back(opts_.identityFile);
    }
    args.emplace_back("-P"); args.emplace_back(std::to_string(p));

    // Remote spec: [user@]host:remotePath. scp receives these as literal argv
    // entries, so there is no shell-metacharacter exposure on our side.
    std::string remote;
    if (!user.empty()) remote = user + "@";
    remote += host + ":" + remotePath;

    args.emplace_back(remote);
    args.emplace_back(localPath);

    std::string err;
    int code = run(args, err);
    res.exitCode = code;
    if (code == 0) {
        res.ok = true;
        res.localPath = localPath;
    } else {
        res.error = err.empty()
            ? ("scp exited with status " + std::to_string(code))
            : err;
    }
    return res;
}

}  // namespace advantage
