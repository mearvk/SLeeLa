/* Self-test for the SLeeLa code verifier. Proves it catches each real gotcha
 * and passes clean SLeeLa. Build:
 *   cc -std=c11 -O2 -Wall -Wextra -pedantic \
 *      ../large_language_models.c ../large_language_models_reasoning.c \
 *      sleela_code_verifier.c sleela_verifier_selftest.c -o sleela_verifier_selftest
 */
#include "sleela_code_verifier.h"
#include <stdio.h>
#include <string.h>

static int fails=0;
static void expect_rule(const char* src, sllmsl_rule want, const char* what){
 sllmsl_finding f; int hit=sllmsl_check_snippet(src,NULL,&f);
 sllmsl_rule got = hit? f.rule : SLLMSL_OK;
 if(got!=want){ fprintf(stderr,"FAIL %s: wanted %s, got %s (%s)\n",
    what, sllmsl_rule_name(want), sllmsl_rule_name(got), f.message); fails++; }
 else printf("  ok: %-28s -> %s\n", what, sllmsl_rule_name(got));
}

int main(void){
 /* Reserved word as identifier. */
 expect_rule("void record(int x) { }", SLLMSL_RESERVED_IDENTIFIER, "method named record()");
 expect_rule("int enum = 3;", SLLMSL_RESERVED_IDENTIFIER, "variable named enum");

 /* close() runtime collision. */
 expect_rule("void close() { }", SLLMSL_CLOSE_COLLISION, "no-arg close()");

 /* Comment forms. */
 expect_rule("/* a block comment */\nclass X { }", SLLMSL_CSTYLE_COMMENT, "C-style comment");
 expect_rule("< this comment runs\n  onto a second line >", SLLMSL_MULTILINE_ANGLE, "multiline single '<'");
 expect_rule("int x = 3;   < inline note >", SLLMSL_INLINE_ANGLE, "inline trailing '<'");
 expect_rule("< the flavor's name >", SLLMSL_APOSTROPHE_COMMENT, "apostrophe in angle comment");

 /* Clean SLeeLa with proper block comment + metadata passes. */
 const char* clean =
  "<< a proper block comment\n   spanning two lines >>\n"
  "#sleela 3.0\n"
  "class Greeter {\n"
  "  // ---- Descriptive metadata (version 1.11 nested-friendly accessors) ----\n"
  "  String metaAuthor = \"SLeeLa project\";\n"
  "  void sealMoment() { }\n"
  "}\n";
 expect_rule(clean, SLLMSL_OK, "clean SLeeLa class");

 /* reserved-word checker recognizes keywords directly. */
 if(!sllmsl_is_reserved_word("record")||!sllmsl_is_reserved_word("native")||
    sllmsl_is_reserved_word("sealMoment")){ fprintf(stderr,"FAIL: reserved-word table\n"); fails++; }
 else printf("  ok: reserved-word table\n");

 if(fails){ fprintf(stderr,"sleela_verifier_selftest: %d FAIL\n",fails); return 1; }
 printf("sleela_verifier_selftest: OK\n");
 return 0;
}
