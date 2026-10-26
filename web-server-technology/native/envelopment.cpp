/*
 * web-server-technology/native/envelopment.cpp
 *
 * Implementation of the Side-2 envelopment model: resolver + config loader.
 * Transport-agnostic by design (no sockets, no HTTP wire handling), so it can
 * be unit-tested on its own and so Side 1 carries no dependency on it.
 *
 * Config grammar (line-oriented; '#' starts a comment; blank lines ignored):
 *
 *   envelopment = on|off
 *   scientist   = true|false
 *   domain      = <host>
 *
 *   program     = <name>                 # begins a program block
 *   kind        = static|sql|meme
 *   mount       = /prefix
 *   root        = /path                  # static
 *   db          = /path                  # sql
 *   db-writable = true|false             # sql
 *   meme-type   = text/plain; charset=utf-8   # meme
 *   meme-body   = <single-line body text>     # meme
 *
 *   warrant     = <cause>                # begins a warrant block
 *   subject     = <host>|*
 *   path        = /prefix                # optional
 *   use         = <program-name>
 *   enabled     = true|false
 *
 * A "program" or "warrant" line opens a new block; subsequent block keys apply
 * to the most recently opened block until the next block opener.
 */
#include "envelopment.h"

#include <cctype>
#include <fstream>
#include <sstream>

