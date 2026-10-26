/*
 * web-server-technology/native/dyn_sql_server.cpp
 *
 * SLeeLa dynamic, SQL-backed HTTP/1.1 server with IPv6/dual-stack listening.
 *
 * Architecture references (model, not copied code): Apache's modular MPM and
 * Tomcat's connector/request-worker pipeline. This server keeps the hardened
 * Grade-1 behaviour (bounded worker pool + connection queue, bounded headers
 * and bodies, strict framing, chunked-decode limits, Host enforcement,
 * keep-alive + timeouts, percent-decode + traversal containment, graceful
 * shutdown, synchronized logging) and adds:
 *
 *   - AF_INET6 dual-stack listening (IPv4-mapped addresses accepted on the same
 *     socket) with automatic AF_INET fallback, so the server answers the full
 *     range of world IP addresses.
 *   - A dynamic /db/... endpoint that runs SQL through the lib/sleela-sql
 *     engine and returns JSON computed per request.
 *
 * The /db endpoint is a dynamic surface, so it is deliberately conservative:
 * it is read-only by default (only SELECT / SELECT COUNT(*) / SHOW TABLES),
 * length-bounded, and never echoes the query string or body into ordinary
 * logs. Mutating statements are gated behind an explicit --db-writable flag.
 */
#if defined(_WIN32) || defined(_WIN64)
#include <iostream>
extern "C" int sleela_wst_dyn_sql_server_run(int, char **) {
  std::cerr << "sleela web-server-technology: the dynamic SQL server is "
               "implemented on the POSIX sockets backend and is not available "
               "on the Windows (Win32/Winsock) backend\n";
  return 2;
}
#else
#include "dyn_sql_server.h"
#include "envelopment.h"
#include "hardening.h"
#include "cpu_warmer.h"
#include "../../lib/sleela-sql/native/include/sleela_sql.h"

#include <arpa/inet.h>
#include <atomic>
#include <cctype>
#include <cerrno>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <mutex>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <signal.h>
#include <sstream>
#include <string>
#include <string_view>
#include <sys/socket.h>
#include <sys/time.h>
#include <thread>
#include <unistd.h>
#include <unordered_map>
#include <vector>

namespace fs = std::filesystem;
namespace {

constexpr size_t HMAX = 64 * 1024;       // max header block bytes
constexpr size_t BMAX = 8 * 1024 * 1024; // max request body bytes
constexpr size_t QMAX = 128;             // max queued connections
constexpr size_t NMAX = 100;             // max header lines
constexpr size_t SQLMAX = 8 * 1024;      // max SQL text length
constexpr size_t ROWMAX = 10000;         // max rows serialized to JSON
constexpr unsigned TMAX = 256, TDEFAULT = 16;

std::atomic<bool> g_stop{false};
void on_signal(int) { g_stop = true; }

struct Req {
  std::string method, target, path, query, version;
  std::unordered_map<std::string, std::string> h;
  std::vector<char> body;
  bool keep = false;
};
struct Resp {
  int code = 200;
  std::string type = "text/plain; charset=utf-8";
  std::vector<char> body;
  bool keep = false, head = false;
};

struct Config {
  fs::path root = ".";
  fs::path db_dir;        // directory of CSV tables; empty => /db disabled
  fs::path log_path;
  bool db_writable = false;
  std::string bind_addr; // empty => any address (dual-stack)
  // Side 2: envelopment. When env.active() is false the server behaves exactly
  // as Side 1 (single program/meme per domain) and never consults this.
  sleela::wst::Envelopment env;

  // Section-3 hardening. All optional; each defaults to off/open.
  // RateLimiter holds a mutex, so Config is non-copyable (passed by ref only).
  mutable sleela::wst::RateLimiter rate;   // admission control (disabled by default)
  sleela::wst::WriteAuth write_auth;       // bearer-token gate on /db mutations
  std::vector<std::string> trusted_proxies;// peers whose Forwarded hdr we honour

