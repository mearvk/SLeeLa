#ifndef SLEELA_REGEX_H
#define SLEELA_REGEX_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct sleela_regex sleela_regex;
typedef struct sleela_regex_span { size_t start; size_t end; int matched; } sleela_regex_span;
typedef struct sleela_regex_error { int code; size_t position; char message[256]; } sleela_regex_error;
enum { SLEELA_REGEX_ICASE=1u<<0, SLEELA_REGEX_NEWLINE=1u<<1, SLEELA_REGEX_NOSUB=1u<<2 };
int sleela_regex_compile(sleela_regex **out,const char *pattern,unsigned flags,sleela_regex_error *error);
void sleela_regex_free(sleela_regex *regex);
int sleela_regex_full_match(const sleela_regex *regex,const char *text,sleela_regex_span *whole);
int sleela_regex_search(const sleela_regex *regex,const char *text,sleela_regex_span *whole);
int sleela_regex_capture_count(const sleela_regex *regex);
int sleela_regex_capture(const sleela_regex *regex,const char *text,size_t capture,sleela_regex_span *span);
int sleela_regex_replace_first(const sleela_regex *regex,const char *text,const char *replacement,char *out,size_t out_size);
size_t sleela_regex_escape(const char *literal,char *out,size_t out_size);
#ifdef __cplusplus
}
#endif
#endif
