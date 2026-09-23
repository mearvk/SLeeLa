#include "http3_server.h"

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

namespace {
void usage() {
  std::cerr
      << "usage: http-server-3 [--addr ADDR] [--port PORT] "
         "--key PRIVATE_KEY --cert CERTIFICATE "
         "[--backend PATH]\n"
      << "default: --addr 0.0.0.0 --port 8082 "
         "--backend wsslserver\n";
}

bool take_value(int argc, char **argv, int &i, std::string &out) {
  if (i + 1 >= argc) return false;
  out = argv[++i];
  return !out.empty();
}

}  // namespace

extern "C" int sleela_http3_server_run(int argc, char **argv) {
  std::string addr = "0.0.0.0";
  std::string port = "8082";
  std::string key;
  std::string cert;
  std::string backend = "wsslserver";

  for (int i = 1; i < argc; ++i) {
    const std::string arg = argv[i];
    if (arg == "--addr") {
      if (!take_value(argc, argv, i, addr)) { usage(); return 2; }
    } else if (arg == "--port") {
      if (!take_value(argc, argv, i, port)) { usage(); return 2; }
    } else if (arg == "--key") {
      if (!take_value(argc, argv, i, key)) { usage(); return 2; }
    } else if (arg == "--cert") {
      if (!take_value(argc, argv, i, cert)) { usage(); return 2; }
    } else if (arg == "--backend") {
      if (!take_value(argc, argv, i, backend)) { usage(); return 2; }
    } else if (arg == "--help" || arg == "-h") {
      usage();
      return 0;
    } else {
      std::cerr << "http-server-3: unknown option: " << arg << "\n";
      usage();
      return 2;
    }
  }

  if (key.empty() || cert.empty()) {
    std::cerr << "http-server-3: HTTP/3 requires a QUIC TLS private key "
                 "and certificate.\n";
    usage();
    return 2;
  }

  // QUIC and HTTP/3 are deliberately not hand-implemented here. HTTP/3
  // requires QUIC v1, TLS 1.3, ALPN "h3", and QPACK. The SLeeLa Grade 3
  // process therefore delegates wire correctness to the ngtcp2 HTTP/3
  // backend (wsslserver) while retaining SLeeLa's command-line contract.
  //
  // The backend is supplied as a path or PATH-resolved executable. This keeps
  // the SLeeLa tree independent of a particular QUIC/TLS build while making
  // the transport dependency explicit and replaceable.
  std::string port_arg = port;
  std::string addr_arg = addr;

  char *const child_argv[] = {
      const_cast<char *>(backend.c_str()),
      const_cast<char *>(addr_arg.c_str()),
      const_cast<char *>(port_arg.c_str()),
      const_cast<char *>(key.c_str()),
      const_cast<char *>(cert.c_str()),
      nullptr
  };

  std::cerr << "SLeeLa HTTP/3 Grade 3 starting on UDP " << addr << ":"
            << port << " via " << backend
            << " (QUIC v1 / TLS 1.3 / ALPN h3 / QPACK)\n";

  execvp(child_argv[0], child_argv);
  const int err = errno;
  std::cerr << "http-server-3: failed to execute HTTP/3 backend '"
            << backend << "': " << std::strerror(err) << "\n";
  return 127;
}
