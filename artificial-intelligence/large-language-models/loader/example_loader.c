#include "example_loader.h"
#include <string.h>

const char *sllme_status_string(sllme_status s){
 switch(s){
  case SLLME_OK: return "ok";
  case SLLME_INVALID_ARGUMENT: return "invalid argument";
  case SLLME_WIDTH_MISMATCH: return "scratch too small for handler widths";
  case SLLME_HANDLER_ERROR: return "handler reported an unrecoverable error";
  case SLLME_EMPTY: return "no items / empty input";
 }
 return "unknown";
}

sllme_status sllme_validate(const sllme_handler *h, const sllme_scratch *s){
 if(!h || !s) return SLLME_INVALID_ARGUMENT;
 if(h->abi_version!=SLLME_ABI_VERSION) return SLLME_INVALID_ARGUMENT;
 if(!h->encode) return SLLME_INVALID_ARGUMENT;
 if(h->feature_width==0 || h->target_width==0) return SLLME_INVALID_ARGUMENT;
 if(!s->features || !s->target) return SLLME_INVALID_ARGUMENT;
 if(s->feature_cap < h->feature_width || s->target_cap < h->target_width)
  return SLLME_WIDTH_MISMATCH;
 return SLLME_OK;
}

/* Encode one item into scratch. On SLLME_ITEM_OK the scratch slices hold a
   fully-written example. The framework zero-fills the slices first so a handler
   that writes a sparse encoding (e.g. one-hot) need only set the nonzero slots,
   and a partial write can never leak stale values. */
static sllme_item_result encode_into(const sllme_handler *h, sllme_scratch *s,
                                     const void *item){
 memset(s->features, 0, h->feature_width*sizeof(double));
 memset(s->target,   0, h->target_width  *sizeof(double));
 return h->encode(h->user_data, item,
                  s->features, h->feature_width,
                  s->target,   h->target_width);
}

sllme_status sllme_load_one(const sllme_handler *h, sllme_scratch *s,
                            const void *item,
                            sllme_sink_fn sink, void *sink_ud,
                            sllme_item_result *result){
 sllme_status v=sllme_validate(h,s);
 if(v!=SLLME_OK) return v;
 if(result) *result=SLLME_ITEM_SKIP;
 sllme_item_result r=encode_into(h,s,item);
 if(result) *result=r;
 if(r==SLLME_ITEM_ERROR) return SLLME_HANDLER_ERROR;
 if(r==SLLME_ITEM_OK && sink){
  (void)sink(sink_ud, s->features, h->feature_width, s->target, h->target_width);
 }
 return SLLME_OK;
}

sllme_status sllme_load_array(const sllme_handler *h, sllme_scratch *s,
                              const void *items, size_t count, size_t stride,
                              sllme_sink_fn sink, void *sink_ud,
                              sllme_report *report){
 if(report){ report->seen=0; report->loaded=0; report->skipped=0; }
 sllme_status v=sllme_validate(h,s);
 if(v!=SLLME_OK) return v;
 if(!items || count==0) return SLLME_EMPTY;
 if(stride==0) return SLLME_INVALID_ARGUMENT;

 const unsigned char *base=(const unsigned char*)items;
 for(size_t i=0;i<count;i++){
  const void *item=base + i*stride;
  if(report) report->seen++;
  sllme_item_result r=encode_into(h,s,item);
  if(r==SLLME_ITEM_ERROR) return SLLME_HANDLER_ERROR;
  if(r==SLLME_ITEM_SKIP){ if(report) report->skipped++; continue; }
  /* SLLME_ITEM_OK */
  if(report) report->loaded++;
  if(sink){
   if(sink(sink_ud, s->features, h->feature_width, s->target, h->target_width)!=0)
    break; /* sink requested stop; honest early return with counts so far */
  }
 }
 return SLLME_OK;
}
