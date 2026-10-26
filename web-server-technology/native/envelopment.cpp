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
#include <memory>
#include <sstream>

#include "../../lib/web-server-technology/native/include/slweb.h"

namespace sleela {
namespace wst {

const char *program_kind_name(ProgramKind k) {
  // Delegate the naming to the shared C core so there is one source of truth.
  slweb_program_kind ck = k == ProgramKind::Static ? SLWEB_PROGRAM_STATIC
                          : k == ProgramKind::Sql  ? SLWEB_PROGRAM_SQL
                                                   : SLWEB_PROGRAM_MEME;
  return slweb_program_kind_name(ck);
}

const Program *Envelopment::find_program(const std::string &name) const {
  for (const auto &p : programs)
    if (p.name == name) return &p;
  return nullptr;
}

namespace {
// Build a transient slweb_envelopment mirror of this server-side Envelopment so
// the Scientist-gate and longest-prefix resolution run in the shared C core.
// The server's richer Program (meme body, roots, db dir) is kept here; slweb
// only needs names/kinds/mounts and the warrant routing keys.
struct SlwebEnvDeleter {
  void operator()(slweb_envelopment *e) const { slweb_env_free(e); }
};
std::unique_ptr<slweb_envelopment, SlwebEnvDeleter> build_mirror(
    const Envelopment &env) {
  std::unique_ptr<slweb_envelopment, SlwebEnvDeleter> m(
      slweb_env_new(env.domain.c_str()));
  if (!m) return m;
  slweb_env_set_enabled(m.get(), env.enabled ? 1 : 0);
  slweb_env_set_scientist(m.get(), env.scientist ? 1 : 0);
  for (const auto &p : env.programs) {
    slweb_program_kind k = p.kind == ProgramKind::Static ? SLWEB_PROGRAM_STATIC
                           : p.kind == ProgramKind::Sql  ? SLWEB_PROGRAM_SQL
                                                         : SLWEB_PROGRAM_MEME;
    slweb_env_add_program(m.get(), p.name.c_str(), k, p.mount.c_str(), "",
                          p.db_writable ? 1 : 0);
  }
  for (const auto &w : env.warrants)
    slweb_env_add_warrant(m.get(), w.cause.c_str(), w.subject.c_str(),
                          w.path_prefix.c_str(), w.program.c_str(),
                          w.enabled ? 1 : 0);
  return m;
}
}  // namespace

bool Envelopment::active() const {
  // Scientist-gate rule lives in the shared C core.
  auto m = build_mirror(*this);
  return m && slweb_env_active(m.get()) != 0;
}

const Program *Envelopment::resolve(const std::string &host,
                                    const std::string &path,
                                    std::string &out_cause) const {
  // Longest-matching-path-prefix resolution and the active() gate are performed
  // by the shared C core (slweb); we map the chosen program NAME back to the
  // server-side Program, which carries the transport details (body/root/dir).
  auto m = build_mirror(*this);
  if (!m) return nullptr;
  char prog[SLWEB_MAX_NAME], cause[SLWEB_MAX_PATH];
  slweb_program_kind kind;
  if (slweb_env_resolve(m.get(), host.c_str(), path.c_str(), prog, cause,
                        &kind) != SLWEB_OK)
    return nullptr;
  const Program *p = find_program(prog);
  if (!p) return nullptr;
  out_cause = cause;
  return p;
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
