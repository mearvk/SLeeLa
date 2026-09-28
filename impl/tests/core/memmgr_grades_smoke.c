#include "sleela_memmgr_grade2.h"
#include "sleela_memmgr_grade3.h"
#include <assert.h>
#include <stdio.h>
int main(void){
    SLMMGrade2 g2; unsigned a,b,c;
    assert(slmm_g2_init(&g2,300)==0);
    assert(slmm_g2_spawn(&g2,1,150,&a)==0);
    assert(slmm_g2_spawn(&g2,2,100,&b)==0);
    assert(slmm_g2_spawn(&g2,3,100,&c)==0);
    assert(slmm_g2_reserve(&g2,a,100)==0);
    assert(slmm_g2_reserve(&g2,b,100)==0);
    assert(slmm_g2_reserve(&g2,c,101)<0);
    assert(slmm_g2_validate(&g2));
    slmm_g2_release(&g2,b,100);
    slmm_g2_close(&g2,c);
    assert(slmm_g2_validate(&g2));

    SLMMGrade3 g3;
    assert(slmm_g3_init(&g3,4096)==0);
    assert(slmm_g3_begin(&g3,SLMM_G3_MEMORY,1024,0)==0);
    assert(slmm_g3_begin(&g3,SLMM_G3_FILE,256,2)==0);
    assert(slmm_g3_begin(&g3,SLMM_G3_SOCKET,512,1)==0);
    assert(slmm_g3_validate(&g3));
    assert(slmm_g3_begin(&g3,SLMM_G3_PROCESS,4096,1)<0);
    slmm_g3_end(&g3,SLMM_G3_FILE,256,2);
    slmm_g3_end(&g3,SLMM_G3_SOCKET,512,1);
    slmm_g3_end(&g3,SLMM_G3_MEMORY,1024,0);
    assert(slmm_g3_validate(&g3));
    printf("memory-manager grades II/III: PASS\n");
    return 0;
}
