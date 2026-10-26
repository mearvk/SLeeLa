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

class Pool {
  std::mutex m;
  std::condition_variable cv;
  std::deque<int> q;
  std::vector<std::thread> w;
  bool end = false;
 public:
  Pool(unsigned n, std::function<void(int)> fn) {
    for (unsigned i = 0; i < n; ++i)
      w.emplace_back([this, fn] {
        for (;;) {
          int fd;
          {
            std::unique_lock<std::mutex> g(m);
            cv.wait(g, [this] { return end || !q.empty(); });
            if (end && q.empty()) return;
            fd = q.front();
            q.pop_front();
          }
          fn(fd);
        }
      });
  }
  bool add(int fd) {
    std::lock_guard<std::mutex> g(m);
    if (end || q.size() >= QMAX) return false;
    q.push_back(fd);
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

Resp db_endpoint(const Req &q, const Config &cfg, std::string &log_detail) {
  Resp r;
  r.keep = q.keep;
  r.type = "application/json";

  if (cfg.db_dir.empty()) {
    r.code = 503;
    std::string b = "{\"error\":\"database endpoint disabled (start with --db DIR)\"}\n";
    r.body.assign(b.begin(), b.end());
    r.keep = false;
    return r;
  }

  if (q.path == "/db/health" || q.path == "/db" || q.path == "/db/") {
    ssql_db db;
    ssql_status st = ssql_open(&db, cfg.db_dir.c_str());
    std::ostringstream o;
    o << "{\"server\":\"SLeeLa\",\"engine\":\"sleela-sql\",\"dialects\":[\"sql\",\"sleelasql\"],"
      << "\"writable\":" << (cfg.db_writable ? "true" : "false") << ","
      << "\"status\":\"" << (st == SSQL_OK ? "ok" : ssql_strerror(st)) << "\"}\n";
    std::string b = o.str();
    r.body.assign(b.begin(), b.end());
    log_detail = "db-health";
    return r;
  }

  if (q.path != "/db/query") {
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
  if (!readonly && !cfg.db_writable) {
    r.code = 403;
    std::string b = "{\"error\":\"this endpoint is read-only; start with --db-writable to allow mutations\"}\n";
    r.body.assign(b.begin(), b.end());
    log_detail = "db-query write-denied";
    return r;
  }

  ssql_db db;
  if (ssql_open(&db, cfg.db_dir.c_str()) != SSQL_OK) {
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
      << ",\"db_writable\":" << (cfg.db_writable ? "true" : "false") << "}\n";
    std::string b = o.str();
    r.type = "application/json";
    r.body.assign(b.begin(), b.end());
    return r;
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

void serve(int fd, const Config &cfg, Log &log) {
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
      log.put("request status=400 detail=" + e);
      break;
    }
    auto st = std::chrono::steady_clock::now();
    std::string detail;
    Resp r = make(q, cfg, detail);
    if (!response(fd, q, r, "SLeeLa-WST/dynamic-sql")) break;
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(
                  std::chrono::steady_clock::now() - st)
                  .count();
    log.put("request method=" + q.method + " path=" + q.path +
            " version=" + q.version + " status=" + std::to_string(r.code) +
            " bytes=" + std::to_string(r.body.size()) +
            " duration_us=" + std::to_string(us) +
            (detail.empty() ? "" : " note=" + detail));
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
      << "  --once           handle one connection then exit (for tests)\n";
}

int run(int ac, char **av) {
  Config cfg;
  int port = 8090;
  unsigned threads = TDEFAULT;
  bool once = false;
  for (int i = 1; i < ac; ++i) {
    std::string a = av[i];
    if (a == "--port" && i + 1 < ac) port = std::atoi(av[++i]);
    else if (a == "--addr" && i + 1 < ac) cfg.bind_addr = av[++i];
    else if (a == "--root" && i + 1 < ac) cfg.root = av[++i];
    else if (a == "--db" && i + 1 < ac) cfg.db_dir = av[++i];
    else if (a == "--db-writable") cfg.db_writable = true;
    else if (a == "--threads" && i + 1 < ac) threads = (unsigned)std::strtoul(av[++i], nullptr, 10);
    else if (a == "--log" && i + 1 < ac) cfg.log_path = av[++i];
    else if (a == "--once") once = true;
    else if (a == "--help" || a == "-h") { usage(); return 0; }
    else { std::cerr << "unknown option: " << a << "\n"; return 2; }
  }
  if (port < 1 || port > 65535 || threads < 1 || threads > TMAX) return 2;

  std::error_code e;
  cfg.root = fs::weakly_canonical(cfg.root, e);
  if (e || !fs::is_directory(cfg.root)) { std::cerr << "invalid document root\n"; return 2; }
  if (!cfg.db_dir.empty()) {
    cfg.db_dir = fs::weakly_canonical(cfg.db_dir, e);
    if (e) { std::cerr << "invalid --db directory\n"; return 2; }
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
          " db_writable=" + (cfg.db_writable ? "yes" : "no"));

  Pool pool(threads, [&](int fd) { serve(fd, cfg, log); });
  while (!g_stop) {
    pollfd p{l, POLLIN, 0};
    int rc = poll(&p, 1, 500);
    if (rc <= 0) continue;
    int fd = accept(l, nullptr, nullptr);
    if (fd < 0) continue;
    if (!pool.add(fd)) {
      close(fd);
      log.put("connection rejected reason=worker-queue-full");
    }
    if (once) g_stop = true;
  }
  shutdown(l, SHUT_RDWR);
  close(l);
  pool.stop();
  log.put("server=stop kind=dynamic-sql");
  return 0;
}

}  // namespace

extern "C" int sleela_wst_dyn_sql_server_run(int argc, char **argv) {
  return run(argc, argv);
}
#endif  // _WIN32
