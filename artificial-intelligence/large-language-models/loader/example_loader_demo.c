/*
 * example_loader_demo.c — the loader-handler idiom across words, maths, objects.
 *
 * Three tiny user handlers show the same idiom; each only implements encode().
 * The framework owns bounded, reused scratch and feeds a real slnn_* net.
 *
 *   1. WORD handler  — encodes a word against a KNOWN vocabulary as a one-hot
 *                      feature over a small id window; target is the next word's
 *                      one-hot. Unknown words are honestly SKIPPED (never faked).
 *   2. MATH handler  — encodes (a, b) -> a+b, normalized; a regression example.
 *   3. OBJECT handler— encodes a flat record of named numeric fields (the
 *                      schema-driven "object") -> feature vector + a label.
 *
 * Build:
 *   cc -std=c11 -O2 -Wall -Wextra -pedantic \
 *      example_loader.c example_loader_slnn.c \
 *      ../large_language_models_vocabulary.c ../large_language_models_base_vocabulary.c \
 *      ../../neural-nets/neural_nets.c example_loader_demo.c -o example_loader_demo -lm
 */
#include "example_loader_slnn.h"
#include "../large_language_models_base_vocabulary.h"
#include <stdio.h>
#include <string.h>

/* ---------- shared: a small id window so one-hot stays tiny and fast ---------- */
#define WINDOW 100  /* encode vocab ids [BASE, BASE+WINDOW) as one-hot */
#define BASE   35   /* first word id (function-words start at 35; core-words run to 134) */

/* ===== 1. WORD handler ===== */
typedef struct { const char *word; const char *next; } word_item;
typedef struct { sllmv_vocabulary *vocab; } word_ctx;

static int onehot_window(sllmv_vocabulary *v, const char *w, double *vec, size_t n){
 uint32_t id;
 if(sllmv_vocabulary_find(v,w,&id)!=SLLMV_OK) return 0;     /* unknown: caller skips */
 if(id<BASE || id>=BASE+n) return 0;                        /* outside window: skip */
 vec[id-BASE]=1.0; return 1;
}
static sllme_item_result word_encode(void *ud, const void *it,
                                     double *f, size_t fw, double *t, size_t tw){
 const word_item *wi=(const word_item*)it; word_ctx *c=(word_ctx*)ud;
 if(fw!=WINDOW || tw!=WINDOW) return SLLME_ITEM_ERROR;
 if(!onehot_window(c->vocab, wi->word, f, fw)) return SLLME_ITEM_SKIP; /* honest skip */
 if(!onehot_window(c->vocab, wi->next, t, tw)) return SLLME_ITEM_SKIP;
 return SLLME_ITEM_OK;
}

/* ===== 2. MATH handler: (a,b) -> normalized a+b ===== */
typedef struct { double a, b; } math_item;
static sllme_item_result math_encode(void *ud, const void *it,
                                     double *f, size_t fw, double *t, size_t tw){
 (void)ud; const math_item *m=(const math_item*)it;
 if(fw!=2 || tw!=1) return SLLME_ITEM_ERROR;
 f[0]=m->a/10.0; f[1]=m->b/10.0; t[0]=(m->a+m->b)/20.0;
 return SLLME_ITEM_OK;
}

/* ===== 3. OBJECT handler: schema-driven flat record ===== */
typedef struct { double politeness, skill, education; int label; } gentleman;
static sllme_item_result object_encode(void *ud, const void *it,
                                       double *f, size_t fw, double *t, size_t tw){
 (void)ud; const gentleman *g=(const gentleman*)it;
 if(fw!=3 || tw!=1) return SLLME_ITEM_ERROR;
 f[0]=g->politeness; f[1]=g->skill; f[2]=g->education;
 t[0]=(g->label!=0)?1.0:0.0;
 return SLLME_ITEM_OK;
}

static int fails=0;
static void ok(int c,const char*w){ printf("  %s %s\n", c?"ok:":"FAIL:", w); if(!c) fails++; }

static slnn_network *make_net(size_t in, size_t hidden, size_t out){
 size_t shape[3]={in,hidden,out};
 slnn_network_config cfg; cfg.abi_version=SLNN_ABI_VERSION; cfg.layer_count=2; cfg.layer_sizes=shape;
 cfg.hidden_activation=SLNN_ACTIVATION_TANH; cfg.output_activation=SLNN_ACTIVATION_SIGMOID; cfg.seed=7;
 slnn_network *n=NULL; slnn_network_create(&cfg,&n); return n;
}