namespace sleela {
namespace wst {

const char *program_kind_name(ProgramKind k) {
  switch (k) {
    case ProgramKind::Static: return "static";
    case ProgramKind::Sql:    return "sql";
    case ProgramKind::Meme:   return "meme";
  }
  return "meme";
}

const Program *Envelopment::find_program(const std::string &name) const {
  for (const auto &p : programs)
    if (p.name == name) return &p;
  return nullptr;
}

bool Envelopment::active() const {
  return enabled && scientist && !warrants.empty();
}

const Program *Envelopment::resolve(const std::string &host,
                                    const std::string &path,
                                    std::string &out_cause) const {
  if (!active()) return nullptr;
  // Longest-matching-path-prefix wins so a broad warrant (path "/") does not
  // shadow a specific one (path "/papers"); ties break by declaration order.
  const Warrant *best = nullptr;
  const Program *best_prog = nullptr;
  size_t best_len = 0;
  bool have = false;
  for (const auto &w : warrants) {
    if (!w.enabled) continue;
    if (!(w.subject == "*" || w.subject == host)) continue;
    if (!w.path_prefix.empty() && path.rfind(w.path_prefix, 0) != 0) continue;
    const Program *p = find_program(w.program);
    if (!p) continue;  // warrant names a missing program; skip, don't crash
    size_t len = w.path_prefix.size();
    if (!have || len > best_len) {
      have = true;
      best = &w;
      best_prog = p;
      best_len = len;
    }
  }
  if (!have) return nullptr;
  out_cause = best->cause;
  return best_prog;
}

// ---- parsing helpers --------------------------------------------------------
namespace {

std::string trim(const std::string &s) {
  size_t a = 0, b = s.size();
  while (a < b && std::isspace((unsigned char)s[a])) ++a;
  while (b > a && std::isspace((unsigned char)s[b - 1])) --b;
  return s.substr(a, b - a);
}

std::string lower(std::string s) {
  for (char &c : s) c = (char)std::tolower((unsigned char)c);
  return s;
}

bool parse_bool(const std::string &v, bool &out) {
  std::string l = lower(trim(v));
  if (l == "on" || l == "true" || l == "yes" || l == "1") { out = true; return true; }
  if (l == "off" || l == "false" || l == "no" || l == "0") { out = false; return true; }
  return false;
}

}  // namespace

// Apply a single top-level (non-block) setting. Block keys are handled by the
// loader because they need the "current block" context.
bool apply_envelopment_setting(const std::string &key, const std::string &value,
                               Envelopment &out, std::string &err) {
  std::string k = lower(trim(key));
  std::string v = trim(value);
  if (k == "envelopment") {
    bool b;
    if (!parse_bool(v, b)) { err = "envelopment must be on/off"; return false; }
    out.enabled = b;
    return true;
  }
  if (k == "scientist") {
    bool b;
    if (!parse_bool(v, b)) { err = "scientist must be true/false"; return false; }
    out.scientist = b;
    return true;
  }
  if (k == "domain") {
    out.domain = v;
    return true;
  }
  // Not a recognized top-level envelopment key; ignore quietly so the same file
  // can hold base-server settings too.
  return true;
}

bool load_envelopment_config(const std::string &path, Envelopment &out,
                             std::string &err) {
  std::ifstream f(path);
  if (!f) { err = "cannot open config file: " + path; return false; }

  enum class Block { None, Program, Warrant };
  Block block = Block::None;
  Program cur_prog;
  Warrant cur_warr;

  auto flush_program = [&]() {
    if (block == Block::Program && !cur_prog.name.empty())
      out.programs.push_back(cur_prog);
  };
  auto flush_warrant = [&]() {
    if (block == Block::Warrant && !cur_warr.cause.empty())
      out.warrants.push_back(cur_warr);
  };

  std::string line;
  int lineno = 0;
  while (std::getline(f, line)) {
    ++lineno;
    // strip comment
    size_t hash = line.find('#');
    if (hash != std::string::npos) line = line.substr(0, hash);
    std::string t = trim(line);
    if (t.empty()) continue;
    size_t eq = t.find('=');
    if (eq == std::string::npos) {
      err = "line " + std::to_string(lineno) + ": expected key=value";
      return false;
    }
    std::string k = lower(trim(t.substr(0, eq)));
    std::string v = trim(t.substr(eq + 1));

    // Block openers.
    if (k == "program") {
      flush_program();
      flush_warrant();
      cur_prog = Program{};
      cur_prog.name = v;
      block = Block::Program;
      continue;
    }
    if (k == "warrant") {
      flush_program();
      flush_warrant();
      cur_warr = Warrant{};
      cur_warr.cause = v;
      block = Block::Warrant;
      continue;
    }

    // Block keys.
    if (block == Block::Program) {
      if (k == "kind") {
        std::string lv = lower(v);
        if (lv == "static") cur_prog.kind = ProgramKind::Static;
        else if (lv == "sql") cur_prog.kind = ProgramKind::Sql;
        else if (lv == "meme") cur_prog.kind = ProgramKind::Meme;
        else { err = "line " + std::to_string(lineno) + ": kind must be static/sql/meme"; return false; }
        continue;
      }
      if (k == "mount") { cur_prog.mount = v; continue; }
      if (k == "root") { cur_prog.root = v; continue; }
      if (k == "db") { cur_prog.db_dir = v; continue; }
      if (k == "db-writable") {
        if (!parse_bool(v, cur_prog.db_writable)) { err = "line " + std::to_string(lineno) + ": db-writable must be true/false"; return false; }
        continue;
      }
      if (k == "meme-type") { cur_prog.meme_type = v; continue; }
      if (k == "meme-body") { cur_prog.meme_body = v; continue; }
      // fall through to top-level settings (e.g. envelopment/scientist/domain)
    } else if (block == Block::Warrant) {
      if (k == "subject") { cur_warr.subject = v; continue; }
      if (k == "path") { cur_warr.path_prefix = v; continue; }
      if (k == "use") { cur_warr.program = v; continue; }
      if (k == "enabled") {
        if (!parse_bool(v, cur_warr.enabled)) { err = "line " + std::to_string(lineno) + ": enabled must be true/false"; return false; }
        continue;
      }
      // fall through
    }

    // Top-level settings (envelopment/scientist/domain) or ignorable keys.
    if (!apply_envelopment_setting(k, v, out, err)) {
      err = "line " + std::to_string(lineno) + ": " + err;
      return false;
    }
  }
  flush_program();
  flush_warrant();
  return true;
}

}  // namespace wst
}  // namespace sleela
