/*
 * sleela_sql.c -- CSV-backed MySQL-subset engine (C implementation).
 * Part of mearvk/Nintendo.
 *
 * Architecture: parse -> compiled IR (ssql_stmt) -> execute.
 *
 *   Surface language   Compiler              Compiled IR     Executor
 *   ----------------   -------------------   -------------   -------------------
 *   SQL            --> compile_sql()     \
 *                                          >-->  ssql_stmt  -->  exec_stmt()
 *   SLeeLaSQL      --> compile_sleela()  /
 *
 * Both dialects lower to the SAME ssql_stmt, so they are feature-complete with
 * each other and share one executor. A compiled statement can carry '?'
 * placeholders and be re-bound and re-run -- the PreparedStatement speed-up.
 *
 * Storage model: one CSV file per table (<dir>/<name>.csv). The first line is
 * the header (column names); each following line is a record.
 */
#include "sleela_sql.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#ifdef _WIN32
#  include <direct.h>
#  include <io.h>
#  define strcasecmp _stricmp
#  define SSQL_MKDIR(path) _mkdir(path)
#  ifndef S_IFMT
#    define S_IFMT _S_IFMT
#  endif
#  ifndef S_IFDIR
#    define S_IFDIR _S_IFDIR
#  endif
#else
#  include <strings.h>
#  include <dirent.h>
#  include <unistd.h>
#  define SSQL_MKDIR(path) mkdir((path), 0775)
#endif

/* ===================================================================== *
 *  Small string helpers
 * ===================================================================== */

static char *str_dup(const char *s) {
    size_t n = strlen(s) + 1;
    char *p = (char *)malloc(n);
    if (p) memcpy(p, s, n);
    return p;
}

/* Trim leading/trailing ASCII whitespace in place; returns the start. */
static char *trim(char *s) {
    while (*s && isspace((unsigned char)*s)) s++;
    char *end = s + strlen(s);
    while (end > s && isspace((unsigned char)end[-1])) *--end = '\0';
    return s;
}

/* Case-insensitive keyword compare of the first word of `s`. */
static int kw_eq(const char *s, const char *kw) {
    size_t n = strlen(kw);
    for (size_t i = 0; i < n; i++) {
        if (tolower((unsigned char)s[i]) != tolower((unsigned char)kw[i])) return 0;
    }
    char c = s[n];
    return (c == '\0' || isspace((unsigned char)c) || c == '(' || c == '*' || c == ';');
}

/* Strip one matching pair of surrounding single/double quotes, in place. */
static char *unquote(char *s) {
    size_t n = strlen(s);
    if (n >= 2 && ((s[0] == '\'' && s[n - 1] == '\'') ||
                   (s[0] == '"'  && s[n - 1] == '"'))) {
        s[n - 1] = '\0';
        return s + 1;
    }
    return s;
}

/*
 * Split `s` on top-level commas (commas not inside single/double quotes) into
 * up to `max` trimmed+unquoted fields. Mutates `s`. Returns the field count.
 */
static int split_fields(char *s, char **out, int max) {
    int n = 0;
    char *start = s;
    char quote = 0;
    for (char *p = s;; p++) {
        if (quote) {
            if (*p == quote) quote = 0;
        } else if (*p == '\'' || *p == '"') {
            quote = *p;
        } else if (*p == ',' || *p == '\0') {
            char save = *p;
            *p = '\0';
            if (n < max) {
                char *f = trim(start);
                out[n++] = unquote(f);
            }
            if (save == '\0') break;
            start = p + 1;
        }
    }
    return n;
}

/* ===================================================================== *
 *  CSV field I/O
 * ===================================================================== */

/* Write one CSV field, quoting only if it contains comma, quote, or newline. */
static void csv_write_field(FILE *out, const char *v) {
    int needs = 0;
    for (const char *p = v; *p; p++) {
        if (*p == ',' || *p == '"' || *p == '\n' || *p == '\r') { needs = 1; break; }
    }
    if (!needs) { fputs(v, out); return; }
    fputc('"', out);
    for (const char *p = v; *p; p++) {
        if (*p == '"') fputc('"', out);
        fputc(*p, out);
    }
    fputc('"', out);
}

static void csv_write_row(FILE *out, char **fields, int n) {
    for (int i = 0; i < n; i++) {
        if (i) fputc(',', out);
        csv_write_field(out, fields[i]);
    }
    fputc('\n', out);
}

/*
 * Parse one CSV line into up to `max` fields (NUL-terminated, dequoted).
 * `line` is mutated. Returns field count.
 */
