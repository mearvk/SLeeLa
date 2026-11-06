/* Self-test for the base-vocabulary loader. Builds standalone:
 *   cc -std=c11 -O2 -Wall -Wextra -pedantic \
 *      ../large_language_models_vocabulary.c \
 *      ../large_language_models_base_vocabulary.c \
 *      base_vocabulary_selftest.c -o base_vocabulary_selftest
 * Returns 0 on success; prints the first failing check otherwise. */
#include "../large_language_models_base_vocabulary.h"
#include <stdio.h>
#include <string.h>

static int fail(const char *msg){ fprintf(stderr, "selftest: FAIL: %s\n", msg); return 1; }

int main(void){
 sllmv_vocabulary *v=NULL;
 if(sllmv_vocabulary_create(&v)!=SLLMV_OK || !v) return fail("create");

 sllmbv_specials sp;
 sllmbv_status st = sllmbv_load_default(v, &sp);
 if(st!=SLLMBV_OK){ fprintf(stderr,"load: %s\n", sllmbv_status_string(st)); return fail("load_default"); }

 /* Reserved special IDs land at the fixed low positions. */
 if(sllmbv_special_id(&sp, SLLMBV_ROLE_PAD) != 0) return fail("pad != 0");
 if(sllmbv_special_id(&sp, SLLMBV_ROLE_UNK) != 1) return fail("unk != 1");
 if(sllmbv_special_id(&sp, SLLMBV_ROLE_BOS) != 2) return fail("bos != 2");
 if(sllmbv_special_id(&sp, SLLMBV_ROLE_EOS) != 3) return fail("eos != 3");

 /* Round-trip: id -> token -> id. */
 const char *tok=NULL;
 if(sllmv_vocabulary_token(v, 0, &tok)!=SLLMV_OK || strcmp(tok,"<pad>")!=0) return fail("token(0) != <pad>");

 uint32_t id=0;
 if(sllmv_vocabulary_find(v, "the", &id)!=SLLMV_OK) return fail("find('the')");
 const char *back=NULL;
 if(sllmv_vocabulary_token(v, id, &back)!=SLLMV_OK || strcmp(back,"the")!=0) return fail("round-trip 'the'");

 if(sllmv_vocabulary_find(v, "computer", &id)!=SLLMV_OK) return fail("find('computer')");

 /* Encode a small line through the seeded whitespace tokenizer. */
 uint32_t *ids=NULL; size_t n=0;
 if(sllmv_vocabulary_encode(v, "the computer is a machine", &ids, &n)!=SLLMV_OK) return fail("encode");
 if(n!=5){ sllmv_vocabulary_free_ids(ids); return fail("encode count != 5"); }
 sllmv_vocabulary_free_ids(ids);

 /* Loading into a non-empty vocabulary must fail closed. */
 st = sllmbv_load_default(v, &sp);
 if(st==SLLMBV_OK) return fail("second load should fail (non-empty)");

 sllmv_vocabulary_destroy(v);
 printf("selftest: OK — base '%s' seeded; specials fixed; round-trip + encode pass\n",
        sllmbv_default_vocabulary_id());
 return 0;
}
