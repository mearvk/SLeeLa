#include "sleela_utf16.h"
#include <stdio.h>
#include <string.h>
static int expect_length(const char *label,const char *s,size_t n,int64_t expected) {
    int64_t actual=-1; int rc=sleela_utf8_utf16_length(s,n,&actual);
    if(rc!=0||actual!=expected){fprintf(stderr,"%s: rc=%d expected=%lld actual=%lld\n",label,rc,(long long)expected,(long long)actual);return 1;} return 0;
}
static int expect_unit(const char *label,const char *s,size_t n,int64_t index,uint16_t expected) {
    uint16_t actual=0; int rc=sleela_utf8_utf16_char_at(s,n,index,&actual);
    if(rc!=0||actual!=expected){fprintf(stderr,"%s: rc=%d expected=U+%04X actual=U+%04X\n",label,rc,(unsigned)expected,(unsigned)actual);return 1;} return 0;
}
int main(void) {
    static const char sample[]="A\xc3\xa9\xf0\x9f\x98\x80Z";
    static const char malformed[]="\xc0\xaf";
    int failures=0; int64_t length=-1; uint16_t unit=0;
    failures+=expect_length("ASCII","Java",4,4);
    failures+=expect_length("empty","",0,0);
    failures+=expect_length("BMP plus supplementary",sample,sizeof(sample)-1,5);
    failures+=expect_unit("ASCII A",sample,sizeof(sample)-1,0,0x0041);
    failures+=expect_unit("BMP e-acute",sample,sizeof(sample)-1,1,0x00e9);
    failures+=expect_unit("high surrogate",sample,sizeof(sample)-1,2,0xd83d);
    failures+=expect_unit("low surrogate",sample,sizeof(sample)-1,3,0xde00);
    failures+=expect_unit("trailing Z",sample,sizeof(sample)-1,4,0x005a);
    if(sleela_utf8_utf16_length(malformed,sizeof(malformed)-1,&length)!=-2)++failures;
    if(sleela_utf8_utf16_char_at(sample,sizeof(sample)-1,5,&unit)!=-3)++failures;
    if(sleela_utf8_utf16_char_at(sample,sizeof(sample)-1,-1,&unit)!=-1)++failures;
    if(sleela_utf8_utf16_length(NULL,1,&length)!=-1)++failures;
    if(failures)return 1;
    puts("UTF16-PRIMITIVES=PASS"); puts("SUPPLEMENTARY-SURROGATES=PASS");
    puts("MALFORMED-UTF8=REJECTED"); return 0;
}
