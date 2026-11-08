// sftp_client.h - Secure SSH/SFTP file retrieval via OpenSSH binaries.
//
// Rather than hand-rolling the SSH protocol, this drives the system's
// hardened OpenSSH client (sftp/scp) as a child process. Arguments are
// passed as an argv vector through posix_spawn -- never through a shell --
// so remote paths and hostnames cannot inject commands.
//
// Host-key policy defaults to strict ("yes"): an unknown or changed host key
// aborts the transfer. Callers may relax to "accept-new" (trust-on-first-use)
// but never to "no" via this API.
#ifndef ADVANTAGE_SFTP_CLIENT_H
#define ADVANTAGE_SFTP_CLIENT_H

#include <cstdint>
#include <string>

namespace advantage {

enum class HostKeyPolicy {
    Strict,     // StrictHostKeyChecking=yes (default; must be known)
    AcceptNew   // StrictHostKeyChecking=accept-new (TOFU, reject changes)
};

struct SftpOptions {
    HostKeyPolicy hostKey = HostKeyPolicy::Strict;
    std::string   identityFile;           // private key path (optional)
    std::string   knownHostsFile;         // override UserKnownHostsFile (optional)
    int           connectTimeoutSec = 30; // ConnectTimeout
    uint16_t      port = 22;              // may be overridden by URI
    bool          batchMode = true;       // no interactive password prompts
};

struct SftpResult {
    bool        ok = false;
    int         exitCode = -1;
    std::string error;       // populated when !ok
    std::string localPath;   // where the fetched file landed
};

class SftpClient {
public:
    explicit SftpClient(SftpOptions opts = {}) : opts_(std::move(opts)) {}

    // Download a remote file to a local path. `user`/`host`/`remotePath`
    // come from a parsed sftp:// or ssh:// URI. Uses scp under the hood.
    SftpResult download(const std::string &user, const std::string &host,
                        uint16_t port, const std::string &remotePath,
                        const std::string &localPath);

    const SftpOptions &options() const { return opts_; }

private:
    SftpOptions opts_;
};

}  // namespace advantage

#endif  // ADVANTAGE_SFTP_CLIENT_H