static int csv_parse_line(char *line, char **out, int max) {
    size_t n = strlen(line);
    while (n && (line[n - 1] == '\n' || line[n - 1] == '\r')) line[--n] = '\0';

    int count = 0;
    char *w = line;
    char *field = line;
    int in_quotes = 0;
    char *p = line;
    for (;; p++) {
        char c = *p;
        if (in_quotes) {
            if (c == '"') {
                if (p[1] == '"') { *w++ = '"'; p++; }
                else in_quotes = 0;
            } else if (c == '\0') {
                break;
            } else {
                *w++ = c;
            }
        } else {
            if (c == '"') {
                in_quotes = 1;
            } else if (c == ',' || c == '\0') {
                *w = '\0';
                if (count < max) out[count++] = field;
                if (c == '\0') break;
                w++;
                field = w;
            } else {
                *w++ = c;
            }
        }
    }
    return count;
}

/* ===================================================================== *
 *  Path helpers
 * ===================================================================== */

/* dir (<=SSQL_MAX_FIELD) + '/' + name (<=SSQL_MAX_FIELD) + ".csv" + NUL */
#define SSQL_MAX_PATH (SSQL_MAX_FIELD * 2 + 8)

static void table_path(const ssql_db *db, const char *name, char *buf, size_t cap) {
    snprintf(buf, cap, "%s/%s.csv", db->dir, name);
}

static int valid_table_name(const char *name) {
    if (!name || !(((*name >= 'A') && (*name <= 'Z')) || ((*name >= 'a') && (*name <= 'z')) || *name == '_')) return 0;
    for (const unsigned char *p = (const unsigned char *)name + 1; *p; ++p) {
        if (!((*p >= 'A' && *p <= 'Z') || (*p >= 'a' && *p <= 'z') || (*p >= '0' && *p <= '9') || *p == '_' || *p == '-')) return 0;
    }
    return 1;
}

static int table_exists(const ssql_db *db, const char *name) {
    char path[SSQL_MAX_PATH];
    table_path(db, name, path, sizeof(path));
    FILE *f = fopen(path, "rb");
    if (f) { fclose(f); return 1; }
    return 0;
}

static int find_col(char **cols, int ncols, const char *name) {
    for (int i = 0; i < ncols; i++)
        if (strcasecmp(cols[i], name) == 0) return i;
    return -1;
}

/* ===================================================================== *
 *  Compiled statement IR
 * ===================================================================== */

typedef enum {
    OP_CREATE,       /* CREATE TABLE                                        */
    OP_DROP,         /* DROP TABLE                                          */
    OP_INSERT,       /* INSERT                                              */
    OP_SELECT,       /* SELECT                                              */
    OP_SHOW_TABLES   /* SHOW TABLES                                         */
} ssql_op;

/*
 * A value slot is either a literal or a '?' placeholder. For placeholders,
 * `param` is the 1-based parameter index; `lit` holds the bound value (or NULL
 * until bound). For literals, `param` is 0 and `lit` is the literal text.
 */
typedef struct {
    int   param;     /* 1-based placeholder index, or 0 for a literal */
    char *lit;       /* literal text, or bound value for a placeholder */
} ssql_val;

struct ssql_stmt {
    ssql_db  *db;
    ssql_op   op;

    char      table[SSQL_MAX_FIELD];

    /* CREATE: column names.  SELECT: projection ("*" => star=1).           */
    char     *cols[SSQL_MAX_COLS];
    int       ncols;
    int       star;            /* SELECT * */

    /* INSERT values (literals and/or placeholders).                        */
    ssql_val  vals[SSQL_MAX_COLS];
    int       nvals;

    /* WHERE <col> = <value|?>  (equality only).                            */
    int       have_where;
    char      where_col[SSQL_MAX_FIELD];
    ssql_val  where_val;

    int       drop_if_exists;
    int       create_if_not_exists;
    int       count_star;

    int       nparams;         /* total '?' placeholders                    */
};

static void val_free(ssql_val *v) {
    if (v && v->lit) { free(v->lit); v->lit = NULL; }
}

void ssql_finalize(ssql_stmt *stmt) {
    if (!stmt) return;
    for (int i = 0; i < stmt->ncols; i++) free(stmt->cols[i]);
    for (int i = 0; i < stmt->nvals; i++) val_free(&stmt->vals[i]);
    val_free(&stmt->where_val);
    free(stmt);
}

/* Build an empty, zeroed statement bound to `db`. */
static ssql_stmt *stmt_new(ssql_db *db) {
    ssql_stmt *s = (ssql_stmt *)calloc(1, sizeof(*s));
    if (s) s->db = db;
    return s;
}

/*
 * Set a value slot from raw token text. A bare "?" becomes a placeholder and is
 * assigned the next parameter index; anything else is stored as a literal
 * (quotes already stripped by split_fields). Returns 0 on OK, -1 on OOM.
 */
