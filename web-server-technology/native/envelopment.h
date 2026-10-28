#ifndef SLEELA_WST_ENVELOPMENT_H
#define SLEELA_WST_ENVELOPMENT_H
/*
 * web-server-technology/native/envelopment.h
 *
 * The envelopment model -- Side 2 of the web server interface.
 *
 * Side 1 (always on): a program goes to a domain and then a meme. One domain
 * resolves to a single coherent response unit. The base server
 * (dyn_sql_server.cpp) implements this directly.
 *
 * Side 2 (this file, opt-in, Scientist-gated): a domain may carry many
 * subjects/warrants for causes, so the website becomes an ENVELOPMENT of many
 * disparate programs behind one domain. A warrant binds a cause to a subject
 * (a host/path matcher) and a program (what runs). Envelopment resolution maps
 * an incoming (host, path) to exactly one program; the base dispatch then runs
 * that program's backing kind (static subtree, SQL surface, or a fixed meme).
 *
 * This module is deliberately self-contained and transport-agnostic: it knows
 * nothing about sockets or HTTP wire format. It is only a model plus a resolver
 * plus a config loader, so it can be unit-tested in isolation and so that the
 * base server never depends on it when envelopment is disabled.
 */
#include <string>
#include <vector>

namespace sleela {
namespace wst {

/* The backing behaviour a program provides when its warrant is selected. */
enum class ProgramKind {
  Static,  /* serve files from a document-root subtree          */
  Sql,     /* expose a /db-style SQL surface over a table dir    */
  Meme     /* return a single fixed response (the "meme")        */
};

/* A named, mounted unit of behaviour. */
struct Program {
  std::string name;        /* unique program name within the envelopment     */
  ProgramKind kind = ProgramKind::Meme;
  std::string mount;       /* path prefix this program owns, e.g. "/a"        */
  std::string root;        /* Static: document root subtree                   */
  std::string db_dir;      /* Sql: directory of CSV tables                    */
  bool db_writable = false;/* Sql: whether mutations are permitted            */
  std::string meme_type;   /* Meme: content-type (default text/plain)         */
  std::string meme_body;   /* Meme: the fixed response body                   */
};

/* A warrant: a cause served for a subject by a program. The "subject" is the
 * match key -- a host (and optional path prefix) under which the cause holds. */
struct Warrant {
  std::string cause;       /* the reason-to-serve, free text (logged)         */
  std::string subject;     /* host to match, e.g. "science.example"; "*" = any*/
  std::string path_prefix; /* optional additional path gate, "" = any         */
  std::string program;     /* name of the Program this warrant activates      */
  bool enabled = true;     /* a disabled warrant never matches                */
};

/* One domain's envelopment: its programs and the warrants that route to them.*/
struct Envelopment {
  bool enabled = false;      /* master switch (Side 2 off => Side 1 only)     */
  bool scientist = false;    /* capability gate: envelopment requires this    */
  std::string domain;        /* the domain this envelopment fronts            */
  std::vector<Program> programs;
  std::vector<Warrant> warrants;

  /* Look up a program by name; nullptr if absent. */
  const Program *find_program(const std::string &name) const;

  /* Is envelopment usable? Requires enabled AND scientist AND >=1 warrant. */
  bool active() const;

  /*
   * Resolve an incoming request to a program. Returns the selected Program and
   * sets out_cause to the matched warrant's cause, or nullptr if no enabled
   * warrant matches (the caller then falls back to Side-1 dispatch). Matching
   * is: subject == host (or "*"), AND path starts with path_prefix (or ""),
   * first enabled warrant in declaration order wins.
   */
  const Program *resolve(const std::string &host, const std::string &path,
                         std::string &out_cause) const;
};

/* Human-readable kind name, for status/diagnostics. */
const char *program_kind_name(ProgramKind k);

/*
 * Load an envelopment from a config file. The format is a tiny, line-oriented
 * key/value grammar (see docs/ENVELOPMENT.md); '#' begins a comment. Unknown
 * keys are ignored so the same file can also configure the base server.
 * Returns true on a successful parse (even if envelopment stays disabled).
 * On a hard error (unreadable file, malformed required field) returns false
 * and fills err.
 */
bool load_envelopment_config(const std::string &path, Envelopment &out,
                             std::string &err);

/*
 * Apply a single "key=value" setting to an envelopment (used by both the config
 * loader and startup-flag overrides so CLI and file share one code path).
 * Returns false and fills err on a malformed directive.
 */
bool apply_envelopment_setting(const std::string &key, const std::string &value,
                               Envelopment &out, std::string &err);

}  // namespace wst
}  // namespace sleela
#endif  // SLEELA_WST_ENVELOPMENT_H
