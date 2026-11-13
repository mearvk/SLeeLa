/* java_lex.c -- see java_lex.h. Plain C99. */
#include "java_lex.h"

#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    const char *s;
    size_t len, pos;
    int line, col;
    char *err;
    size_t errlen;
} lx;

static int peekc(const lx *x, size_t o) { return x->pos + o < x->len ? (unsigned char)x->s[x->pos + o] : -1; }

static void adv(lx *x) {
    if (x->pos >= x->len) return;
    if (x->s[x->pos] == '\n') { x->line++; x->col = 1; } else x->col++;
    x->pos++;
}

static int lerr(lx *x, const char *msg) {
    snprintf(x->err, x->errlen, "%d:%d: %s", x->line, x->col, msg);
    return -1;
}

static int push(jtokens *t, jt_kind k, char *text, long long iv, int line, int col) {
    if (t->n == t->cap) {
        size_t nc = t->cap ? t->cap * 2 : 1024;
        jtoken *p = (jtoken *)realloc(t->v, nc * sizeof *p);
        if (!p) return -1;
        t->v = p; t->cap = nc;
    }
    t->v[t->n].kind = k;
    t->v[t->n].text = text;
    t->v[t->n].ival = iv;
    t->v[t->n].line = line;
    t->v[t->n].col = col;
    t->n++;
    return 0;
}

static char *dupn(const char *s, size_t n) {
    char *p = (char *)malloc(n + 1);
    if (!p) return NULL;
    memcpy(p, s, n);
    p[n] = '\0';
    return p;
}

static int is_id_start(int c) { return c == '_' || c == '$' || isalpha(c) || c >= 0x80; }
static int is_id_part(int c)  { return is_id_start(c) || isdigit(c); }

/* append a code point as UTF-8 */
static size_t put_utf8(char *o, unsigned long cp) {
    if (cp < 0x80) { o[0] = (char)cp; return 1; }
    if (cp < 0x800) { o[0] = (char)(0xC0 | (cp >> 6)); o[1] = (char)(0x80 | (cp & 0x3F)); return 2; }
    if (cp < 0x10000) {
        o[0] = (char)(0xE0 | (cp >> 12)); o[1] = (char)(0x80 | ((cp >> 6) & 0x3F));
        o[2] = (char)(0x80 | (cp & 0x3F)); return 3;
    }
    o[0] = (char)(0xF0 | (cp >> 18)); o[1] = (char)(0x80 | ((cp >> 12) & 0x3F));
    o[2] = (char)(0x80 | ((cp >> 6) & 0x3F)); o[3] = (char)(0x80 | (cp & 0x3F)); return 4;
}

/* Decode one escape after the backslash. Returns the code point or -1. */
static long read_escape(lx *x) {
    int c = peekc(x, 0);
    long v;
    switch (c) {
        case 'n': adv(x); return '\n';
        case 't': adv(x); return '\t';
        case 'b': adv(x); return '\b';
        case 'r': adv(x); return '\r';
        case 'f': adv(x); return '\f';
        case 's': adv(x); return ' ';
        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': {
            int n = 0; v = 0;
            int maxd = (c <= '3') ? 3 : 2;
            while (n < maxd && peekc(x, 0) >= '0' && peekc(x, 0) <= '7') { v = v * 8 + (peekc(x, 0) - '0'); adv(x); n++; }
            return v;
        }
        case '"': adv(x); return '"';
        case '\'': adv(x); return '\'';
        case '\\': adv(x); return '\\';
        case 'u': {
            int i;
            while (peekc(x, 0) == 'u') adv(x);
            v = 0;
            for (i = 0; i < 4; i++) {
                int h = peekc(x, 0);
                if (!isxdigit(h)) return -1;
                v = v * 16 + (isdigit(h) ? h - '0' : (tolower(h) - 'a' + 10));
                adv(x);
            }
            return v;
        }
        default: return -1;
    }
}

