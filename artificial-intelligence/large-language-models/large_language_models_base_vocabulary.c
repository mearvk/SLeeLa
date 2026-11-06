#include "large_language_models_base_vocabulary.h"
#include <string.h>

/* ---- Compiled-in default seed set: sleela-base-en-1.0 ----
   This mirrors tokenizers/base-vocabulary.json. The ordering here IS the
   contract: specials first (ascending from id 0), then ordinary tokens. The
   loader verifies, token by token, that the store hands back the same IDs. */

static const char *const kDefaultVocabularyId = "sleela-base-en-1.0";

static const char *const kSpecialTokens[] = {
 "<pad>", "<unk>", "<bos>", "<eos>", "<sep>", "<mask>", "<sys>", "<user>", "<asst>", "<tool>"
};
static const sllmbv_role kSpecialRoles[] = {
 SLLMBV_ROLE_PAD, SLLMBV_ROLE_UNK, SLLMBV_ROLE_BOS, SLLMBV_ROLE_EOS, SLLMBV_ROLE_SEP,
 SLLMBV_ROLE_MASK, SLLMBV_ROLE_SYSTEM, SLLMBV_ROLE_USER, SLLMBV_ROLE_ASSISTANT, SLLMBV_ROLE_TOOL
};
static const size_t kSpecialCount = sizeof(kSpecialTokens)/sizeof(kSpecialTokens[0]);

static const char *const kTokens[] = {
 "0","1","2","3","4","5","6","7","8","9",
 ".",",","?","!",";",":","-","(",")","\"","'",
 "+","=","/","%",
 "the","a","an","and","or","but","if","then","else","not","no","yes",
 "of","to","in","on","at","by","for","from","with","without","as",
 "is","are","was","were","be","been","being","has","have","had",
 "do","does","did","will","would","can","could","should","may","might","must",
 "this","that","these","those","it","its","they","them","we","you","i","he","she",
 "who","what","when","where","why","how","which",
 "all","any","some","each","every","more","most","than","very",
 "computer","machine","program","data","model","language","token","word","number","value",
 "system","process","function","code","source","text","input","output","train","learn",
 "answer","question","example","true","false","please","thank"
};
static const size_t kTokenCount = sizeof(kTokens)/sizeof(kTokens[0]);

const char *sllmbv_default_vocabulary_id(void){ return kDefaultVocabularyId; }

const char *sllmbv_status_string(sllmbv_status status){
 switch(status){
  case SLLMBV_OK: return "ok";
  case SLLMBV_INVALID_ARGUMENT: return "invalid argument";
  case SLLMBV_OUT_OF_MEMORY: return "out of memory";
  case SLLMBV_PARSE_ERROR: return "parse error";
  case SLLMBV_ID_MISMATCH: return "token did not land at its declared id";
  case SLLMBV_DUPLICATE_TOKEN: return "duplicate token in base set";
  case SLLMBV_ROLE_UNSET: return "special-token role not declared";
 }
 return "unknown status";
}

uint32_t sllmbv_special_id(const sllmbv_specials *specials, sllmbv_role role){
 if(!specials || role<0 || role>=SLLMBV_ROLE_COUNT) return SLLMBV_NO_ID;
 return specials->id[role];
}

static sllmbv_status map_vocab_status(sllmv_status st){
 switch(st){
  case SLLMV_OK: return SLLMBV_OK;
  case SLLMV_OUT_OF_MEMORY: return SLLMBV_OUT_OF_MEMORY;
  case SLLMV_INVALID_ARGUMENT: return SLLMBV_INVALID_ARGUMENT;
  default: return SLLMBV_PARSE_ERROR;
 }
}

sllmbv_status sllmbv_load_arrays(
 sllmv_vocabulary *vocabulary,
 const char *const *special_tokens, const sllmbv_role *special_roles, size_t special_count,
 const char *const *tokens, size_t token_count,
 sllmbv_specials *specials)
{
 if(!vocabulary) return SLLMBV_INVALID_ARGUMENT;
 if(special_count && (!special_tokens || !special_roles)) return SLLMBV_INVALID_ARGUMENT;
 if(token_count && !tokens) return SLLMBV_INVALID_ARGUMENT;
 /* The reserved-ID guarantee only holds if we seed into an empty store. */
 if(sllmv_vocabulary_size(vocabulary)!=0) return SLLMBV_ID_MISMATCH;

 sllmbv_specials resolved;
 for(size_t r=0;r<SLLMBV_ROLE_COUNT;r++) resolved.id[r]=SLLMBV_NO_ID;

 uint32_t expected=0;
 for(size_t i=0;i<special_count;i++,expected++){
  const char *tok=special_tokens[i];
  if(!tok || !*tok) return SLLMBV_INVALID_ARGUMENT;
  uint32_t id=0;
  /* A duplicate add returns the existing id instead of a fresh one. */
  uint32_t before=(uint32_t)sllmv_vocabulary_size(vocabulary);
  sllmv_status st=sllmv_vocabulary_add(vocabulary,tok,&id);
  if(st!=SLLMV_OK) return map_vocab_status(st);
  if(id!=expected) return SLLMBV_ID_MISMATCH;
  if((uint32_t)sllmv_vocabulary_size(vocabulary)!=before+1) return SLLMBV_DUPLICATE_TOKEN;
  sllmbv_role role=special_roles[i];
  if(role<0 || role>=SLLMBV_ROLE_COUNT) return SLLMBV_INVALID_ARGUMENT;
  resolved.id[role]=id;
 }
 for(size_t i=0;i<token_count;i++,expected++){
  const char *tok=tokens[i];
  if(!tok || !*tok) return SLLMBV_INVALID_ARGUMENT;
  uint32_t id=0;
  uint32_t before=(uint32_t)sllmv_vocabulary_size(vocabulary);
  sllmv_status st=sllmv_vocabulary_add(vocabulary,tok,&id);
  if(st!=SLLMV_OK) return map_vocab_status(st);
  if(id!=expected) return SLLMBV_ID_MISMATCH;
  if((uint32_t)sllmv_vocabulary_size(vocabulary)!=before+1) return SLLMBV_DUPLICATE_TOKEN;
 }
 if(specials) *specials=resolved;
 return SLLMBV_OK;
}

sllmbv_status sllmbv_load_default(sllmv_vocabulary *vocabulary, sllmbv_specials *specials){
 return sllmbv_load_arrays(vocabulary,
  kSpecialTokens, kSpecialRoles, kSpecialCount,
  kTokens, kTokenCount,
  specials);
}
