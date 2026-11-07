#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
static bool get(const int *a,size_t n,int i,int *v){if(i<0||(size_t)i>=n||!a||!v)return false;*v=a[i];return true;}
static bool cont(unsigned char b){return b>=0x80&&b<=0xBF;}
static bool valid(const unsigned char*s,size_t n){
 size_t i=0;while(i<n){unsigned char a=s[i],b=0;
 if(a<=0x7F){i++;continue;}
 if(a>=0xC2&&a<=0xDF){if(i+1>=n||!cont(s[i+1]))return false;i+=2;continue;}
 if(a>=0xE0&&a<=0xEF){if(i+2>=n||!cont(s[i+2]))return false;b=s[i+1];if(a==0xE0&&(b<0xA0||b>0xBF))return false;if(a==0xED&&(b<0x80||b>0x9F))return false;if(a!=0xE0&&a!=0xED&&!cont(b))return false;i+=3;continue;}
 if(a>=0xF0&&a<=0xF4){if(i+3>=n||!cont(s[i+2])||!cont(s[i+3]))return false;b=s[i+1];if(a==0xF0&&(b<0x90||b>0xBF))return false;if(a==0xF4&&(b<0x80||b>0x8F))return false;if(a!=0xF0&&a!=0xF4&&!cont(b))return false;i+=4;continue;}return false;}return true;
}
static size_t cps(const unsigned char*s,size_t n){size_t c=0;for(size_t i=0;i<n;){unsigned char b=s[i];i+=b<0x80?1:(b<0xE0?2:(b<0xF0?3:4));c++;}return c;}
int main(void){int a[]={11,22,33},one[]={7},v=0,first=0,last=0;const unsigned char s[]={'A',0xC3,0xA9,0xF0,0x9F,0x98,0x80},bad[]={0xC0,0xAF};size_t n=sizeof s;
get(a,3,0,&first);get(a,3,2,&last);printf("EMPTY_LENGTH=0\nSINGLE_LENGTH=%zu\nFIRST_VALUE=%d\nLAST_VALUE=%d\n",sizeof(one)/sizeof(one[0]),first,last);
printf("NEGATIVE_INDEX=%s\nONE_PAST_END=%s\n",get(a,3,-1,&v)?"ACCEPTED":"REJECTED",get(a,3,3,&v)?"ACCEPTED":"REJECTED");
printf("UTF8_ROUNDTRIP=%s\nUTF8_INVALID=%s\nUTF8_BYTES=%zu\nUNICODE_CODEPOINTS=%zu\n",valid(s,n)&&memcmp(s,"A\xC3\xA9\xF0\x9F\x98\x80",n)==0?"PASS":"FAIL",valid(bad,sizeof bad)?"ACCEPTED":"REJECTED",n,cps(s,n));return 0;}