static int lex_number(lx *x, jtokens *t, int line, int col) {
    size_t start = x->pos;
    char buf[128];
    size_t bl = 0;
    int base = 10, isfloat = 0, c;
    unsigned long long u = 0;

    if (peekc(x, 0) == '0' && (peekc(x, 1) == 'x' || peekc(x, 1) == 'X')) { base = 16; adv(x); adv(x); }
    else if (peekc(x, 0) == '0' && (peekc(x, 1) == 'b' || peekc(x, 1) == 'B')) { base = 2; adv(x); adv(x); }

    if (base != 10) {
        int any = 0;
        for (;;) {
            c = peekc(x, 0);
            if (c == '_') { adv(x); continue; }
            if (base == 16 ? isxdigit(c) : (c == '0' || c == '1')) {
                int d = isdigit(c) ? c - '0' : tolower(c) - 'a' + 10;
                if (u > (0xFFFFFFFFFFFFFFFFULL - (unsigned)d) / (unsigned)base) return lerr(x, "integer literal too large");
                u = u * (unsigned)base + (unsigned)d; any = 1; adv(x);
            } else break;
        }
        if (!any) return lerr(x, "malformed numeric literal");
    } else {
        for (;;) {
            c = peekc(x, 0);
            if (c == '_') { adv(x); continue; }
            if (isdigit(c)) { if (bl < sizeof buf - 2) buf[bl++] = (char)c; adv(x); }
            else if (c == '.' && isdigit(peekc(x, 1))) { isfloat = 1; if (bl < sizeof buf - 2) buf[bl++] = '.'; adv(x); }
            else if (c == '.' && !is_id_start(peekc(x, 1)) && peekc(x, 1) != '.') { isfloat = 1; if (bl < sizeof buf - 2) { buf[bl++] = '.'; buf[bl++] = '0'; } adv(x); }
            else if ((c == 'e' || c == 'E') && (isdigit(peekc(x, 1)) || ((peekc(x, 1) == '+' || peekc(x, 1) == '-') && isdigit(peekc(x, 2))))) {
                isfloat = 1; if (bl < sizeof buf - 2) buf[bl++] = 'e'; adv(x);
                if (peekc(x, 0) == '+' || peekc(x, 0) == '-') { if (bl < sizeof buf - 2) buf[bl++] = (char)peekc(x, 0); adv(x); }
            } else break;
        }
        buf[bl] = '\0';
    }
    c = peekc(x, 0);
    if (c == 'f' || c == 'F') { adv(x); if (base != 10) return lerr(x, "malformed numeric literal"); return push(t, JT_FLOAT, dupn(buf, bl), 0, line, col); }
    if (c == 'd' || c == 'D') { adv(x); if (base != 10) return lerr(x, "malformed numeric literal"); isfloat = 1; }
    if (isfloat) {
        if (base != 10) return lerr(x, "hexadecimal floating literals are not supported");
        if (!strchr(buf, '.') && !strchr(buf, 'e')) { if (bl < sizeof buf - 3) { buf[bl++] = '.'; buf[bl++] = '0'; buf[bl] = '\0'; } }
        return push(t, JT_DOUBLE, dupn(buf, strlen(buf)), 0, line, col);
    }
    if (base == 10) {
        /* octal: leading 0 followed by digits */
        if (bl > 1 && buf[0] == '0') {
            size_t i;
            base = 8; u = 0;
            for (i = 1; i < bl; i++) {
                if (buf[i] > '7') return lerr(x, "invalid octal literal");
                u = u * 8 + (unsigned)(buf[i] - '0');
            }
        } else {
            size_t i;
            u = 0;
            for (i = 0; i < bl; i++) {
                unsigned d = (unsigned)(buf[i] - '0');
                if (u > (0xFFFFFFFFFFFFFFFFULL - d) / 10ULL) return lerr(x, "integer literal too large");
                u = u * 10 + d;
            }
        }
    }
    if (peekc(x, 0) == 'l' || peekc(x, 0) == 'L') {
        adv(x);
        if (base == 10 && u > 9223372036854775808ULL) return lerr(x, "long literal out of range");
        /* 9223372036854775808L is only valid under unary minus: keep as wrapped value */
        return push(t, JT_LONG, NULL, (long long)u, line, col);
    }
    if (base == 10) { if (u > 2147483648ULL) return lerr(x, "int literal out of range"); }
    else { if (u > 0xFFFFFFFFULL) return lerr(x, "int literal out of range"); u = (unsigned long long)(int32_t)(uint32_t)u; }
    (void)start;
    return push(t, JT_INT, NULL, (long long)u, line, col);
}

static const char *const OPS[] = {
    ">>>=", "<<=", ">>=", ">>>", "...", "->", "::", "++", "--", "&&", "||", "==", "!=", "<=", ">=",
    "+=", "-=", "*=", "/=", "&=", "|=", "^=", "%=", "<<", ">>",
    "(", ")", "{", "}", "[", "]", ";", ",", ".", "@", "=", ">", "<", "!", "~", "?", ":",
    "+", "-", "*", "/", "&", "|", "^", "%", NULL
};