static int val_set(ssql_stmt *st, ssql_val *v, const char *token) {
    if (strcmp(token, "?") == 0) {
        v->param = ++st->nparams;
        v->lit = NULL;             /* unbound until ssql_bind */
        return 0;
    }
    v->param = 0;
    v->lit = str_dup(token);
    return v->lit ? 0 : -1;
}

/* ===================================================================== *
 *  SQL compiler  (text -> ssql_stmt)
 * ===================================================================== */

/* CREATE TABLE <name> (<c1>, <c2>, ...) */
static ssql_status sql_create(ssql_stmt *st, char *rest) {
    if (kw_eq(rest, "if")) {
        rest = trim(rest + 2);
        if (!kw_eq(rest, "not")) return SSQL_ERR_SYNTAX;
        rest = trim(rest + 3);
        if (!kw_eq(rest, "exists")) return SSQL_ERR_SYNTAX;
        rest = trim(rest + 6);
        st->create_if_not_exists = 1;
    }
    char *lp = strchr(rest, '(');
    char *rp = strrchr(rest, ')');
    if (!lp || !rp || rp < lp) return SSQL_ERR_SYNTAX;
    *lp = '\0';
    char *name = trim(rest);
    if (!*name) return SSQL_ERR_SYNTAX;
    snprintf(st->table, sizeof(st->table), "%s", name);

    *rp = '\0';
    char *cols[SSQL_MAX_COLS];
    int ncols = split_fields(lp + 1, cols, SSQL_MAX_COLS);
    if (ncols < 1) return SSQL_ERR_SYNTAX;
    for (int i = 0; i < ncols; i++) {
        st->cols[i] = str_dup(cols[i]);
        if (!st->cols[i]) return SSQL_ERR_OOM;
    }
    st->ncols = ncols;
    st->op = OP_CREATE;
    return SSQL_OK;
}

/* DROP TABLE [IF EXISTS] <name> */
static ssql_status sql_drop(ssql_stmt *st, char *rest) {
    char *name = trim(rest);
    if (kw_eq(name, "if")) {
        name = trim(name + 2);
        if (kw_eq(name, "exists")) { name = trim(name + 6); st->drop_if_exists = 1; }
    }
    if (!*name) return SSQL_ERR_SYNTAX;
    snprintf(st->table, sizeof(st->table), "%s", name);
    st->op = OP_DROP;
    return SSQL_OK;
}

/* INSERT INTO <name> [(c,...)] VALUES (v,...) */
static ssql_status sql_insert(ssql_stmt *st, char *rest) {
    if (!kw_eq(rest, "into")) return SSQL_ERR_SYNTAX;
    rest = trim(rest + 4);

    char *vpos = rest;
    while (*vpos && !kw_eq(vpos, "values")) vpos++;
    if (!*vpos) return SSQL_ERR_SYNTAX;

    char nameseg[SSQL_MAX_FIELD];
    size_t seglen = (size_t)(vpos - rest);
    if (seglen >= sizeof(nameseg)) return SSQL_ERR_SYNTAX;
    memcpy(nameseg, rest, seglen);
    nameseg[seglen] = '\0';
    char *paren = strchr(nameseg, '(');
    if (paren) *paren = '\0';          /* positional insert; column list ignored */
    char *name = trim(nameseg);
    if (!*name) return SSQL_ERR_SYNTAX;
    snprintf(st->table, sizeof(st->table), "%s", name);

    char *lp = strchr(vpos, '(');
    char *rp = strrchr(vpos, ')');
    if (!lp || !rp || rp < lp) return SSQL_ERR_SYNTAX;
    *rp = '\0';
    char *vals[SSQL_MAX_COLS];
    int nvals = split_fields(lp + 1, vals, SSQL_MAX_COLS);
    if (nvals < 1) return SSQL_ERR_SYNTAX;
    for (int i = 0; i < nvals; i++)
        if (val_set(st, &st->vals[i], vals[i]) != 0) return SSQL_ERR_OOM;
    st->nvals = nvals;
    st->op = OP_INSERT;
    return SSQL_OK;
}

/* Parse "<col> = <value|?>" into st->where_*. `expr` is mutated. */
static ssql_status parse_where(ssql_stmt *st, char *expr) {
    char *eq = strchr(expr, '=');
    if (!eq) return SSQL_ERR_SYNTAX;
    *eq = '\0';
    char *wc = trim(expr);
    char *wv = trim(eq + 1);
    if (!*wc) return SSQL_ERR_SYNTAX;
    snprintf(st->where_col, sizeof(st->where_col), "%s", wc);
    if (strcmp(wv, "?") != 0) wv = unquote(wv);
    if (val_set(st, &st->where_val, wv) != 0) return SSQL_ERR_OOM;
    st->have_where = 1;
    return SSQL_OK;
}

