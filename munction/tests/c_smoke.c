#include "../c/munction.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
typedef struct { unsigned char data[256]; size_t len, pos; int latched, closed; } mem;
static int open_(void*c,const char*a){(void)c;return a&&*a?0:-1;}
static int64_t send_(void*c,const uint8_t*d,size_t n){mem*m=c;if(n>sizeof m->data)return -1;memcpy(m->data,d,n);m->len=n;m->pos=0;return (int64_t)n;}
static int64_t consume_(void*c,uint8_t*out,size_t cap,uint64_t*seq){mem*m=c;if(m->pos>=m->len)return -1;size_t n=m->len-m->pos;if(n>cap)n=cap;memcpy(out,m->data+m->pos,n);m->pos+=n;if(seq)*seq=1;return (int64_t)n;}
static int thatch_(void*c,const char*s){(void)c;return s&&*s?0:-1;}
static int observe_(void*c,char*out,size_t cap){mem*m=c;return snprintf(out,cap,"memory:len=%zu",m->len);}
static int latch_(void*c){((mem*)c)->latched=1;return 0;}
static int close_(void*c){((mem*)c)->closed=1;return 0;}
static void destroy_(void*c){(void)c;}
int main(void){
 mem state={0};
 sleela_munction_channel ch={"memory",open_,send_,consume_,thatch_,observe_,latch_,close_,destroy_,&state};
 sleela_munction*m=sleela_munction_start("c-smoke");
 assert(m);assert(sleela_munction_connect(m,&ch,"local")==0);
 assert(sleela_munction_send(m,(const uint8_t*)"hello",5)==5);
 uint8_t out[16];assert(sleela_munction_consume(m,out,sizeof out)==5);assert(memcmp(out,"hello",5)==0);
 assert(sleela_munction_latch(m)==0);assert(sleela_munction_call_count(m)==5);
 char receipt[2048];assert(sleela_munction_close(m,receipt,sizeof receipt)==SLEELA_MUNCTION_REACHED);
 assert(state.latched&&state.closed);assert(strstr(receipt,"coherent=true"));
 puts("munction C smoke: PASS");return 0;
}
