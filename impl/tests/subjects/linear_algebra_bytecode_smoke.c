#include "sleela_core.h"
#include "linear_algebra.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int emit_const_int(SLVM *vm, int64_t value) {
    int c=slvm_add_const_int(vm,value);
    return c>=0 && slvm_emit(vm,OP_CONST,c)>=0;
}
static int emit_const_double(SLVM *vm, double value) {
    int c=slvm_add_const_double(vm,value);
    return c>=0 && slvm_emit(vm,OP_CONST,c)>=0;
}
static int emit_array_set(SLVM *vm,int local,int index,double value) {
    return slvm_emit(vm,OP_LOADL,local)>=0 &&
           emit_const_int(vm,index) && emit_const_double(vm,value) &&
           slvm_emit0(vm,OP_ARRSET)>=0 && slvm_emit0(vm,OP_POP)>=0;
}
static int emit_math_frame(SLVM *vm,int a,int b,int out,int operation,
                           int scalar_result) {
    if(slvm_emit(vm,OP_LOADL,a)<0) return 0;
    if(b>=0) { if(slvm_emit(vm,OP_LOADL,b)<0) return 0; }
    else if(!emit_const_int(vm,-1)) return 0;
    if(out>=0) { if(slvm_emit(vm,OP_LOADL,out)<0) return 0; }
    else if(!emit_const_int(vm,-1)) return 0;
    if(!emit_const_int(vm,-1)) return 0; /* aux */
    const int dims[8]={3,1,b>=0?3:0,b>=0?1:0,scalar_result?1:3,1,0,0};
    for(int i=0;i<8;i++) if(!emit_const_int(vm,dims[i])) return 0;
    if(!emit_const_double(vm,0.0) || !emit_const_int(vm,0)) return 0;
    return slvm_emit(vm,OP_MATH_CALL,operation)>=0;
}

int main(void) {
    SLVM *vm=slvm_new();
    if(!vm) return 1;
    int fn=slvm_begin_func(vm,"main",0,3);
    int ok=fn>=0;
    if(ok) ok=emit_const_int(vm,3) && slvm_emit0(vm,OP_NEWARRAY)>=0 && slvm_emit(vm,OP_STOREL,0)>=0;
    if(ok) ok=emit_const_int(vm,3) && slvm_emit0(vm,OP_NEWARRAY)>=0 && slvm_emit(vm,OP_STOREL,1)>=0;
    if(ok) ok=emit_const_int(vm,3) && slvm_emit0(vm,OP_NEWARRAY)>=0 && slvm_emit(vm,OP_STOREL,2)>=0;
    if(ok) ok=emit_array_set(vm,0,0,1.0)&&emit_array_set(vm,0,1,2.0)&&emit_array_set(vm,0,2,3.0);
    if(ok) ok=emit_array_set(vm,1,0,4.0)&&emit_array_set(vm,1,1,5.0)&&emit_array_set(vm,1,2,6.0);
    if(ok) ok=emit_math_frame(vm,0,1,2,SLMATH_OP_VECTOR_ADD,0);
    if(ok) ok=slvm_emit0(vm,OP_POP)>=0;
    if(ok) ok=emit_math_frame(vm,2,0,-1,SLMATH_OP_VECTOR_DOT,1);
    if(ok) ok=slvm_emit0(vm,OP_RET)>=0;
    if(ok) { slvm_end_func(vm); slvm_set_entry(vm,fn); ok=slvm_run(vm)==SLR_OK; }
    SLValue result=slvm_result(vm);
    if(!ok || result.type!=SL_DOUBLE || result.as.d!=44.0) {
        fprintf(stderr,"math bytecode smoke: execution failed (%s)\n",slvm_error(vm)?slvm_error(vm):"wrong result");
        slvm_free(vm); return 1;
    }

    const char *artifact="/tmp/sleela-native-math-bytecode-smoke.sleela";
    if(slvm_save_file(vm,artifact)!=0) { fprintf(stderr,"math bytecode smoke: artifact save failed\n"); slvm_free(vm); return 1; }
    SLVM *loaded=slvm_load_file(artifact);
    unlink(artifact);
    if(!loaded || slvm_run(loaded)!=SLR_OK) {
        fprintf(stderr,"math bytecode smoke: artifact reload/execute failed\n");
        if(loaded) slvm_free(loaded);
        slvm_free(vm); return 1;
    }
    result=slvm_result(loaded);
    ok=result.type==SL_DOUBLE && result.as.d==44.0;
    slvm_free(loaded); slvm_free(vm);
    if(!ok) { fprintf(stderr,"math bytecode smoke: artifact result mismatch\n"); return 1; }
    puts("native math bytecode + artifact smoke: PASS");
    return 0;
}