/* SELECT * | c,... FROM <name> [WHERE col = value] */
static ssql_status sql_select(ssql_stmt *st, char *rest) {
    char *fpos = rest;
    while (*fpos && !kw_eq(fpos, "from")) fpos++;
    if (!*fpos) return SSQL_ERR_SYNTAX;
    char projbuf[SSQL_MAX_FIELD];
    size_t plen = (size_t)(fpos - rest);
    if (plen >= sizeof(projbuf)) return SSQL_ERR_SYNTAX;
    memcpy(projbuf, rest, plen);
    projbuf[plen] = '\0';
    char *proj = trim(projbuf);

    char *after = trim(fpos + 4);
    char *wpos = after;
    while (*wpos && !kw_eq(wpos, "where")) wpos++;
    char namebuf[SSQL_MAX_FIELD];
    size_t nlen = (size_t)(wpos - after);
    if (nlen >= sizeof(namebuf)) return SSQL_ERR_SYNTAX;
    memcpy(namebuf, after, nlen);
    namebuf[nlen] = '\0';
    char *name = trim(namebuf);
    if (!*name) return SSQL_ERR_SYNTAX;
    snprintf(st->table, sizeof(st->table), "%s", name);

    if (strcasecmp(proj, "count(*)") == 0) {
        st->count_star = 1;
    } else if (strcmp(proj, "*") == 0) {
        st->star = 1;
    } else {
        char *pcols[SSQL_MAX_COLS];
        int np = split_fields(proj, pcols, SSQL_MAX_COLS);
        if (np < 1) return SSQL_ERR_SYNTAX;
        for (int i = 0; i < np; i++) {
            st->cols[i] = str_dup(pcols[i]);
            if (!st->cols[i]) return SSQL_ERR_OOM;
        }
        st->ncols = np;
    }

    if (*wpos) {
        char *expr = trim(wpos + 5);
        ssql_status s = parse_where(st, expr);
        if (s != SSQL_OK) return s;
    }
    st->op = OP_SELECT;
    return SSQL_OK;
}

static ssql_status compile_sql(ssql_stmt *st, char *stmt) {
    if (kw_eq(stmt, "create")) {
        char *r = trim(stmt + 6);
        if (!kw_eq(r, "table")) return SSQL_ERR_SYNTAX;
        return sql_create(st, trim(r + 5));
    }
    if (kw_eq(stmt, "drop")) {
        char *r = trim(stmt + 4);
        if (!kw_eq(r, "table")) return SSQL_ERR_SYNTAX;
        return sql_drop(st, trim(r + 5));
    }
    if (kw_eq(stmt, "insert")) return sql_insert(st, trim(stmt + 6));
    if (kw_eq(stmt, "select")) return sql_select(st, trim(stmt + 6));
    if (kw_eq(stmt, "show")) {
        char *r = trim(stmt + 4);
        if (kw_eq(r, "tables")) { st->op = OP_SHOW_TABLES; return SSQL_OK; }
        return SSQL_ERR_SYNTAX;
    }
    return SSQL_ERR_SYNTAX;
}

/* ===================================================================== *
 *  SLeeLaSQL compiler  (fluent dialect -> ssql_stmt)
 *
 *  Grammar (dot-chained calls; '?' is a placeholder):
 *
 *    table("games").create(id, title, year)
 *    into("games").insert(?, ?, ?)
 *    from("games").select(title, year).where(year == ?)
 *    from("games").select(*)
 *    from("games").drop()                 // or drop(ifExists)
 *    tables()
 *
 *  It is deliberately a thin, 1:1 lowering onto the same IR as SQL -- same
 *  features, same executor, same placeholders -- just a cleaner surface.
 * ===================================================================== */

/* Pull the argument text inside the FIRST (...) that follows `*pp`. Advances
 * `*pp` past the closing ')'. Writes a NUL-terminated copy into `out`. */
static int take_parens(char **pp, char *out, size_t cap) {
    char *p = *pp;
    while (*p && *p != '(') p++;
    if (*p != '(') return -1;
    p++;
    int depth = 1;
    char quote = 0;
    size_t n = 0;
    while (*p) {
        char c = *p;
        if (quote) { if (c == quote) quote = 0; }
        else if (c == '\'' || c == '"') quote = c;
        else if (c == '(') depth++;
        else if (c == ')') { depth--; if (depth == 0) { p++; break; } }
        if (depth == 0) break;
        if (n + 1 < cap) out[n++] = c;
        p++;
    }
    out[n] = '\0';
    *pp = p;
    return 0;
}

