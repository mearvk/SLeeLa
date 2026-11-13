/* java_lex.h -- Java source tokenizer (JLS chapter 3) for the sleelactl converter.
 *
 * Pure C99. Produces a flat token array; comments and whitespace are dropped.
 * Literals keep their source spelling; numeric literals are also decoded so the
 * C++ translator never has to re-parse digits (hex, octal, binary, underscores,
 * L/f/d suffixes, exponent forms).
 */
#ifndef SLEELACTL_JAVA_LEX_H
#define SLEELACTL_JAVA_LEX_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    JT_EOF = 0,
    JT_IDENT,      /* identifiers and keywords (the parser tells them apart) */
    JT_INT,        /* int literal   : ival holds the value (already range-checked) */
    JT_LONG,       /* long literal  : ival holds the value */
    JT_FLOAT,      /* float literal (suffix f/F) */
    JT_DOUBLE,     /* double literal: text holds a decimal spelling SLeeLa accepts */
    JT_STRING,     /* string literal: text holds the DECODED value (UTF-8) */
    JT_CHAR,       /* char literal  : ival holds the UTF-16 code unit */
    JT_TEXTBLOCK,  /* text block    : text holds the decoded value */
    JT_OP          /* operator / separator: text holds the spelling */
} jt_kind;

typedef struct jtoken {
    jt_kind kind;
    char   *text;
    long long ival;
    int     line, col;
} jtoken;

typedef struct jtokens {
    jtoken *v;
    size_t  n, cap;
} jtokens;

/* 0 on success. On failure err holds "line:col: message" and -1 is returned. */
int  jlex(const char *src, size_t len, jtokens *out, char *err, size_t errlen);
void jtokens_free(jtokens *t);

#ifdef __cplusplus
}
#endif
#endif