  // CPU keep-warm idle floor. ON BY DEFAULT (admin may disable). The running
  // warmer is owned by run(); this holds the configured band for status, and
  // warm_live points at the running warmer so /__sleela/status can report the
  // live measured utilization (nullptr until started / when disabled).
  sleela::wst::CpuWarmerConfig warm;
  const sleela::wst::CpuWarmer *warm_live = nullptr;
};

std::string lo(std::string s) {
  for (char &c : s) c = (char)std::tolower((unsigned char)c);
  return s;
}

const char *reason(int s) {
  switch (s) {
    case 200: return "OK";
    case 204: return "No Content";
    case 400: return "Bad Request";
    case 401: return "Unauthorized";
    case 403: return "Forbidden";
    case 404: return "Not Found";
    case 405: return "Method Not Allowed";
    case 413: return "Content Too Large";
    case 422: return "Unprocessable Entity";
    case 431: return "Request Header Fields Too Large";
    case 500: return "Internal Server Error";
    case 503: return "Service Unavailable";
    default:  return "Error";
  }
}

bool sendall(int fd, const char *p, size_t n) {
  while (n) {
    ssize_t w = send(fd, p, n, MSG_NOSIGNAL);
    if (w < 0) { if (errno == EINTR) continue; return false; }
    if (!w) return false;
    p += w; n -= (size_t)w;
  }
  return true;
}

bool recvmore(int fd, std::string &b) {
  char x[16384];
  ssize_t n = recv(fd, x, sizeof x, 0);
  if (n < 0) { if (errno == EINTR) return true; return false; }
  if (!n) return false;
  if (b.size() + (size_t)n > HMAX + BMAX + 16384) return false;
  b.append(x, (size_t)n);
  return true;
}

bool token(std::string_view s) {
  if (s.empty()) return false;
  for (unsigned char c : s)
    if (c <= 32 || c >= 127 ||
        std::string_view("()<>@,;:\\\"/[]?={} \t").find((char)c) !=
            std::string_view::npos)
      return false;
  return true;
}

// ---- JSON string escaping (RFC 8259) ---------------------------------------
std::string json_escape(const std::string &s) {
  std::string o;
  o.reserve(s.size() + 8);
  for (unsigned char c : s) {
    switch (c) {
      case '"':  o += "\\\""; break;
      case '\\': o += "\\\\"; break;
      case '\b': o += "\\b"; break;
      case '\f': o += "\\f"; break;
      case '\n': o += "\\n"; break;
      case '\r': o += "\\r"; break;
      case '\t': o += "\\t"; break;
      default:
        if (c < 0x20) {
          char buf[8];
          std::snprintf(buf, sizeof buf, "\\u%04x", c);
          o += buf;
        } else {
          o += (char)c;
        }
    }
  }
  return o;
}

// ---- HTTP/1.x request parsing (hardened) -----------------------------------
int hx(char c) {
  if (c >= '0' && c <= '9') return c - '0';
  if (c >= 'a' && c <= 'f') return c - 'a' + 10;
  if (c >= 'A' && c <= 'F') return c - 'A' + 10;
  return -1;
}

bool parse(int fd, std::string &b, Req &r, std::string &e) {
  size_t z = b.find("\r\n\r\n");
  if (z == std::string::npos) return false;
  std::istringstream in(b.substr(0, z));
  std::string line, extra;
  if (!std::getline(in, line)) { e = "bad request"; return false; }
  if (!line.empty() && line.back() == '\r') line.pop_back();
  std::istringstream f(line);
  if (!(f >> r.method >> r.target >> r.version) || (f >> extra)) {
    e = "bad request line"; return false;
  }
  if (r.method.size() > 32 || !token(r.method)) { e = "bad method"; return false; }
  if (r.target.empty() || r.target.size() > 16384) { e = "bad target"; return false; }
  if (r.version != "HTTP/1.0" && r.version != "HTTP/1.1") {
    e = "unsupported HTTP version"; return false;
  }
  size_t hc = 0;
  while (std::getline(in, line)) {
    if (!line.empty() && line.back() == '\r') line.pop_back();
    if (line.empty()) continue;
    if (++hc > NMAX) { e = "too many headers"; return false; }
    size_t c = line.find(':');
    if (c == std::string::npos || c == 0) { e = "bad header"; return false; }
    std::string n = lo(line.substr(0, c)), v = line.substr(c + 1);
    while (!v.empty() && (v[0] == ' ' || v[0] == '\t')) v.erase(v.begin());
    if (!token(n) || v.size() > 16384) { e = "bad header"; return false; }
    if (r.h.count(n) && n == "content-length") {
      e = "duplicate content-length"; return false;
    }
    r.h[n] = v;
  }
  if (r.version == "HTTP/1.1" && !r.h.count("host")) { e = "Host required"; return false; }

  bool cl = false;
  uint64_t len = 0;
  if (r.h.count("content-length")) {
    cl = true;
    for (char c : r.h["content-length"]) {
      if (c < '0' || c > '9') { e = "bad content-length"; return false; }
      uint64_t n = len * 10 + (c - '0');
      if (n < len || n > BMAX) { e = "body too large"; return false; }
      len = n;
    }
  }
  if (r.h.count("transfer-encoding")) {
    if (r.version == "HTTP/1.0") { e = "Transfer-Encoding is invalid for HTTP/1.0"; return false; }
    if (cl || lo(r.h["transfer-encoding"]) != "chunked") { e = "invalid transfer-encoding"; return false; }
  }
  size_t used = z + 4;
  if (r.h.count("transfer-encoding")) {
    for (;;) {
      size_t le = b.find("\r\n", used);
      while (le == std::string::npos) {
        if (!recvmore(fd, b)) { e = "incomplete chunk"; return false; }
        le = b.find("\r\n", used);
      }
      std::string s = b.substr(used, le - used);
      size_t q = s.find(';');
      if (q != std::string::npos) s.resize(q);
      if (s.empty() || s.size() > 16) { e = "bad chunk size"; return false; }
      uint64_t n = 0;
      for (char c : s) {
        int d = hx(c);
        if (d < 0) { e = "bad chunk size"; return false; }
        n = n * 16 + d;
        if (n > BMAX) { e = "body too large"; return false; }
      }
      used = le + 2;
      while (b.size() < used + n + 2)
        if (!recvmore(fd, b)) { e = "incomplete chunk body"; return false; }
      r.body.insert(r.body.end(), b.data() + used, b.data() + used + n);
      used += (size_t)n;
      if (b.compare(used, 2, "\r\n")) { e = "bad chunk terminator"; return false; }
      used += 2;
      if (!n) break;
    }
  } else if (len) {
    while (b.size() < used + len)
      if (!recvmore(fd, b)) { e = "incomplete body"; return false; }
    r.body.assign(b.data() + used, b.data() + used + len);
    used += (size_t)len;
  }
  b.erase(0, used);
  std::string cv = r.h.count("connection") ? lo(r.h["connection"]) : "";
  r.keep = r.version == "HTTP/1.1" ? cv.find("close") == std::string::npos
                                   : cv.find("keep-alive") != std::string::npos;
  size_t q = r.target.find('?');
  if (q == std::string::npos) {
    r.path = r.target;
  } else {
    r.path = r.target.substr(0, q);
    r.query = r.target.substr(q + 1);
  }
  if (r.path.empty()) r.path = "/";
  return true;
}

std::string percent_decode(const std::string &u, bool &ok) {
  std::string d;
  ok = true;
  for (size_t i = 0; i < u.size(); ++i) {
    char c = u[i];
    if (c == '+') { d.push_back(' '); continue; }
    if (c == '%') {
      if (i + 2 >= u.size()) { ok = false; return d; }
      int a = hx(u[i + 1]), b = hx(u[i + 2]);
      if (a < 0 || b < 0) { ok = false; return d; }
      c = (char)((a << 4) | b);
      i += 2;
    }
    d.push_back(c);
  }
  return d;
}

// ---- static file serving ----------------------------------------------------
// The request path (e.g. "/index.html") is treated as root-relative: leading
// slashes are stripped so the URL maps under the document root rather than the
// filesystem root. Percent escapes are decoded, then the result is normalized
// and verified to stay within root (no "..", no absolute escape, canonical
// prefix check) so traversal is contained.
bool safe(const fs::path &root, const std::string &u, fs::path &o) {
  std::string d;
  for (size_t i = 0; i < u.size(); ++i) {
    unsigned char c = u[i];
    if (c == '%') {
      if (i + 2 >= u.size()) return false;
      int a = hx(u[i + 1]), b = hx(u[i + 2]);
      if (a < 0 || b < 0) return false;
      c = (unsigned char)((a << 4) | b);
      i += 2;
    }
    if (!c || c == '\\' || c == '\r' || c == '\n') return false;
    d.push_back((char)c);
  }
  // Make the decoded path root-relative by dropping any leading slashes.
  size_t start = 0;
  while (start < d.size() && d[start] == '/') ++start;
  d = d.substr(start);
  if (d.empty()) d = ".";
  fs::path p = fs::path(d).lexically_normal();
  if (p.is_absolute()) return false;
  for (const auto &x : p) if (x == "..") return false;
  std::error_code ec, ec2;
  auto rr = fs::weakly_canonical(root, ec), cc = fs::weakly_canonical(root / p, ec2);
  if (ec || ec2) return false;
  auto a = rr.begin(), b = cc.begin();
  for (; a != rr.end() && b != cc.end() && *a == *b; ++a, ++b) {}
  if (a != rr.end()) return false;
  o = cc;
  return true;
}

std::string mime(const fs::path &p) {
  auto e = lo(p.extension().string());
  if (e == ".html" || e == ".htm") return "text/html; charset=utf-8";
  if (e == ".css") return "text/css; charset=utf-8";
  if (e == ".js") return "text/javascript; charset=utf-8";
  if (e == ".json") return "application/json";
  if (e == ".txt" || e == ".md") return "text/plain; charset=utf-8";
  if (e == ".xml") return "application/xml";
  if (e == ".svg") return "image/svg+xml";
  if (e == ".png") return "image/png";
  if (e == ".jpg" || e == ".jpeg") return "image/jpeg";
  return "application/octet-stream";
}

// ---- logging & worker pool ---------------------------------------------------
class Log {
  std::mutex m;
  std::ofstream f;
 public:
  explicit Log(const fs::path &p) {
    std::error_code e;
    fs::create_directories(p.parent_path(), e);
    f.open(p, std::ios::app);
  }
  void put(const std::string &s) {
    std::lock_guard<std::mutex> g(m);
    std::cerr << s << '\n';
    if (f) f << s << '\n';
  }
};

// An accepted connection: its descriptor plus the immediate TCP peer IP (used
// for rate limiting, logging, and trusted-proxy client-IP resolution).
struct Conn {
  int fd;
  std::string peer_ip;
};

class Pool {
  std::mutex m;
  std::condition_variable cv;
  std::deque<Conn> q;
  std::vector<std::thread> w;
  bool end = false;
 public:
  Pool(unsigned n, std::function<void(Conn)> fn) {
    for (unsigned i = 0; i < n; ++i)
      w.emplace_back([this, fn] {
        for (;;) {
          Conn c;
          {
            std::unique_lock<std::mutex> g(m);
            cv.wait(g, [this] { return end || !q.empty(); });
            if (end && q.empty()) return;
            c = q.front();
            q.pop_front();
          }
          fn(c);
        }
      });
  }
  bool add(const Conn &c) {
    std::lock_guard<std::mutex> g(m);
    if (end || q.size() >= QMAX) return false;
    q.push_back(c);
    cv.notify_one();
    return true;
  }
  void stop() {
    { std::lock_guard<std::mutex> g(m); end = true; }
    cv.notify_all();
    for (auto &t : w) if (t.joinable()) t.join();
  }
};

bool response(int fd, const Req &q, const Resp &r, const std::string &server) {
  std::ostringstream h;
  h << q.version << ' ' << r.code << ' ' << reason(r.code)
    << "\r\nServer: " << server << "\r\nContent-Type: " << r.type << "\r\n";
  if (r.code != 204) h << "Content-Length: " << r.body.size() << "\r\n";
  h << "Connection: " << (r.keep ? "keep-alive" : "close") << "\r\n\r\n";
  auto s = h.str();
  return sendall(fd, s.data(), s.size()) &&
         (r.head || r.body.empty() || sendall(fd, r.body.data(), r.body.size()));
}

// ---- dynamic /db endpoint ----------------------------------------------------
//
// GET  /db/health                      -> engine status JSON
// GET  /db/query?sql=SELECT...         -> rows as JSON (read-only)
// POST /db/query  (body = SQL)         -> rows as JSON; writable only with flag
//
// The engine writes CSV to a FILE*; we capture it via open_memstream and
// convert the first line (header) + following lines into a JSON object.
std::string csv_to_json(const std::string &csv, bool &ok) {
  ok = true;
  std::vector<std::string> lines;
  std::string cur;
  for (char c : csv) {
    if (c == '\n') { lines.push_back(cur); cur.clear(); }
    else if (c != '\r') cur.push_back(c);
  }
  if (!cur.empty()) lines.push_back(cur);

  auto split = [](const std::string &l) {
    std::vector<std::string> out;
    std::string field;
    for (char c : l) {
      if (c == ',') { out.push_back(field); field.clear(); }
      else field.push_back(c);
    }
    out.push_back(field);
    return out;
  };

  std::ostringstream o;
  if (lines.empty()) {
    o << "{\"columns\":[],\"rows\":[],\"row_count\":0}";
    return o.str();
  }
  auto cols = split(lines[0]);
  o << "{\"columns\":[";
  for (size_t i = 0; i < cols.size(); ++i) {
    if (i) o << ',';
    o << '"' << json_escape(cols[i]) << '"';
  }
  o << "],\"rows\":[";
  size_t rows = 0;
  for (size_t i = 1; i < lines.size() && rows < ROWMAX; ++i, ++rows) {
    auto vals = split(lines[i]);
    if (rows) o << ',';
    o << '{';
    for (size_t c = 0; c < cols.size(); ++c) {
      if (c) o << ',';
      o << '"' << json_escape(cols[c]) << "\":\""
        << json_escape(c < vals.size() ? vals[c] : std::string()) << '"';
    }
    o << '}';
  }
  o << "],\"row_count\":" << rows << '}';
  return o.str();
}

// classify a statement's leading keyword so the read-only policy can be enforced
bool statement_is_readonly(const std::string &sql) {
  size_t i = 0;
  while (i < sql.size() && std::isspace((unsigned char)sql[i])) ++i;
  std::string head;
  while (i < sql.size() && (std::isalpha((unsigned char)sql[i]) || sql[i] == '(')) {
    head.push_back((char)std::tolower((unsigned char)sql[i]));
    ++i;
  }
  // classic SQL read verbs
  if (head.rfind("select", 0) == 0) return true;
  if (head.rfind("show", 0) == 0) return true;
  // fluent SLeeLaSQL read forms begin with from(... or tables(
  if (head.rfind("from", 0) == 0) return true;
  if (head.rfind("tables", 0) == 0) return true;
  return false;
}

// Execute the /db protocol (health + query) against an explicit table dir and
// write policy. Used both by Side-1 (--db/--db-writable) and by Side-2
// envelopment SQL programs, which supply their own per-program dir/policy.
// query_path is the request path with any program mount prefix already removed,
// so an enveloped SQL program at mount "/lab" still answers "/db/query".
// Bind an ssql_db to a directory WITHOUT the per-request stat()/mkdir() that
// ssql_open performs. Safe because every db directory the server will use is
// canonicalized and validated once at startup (Side-1 --db and each envelopment
// SQL program). This removes a filesystem syscall from the hot request path --
// the handle-reuse optimization. Returns false only if the path is too long.
bool bind_db(ssql_db *db, const std::string &dir) {
  if (dir.empty() || dir.size() >= sizeof(db->dir)) return false;
  std::snprintf(db->dir, sizeof(db->dir), "%s", dir.c_str());
  return true;
}

Resp db_endpoint_on(const Req &q, const std::string &query_path,
                    const fs::path &db_dir, bool db_writable,
                    const sleela::wst::WriteAuth &write_auth,
                    std::string &log_detail) {
  Resp r;
  r.keep = q.keep;
  r.type = "application/json";

  if (db_dir.empty()) {
    r.code = 503;
    std::string b = "{\"error\":\"database endpoint disabled (start with --db DIR)\"}\n";
    r.body.assign(b.begin(), b.end());
    r.keep = false;
    return r;
  }

  if (query_path == "/db/health" || query_path == "/db" || query_path == "/db/") {
    ssql_db db;
    bool bound = bind_db(&db, db_dir.string());
    std::ostringstream o;
    o << "{\"server\":\"SLeeLa\",\"engine\":\"sleela-sql\",\"dialects\":[\"sql\",\"sleelasql\"],"
      << "\"writable\":" << (db_writable ? "true" : "false") << ","
      << "\"status\":\"" << (bound ? "ok" : "bad-path") << "\"}\n";
    std::string b = o.str();
    r.body.assign(b.begin(), b.end());
    log_detail = "db-health";
    return r;
  }

  if (query_path != "/db/query") {
    r.code = 404;
    std::string b = "{\"error\":\"unknown db route\"}\n";
    r.body.assign(b.begin(), b.end());
    return r;
  }

  // Extract the SQL text: ?sql=... for GET, request body for POST.
  std::string sql;
  if (q.method == "GET") {
    // parse query string for sql=
    std::string_view qs = q.query;
    size_t pos = 0;
    while (pos < qs.size()) {
      size_t amp = qs.find('&', pos);
      std::string pair(qs.substr(pos, amp == std::string::npos ? std::string::npos : amp - pos));
      size_t eq = pair.find('=');
      if (eq != std::string::npos && pair.substr(0, eq) == "sql") {
        bool ok;
        sql = percent_decode(pair.substr(eq + 1), ok);
        if (!ok) sql.clear();
      }
      if (amp == std::string::npos) break;
      pos = amp + 1;
    }
  } else if (q.method == "POST") {
    sql.assign(q.body.begin(), q.body.end());
  } else {
    r.code = 405;
    std::string b = "{\"error\":\"use GET ?sql= or POST body\"}\n";
    r.body.assign(b.begin(), b.end());
    r.keep = false;
    return r;
  }

  // trim
  while (!sql.empty() && std::isspace((unsigned char)sql.back())) sql.pop_back();
  size_t s0 = 0;
  while (s0 < sql.size() && std::isspace((unsigned char)sql[s0])) ++s0;
  sql = sql.substr(s0);

  if (sql.empty()) {
    r.code = 400;
    std::string b = "{\"error\":\"missing sql\"}\n";
    r.body.assign(b.begin(), b.end());
    log_detail = "db-query empty";
    return r;
  }
  if (sql.size() > SQLMAX) {
    r.code = 413;
    std::string b = "{\"error\":\"sql too long\"}\n";
    r.body.assign(b.begin(), b.end());
    r.keep = false;
    log_detail = "db-query oversize";
    return r;
  }
  // reject multiple statements: a ';' that is not the final character
  size_t semi = sql.find(';');
  if (semi != std::string::npos && semi + 1 < sql.size()) {
    // allow a single trailing ';' plus whitespace
    bool only_ws = true;
    for (size_t i = semi + 1; i < sql.size(); ++i)
      if (!std::isspace((unsigned char)sql[i])) { only_ws = false; break; }
    if (!only_ws) {
      r.code = 422;
      std::string b = "{\"error\":\"only a single statement is permitted\"}\n";
      r.body.assign(b.begin(), b.end());
      log_detail = "db-query multi-statement-rejected";
      return r;
    }
  }

  bool readonly = statement_is_readonly(sql);
  if (!readonly && !db_writable) {
    r.code = 403;
    std::string b = "{\"error\":\"this endpoint is read-only; start with --db-writable to allow mutations\"}\n";
    r.body.assign(b.begin(), b.end());
    log_detail = "db-query write-denied";
    return r;
  }
  // Writes additionally require a valid bearer token when one is configured.
  if (!readonly && write_auth.required()) {
    auto ai = q.h.find("authorization");
    std::string authz = ai != q.h.end() ? ai->second : std::string();
    if (!write_auth.authorize(authz)) {
      r.code = 401;
      std::string b = "{\"error\":\"write authorization required\"}\n";
      r.body.assign(b.begin(), b.end());
      r.keep = false;
      log_detail = "db-query write-unauthorized";
      return r;
    }
  }

  ssql_db db;
  if (!bind_db(&db, db_dir.string())) {
    r.code = 500;
    std::string b = "{\"error\":\"cannot open database\"}\n";
    r.body.assign(b.begin(), b.end());
    r.keep = false;
    log_detail = "db-open-failed";
    return r;
  }

  char *buf = nullptr;
  size_t buflen = 0;
  FILE *mem = open_memstream(&buf, &buflen);
  ssql_status st;
  if (mem) {
    st = ssql_exec(&db, sql.c_str(), mem);
    fclose(mem);
  } else {
    st = ssql_exec(&db, sql.c_str(), nullptr);
  }

  if (st != SSQL_OK) {
    r.code = 422;
    std::ostringstream o;
    o << "{\"error\":\"" << json_escape(ssql_strerror(st)) << "\"}\n";
    std::string b = o.str();
    r.body.assign(b.begin(), b.end());
    if (buf) free(buf);
    log_detail = std::string("db-query sql-error status=") + std::to_string((int)st);
    return r;
  }

  std::string csv = buf ? std::string(buf, buflen) : std::string();
  if (buf) free(buf);

  std::string body;
  if (readonly) {
    bool ok;
    body = csv_to_json(csv, ok) + "\n";
  } else {
    // mutation succeeded: engine returns no rows
    body = "{\"ok\":true,\"mutated\":true}\n";
  }
  r.body.assign(body.begin(), body.end());
  log_detail = std::string("db-query ok readonly=") + (readonly ? "1" : "0");
  return r;
}

// Side-1 /db endpoint: server-wide db dir + policy, path used verbatim.
Resp db_endpoint(const Req &q, const Config &cfg, std::string &log_detail) {
  return db_endpoint_on(q, q.path, cfg.db_dir, cfg.db_writable, cfg.write_auth,
                        log_detail);
}

// ---- static + meme program handlers (shared by Side 1 and envelopment) ------
Resp serve_static(const Req &q, const fs::path &root, const std::string &rel_path) {
  Resp r;
  r.keep = q.keep;
  r.head = q.method == "HEAD";
  if (q.method == "POST") {
    r.code = 405; r.keep = false;
    std::string b = "Method Not Allowed\n";
    r.body.assign(b.begin(), b.end());
    return r;
  }
  fs::path p;
  if (!safe(root, rel_path, p)) {
    r.code = 403; r.keep = false;
    std::string b = "Forbidden\n";
    r.body.assign(b.begin(), b.end());
    return r;
  }
  std::error_code e;
  if (fs::is_directory(p, e)) p /= "index.html";
  if (!fs::is_regular_file(p, e)) {
    r.code = 404;
    std::string b = "Not Found\n";
    r.body.assign(b.begin(), b.end());
    return r;
  }
  auto n = fs::file_size(p, e);
  if (e || n > BMAX) {
    r.code = 413; r.keep = false;
    std::string b = "Content Too Large\n";
    r.body.assign(b.begin(), b.end());
    return r;
  }
  std::ifstream f(p, std::ios::binary);
  if (!f) {
    r.code = 403; r.keep = false;
    std::string b = "Forbidden\n";
    r.body.assign(b.begin(), b.end());
    return r;
  }
  r.type = mime(p);
  r.body.resize((size_t)n);
  if (n) f.read(r.body.data(), (std::streamsize)n);
  if (!f && n) { r.code = 500; r.body.clear(); r.keep = false; }
  return r;
}

// ---- envelopment program runner (Side 2) ------------------------------------
// A warrant has already selected `prog`; run its backing kind. `rel` is the
// request path with the program's mount prefix stripped (so a program sees its
// own root-relative path space). This is the only entry point Side 2 adds to
// request handling, and it reuses the same static/SQL machinery as Side 1.
Resp run_program(const Req &q, const sleela::wst::Program &prog,
                 const std::string &rel,
                 const sleela::wst::WriteAuth &write_auth,
                 std::string &log_detail) {
  using sleela::wst::ProgramKind;
  switch (prog.kind) {
    case ProgramKind::Meme: {
      Resp r;
      r.keep = q.keep;
      r.type = prog.meme_type.empty() ? "text/plain; charset=utf-8" : prog.meme_type;
      std::string b = prog.meme_body;
      if (b.empty() || b.back() != '\n') b.push_back('\n');
      r.body.assign(b.begin(), b.end());
      log_detail = "program kind=meme name=" + prog.name;
      return r;
    }
    case ProgramKind::Static: {
      std::string rp = rel.empty() ? "/" : rel;
      Resp r = serve_static(q, prog.root, rp);
      log_detail = "program kind=static name=" + prog.name;
      return r;
    }
    case ProgramKind::Sql: {
      // Map the program's own path space onto the /db protocol: the program
      // owns "/db..." under its mount. Anything else under the program is 404.
      std::string qp = rel;
      if (qp.empty() || qp == "/") qp = "/db/health";
      Resp r = db_endpoint_on(q, qp, prog.db_dir, prog.db_writable, write_auth,
                              log_detail);
      log_detail = "program kind=sql name=" + prog.name + " " + log_detail;
      return r;
    }
  }
  Resp r;
  r.code = 500;
  std::string b = "Internal Server Error\n";
  r.body.assign(b.begin(), b.end());
  return r;
}

// ---- request dispatch --------------------------------------------------------
Resp make(const Req &q, const Config &cfg, std::string &log_detail) {
  Resp r;
  r.keep = q.keep;
  r.head = q.method == "HEAD";

  if (q.method != "GET" && q.method != "HEAD" && q.method != "POST" &&
      q.method != "OPTIONS") {
    r.code = 405; r.keep = false;
    std::string b = "Method Not Allowed\n";
    r.body.assign(b.begin(), b.end());
    return r;
  }
  if (q.method == "OPTIONS") { r.code = 204; return r; }

  if (q.path == "/__sleela/health") {
    std::string b = "SLeeLa dynamic SQL server OK\n";
    r.body.assign(b.begin(), b.end());
    return r;
  }
  if (q.path == "/__sleela/status") {
    std::ostringstream o;
    o << "{\"server\":\"SLeeLa\",\"kind\":\"web-server-technology/dynamic-sql\","
      << "\"http\":\"1.1\",\"dual_stack\":true,\"db_endpoint\":"
      << (cfg.db_dir.empty() ? "false" : "true")
      << ",\"db_writable\":" << (cfg.db_writable ? "true" : "false")
      << ",\"envelopment\":" << (cfg.env.active() ? "true" : "false")
      << ",\"scientist\":" << (cfg.env.scientist ? "true" : "false")
      << ",\"programs\":" << cfg.env.programs.size()
      << ",\"warrants\":" << cfg.env.warrants.size()
      << ",\"rate_limit\":" << (cfg.rate.enabled() ? "true" : "false")
      << ",\"write_auth\":" << (cfg.write_auth.required() ? "true" : "false")
      << ",\"trusted_proxies\":" << cfg.trusted_proxies.size()
      << ",\"cpu_warm\":" << (cfg.warm.enabled ? "true" : "false")
      << ",\"cpu_warm_floor_pct\":" << cfg.warm.floor_pct
      << ",\"cpu_warm_ceiling_pct\":" << cfg.warm.ceiling_pct
      << ",\"cpu_warm_cores\":" << (cfg.warm_live ? cfg.warm_live->threads() : 0u);
    if (cfg.warm_live) {
      char pbuf[32];
      std::snprintf(pbuf, sizeof pbuf, "%.2f", cfg.warm_live->measured_pct());
      o << ",\"cpu_warm_measured_pct\":" << pbuf;
    }
    o << "}\n";
    std::string b = o.str();
    r.type = "application/json";
    r.body.assign(b.begin(), b.end());
    return r;
  }

  if (q.path == "/__sleela/warm") {
    // Per-core keep-warm readings. Reports the configured band plus the live
    // measured utilization of each warmer thread (empty when disabled).
    std::ostringstream o;
    o << "{\"enabled\":" << (cfg.warm.enabled ? "true" : "false")
      << ",\"floor_pct\":" << cfg.warm.floor_pct
      << ",\"ceiling_pct\":" << cfg.warm.ceiling_pct
      << ",\"cores\":" << (cfg.warm_live ? cfg.warm_live->threads() : 0u)
      << ",\"per_core_pct\":[";
    if (cfg.warm_live) {
      auto v = cfg.warm_live->measured_per_core();
      for (size_t i = 0; i < v.size(); ++i) {
        if (i) o << ',';
        char pbuf[32];
        std::snprintf(pbuf, sizeof pbuf, "%.2f", v[i]);
        o << pbuf;
      }
    }
    o << "]}\n";
    std::string b = o.str();
    r.type = "application/json";
    r.body.assign(b.begin(), b.end());
    return r;
  }

  // Side 2: envelopment. Only consulted when active (enabled + scientist +
  // >=1 warrant). A matched warrant selects one program, which is then run in
  // its own mount-relative path space. If nothing matches we fall through to
  // the unchanged Side-1 dispatch below.
  if (cfg.env.active()) {
    std::string host;
    auto hi = q.h.find("host");
    if (hi != q.h.end()) {
      host = hi->second;
      size_t colon = host.find(':');  // strip :port
      if (colon != std::string::npos) host = host.substr(0, colon);
    }
    std::string cause;
    const sleela::wst::Program *prog = cfg.env.resolve(host, q.path, cause);
    if (prog) {
      // compute the program-relative path by removing the mount prefix
      std::string rel = q.path;
      if (!prog->mount.empty() && prog->mount != "/" &&
          rel.rfind(prog->mount, 0) == 0) {
        rel = rel.substr(prog->mount.size());
        if (rel.empty()) rel = "/";
        if (rel[0] != '/') rel = "/" + rel;
      }
      std::string pdetail;
      Resp r2 = run_program(q, *prog, rel, cfg.write_auth, pdetail);
      log_detail = "envelopment cause=\"" + cause + "\" host=" +
                   (host.empty() ? "-" : host) + " " + pdetail;
      return r2;
    }
    // no warrant matched: fall through to Side-1 behaviour
  }

  // dynamic SQL surface
  if (q.path == "/db" || q.path.rfind("/db/", 0) == 0) {
    return db_endpoint(q, cfg, log_detail);
  }

  // static file serving (HEAD on files handled by r.head)
  if (q.method == "POST") {
    r.code = 405; r.keep = false;
    std::string b = "Method Not Allowed\n";
    r.body.assign(b.begin(), b.end());
    return r;
  }
  fs::path p;
  if (!safe(cfg.root, q.path, p)) {
    r.code = 403; r.keep = false;
    std::string b = "Forbidden\n";
    r.body.assign(b.begin(), b.end());
    return r;
  }
  std::error_code e;
  if (fs::is_directory(p, e)) p /= "index.html";
  if (!fs::is_regular_file(p, e)) {
    r.code = 404;
    std::string b = "Not Found\n";
    r.body.assign(b.begin(), b.end());
    return r;
  }
  auto n = fs::file_size(p, e);
  if (e || n > BMAX) {
    r.code = 413; r.keep = false;
    std::string b = "Content Too Large\n";
    r.body.assign(b.begin(), b.end());
    return r;
  }
  std::ifstream f(p, std::ios::binary);
  if (!f) {
    r.code = 403; r.keep = false;
    std::string b = "Forbidden\n";
    r.body.assign(b.begin(), b.end());
    return r;
  }
  r.type = mime(p);
  r.body.resize((size_t)n);
  if (n) f.read(r.body.data(), (std::streamsize)n);
  if (!f && n) { r.code = 500; r.body.clear(); r.keep = false; }
  return r;
}

// Effective client IP for a request: the immediate peer unless it is a trusted
// proxy that supplied a Forwarded / X-Forwarded-For header (see hardening.cpp).
std::string effective_client_ip(const Req &q, const std::string &peer_ip,
                                const Config &cfg) {
  if (cfg.trusted_proxies.empty()) return peer_ip;
  std::string xff;
  auto xi = q.h.find("x-forwarded-for");
  if (xi != q.h.end()) xff = xi->second;
  if (xff.empty()) {
    // RFC 7239 Forwarded: for=1.2.3.4; take the first for= token.
    auto fi = q.h.find("forwarded");
    if (fi != q.h.end()) {
      const std::string &fwd = fi->second;
      size_t p = lo(fwd).find("for=");
      if (p != std::string::npos) {
        size_t s = p + 4;
        size_t e2 = s;
        while (e2 < fwd.size() && fwd[e2] != ';' && fwd[e2] != ',') ++e2;
        xff = fwd.substr(s, e2 - s);
        // strip surrounding quotes/brackets
        std::string t;
        for (char c : xff) if (c != '"' && c != '[' && c != ']') t.push_back(c);
        xff = t;
      }
    }
  }
  return sleela::wst::client_ip_from(peer_ip, xff, cfg.trusted_proxies);
}

void serve(Conn conn, const Config &cfg, Log &log) {
  const int fd = conn.fd;
  timeval tv{15, 0};
  setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof tv);
  setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof tv);
  std::string b;
  b.reserve(16384);
  for (;;) {
    if (g_stop) break;
    if (b.find("\r\n\r\n") == std::string::npos) {
      if (b.size() > HMAX || !recvmore(fd, b)) break;
    }
    Req q;
    std::string e;
    if (!parse(fd, b, q, e)) {
      Req bad; bad.version = "HTTP/1.0";
      Resp er; er.code = 400; er.keep = false;
      std::string msg = "Bad Request\n";
      er.body.assign(msg.begin(), msg.end());
      response(fd, bad, er, "SLeeLa-WST");
      log.put("request client=" + conn.peer_ip + " status=400 detail=" + e);
      break;
    }

    const std::string client = effective_client_ip(q, conn.peer_ip, cfg);

    // Section-3 admission control: windowed rate limit per client (and global).
    if (cfg.rate.enabled() && !cfg.rate.allow(client)) {
      Resp er;
      er.code = 503; er.keep = false;
      er.type = "application/json";
      std::string msg = "{\"error\":\"rate limit exceeded; retry later\"}\n";
      er.body.assign(msg.begin(), msg.end());
      // A Retry-After hint is added by response() only for the generic path; we
      // keep it simple and signal 503 with Connection: close.
      response(fd, q, er, "SLeeLa-WST/dynamic-sql");
      log.put("request client=" + client + " method=" + q.method + " path=" +
              q.path + " status=503 note=rate-limited");
      break;
    }

    auto st = std::chrono::steady_clock::now();
    std::string detail;
    Resp r = make(q, cfg, detail);
    if (!response(fd, q, r, "SLeeLa-WST/dynamic-sql")) break;
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(
                  std::chrono::steady_clock::now() - st)
                  .count();
    log.put("request client=" + client + " method=" + q.method + " path=" +
            q.path + " version=" + q.version + " status=" +
            std::to_string(r.code) + " bytes=" +
            std::to_string(r.body.size()) + " duration_us=" +
            std::to_string(us) + (detail.empty() ? "" : " note=" + detail));
    if (!q.keep || !r.keep) break;
  }
  close(fd);
}