int main(void){
 /* Known vocabulary (seed base set) — the words we encode against. */
 sllmv_vocabulary *vocab=NULL; sllmv_vocabulary_create(&vocab);
 sllmbv_load_default(vocab,NULL);

 /* ---- 1. WORDS -> train a real net against the known vocabulary ---- */
 word_ctx wc={vocab};
 sllme_handler wh={0}; wh.abi_version=SLLME_ABI_VERSION; wh.feature_width=WINDOW; wh.target_width=WINDOW;
 wh.encode=word_encode; wh.user_data=&wc; 
 word_item words[]={ {"the","computer"}, {"a","machine"}, {"the","program"},
                     {"zzzznotaword","the"} /* unknown -> skipped */ };
 double fbuf[WINDOW], tbuf[WINDOW];
 sllme_scratch sc={fbuf,tbuf,WINDOW,WINDOW};
 slnn_network *wnet=make_net(WINDOW,24,WINDOW);
 sllme_slnn_sink wsink; sllme_slnn_sink_init(&wsink,wnet,0.1);
 sllme_report wr;
 sllme_status ws=sllme_load_array(&wh,&sc, words, sizeof(words)/sizeof(words[0]), sizeof(words[0]),
                                  sllme_slnn_sink_train,&wsink,&wr);
 printf("== WORDS (encoded against known vocabulary, trained a real net) ==\n");
 printf(" status=%s seen=%zu loaded=%zu skipped=%zu trained=%zu errors=%zu\n",
        sllme_status_string(ws), wr.seen, wr.loaded, wr.skipped, wsink.trained, wsink.errors);
 ok(ws==SLLME_OK && wr.loaded==3 && wr.skipped==1 && wsink.trained==3, "words: 3 trained, 1 unknown skipped");

 /* ---- 2. MATHS ---- */
 sllme_handler mh={0}; mh.abi_version=SLLME_ABI_VERSION; mh.feature_width=2; mh.target_width=1; mh.encode=math_encode;
 math_item maths[]={ {1,2},{3,4},{5,5},{2,7} };
 double mf[2], mt[1]; sllme_scratch msc={mf,mt,2,1};
 slnn_network *mnet=make_net(2,8,1);
 sllme_slnn_sink msink; sllme_slnn_sink_init(&msink,mnet,0.2);
 sllme_report mr;
 sllme_status ms=sllme_load_array(&mh,&msc, maths,4,sizeof(math_item), sllme_slnn_sink_train,&msink,&mr);
 printf("== MATHS ((a,b)->a+b) ==\n status=%s loaded=%zu trained=%zu\n",
        sllme_status_string(ms), mr.loaded, msink.trained);
 ok(ms==SLLME_OK && mr.loaded==4 && msink.trained==4, "maths: 4 trained");

 /* ---- 3. OBJECTS (schema-driven flat record) ---- */
 sllme_handler oh={0}; oh.abi_version=SLLME_ABI_VERSION; oh.feature_width=3; oh.target_width=1; oh.encode=object_encode;
 gentleman objs[]={ {0.9,0.8,0.7,1}, {0.2,0.3,0.1,0}, {0.8,0.9,0.85,1} };
 double of[3], ot[1]; sllme_scratch osc={of,ot,3,1};
 slnn_network *onet=make_net(3,6,1);
 sllme_slnn_sink osink; sllme_slnn_sink_init(&osink,onet,0.2);
 sllme_report orr;
 sllme_status os=sllme_load_array(&oh,&osc, objs,3,sizeof(gentleman), sllme_slnn_sink_train,&osink,&orr);
 printf("== OBJECTS (schema-driven record) ==\n status=%s loaded=%zu trained=%zu\n",
        sllme_status_string(os), orr.loaded, osink.trained);
 ok(os==SLLME_OK && orr.loaded==3 && osink.trained==3, "objects: 3 trained");

 /* ---- fail-closed: scratch too small, bad handler ---- */
 printf("== careful fail-closed checks ==\n");
 sllme_scratch tiny={fbuf,tbuf,1,1};
 ok(sllme_validate(&wh,&tiny)==SLLME_WIDTH_MISMATCH, "scratch too small -> WIDTH_MISMATCH");
 sllme_handler bad={0}; bad.abi_version=SLLME_ABI_VERSION; bad.feature_width=2; bad.target_width=1; /* no encode */
 ok(sllme_validate(&bad,&msc)==SLLME_INVALID_ARGUMENT, "missing encode -> INVALID_ARGUMENT");

 slnn_network_destroy(wnet); slnn_network_destroy(mnet); slnn_network_destroy(onet);
 sllmv_vocabulary_destroy(vocab);
 if(fails){ fprintf(stderr,"example_loader_demo: %d FAIL\n",fails); return 1; }
 printf("\nexample_loader_demo: OK\n");
 return 0;
}
