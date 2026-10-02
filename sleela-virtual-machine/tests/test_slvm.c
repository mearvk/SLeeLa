#include "slvm.h"
#include <assert.h>
int main(void){const uint8_t p[]={SLVM_OP_CONST,2,0,0,0,0,0,0,0,SLVM_OP_CONST,3,0,0,0,0,0,0,0,SLVM_OP_ADD,SLVM_OP_HALT};slvm_word_t s[16];slvm_t v;slvm_init(&v,p,sizeof(p),s,16);assert(slvm_run(&v)==SLVM_HALTED);assert(v.stack_size==1&&v.stack[0]==5);return 0;}