// ---- dual-stack listener -----------------------------------------------------
// Prefer an AF_INET6 socket with IPV6_V6ONLY disabled, so one socket accepts
// both IPv6 and IPv4-mapped (::ffff:a.b.c.d) peers -- i.e. all world IPs. If
// the host cannot provide dual-stack, fall back to a plain IPv4 socket.
int listen_dual_stack(const std::string &bind_addr, int port, bool &dual, std::string &err) {
  dual = false;
  // Try IPv6 first (dual-stack).
  int f = socket(AF_INET6, SOCK_STREAM, 0);
  if (f >= 0) {
    int one = 1, zero = 0;
    setsockopt(f, SOL_SOCKET, SO_REUSEADDR, &one, sizeof one);
    // Disable v6-only so IPv4-mapped addresses are accepted on this socket.
    setsockopt(f, IPPROTO_IPV6, IPV6_V6ONLY, &zero, sizeof zero);
    sockaddr_in6 a{};
    a.sin6_family = AF_INET6;
    a.sin6_port = htons((uint16_t)port);
    if (bind_addr.empty()) {
      a.sin6_addr = in6addr_any;
    } else if (inet_pton(AF_INET6, bind_addr.c_str(), &a.sin6_addr) != 1) {
      // caller passed an IPv4 literal for an explicit bind; fall through to v4
      close(f);
      f = -1;
    }
    if (f >= 0) {
      if (bind(f, (sockaddr *)&a, sizeof a) == 0 && listen(f, 256) == 0) {
        dual = bind_addr.empty();
        return f;
      }
      close(f);
    }
  }
  // Fallback: IPv4 only.
  f = socket(AF_INET, SOCK_STREAM, 0);
  if (f < 0) { err = "socket() failed"; return -1; }
  int one = 1;
  setsockopt(f, SOL_SOCKET, SO_REUSEADDR, &one, sizeof one);
  sockaddr_in a{};
  a.sin_family = AF_INET;
  a.sin_port = htons((uint16_t)port);
  if (bind_addr.empty()) {
    a.sin_addr.s_addr = htonl(INADDR_ANY);
  } else if (inet_pton(AF_INET, bind_addr.c_str(), &a.sin_addr) != 1) {
    close(f); err = "invalid bind address"; return -1;
  }
  if (bind(f, (sockaddr *)&a, sizeof a) < 0 || listen(f, 256) < 0) {
    close(f); err = "bind/listen failed"; return -1;
  }
  return f;
}