/* Read the chain method name at `*pp` (letters), lower-cased into `out`. */
static int take_method(char **pp, char *out, size_t cap) {
    char *p = *pp;
    while (*p == '.' || isspace((unsigned char)*p)) p++;
    size_t n = 0;
    while (isalpha((unsigned char)*p)) {
        if (n + 1 < cap) out[n++] = (char)tolower((unsigned char)*p);
        p++;
    }
    out[n] = '\0';
    *pp = p;
    return n > 0 ? 0 : -1;
}

/* Translate "a == b" / "a = b" inside a where(...) to the shared parser form. */
static ssql_status sleela_where(ssql_stmt *st, char *expr) {
    /* accept '==' by collapsing to '=' */
    char *dd = strstr(expr, "==");
    if (dd) { dd[0] = '='; memmove(dd + 1, dd + 2, strlen(dd + 2) + 1); }
    return parse_where(st, expr);
}

static ssql_status compile_sleela(ssql_stmt *st, char *src) {
    char method[64];
    char args[SSQL_MAX_FIELD];
    char *p = src;

    if (take_method(&p, method, sizeof(method)) != 0) return SSQL_ERR_SYNTAX;

    if (strcmp(method, "tables") == 0) {
        st->op = OP_SHOW_TABLES;
        return SSQL_OK;
    }

    /* The three roots that carry a table name: table(...), into(...), from(...) */
    int is_table = (strcmp(method, "table") == 0);
    int is_into  = (strcmp(method, "into")  == 0);
    int is_from  = (strcmp(method, "from")  == 0);
    if (!is_table && !is_into && !is_from) return SSQL_ERR_SYNTAX;

    if (take_parens(&p, args, sizeof(args)) != 0) return SSQL_ERR_SYNTAX;
    {
        char *nm = unquote(trim(args));
        if (!*nm) return SSQL_ERR_SYNTAX;
        snprintf(st->table, sizeof(st->table), "%s", nm);
    }

    /* second method: the verb */
    if (take_method(&p, method, sizeof(method)) != 0) return SSQL_ERR_SYNTAX;

    if (is_table && strcmp(method, "create") == 0) {
        if (take_parens(&p, args, sizeof(args)) != 0) return SSQL_ERR_SYNTAX;
        char *cols[SSQL_MAX_COLS];
        int nc = split_fields(args, cols, SSQL_MAX_COLS);
        if (nc < 1) return SSQL_ERR_SYNTAX;
        for (int i = 0; i < nc; i++) {
            st->cols[i] = str_dup(cols[i]);
            if (!st->cols[i]) return SSQL_ERR_OOM;
        }
        st->ncols = nc;
        st->op = OP_CREATE;
        return SSQL_OK;
    }

    if (is_from && strcmp(method, "drop") == 0) {
        if (take_parens(&p, args, sizeof(args)) != 0) return SSQL_ERR_SYNTAX;
        char *a = trim(args);
        if (strcasecmp(a, "ifexists") == 0 || strcasecmp(a, "if_exists") == 0)
            st->drop_if_exists = 1;
        st->op = OP_DROP;
        return SSQL_OK;
    }

    if (is_into && strcmp(method, "insert") == 0) {
        if (take_parens(&p, args, sizeof(args)) != 0) return SSQL_ERR_SYNTAX;
        char *vals[SSQL_MAX_COLS];
        int nv = split_fields(args, vals, SSQL_MAX_COLS);
        if (nv < 1) return SSQL_ERR_SYNTAX;
        for (int i = 0; i < nv; i++)
            if (val_set(st, &st->vals[i], vals[i]) != 0) return SSQL_ERR_OOM;
        st->nvals = nv;
        st->op = OP_INSERT;
        return SSQL_OK;
    }

    if (is_from && strcmp(method, "select") == 0) {
        if (take_parens(&p, args, sizeof(args)) != 0) return SSQL_ERR_SYNTAX;
        char *proj = trim(args);
        if (strcasecmp(proj, "count(*)") == 0) {
            st->count_star = 1;
        } else if (strcmp(proj, "*") == 0 || *proj == '\0') {
            st->star = 1;
        } else {
            char *pcols[SSQL_MAX_COLS];
            int np = split_fields(proj, pcols, SSQL_MAX_COLS);
            if (np < 1) return SSQL_ERR_SYNTAX;
            for (int i = 0; i < np; i++) {
                st->cols[i] = str_dup(pcols[i]);
                if (!st->cols[i]) return SSQL_ERR_OOM;
            }
            st->ncols = np;
        }
        st->op = OP_SELECT;
        /* optional trailing .where(...) */
        char saved[64];
        char *probe = p;
        if (take_method(&probe, saved, sizeof(saved)) == 0 &&
            strcmp(saved, "where") == 0) {
            p = probe;
            if (take_parens(&p, args, sizeof(args)) != 0) return SSQL_ERR_SYNTAX;
            ssql_status s = sleela_where(st, trim(args));
            if (s != SSQL_OK) return s;
        }
        return SSQL_OK;
    }

    return SSQL_ERR_SYNTAX;
}

