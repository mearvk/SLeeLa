// transport_cli.cpp - Command-line front end for the secure transport layer.
//
// Subcommands:
//   review   <url>                         HTTP HEAD / metadata, no download
//   fetch    <url> [--store DIR] [--sha256 HEX] [--insecure] [--accept-new]
//                                           obtain + download + store (idempotent)
//   keystore <file> put  <name> <value>     store a secret (prompts passphrase via env)
//   keystore <file> get  <name>             print a secret
//   keystore <file> list                    list secret names
//   keystore <file> rm   <name>             remove a secret
//   kex      <group>                        demo a DH/ECDH exchange + session key
//
// The keystore passphrase is read from the ADVANTAGE_KS_PASS environment
// variable so it never appears in argv / process listings.
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "acquire.h"
#include "kex.h"
#include "keystore.h"

namespace {

int usage() {
    std::cerr <<
        "usage:\n"
        "  transport review   <url>\n"
        "  transport fetch    <url> [--store DIR] [--sha256 HEX] [--insecure] [--accept-new]\n"
        "  transport keystore <file> put <name> <value>\n"
        "  transport keystore <file> get <name>\n"
        "  transport keystore <file> list\n"
        "  transport keystore <file> rm <name>\n"
        "  transport kex <x25519|p256|ffdhe2048|ffdhe3072>\n"
        "\n"
        "keystore passphrase is read from $ADVANTAGE_KS_PASS\n";
    return 2;
}

std::string hexdump(const uint8_t *d, size_t n) {
    static const char *hx = "0123456789abcdef";
    std::string s;
    for (size_t i = 0; i < n; ++i) { s.push_back(hx[d[i] >> 4]); s.push_back(hx[d[i] & 15]); }
    return s;
}

int do_review(const std::string &url) {
    advantage::AcquisitionStore store("./advantage-store");
    advantage::AcquireResult r = store.review(url);
    if (!r.ok) { std::cerr << "review failed: " << r.error << "\n"; return 1; }
    std::cout << "OK";
    if (r.httpStatus) std::cout << " (HTTP " << r.httpStatus << ")";
    std::cout << "\n";
    for (const auto &kv : r.metadata) std::cout << "  " << kv.first << ": " << kv.second << "\n";
    return 0;
}

int do_fetch(int argc, char **argv) {
    if (argc < 1) return usage();
    std::string url = argv[0];
    std::string storeDir = "./advantage-store";
    advantage::AcquireOptions opts;
    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "--store" && i + 1 < argc) storeDir = argv[++i];
        else if (a == "--sha256" && i + 1 < argc) opts.expectedSha256 = argv[++i];
        else if (a == "--insecure") { opts.http.verifyPeer = false; opts.http.verifyHost = false; }
        else if (a == "--accept-new") opts.sftp.hostKey = advantage::HostKeyPolicy::AcceptNew;
        else { std::cerr << "unknown option: " << a << "\n"; return usage(); }
    }
    advantage::AcquisitionStore store(storeDir);
    advantage::AcquireResult r = store.fetch(url, opts);
    if (!r.ok) { std::cerr << "fetch failed: " << r.error << "\n"; return 1; }
    std::cout << (r.fromCache ? "cached " : "stored ") << r.sha256 << "\n";
    std::cout << "  path: " << r.objectPath << "\n";
    std::cout << "  size: " << r.size << " bytes\n";
    if (r.httpStatus) std::cout << "  http: " << r.httpStatus << "\n";
    return 0;
}

const char *req_pass() {
    const char *p = std::getenv("ADVANTAGE_KS_PASS");
    if (!p || !*p) {
        std::cerr << "error: set ADVANTAGE_KS_PASS to the keystore passphrase\n";
        return nullptr;
    }
    return p;
}