void usage() {
  std::cout
      << "usage: sleela web-server (dynamic SQL)\n"
      << "  --port N         listen port (default 8090)\n"
      << "  --addr ADDR      bind address (default: all, dual-stack IPv6+IPv4)\n"
      << "  --root DIR       document root for static files (default .)\n"
      << "  --db DIR         enable /db SQL endpoint against this table dir\n"
      << "  --db-writable    allow mutating SQL (default read-only)\n"
      << "  --threads N      worker threads (default 16)\n"
      << "  --log FILE       operational log file\n"
      << "  --once           handle one connection then exit (for tests)\n"
      << "\n"
      << "  Side 2 -- envelopment (many programs per domain; Scientist-gated):\n"
      << "  --config FILE    load an envelopment config file (programs/warrants)\n"
      << "  --envelopment    enable envelopment (requires --scientist too)\n"
      << "  --scientist      assert the Scientist capability that gates Side 2\n"
      << "\n"
      << "  Hardening (Section 3):\n"
      << "  --rate-per-peer N    max requests per window per client IP (0=off)\n"
      << "  --rate-global N      max requests per window across all peers (0=off)\n"
      << "  --rate-window-ms N   rate-limit window in ms (default 1000)\n"
      << "  --write-token TOK    require 'Authorization: Bearer TOK' for /db writes\n"
      << "  --trusted-proxy IP   honour Forwarded/X-Forwarded-For from this peer\n"
      << "                       (repeatable; prevents client IP spoofing)\n"
      << "\n"
      << "  CPU keep-warm idle floor (ON by default; administrator may disable):\n"
      << "  --no-warm            disable the keep-warm idle floor\n"
      << "  --warm-floor N       target floor percent per core (default 11)\n"
      << "  --warm-ceiling N     target ceiling percent per core (default 14)\n"
      << "  --warm-cores N       max warmer threads, clamped 1..16 (default 16)\n";
}

