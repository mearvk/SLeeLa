#include "sleela_server.hpp"
#include <charconv>
#include <cstdio>
#include <cstdint>
#include <iostream>
#include <limits>
#include <string>
#include <string_view>

using namespace sleela::server;

namespace {
bool parse_unsigned(std::string_view text, std::uint64_t& value) {
 if (text.empty()) return false;
 value = 0;
 const auto result = std::from_chars(text.data(), text.data() + text.size(), value, 10);
 return result.ec == std::errc{} && result.ptr == text.data() + text.size();
}
void usage(std::ostream& out) {
 out << "SLeeLa Server Edition\n"
     << " Usage: sleelas [options]\n"
     << " --bind ADDRESS       IPv4 address (default: 127.0.0.1)\n"
     << " --allow-remote       Permit a non-loopback bind (requires protected deployment)\n"
     << " --port PORT          TCP port 1..65535 (default: 19866)\n"
     << " --log FILE           Structured log file\n"
     << " --log-payload         Log payload byte counts (not payload contents)\n"
     << " --max-packet BYTES    Maximum wire packet size\n"
     << " --help                Show this help\n";
}
bool is_loopback_ipv4(const std::string& address) {
 unsigned a = 0, b = 0, c = 0, d = 0;
 char tail = '\0';
 if (std::sscanf(address.c_str(), "%u.%u.%u.%u%c", &a, &b, &c, &d, &tail) != 4)
  return false;
 return a == 127 && b <= 255 && c <= 255 && d <= 255;
}
}
int main(int argc, char** argv) {
 ServerConfig cfg;
 bool allow_remote = false;
 bool show_help = false;
 for (int i = 1; i < argc; ++i) {
  const std::string arg(argv[i]);
  auto require_value = [&](const char* option) -> const char* {
   if (i + 1 >= argc || argv[i + 1] == nullptr || argv[i + 1][0] == '\0') {
    std::cerr << "Missing value for " << option << "\n";
    return nullptr;
   }
   return argv[++i];
  };
  if (arg == "--help" || arg == "-h") {
   show_help = true;
  } else if (arg == "--allow-remote") {
   allow_remote = true;
  } else if (arg == "--bind") {
   const char* value = require_value("--bind");
   if (!value) return 2;
   cfg.bind_address = value;
  } else if (arg == "--port") {
   const char* value = require_value("--port");
   if (!value) return 2;
   std::uint64_t parsed = 0;
   if (!parse_unsigned(value, parsed) || parsed < 1 || parsed > 65535) {
    std::cerr << "Invalid --port: expected an integer from 1 to 65535\n";
    return 2;
   }
   cfg.port = static_cast<std::uint16_t>(parsed);
  } else if (arg == "--log") {
   const char* value = require_value("--log");
   if (!value) return 2;
   cfg.log_file = value;
  } else if (arg == "--max-packet") {
   const char* value = require_value("--max-packet");
   if (!value) return 2;
   std::uint64_t parsed = 0;
   if (!parse_unsigned(value, parsed) || parsed < 40 ||
       parsed > static_cast<std::uint64_t>(std::numeric_limits<std::size_t>::max())) {
    std::cerr << "Invalid --max-packet: value must be at least 40 and fit size_t\n";
    return 2;
   }
   cfg.max_packet_bytes = static_cast<std::size_t>(parsed);
  } else if (arg == "--log-payload") {
   cfg.log_payload = true;
  } else {
   std::cerr << "Unknown option: " << arg << "\n";
   usage(std::cerr);
   return 2;
  }
 }
 if (show_help) {
  usage(std::cout);
  return 0;
 }
 if (!is_loopback_ipv4(cfg.bind_address) && !allow_remote) {
  std::cerr << "Refusing non-loopback bind without --allow-remote. "
               "Remote exposure requires a protected, authenticated deployment boundary.\n";
  return 2;
 }
 if (!is_loopback_ipv4(cfg.bind_address)) {
  std::cerr << "WARNING: remote binding requested. This server path does not provide built-in "
               "TLS or authenticated peer identity; do not expose it directly to untrusted networks.\n";
 }
 try {
  SleelaServer server(cfg);
  if (!server.start()) {
   std::cerr << "SLeeLa server: start failed; verify bind address, port, and permissions\n";
   return 1;
  }
  std::cout << "SLeeLa Server Edition listening on " << cfg.bind_address << ":" << cfg.port << "\n";
  server.run();
 } catch (const std::exception& e) {
  std::cerr << "SLeeLa server fatal: " << e.what() << "\n";
  return 2;
 }
 return 0;
}
