#include "slvm.h"
#include <string.h>
static int push(slvm_t*v,slvm_word_t x){if(v->stack_size>=v->stack_capacity)return 0;v->stack[v->stack_size++]=x;return 1;}
static int pop(slvm_t*v,slvm_word_t*x){if(!v->stack_size)return 0;*x=v->stack[--v->stack_size];return 1;}
static int u64(const slvm_t*v,slvm_pc_t p,slvm_word_t*x){if(p+8>v->code_size)return 0;memcpy(x,v->code+p,8);return 1;}
static int i32(const slvm_t*v,slvm_pc_t p,int32_t*x){if(p+4>v->code_size)return 0;memcpy(x,v->code+p,4);return 1;}
void slvm_init(slvm_t*v,const uint8_t*c,size_t n,slvm_word_t*s,size_t cap){if(!v)return;v->code=c;v->code_size=n;v->stack=s;v->stack_capacity=cap;v->stack_size=0;v->pc=0;v->halted=0;}
slvm_status_t slvm_step(slvm_t*v){
 if(!v||!v->code||v->pc>=v->code_size)return SLVM_ERROR;if(v->halted)return SLVM_HALTED;
 uint8_t op=v->code[v->pc++];
 switch((slvm_opcode_t)op){
 case SLVM_OP_NOP:return SLVM_OK;
 case SLVM_OP_HALT:v->halted=1;return SLVM_HALTED;
 case SLVM_OP_CONST:{slvm_word_t x;if(!u64(v,v->pc,&x))return SLVM_ERROR;v->pc+=8;return push(v,x)?SLVM_OK:SLVM_STACK_OVERFLOW;}
 case SLVM_OP_POP:{slvm_word_t x;return pop(v,&x)?SLVM_OK:SLVM_STACK_UNDERFLOW;}
 case SLVM_OP_DUP:if(!v->stack_size)return SLVM_STACK_UNDERFLOW;return push(v,v->stack[v->stack_size-1])?SLVM_OK:SLVM_STACK_OVERFLOW;
 case SLVM_OP_ADD:case SLVM_OP_SUB:case SLVM_OP_MUL:case SLVM_OP_DIV:{slvm_word_t b,a;if(!pop(v,&b)||!pop(v,&a))return SLVM_STACK_UNDERFLOW;if(op==SLVM_OP_ADD)a+=b;else if(op==SLVM_OP_SUB)a-=b;else if(op==SLVM_OP_MUL)a*=b;else{if(!b)return SLVM_ERROR;a/=b;}return push(v,a)?SLVM_OK:SLVM_STACK_OVERFLOW;}
 case SLVM_OP_NEG:{slvm_word_t x;if(!pop(v,&x))return SLVM_STACK_UNDERFLOW;return push(v,(slvm_word_t)(-(int64_t)x))?SLVM_OK:SLVM_STACK_OVERFLOW;}
 case SLVM_OP_EQ:case SLVM_OP_LT:case SLVM_OP_GT:{slvm_word_t b,a;if(!pop(v,&b)||!pop(v,&a))return SLVM_STACK_UNDERFLOW;slvm_word_t r=op==SLVM_OP_EQ?(a==b):op==SLVM_OP_LT?(a<b):(a>b);return push(v,r)?SLVM_OK:SLVM_STACK_OVERFLOW;}
 case SLVM_OP_JUMP:case SLVM_OP_JUMP_IF_FALSE:{int32_t off;if(!i32(v,v->pc,&off))return SLVM_ERROR;v->pc+=4;if(op==SLVM_OP_JUMP_IF_FALSE){slvm_word_t c;if(!pop(v,&c))return SLVM_STACK_UNDERFLOW;if(c)return SLVM_OK;}if(off<0&&(uint64_t)(-off)>v->pc)return SLVM_ERROR;if(off>0&&(uint64_t)off>v->code_size-v->pc)return SLVM_ERROR;v->pc=(slvm_pc_t)((int64_t)v->pc+off);return SLVM_OK;}
 case SLVM_OP_RETURN:v->halted=1;return SLVM_HALTED;
 default:return SLVM_INVALID_OPCODE;
 }}
slvm_status_t slvm_run(slvm_t*v){if(!v)return SLVM_ERROR;for(;;){slvm_status_t s=slvm_step(v);if(s!=SLVM_OK)return s;}}