int run(int ac, char **av) {
  Config cfg;
  int port = 8090;
  unsigned threads = TDEFAULT;
  bool once = false;
  std::string config_path;
  bool cli_envelopment = false, cli_scientist = false;
  // Section-3 hardening options (0/empty => feature off).
  unsigned rate_per_peer = 0, rate_global = 0, rate_window_ms = 1000;
  std::string write_token;
  std::vector<std::string> trusted_proxies;
  for (int i = 1; i < ac; ++i) {
    std::string a = av[i];
    if (a == "--port" && i + 1 < ac) port = std::atoi(av[++i]);
    else if (a == "--addr" && i + 1 < ac) cfg.bind_addr = av[++i];
    else if (a == "--root" && i + 1 < ac) cfg.root = av[++i];
    else if (a == "--db" && i + 1 < ac) cfg.db_dir = av[++i];
    else if (a == "--db-writable") cfg.db_writable = true;
    else if (a == "--threads" && i + 1 < ac) threads = (unsigned)std::strtoul(av[++i], nullptr, 10);
    else if (a == "--log" && i + 1 < ac) cfg.log_path = av[++i];
    else if (a == "--config" && i + 1 < ac) config_path = av[++i];
    else if (a == "--envelopment") cli_envelopment = true;
    else if (a == "--scientist") cli_scientist = true;
    else if (a == "--rate-per-peer" && i + 1 < ac) rate_per_peer = (unsigned)std::strtoul(av[++i], nullptr, 10);
    else if (a == "--rate-global" && i + 1 < ac) rate_global = (unsigned)std::strtoul(av[++i], nullptr, 10);
    else if (a == "--rate-window-ms" && i + 1 < ac) rate_window_ms = (unsigned)std::strtoul(av[++i], nullptr, 10);
    else if (a == "--write-token" && i + 1 < ac) write_token = av[++i];
    else if (a == "--trusted-proxy" && i + 1 < ac) trusted_proxies.push_back(av[++i]);
    else if (a == "--no-warm") cfg.warm.enabled = false;
    else if (a == "--warm") cfg.warm.enabled = true;  // explicit (already default)
    else if (a == "--warm-floor" && i + 1 < ac) cfg.warm.floor_pct = (unsigned)std::strtoul(av[++i], nullptr, 10);
    else if (a == "--warm-ceiling" && i + 1 < ac) cfg.warm.ceiling_pct = (unsigned)std::strtoul(av[++i], nullptr, 10);
    else if (a == "--warm-cores" && i + 1 < ac) cfg.warm.max_cores = (unsigned)std::strtoul(av[++i], nullptr, 10);
    else if (a == "--once") once = true;
    else if (a == "--help" || a == "-h") { usage(); return 0; }
    else { std::cerr << "unknown option: " << a << "\n"; return 2; }
  }
  if (port < 1 || port > 65535 || threads < 1 || threads > TMAX) return 2;

  // Resolve hardening settings from the config file first (if any), then apply
  // CLI flags on top so the command line overrides the file. This gives the
  // "startup settings OR configuration file settings" control surface for the
  // Section-3 features, matching the envelopment control surface.
  bool cli_rate_per_peer = false, cli_rate_global = false, cli_rate_window = false;
  bool cli_warm = false, cli_warm_floor = false, cli_warm_ceiling = false, cli_warm_cores = false;
  // (track which CLI flags were explicitly set so file values are not
  //  silently clobbered, and so the CLI overrides the config file)
  for (int i = 1; i < ac; ++i) {
    std::string a = av[i];
    if (a == "--rate-per-peer") cli_rate_per_peer = true;
    else if (a == "--rate-global") cli_rate_global = true;
    else if (a == "--rate-window-ms") cli_rate_window = true;
    else if (a == "--warm" || a == "--no-warm") cli_warm = true;
    else if (a == "--warm-floor") cli_warm_floor = true;
    else if (a == "--warm-ceiling") cli_warm_ceiling = true;
    else if (a == "--warm-cores") cli_warm_cores = true;
  }
  if (!config_path.empty()) {
    sleela::wst::HardeningSettings hs;
    std::string herr;
    if (!load_hardening_config(config_path, hs, herr)) {
      std::cerr << "config: " << herr << "\n";
      return 2;
    }
    if (hs.has_rate_per_peer && !cli_rate_per_peer) rate_per_peer = hs.rate_per_peer;
    if (hs.has_rate_global && !cli_rate_global) rate_global = hs.rate_global;
    if (hs.has_rate_window && !cli_rate_window) rate_window_ms = hs.rate_window_ms;
    if (hs.has_write_token && write_token.empty()) write_token = hs.write_token;
    for (const auto &p : hs.trusted_proxies) trusted_proxies.push_back(p);
    // CPU warmer from the file (CLI --warm/--no-warm/--warm-* override below).
    if (hs.has_warm && !cli_warm) cfg.warm.enabled = hs.warm_enabled;
    if (hs.has_warm_floor && !cli_warm_floor) cfg.warm.floor_pct = hs.warm_floor;
    if (hs.has_warm_ceiling && !cli_warm_ceiling) cfg.warm.ceiling_pct = hs.warm_ceiling;
    if (hs.has_warm_cores && !cli_warm_cores) cfg.warm.max_cores = hs.warm_cores;
  }

  if (rate_window_ms < 1) rate_window_ms = 1000;
  cfg.rate.configure(rate_per_peer, rate_global, std::chrono::milliseconds(rate_window_ms));
  if (!write_token.empty()) cfg.write_auth.set_token(write_token);
  cfg.trusted_proxies = trusted_proxies;
  // Validate/clamp the keep-warm band into the in-contract range.
  cfg.warm = sleela::wst::sanitize(cfg.warm);

  std::error_code e;
  cfg.root = fs::weakly_canonical(cfg.root, e);
  if (e || !fs::is_directory(cfg.root)) { std::cerr << "invalid document root\n"; return 2; }
  if (!cfg.db_dir.empty()) {
    cfg.db_dir = fs::weakly_canonical(cfg.db_dir, e);
    if (e) { std::cerr << "invalid --db directory\n"; return 2; }
    // Create/validate once at startup so the request path can bind the engine
    // without a per-request stat()/mkdir() (ssql_open's work, done here once).
    std::error_code ce;
    if (!fs::is_directory(cfg.db_dir, ce)) {
      fs::create_directories(cfg.db_dir, ce);
      if (ce || !fs::is_directory(cfg.db_dir, ce)) {
        std::cerr << "cannot create --db directory\n"; return 2;
      }
    }
  }

  // Side 2: load envelopment from the config file (if any), then let the CLI
  // flags turn the gates on. The CLI can enable but is still subject to the
  // Scientist capability -- envelopment only activates with BOTH gates set and
  // at least one warrant. This is the "startup settings OR config file" control
  // surface the design calls for.
  if (!config_path.empty()) {
    std::string cerr_msg;
    if (!load_envelopment_config(config_path, cfg.env, cerr_msg)) {
      std::cerr << "config: " << cerr_msg << "\n";
      return 2;
    }
  }
  if (cli_envelopment) cfg.env.enabled = true;
  if (cli_scientist) cfg.env.scientist = true;

  if (cfg.env.enabled && !cfg.env.scientist) {
    std::cerr << "envelopment: refused -- Side 2 requires the Scientist "
                 "capability (pass --scientist or set scientist=true)\n";
    return 2;
  }
  // Canonicalize and validate each program's backing resource.
  for (auto &prog : cfg.env.programs) {
    if (prog.kind == sleela::wst::ProgramKind::Static && !prog.root.empty()) {
      auto cr = fs::weakly_canonical(prog.root, e);
      if (e || !fs::is_directory(cr)) {
        std::cerr << "envelopment: program '" << prog.name
                  << "' has an invalid static root\n";
        return 2;
      }
      prog.root = cr.string();
    }
    if (prog.kind == sleela::wst::ProgramKind::Sql && !prog.db_dir.empty()) {
      auto cd = fs::weakly_canonical(prog.db_dir, e);
      if (e) {
        std::cerr << "envelopment: program '" << prog.name
                  << "' has an invalid db directory\n";
        return 2;
      }
      std::error_code ce;
      if (!fs::is_directory(cd, ce)) {
        fs::create_directories(cd, ce);
        if (ce || !fs::is_directory(cd, ce)) {
          std::cerr << "envelopment: program '" << prog.name
                    << "' db directory cannot be created\n";
          return 2;
        }
      }
      prog.db_dir = cd.string();
    }
  }

  if (cfg.log_path.empty()) cfg.log_path = cfg.root / ".sleela-wst-dynamic-sql.log";

  signal(SIGINT, on_signal);
  signal(SIGTERM, on_signal);

  bool dual = false;
  std::string err;
  int l = listen_dual_stack(cfg.bind_addr, port, dual, err);
  if (l < 0) { std::cerr << "listen: " << err << "\n"; return 1; }

  Log log(cfg.log_path);
  log.put(std::string("server=start kind=dynamic-sql port=") + std::to_string(port) +
          " dual_stack=" + (dual ? "yes" : "no") +
          " db=" + (cfg.db_dir.empty() ? "disabled" : cfg.db_dir.string()) +
          " db_writable=" + (cfg.db_writable ? "yes" : "no") +
          " envelopment=" + (cfg.env.active() ? "active"
                             : cfg.env.enabled ? "enabled-inactive" : "off") +
          " scientist=" + (cfg.env.scientist ? "yes" : "no") +
          " programs=" + std::to_string(cfg.env.programs.size()) +
          " warrants=" + std::to_string(cfg.env.warrants.size()) +
          " rate_limit=" + (cfg.rate.enabled() ? "on" : "off") +
          " write_auth=" + (cfg.write_auth.required() ? "on" : "off") +
          " trusted_proxies=" + std::to_string(cfg.trusted_proxies.size()));

  // Start the CPU keep-warm idle floor (on by default). Owned here; stopped on
  // shutdown. It runs low-priority so request handling always preempts it.
  sleela::wst::CpuWarmer warmer(cfg.warm);
  warmer.start();
  cfg.warm_live = &warmer;
  if (cfg.warm.enabled) {
    log.put(std::string("cpu-warm=on floor=") + std::to_string(cfg.warm.floor_pct) +
            "% ceiling=" + std::to_string(cfg.warm.ceiling_pct) +
            "% cores=" + std::to_string(warmer.threads()));
  } else {
    log.put("cpu-warm=off (administrator disabled)");
  }

  Pool pool(threads, [&](Conn c) { serve(c, cfg, log); });
  auto last_sweep = std::chrono::steady_clock::now();
  while (!g_stop) {
    pollfd p{l, POLLIN, 0};
    int rc = poll(&p, 1, 500);
    // Periodically reclaim aged-out per-peer rate-limit windows.
    if (cfg.rate.enabled()) {
      auto now = std::chrono::steady_clock::now();
      if (now - last_sweep > std::chrono::seconds(10)) {
        cfg.rate.sweep(now);
        last_sweep = now;
      }
    }
    if (rc <= 0) continue;
    sockaddr_storage ss{};
    socklen_t slen = sizeof ss;
    int fd = accept(l, (sockaddr *)&ss, &slen);
    if (fd < 0) continue;
    // Resolve the immediate peer IP for rate limiting / logging.
    char ipbuf[INET6_ADDRSTRLEN] = {0};
    std::string peer_ip;
    if (ss.ss_family == AF_INET6) {
      auto *a6 = (sockaddr_in6 *)&ss;
      if (inet_ntop(AF_INET6, &a6->sin6_addr, ipbuf, sizeof ipbuf)) peer_ip = ipbuf;
      // Normalise IPv4-mapped (::ffff:a.b.c.d) to the bare IPv4 form.
      const std::string pfx = "::ffff:";
      if (peer_ip.rfind(pfx, 0) == 0) peer_ip = peer_ip.substr(pfx.size());
    } else if (ss.ss_family == AF_INET) {
      auto *a4 = (sockaddr_in *)&ss;
      if (inet_ntop(AF_INET, &a4->sin_addr, ipbuf, sizeof ipbuf)) peer_ip = ipbuf;
    }
    Conn conn{fd, peer_ip};
    if (!pool.add(conn)) {
      close(fd);
      log.put("connection rejected client=" + peer_ip + " reason=worker-queue-full");
    }
    if (once) g_stop = true;
  }
  shutdown(l, SHUT_RDWR);
  close(l);
  pool.stop();
  cfg.warm_live = nullptr;
  warmer.stop();
  log.put("server=stop kind=dynamic-sql");
  return 0;
}

}  // namespace

extern "C" int sleela_wst_dyn_sql_server_run(int argc, char **argv) {
  return run(argc, argv);
}
#endif  // _WIN32
