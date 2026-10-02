#include "slvm.h"
#include <string.h>
#include <stdint.h>
#include "slvm_security.h"
#include "slvm_io_heuristic.h"
static int push(slvm_t*v,slvm_word_t x){if(v->stack_size>=v->stack_capacity)return 0;v->stack[v->stack_size++]=x;return 1;}
static int pop(slvm_t*v,slvm_word_t*x){if(!v->stack_size)return 0;*x=v->stack[--v->stack_size];return 1;}
static int u64(const slvm_t*v,slvm_pc_t p,slvm_word_t*x){if(p+8>v->code_size)return 0;memcpy(x,v->code+p,8);return 1;}
static int i32(const slvm_t*v,slvm_pc_t p,int32_t*x){if(p+4>v->code_size)return 0;memcpy(x,v->code+p,4);return 1;}
void slvm_init(slvm_t*v,const uint8_t*c,size_t n,slvm_word_t*s,size_t cap){if(!v)return;v->code=c;v->code_size=n;v->stack=s;v->stack_capacity=cap;v->stack_size=0;v->pc=0;v->halted=0;slvm_security_init(&v->security);slvm_io_heuristic_init(&v->io_heuristic);gc_init(&v->gc,1024ULL*1024ULL);v->memory_limit=SLVM_DEFAULT_MEMORY_LIMIT;slvm_memory_security_init(&v->memory_security,v->memory_limit);}
void slvm_set_memory_limit(slvm_t*v,size_t b){if(!v)return;v->memory_limit=b?b:SLVM_DEFAULT_MEMORY_LIMIT;slvm_memory_security_set_ceiling(&v->memory_security,v->memory_limit);}
size_t slvm_get_memory_limit(const slvm_t*v){return v?v->memory_limit:0;}
SLGCObject*slvm_gc_allocate(slvm_t*v,size_t b,SLGCMarkFn m,SLGCDestroyFn d,void*c){if(!v)return NULL;size_t u=gc_bytes(&v->gc);if(b>v->memory_limit||u>v->memory_limit-b)return NULL;if(!slvm_security_observe_resource(&v->security,1))return NULL;slvm_memory_decision_t md=slvm_memory_security_check(&v->memory_security,b,1);if(md==SLVM_MEMORY_DENY||md==SLVM_MEMORY_THROTTLE)return NULL;SLGCObject*o=gc_allocate(&v->gc,b,m,d,c);if(!o)slvm_memory_security_record_free(&v->memory_security,b);return o;}
size_t slvm_gc_collect(slvm_t*v){if(!v)return 0;size_t reclaimed=gc_collect(&v->gc);if(reclaimed)slvm_memory_security_record_free(&v->memory_security,reclaimed);return reclaimed;}
size_t slvm_gc_bytes(const slvm_t*v){return v?gc_bytes(&v->gc):0;}
int slvm_security_allow_io(slvm_t*v,slvm_io_kind_t k,size_t b){if(!v)return 0;if(!slvm_security_observe_io(&v->security,k,b))return 0;slvm_io_decision_t d=slvm_io_heuristic_observe(&v->io_heuristic,k,b,(uint32_t)v->security.window_requests,slvm_security_state(&v->security));return d!=SLVM_IO_HEURISTIC_BLOCK;}
void slvm_security_reset(slvm_t*v){if(v)slvm_security_reset_window(&v->security);}
slvm_status_t slvm_step(slvm_t*v){
 if(!v||!v->code||v->pc>=v->code_size)return SLVM_ERROR;if(v->halted)return SLVM_HALTED;if(!slvm_security_observe_instruction(&v->security))return SLVM_ERROR;
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

int slvm_security_allow_io(slvm_t *vm, slvm_io_kind_t kind, size_t bytes) {
    if (!vm) return 0;
    if (!slvm_security_observe_io(&vm->security, kind, bytes)) return 0;
    const slvm_io_decision_t decision = slvm_io_heuristic_observe(
        &vm->io_heuristic, kind, bytes,
        (uint32_t)vm->security.window_requests,
        slvm_security_state(&vm->security));
    return decision != SLVM_IO_HEURISTIC_BLOCK;
}

void slvm_security_reset(slvm_t *vm) {
    if (!vm) return;
    slvm_security_reset_window(&vm->security);
}