int jlex(const char *src, size_t len, jtokens *out, char *err, size_t errlen) {
    lx x;
    memset(out, 0, sizeof *out);
    x.s = src; x.len = len; x.pos = 0; x.line = 1; x.col = 1; x.err = err; x.errlen = errlen;

    for (;;) {
        int c, line, col;
        /* whitespace and comments */
        for (;;) {
            c = peekc(&x, 0);
            if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f') { adv(&x); continue; }
            if (c == '/' && peekc(&x, 1) == '/') { while (peekc(&x, 0) != -1 && peekc(&x, 0) != '\n') adv(&x); continue; }
            if (c == '/' && peekc(&x, 1) == '*') {
                adv(&x); adv(&x);
                while (!(peekc(&x, 0) == '*' && peekc(&x, 1) == '/')) {
                    if (peekc(&x, 0) == -1) return lerr(&x, "unterminated comment");
                    adv(&x);
                }
                adv(&x); adv(&x); continue;
            }
            break;
        }
        line = x.line; col = x.col;
        c = peekc(&x, 0);
        if (c == -1) { push(out, JT_EOF, NULL, 0, line, col); return 0; }

        if (is_id_start(c)) {
            size_t s = x.pos;
            while (is_id_part(peekc(&x, 0))) adv(&x);
            if (push(out, JT_IDENT, dupn(src + s, x.pos - s), 0, line, col)) return lerr(&x, "out of memory");
            continue;
        }
        if (isdigit(c) || (c == '.' && isdigit(peekc(&x, 1)))) {
            if (c == '.') {
                /* leading-dot double like .5 */
                size_t s = x.pos; char b[64]; size_t bl = 0;
                b[bl++] = '0';
                while (peekc(&x, 0) == '.' || isdigit(peekc(&x, 0)) || peekc(&x, 0) == '_') { if (peekc(&x, 0) != '_' && bl < 60) b[bl++] = (char)peekc(&x, 0); adv(&x); }
                if (peekc(&x, 0) == 'f' || peekc(&x, 0) == 'F') { adv(&x); if (push(out, JT_FLOAT, dupn(b, bl), 0, line, col)) return -1; continue; }
                if (peekc(&x, 0) == 'd' || peekc(&x, 0) == 'D') adv(&x);
                (void)s;
                if (push(out, JT_DOUBLE, dupn(b, bl), 0, line, col)) return -1;
                continue;
            }
            if (lex_number(&x, out, line, col)) return -1;
            continue;
        }
        if (c == '"') {
            char *buf;
            size_t bl = 0, cap;
            if (peekc(&x, 1) == '"' && peekc(&x, 2) == '"') {
                /* text block: strip the incidental indentation per JLS 3.10.6 (simplified:
                   common leading whitespace of non-blank lines, trailing space removal) */
                size_t s, e;
                char *raw, *lines[4096];
                int nl = 0, i, minind = 1 << 30;
                adv(&x); adv(&x); adv(&x);
                while (peekc(&x, 0) == ' ' || peekc(&x, 0) == '\t') adv(&x);
                if (peekc(&x, 0) == '\r') adv(&x);
                if (peekc(&x, 0) != '\n') return lerr(&x, "text block opening delimiter must end its line");
                adv(&x);
                s = x.pos;
                for (;;) {
                    if (peekc(&x, 0) == -1) return lerr(&x, "unterminated text block");
                    if (peekc(&x, 0) == '\\') { adv(&x); adv(&x); continue; }
                    if (peekc(&x, 0) == '"' && peekc(&x, 1) == '"' && peekc(&x, 2) == '"') break;
                    adv(&x);
                }
                e = x.pos;
                adv(&x); adv(&x); adv(&x);
                raw = dupn(src + s, e - s);
                /* split into lines */
                {
                    char *p = raw;
                    lines[nl++] = p;
                    while (*p) {
                        if (*p == '\n' && nl < 4095) { *p = '\0'; lines[nl++] = p + 1; }
                        p++;
                    }
                }
                for (i = 0; i < nl; i++) {
                    char *L = lines[i];
                    size_t k = strlen(L);
                    int blank, ind = 0;
                    if (k && L[k - 1] == '\r') L[--k] = '\0';
                    { size_t q; blank = 1; for (q = 0; q < k; q++) if (L[q] != ' ' && L[q] != '\t') { blank = 0; break; } }
                    if (blank && i != nl - 1) continue;
                    while (L[ind] == ' ' || L[ind] == '\t') ind++;
                    if (ind < minind) minind = ind;
                }
                if (minind == (1 << 30)) minind = 0;
                cap = (size_t)(e - s) * 2 + 16;
                buf = (char *)malloc(cap);
                for (i = 0; i < nl; i++) {
                    char *L = lines[i];
                    size_t k = strlen(L), q = 0;
                    int last = (i == nl - 1);
                    int blank = 1;
                    { size_t z; for (z = 0; z < k; z++) if (L[z] != ' ' && L[z] != '\t') { blank = 0; break; } }
                    if (blank) { if (!last) buf[bl++] = '\n'; continue; }
                    while (q < (size_t)minind && q < k) q++;
                    while (k > q && (L[k - 1] == ' ' || L[k - 1] == '\t')) k--;
                    if (last) { /* closing delimiter on same line as content: keep content, no newline */
                        size_t z; for (z = q; z < k; z++) buf[bl++] = L[z];
                    } else {
                        size_t z; for (z = q; z < k; z++) buf[bl++] = L[z];
                        buf[bl++] = '\n';
                    }
                }
                free(raw);
                buf[bl] = '\0';
                /* process escapes in the stripped text */
                {
                    char *o = (char *)malloc(bl + 8);
                    size_t oi = 0, ii = 0;
                    while (ii < bl) {
                        if (buf[ii] == '\\' && ii + 1 < bl) {
                            char n = buf[ii + 1];
                            ii += 2;
                            switch (n) {
                                case 'n': o[oi++] = '\n'; break;
                                case 't': o[oi++] = '\t'; break;
                                case 'r': o[oi++] = '\r'; break;
                                case 's': o[oi++] = ' '; break;
                                case '"': o[oi++] = '"'; break;
                                case '\\': o[oi++] = '\\'; break;
                                case '\'': o[oi++] = '\''; break;
                                case '\n': break; /* line continuation */
                                default: free(o); free(buf); return lerr(&x, "unsupported escape in text block");
                            }
                        } else o[oi++] = buf[ii++];
                    }
                    o[oi] = '\0';
                    free(buf);
                    if (push(out, JT_TEXTBLOCK, o, 0, line, col)) return -1;
                }
                continue;
            }
            adv(&x);
            cap = 64;
            buf = (char *)malloc(cap);
            for (;;) {
                int ch = peekc(&x, 0);
                if (ch == -1 || ch == '\n') { free(buf); return lerr(&x, "unterminated string literal"); }
                if (ch == '"') { adv(&x); break; }
                if (bl + 8 > cap) { cap *= 2; buf = (char *)realloc(buf, cap); }
                if (ch == '\\') {
                    long v;
                    adv(&x);
                    v = read_escape(&x);
                    if (v < 0) { free(buf); return lerr(&x, "invalid escape sequence"); }
                    /* surrogate pair via two \\u escapes */
                    if (v >= 0xD800 && v <= 0xDBFF && peekc(&x, 0) == '\\' && peekc(&x, 1) == 'u') {
                        long lo;
                        adv(&x);
                        lo = read_escape(&x);
                        if (lo >= 0xDC00 && lo <= 0xDFFF) v = 0x10000 + ((v - 0xD800) << 10) + (lo - 0xDC00);
                        else { free(buf); return lerr(&x, "unpaired surrogate escape"); }
                    } else if (v >= 0xD800 && v <= 0xDFFF) { free(buf); return lerr(&x, "unpaired surrogate escape"); }
                    bl += put_utf8(buf + bl, (unsigned long)v);
                } else { buf[bl++] = (char)ch; adv(&x); }
            }
            buf[bl] = '\0';
            if (push(out, JT_STRING, buf, 0, line, col)) return -1;
            continue;
        }
        if (c == '\'') {
            long v;
            adv(&x);
            if (peekc(&x, 0) == '\\') { adv(&x); v = read_escape(&x); if (v < 0) return lerr(&x, "invalid escape sequence"); }
            else {
                /* decode one UTF-8 scalar into a UTF-16 unit */
                unsigned char b0 = (unsigned char)peekc(&x, 0);
                if (b0 < 0x80) { v = b0; adv(&x); }
                else if ((b0 & 0xE0) == 0xC0) { v = b0 & 0x1F; adv(&x); v = (v << 6) | (peekc(&x, 0) & 0x3F); adv(&x); }
                else if ((b0 & 0xF0) == 0xE0) { v = b0 & 0x0F; adv(&x); v = (v << 6) | (peekc(&x, 0) & 0x3F); adv(&x); v = (v << 6) | (peekc(&x, 0) & 0x3F); adv(&x); }
                else return lerr(&x, "character literal outside the basic multilingual plane");
            }
            if (peekc(&x, 0) != '\'') return lerr(&x, "unterminated character literal");
            adv(&x);
            if (push(out, JT_CHAR, NULL, v, line, col)) return -1;
            continue;
        }
        {
            int i, hit = 0;
            for (i = 0; OPS[i]; i++) {
                size_t L = strlen(OPS[i]);
                if (x.pos + L <= x.len && strncmp(src + x.pos, OPS[i], L) == 0) {
                    size_t k;
                    for (k = 0; k < L; k++) adv(&x);
                    if (push(out, JT_OP, dupn(OPS[i], L), 0, line, col)) return lerr(&x, "out of memory");
                    hit = 1;
                    break;
                }
            }
            if (!hit) { char m[64]; snprintf(m, sizeof m, "unexpected character '%c'", c); return lerr(&x, m); }
        }
    }
}

void jtokens_free(jtokens *t) {
    size_t i;
    for (i = 0; i < t->n; i++) free(t->v[i].text);
    free(t->v);
    memset(t, 0, sizeof *t);
}