/* ===================================================================== *
 *  Dialect sniffing
 * ===================================================================== */

/*
 * Heuristic: SLeeLaSQL always begins with one of its fluent roots --
 * table(, into(, from(, tables( -- so a statement whose first identifier is one
 * of those and is immediately followed by '(' is SLeeLaSQL. Everything else is
 * treated as SQL. (AUTO only; callers can force a dialect.)
 */
static ssql_dialect sniff(const char *stmt) {
    const char *p = stmt;
    while (*p && isspace((unsigned char)*p)) p++;
    char word[16];
    size_t n = 0;
    while (isalpha((unsigned char)*p) && n + 1 < sizeof(word))
        word[n++] = (char)tolower((unsigned char)*p++);
    word[n] = '\0';
    while (*p && isspace((unsigned char)*p)) p++;
    if (*p != '(') return SSQL_DIALECT_SQL;
    if (!strcmp(word, "table") || !strcmp(word, "into") ||
        !strcmp(word, "from")  || !strcmp(word, "tables"))
        return SSQL_DIALECT_SLEELA;
    return SSQL_DIALECT_SQL;
}

/* ===================================================================== *
 *  Executor  (ssql_stmt -> effect / rows)
 * ===================================================================== */

/* Resolve a value slot to its effective string (literal or bound value). */
static ssql_status val_resolve(const ssql_val *v, const char **out) {
    if (v->param != 0 && v->lit == NULL) return SSQL_ERR_BIND;
    *out = v->lit ? v->lit : "";
    return SSQL_OK;
}

static ssql_status exec_create(ssql_stmt *st) {
    if (table_exists(st->db, st->table)) return st->create_if_not_exists ? SSQL_OK : SSQL_ERR_EXISTS;
    char path[SSQL_MAX_PATH];
    table_path(st->db, st->table, path, sizeof(path));
    FILE *f = fopen(path, "wb");
    if (!f) return SSQL_ERR_IO;
    csv_write_row(f, st->cols, st->ncols);
    fclose(f);
    return SSQL_OK;
}

static ssql_status exec_drop(ssql_stmt *st) {
    if (!table_exists(st->db, st->table))
        return st->drop_if_exists ? SSQL_OK : SSQL_ERR_NOTABLE;
    char path[SSQL_MAX_PATH];
    table_path(st->db, st->table, path, sizeof(path));
    return remove(path) == 0 ? SSQL_OK : SSQL_ERR_IO;
}

static ssql_status exec_insert(ssql_stmt *st) {
    /* arity check against the stored header */
    char path[SSQL_MAX_PATH];
    table_path(st->db, st->table, path, sizeof(path));
    FILE *f = fopen(path, "rb");
    if (!f) return SSQL_ERR_NOTABLE;
    char hdr[SSQL_MAX_FIELD * 8];
    char *cols[SSQL_MAX_COLS];
    int ncols = 0;
    if (fgets(hdr, sizeof(hdr), f)) ncols = csv_parse_line(hdr, cols, SSQL_MAX_COLS);
    fclose(f);
    if (st->nvals != ncols) return SSQL_ERR_ARITY;

    /* resolve each value (literal or bound placeholder) */
    const char *resolved[SSQL_MAX_COLS];
    for (int i = 0; i < st->nvals; i++) {
        ssql_status s = val_resolve(&st->vals[i], &resolved[i]);
        if (s != SSQL_OK) return s;
    }

    f = fopen(path, "ab");
    if (!f) return SSQL_ERR_IO;
    csv_write_row(f, (char **)resolved, st->nvals);
    fclose(f);
    return SSQL_OK;
}