int do_keystore(int argc, char **argv) {
    if (argc < 2) return usage();
    std::string file = argv[0];
    std::string op = argv[1];
    const char *pass = req_pass();
    if (!pass) return 1;

    ks_store *ks = nullptr;
    ks_status st = ks_open(file.c_str(), pass, &ks);
    if (st != KS_OK) { std::cerr << "keystore open failed: " << ks_strerror(st) << "\n"; return 1; }

    int rc = 0;
    if (op == "put" && argc >= 4) {
        st = ks_put_str(ks, argv[2], argv[3]);
        if (st != KS_OK) { std::cerr << "put failed: " << ks_strerror(st) << "\n"; rc = 1; }
        else std::cout << "stored '" << argv[2] << "'\n";
    } else if (op == "get" && argc >= 3) {
        uint8_t *v = nullptr; size_t n = 0;
        st = ks_get(ks, argv[2], &v, &n);
        if (st != KS_OK) { std::cerr << "get failed: " << ks_strerror(st) << "\n"; rc = 1; }
        else { std::cout.write(reinterpret_cast<char *>(v), (std::streamsize)n); std::cout << "\n"; ks_free_secret(v, n); }
    } else if (op == "list") {
        size_t count = ks_count(ks);
        std::vector<char *> names(count ? count : 1);
        int n = ks_list(ks, names.data(), count);
        if (n < 0) { std::cerr << "list failed: " << ks_strerror((ks_status)n) << "\n"; rc = 1; }
        else { for (int i = 0; i < n; ++i) { std::cout << names[i] << "\n"; free(names[i]); } }
    } else if (op == "rm" && argc >= 3) {
        st = ks_remove(ks, argv[2]);
        if (st != KS_OK) { std::cerr << "rm failed: " << ks_strerror(st) << "\n"; rc = 1; }
        else std::cout << "removed '" << argv[2] << "'\n";
    } else {
        ks_close(ks);
        return usage();
    }
    ks_close(ks);
    return rc;
}

int do_kex(const std::string &groupName) {
    kex_group g;
    if (groupName == "x25519") g = KEX_X25519;
    else if (groupName == "p256") g = KEX_P256;
    else if (groupName == "ffdhe2048") g = KEX_FFDHE2048;
    else if (groupName == "ffdhe3072") g = KEX_FFDHE3072;
    else { std::cerr << "unknown group: " << groupName << "\n"; return usage(); }

    kex_keypair *a = nullptr, *b = nullptr;
    if (kex_generate(g, &a) != KEX_OK || kex_generate(g, &b) != KEX_OK) {
        std::cerr << "keygen failed\n"; kex_free(a); kex_free(b); return 1;
    }

    std::vector<uint8_t> apub(4096), bpub(4096);
    size_t al = apub.size(), bl = bpub.size();
    if (kex_export_public(a, apub.data(), &al) != KEX_OK ||
        kex_export_public(b, bpub.data(), &bl) != KEX_OK) {
        std::cerr << "export failed\n"; kex_free(a); kex_free(b); return 1;
    }
    apub.resize(al); bpub.resize(bl);

    uint8_t ka[32], kb[32];
    const char *info = "advantage/transport demo session";
    kex_status s1 = kex_derive(a, bpub.data(), bpub.size(), info, ka, sizeof ka);
    kex_status s2 = kex_derive(b, apub.data(), apub.size(), info, kb, sizeof kb);
    int rc = 0;
    if (s1 != KEX_OK || s2 != KEX_OK) {
        std::cerr << "derive failed: " << kex_strerror(s1) << " / " << kex_strerror(s2) << "\n";
        rc = 1;
    } else {
        std::cout << "group: " << kex_group_name(g) << "\n";
        std::cout << "A session key: " << hexdump(ka, sizeof ka) << "\n";
        std::cout << "B session key: " << hexdump(kb, sizeof kb) << "\n";
        std::cout << "match: " << (memcmp(ka, kb, sizeof ka) == 0 ? "yes" : "NO") << "\n";
        if (memcmp(ka, kb, sizeof ka) != 0) rc = 1;
    }
    kex_free(a); kex_free(b);
    return rc;
}

}  // namespace

int main(int argc, char **argv) {
    if (argc < 2) return usage();
    std::string cmd = argv[1];

    if (cmd == "review" && argc >= 3) return do_review(argv[2]);
    if (cmd == "fetch") return do_fetch(argc - 2, argv + 2);
    if (cmd == "keystore") return do_keystore(argc - 2, argv + 2);
    if (cmd == "kex" && argc >= 3) return do_kex(argv[2]);
    return usage();
}
