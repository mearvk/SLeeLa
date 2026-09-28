#include "../cpp/munction.hpp"
#include <cassert>
#include <cstring>
#include <iostream>
struct Mem { std::string data; bool latched=false, closed=false; };
static int open_(void*c,const char*a){(void)c;return a&&*a?0:-1;}
static int64_t send_(void*c,const uint8_t*d,size_t n){auto*m=(Mem*)c;m->data.assign((const char*)d,n);return (int64_t)n;}
static int64_t consume_(void*c,uint8_t*out,size_t cap,uint64_t*seq){auto*m=(Mem*)c;if(m->data.empty())return -1;size_t n=m->data.size();if(n>cap)n=cap;memcpy(out,m->data.data(),n);m->data.clear();if(seq)*seq=1;return (int64_t)n;}
static int thatch_(void*,const char*s){return s&&*s?0:-1;}
static int observe_(void*,char*out,size_t cap){return snprintf(out,cap,"memory");}
static int latch_(void*c){((Mem*)c)->latched=true;return 0;}
static int close_(void*c){((Mem*)c)->closed=true;return 0;}
static void destroy_(void*){}
int main(){
 Mem state;
 sleela::munction::Channel ch({ "memory",open_,send_,consume_,thatch_,observe_,latch_,close_,destroy_,&state });
 auto m=sleela::munction::Munction::start("cpp-smoke");
 m.connect(ch,"local").send("hello").thatch("identity").latch();
 auto receipt=m.closeWithReceipt();
 assert(receipt.reached());assert(state.latched&&state.closed);
 std::cout<<"munction C++ smoke: PASS\n";
}