static ssql_status exec_select(ssql_stmt *st, FILE *out) {
    const char *where_val = NULL;
    if (st->have_where) {
        ssql_status s = val_resolve(&st->where_val, &where_val);
        if (s != SSQL_OK) return s;
    }

    char path[SSQL_MAX_PATH];
    table_path(st->db, st->table, path, sizeof(path));
    FILE *f = fopen(path, "rb");
    if (!f) return SSQL_ERR_NOTABLE;

    char line[SSQL_MAX_FIELD * 8];
    char hdrbuf[SSQL_MAX_FIELD * 8];
    char *cols[SSQL_MAX_COLS];
    int ncols = 0;
    if (fgets(hdrbuf, sizeof(hdrbuf), f)) ncols = csv_parse_line(hdrbuf, cols, SSQL_MAX_COLS);

    int proj_idx[SSQL_MAX_COLS];
    int nproj = 0;
    if (st->star) {
        for (int i = 0; i < ncols; i++) proj_idx[nproj++] = i;
    } else {
        for (int i = 0; i < st->ncols; i++) {
            int idx = find_col(cols, ncols, st->cols[i]);
            if (idx < 0) { fclose(f); return SSQL_ERR_NOCOL; }
            proj_idx[nproj++] = idx;
        }
    }

    int where_idx = -1;
    if (st->have_where) {
        where_idx = find_col(cols, ncols, st->where_col);
        if (where_idx < 0) { fclose(f); return SSQL_ERR_NOCOL; }
    }

    long long count = 0;
    if (out && !st->count_star) {
        for (int i = 0; i < nproj; i++) {
            if (i) fputc(',', out);
            csv_write_field(out, cols[proj_idx[i]]);
        }
        fputc('\n', out);
    }

    while (fgets(line, sizeof(line), f)) {
        char rowtmp[SSQL_MAX_FIELD * 8];
        snprintf(rowtmp, sizeof(rowtmp), "%s", line);
        char *fields[SSQL_MAX_COLS];
        int nf = csv_parse_line(rowtmp, fields, SSQL_MAX_COLS);
        if (nf == 0) continue;
        if (st->have_where) {
            if (where_idx >= nf) continue;
            if (strcmp(fields[where_idx], where_val) != 0) continue;
        }
        if (st->count_star) { count++; continue; }
        if (out) {
            for (int i = 0; i < nproj; i++) {
                if (i) fputc(',', out);
                int idx = proj_idx[i];
                csv_write_field(out, idx < nf ? fields[idx] : "");
            }
            fputc('\n', out);
        }
    }
    fclose(f);
    if (st->count_star && out) fprintf(out, "COUNT(*)\n%lld\n", count);
    return SSQL_OK;
}

static void emit_table_name(FILE *out, const char *filename) {
    size_t len = strlen(filename);
    if (len <= 4 || strcmp(filename + len - 4, ".csv") != 0) return;
    if (out) {
        char base[SSQL_MAX_FIELD];
        size_t blen = len - 4;
        if (blen >= sizeof(base)) blen = sizeof(base) - 1;
        memcpy(base, filename, blen);
        base[blen] = '\0';
        csv_write_field(out, base);
        fputc('\n', out);
    }
}

static ssql_status exec_show_tables(ssql_stmt *st, FILE *out) {
    if (out) fputs("Tables_in_sleela_sql\n", out);
#ifdef _WIN32
    {
        char pattern[SSQL_MAX_PATH];
        struct _finddata_t entry;
        intptr_t handle;
        snprintf(pattern, sizeof(pattern), "%s/*.csv", st->db->dir);
        handle = _findfirst(pattern, &entry);
        if (handle == -1L) return errno == ENOENT ? SSQL_OK : SSQL_ERR_IO;
        do { emit_table_name(out, entry.name); } while (_findnext(handle, &entry) == 0);
        _findclose(handle);
    }
#else
    {
        DIR *d = opendir(st->db->dir);
        struct dirent *e;
        if (!d) return SSQL_ERR_IO;
        while ((e = readdir(d)) != NULL) emit_table_name(out, e->d_name);
        closedir(d);
    }
#endif
    return SSQL_OK;
}

static ssql_status exec_stmt(ssql_stmt *st, FILE *out) {
    switch (st->op) {
        case OP_CREATE:      return exec_create(st);
        case OP_DROP:        return exec_drop(st);
        case OP_INSERT:      return exec_insert(st);
        case OP_SELECT:      return exec_select(st, out);
        case OP_SHOW_TABLES: return exec_show_tables(st, out);
        default:             return SSQL_ERR_SYNTAX;
    }
}

/* ===================================================================== *
 *  Public API
 * ===================================================================== */

ssql_status ssql_open(ssql_db *db, const char *dir) {
    struct stat st;
    if (!db || !dir || !*dir) return SSQL_ERR_ARG;
    if (strlen(dir) >= sizeof(db->dir)) return SSQL_ERR_ARG;
    if (stat(dir, &st) != 0) {
        if (errno != ENOENT || SSQL_MKDIR(dir) != 0) return SSQL_ERR_IO;
        if (stat(dir, &st) != 0) return SSQL_ERR_IO;
    }
#ifdef _WIN32
    if ((st.st_mode & _S_IFMT) != _S_IFDIR) return SSQL_ERR_IO;
#else
    if (!S_ISDIR(st.st_mode)) return SSQL_ERR_IO;
#endif
    snprintf(db->dir, sizeof(db->dir), "%s", dir);
    return SSQL_OK;
}

