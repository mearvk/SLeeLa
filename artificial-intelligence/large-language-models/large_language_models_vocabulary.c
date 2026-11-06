#include "large_language_models_vocabulary.h"
#include <stdlib.h>
#include <string.h>
struct sllmv_vocabulary { char **tokens; size_t count, token_cap; uint32_t *slots; size_t slot_cap; };
static uint64_t hash_token(const char *s){uint64_t h=1469598103934665603ULL;while(*s){h^=(unsigned char)*s++;h*=1099511628211ULL;}return h;}
static size_t locate(const sllmv_vocabulary *v,const char *s,int *found){
 size_t mask=v->slot_cap-1,pos=(size_t)hash_token(s)&mask;
 while(v->slots[pos]){uint32_t id=v->slots[pos]-1;if(strcmp(v->tokens[id],s)==0){*found=1;return pos;}pos=(pos+1)&mask;}
 *found=0;return pos;
}
static int rehash(sllmv_vocabulary *v,size_t cap){
 uint32_t *slots=(uint32_t*)calloc(cap,sizeof(*slots));if(!slots)return 0;
 size_t old=v->slot_cap;uint32_t *prev=v->slots;v->slots=slots;v->slot_cap=cap;
 for(size_t i=0;i<v->count;i++){int f;size_t p=locate(v,v->tokens[i],&f);v->slots[p]=(uint32_t)i+1;}
 (void)old;free(prev);return 1;
}
sllmv_status sllmv_vocabulary_create(sllmv_vocabulary **out){
 if(!out)return SLLMV_INVALID_ARGUMENT;*out=NULL;
 sllmv_vocabulary *v=(sllmv_vocabulary*)calloc(1,sizeof(*v));if(!v)return SLLMV_OUT_OF_MEMORY;
 v->slot_cap=16;v->slots=(uint32_t*)calloc(v->slot_cap,sizeof(*v->slots));
 if(!v->slots){free(v);return SLLMV_OUT_OF_MEMORY;}*out=v;return SLLMV_OK;
}
void sllmv_vocabulary_destroy(sllmv_vocabulary *v){if(!v)return;for(size_t i=0;i<v->count;i++)free(v->tokens[i]);free(v->tokens);free(v->slots);free(v);}
size_t sllmv_vocabulary_size(const sllmv_vocabulary *v){return v?v->count:0;}
sllmv_status sllmv_vocabulary_find(const sllmv_vocabulary *v,const char *s,uint32_t *id){
 if(!v||!s||!*s||!id)return SLLMV_INVALID_ARGUMENT;int f;size_t p=locate(v,s,&f);if(!f)return SLLMV_NOT_FOUND;*id=v->slots[p]-1;return SLLMV_OK;
}
sllmv_status sllmv_vocabulary_add(sllmv_vocabulary *v,const char *s,uint32_t *id){
 if(!v||!s||!*s||!id)return SLLMV_INVALID_ARGUMENT;
 sllmv_status st=sllmv_vocabulary_find(v,s,id);if(st==SLLMV_OK)return st;
 if(v->count>=UINT32_MAX)return SLLMV_OUT_OF_MEMORY;
 if((v->count+1)*10>=v->slot_cap*7&&!rehash(v,v->slot_cap*2))return SLLMV_OUT_OF_MEMORY;
 if(v->count==v->token_cap){size_t nc=v->token_cap?v->token_cap*2:16;char **p=(char**)realloc(v->tokens,nc*sizeof(*p));if(!p)return SLLMV_OUT_OF_MEMORY;v->tokens=p;v->token_cap=nc;}
 size_t n=strlen(s);char *copy=(char*)malloc(n+1);if(!copy)return SLLMV_OUT_OF_MEMORY;memcpy(copy,s,n+1);
 uint32_t next=(uint32_t)v->count;v->tokens[v->count++]=copy;int f;size_t pos=locate(v,s,&f);v->slots[pos]=next+1;*id=next;return SLLMV_OK;
}
sllmv_status sllmv_vocabulary_token(const sllmv_vocabulary *v,uint32_t id,const char **out){
 if(!v||!out)return SLLMV_INVALID_ARGUMENT;if((size_t)id>=v->count)return SLLMV_NOT_FOUND;*out=v->tokens[id];return SLLMV_OK;
}
sllmv_status sllmv_vocabulary_encode(const sllmv_vocabulary *v,const char *text,uint32_t **out,size_t *count){
 if(!v||!text||!out||!count)return SLLMV_INVALID_ARGUMENT;*out=NULL;*count=0;
 size_t cap=0,n=0;uint32_t *ids=NULL;const char *p=text;
 while(*p){while(*p==' '||*p=='\\t'||*p=='\\r'||*p=='\\n')p++;if(!*p)break;const char *start=p;while(*p&&*p!=' '&&*p!='\\t'&&*p!='\\r'&&*p!='\\n')p++;
 size_t len=(size_t)(p-start);char *word=(char*)malloc(len+1);if(!word){free(ids);return SLLMV_OUT_OF_MEMORY;}memcpy(word,start,len);word[len]=0;
 uint32_t id;sllmv_status st=sllmv_vocabulary_find(v,word,&id);free(word);if(st!=SLLMV_OK){free(ids);return st;}
 if(n==cap){size_t nc=cap?cap*2:16;uint32_t *q=(uint32_t*)realloc(ids,nc*sizeof(*q));if(!q){free(ids);return SLLMV_OUT_OF_MEMORY;}ids=q;cap=nc;}ids[n++]=id;
 }
 *out=ids;*count=n;return SLLMV_OK;
}
void sllmv_vocabulary_free_ids(uint32_t *ids){free(ids);}