ssql_status ssql_prepare(ssql_db *db, const char *text,
                         ssql_dialect dialect, ssql_stmt **out_stmt) {
    if (!db || !text || !out_stmt) return SSQL_ERR_ARG;
    *out_stmt = NULL;

    char *buf = str_dup(text);
    if (!buf) return SSQL_ERR_OOM;
    char *stmt = trim(buf);
    size_t n = strlen(stmt);
    while (n && (stmt[n - 1] == ';' || isspace((unsigned char)stmt[n - 1])))
        stmt[--n] = '\0';
    if (!*stmt) { free(buf); return SSQL_ERR_SYNTAX; }

    if (dialect == SSQL_DIALECT_AUTO) dialect = sniff(stmt);

    ssql_stmt *st = stmt_new(db);
    if (!st) { free(buf); return SSQL_ERR_OOM; }

    ssql_status s = (dialect == SSQL_DIALECT_SLEELA)
                        ? compile_sleela(st, stmt)
                        : compile_sql(st, stmt);
    if (s == SSQL_OK && st->op != OP_SHOW_TABLES && !valid_table_name(st->table)) s = SSQL_ERR_SYNTAX;
    free(buf);
    if (s != SSQL_OK) { ssql_finalize(st); return s; }

    *out_stmt = st;
    return SSQL_OK;
}

int ssql_param_count(const ssql_stmt *stmt) {
    return stmt ? stmt->nparams : 0;
}

/* Point at the value slot for the given 1-based placeholder index, or NULL. */
static ssql_val *slot_for_param(ssql_stmt *stmt, int index) {
    for (int i = 0; i < stmt->nvals; i++)
        if (stmt->vals[i].param == index) return &stmt->vals[i];
    if (stmt->have_where && stmt->where_val.param == index)
        return &stmt->where_val;
    return NULL;
}

ssql_status ssql_bind(ssql_stmt *stmt, int index, const char *value) {
    if (!stmt || !value || index < 1 || index > stmt->nparams)
        return SSQL_ERR_BIND;
    ssql_val *v = slot_for_param(stmt, index);
    if (!v) return SSQL_ERR_BIND;
    char *copy = str_dup(value);
    if (!copy) return SSQL_ERR_OOM;
    if (v->lit) free(v->lit);
    v->lit = copy;
    return SSQL_OK;
}

void ssql_reset(ssql_stmt *stmt) {
    if (!stmt) return;
    for (int i = 0; i < stmt->nvals; i++)
        if (stmt->vals[i].param != 0) val_free(&stmt->vals[i]);
    if (stmt->have_where && stmt->where_val.param != 0)
        val_free(&stmt->where_val);
}

ssql_status ssql_run(ssql_stmt *stmt, FILE *out) {
    if (!stmt) return SSQL_ERR_ARG;
    return exec_stmt(stmt, out);
}

ssql_status ssql_exec_dialect(ssql_db *db, const char *text,
                              ssql_dialect dialect, FILE *out) {
    ssql_stmt *st = NULL;
    ssql_status s = ssql_prepare(db, text, dialect, &st);
    if (s != SSQL_OK) return s;
    s = ssql_run(st, out);
    ssql_finalize(st);
    return s;
}

ssql_status ssql_exec(ssql_db *db, const char *text, FILE *out) {
    return ssql_exec_dialect(db, text, SSQL_DIALECT_AUTO, out);
}

const char *ssql_strerror(ssql_status s) {
    switch (s) {
        case SSQL_OK:          return "ok";
        case SSQL_ERR_IO:      return "I/O error";
        case SSQL_ERR_SYNTAX:  return "syntax error";
        case SSQL_ERR_NOTABLE: return "no such table";
        case SSQL_ERR_EXISTS:  return "table already exists";
        case SSQL_ERR_NOCOL:   return "no such column";
        case SSQL_ERR_ARITY:   return "value count does not match column count";
        case SSQL_ERR_ARG:     return "bad argument";
        case SSQL_ERR_BIND:    return "unbound or out-of-range placeholder";
        case SSQL_ERR_OOM:     return "out of memory";
        default:               return "unknown error";
    }
}

const char *ssql_dialect_name(ssql_dialect d) {
    switch (d) {
        case SSQL_DIALECT_SQL:    return "sql";
        case SSQL_DIALECT_SLEELA: return "sleelasql";
        default:                  return "auto";
    }
}
