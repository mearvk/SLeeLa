#include "../include/sleela_regex.h"
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct sleela_regex { regex_t native; int captures; };
static void clear_error(sleela_regex_error *e){if(e){e->code=0;e->position=0;e->message[0]='\0';}}
static void set_error(sleela_regex_error *e,int code,const char *msg){if(e){e->code=code;e->position=0;snprintf(e->message,sizeof(e->message),"%s",msg?msg:"regex error");}}
int sleela_regex_compile(sleela_regex **out,const char *pattern,unsigned flags,sleela_regex_error *error){
 if(!out||!pattern){set_error(error,1,"null output or pattern");return 1;}
 clear_error(error);
 *out=NULL;
 sleela_regex *r=(sleela_regex*)calloc(1,sizeof(*r));
 if(!r){set_error(error,2,"allocation failure");return 2;}
 int f=REG_EXTENDED;if(flags&SLEELA_REGEX_ICASE)f|=REG_ICASE;if(flags&SLEELA_REGEX_NEWLINE)f|=REG_NEWLINE;if(flags&SLEELA_REGEX_NOSUB)f|=REG_NOSUB;
 int rc=regcomp(&r->native,pattern,f);if(rc){char buf[256];regerror(rc,&r->native,buf,sizeof(buf));set_error(error,rc,buf);free(r);return rc;}
 r->captures=(int)r->native.re_nsub;*out=r;return 0;
}
void sleela_regex_free(sleela_regex *r){if(r){regfree(&r->native);free(r);}}
static int exec_span(const sleela_regex *r,const char *text,int eflags,size_t index,sleela_regex_span *s){
 if(!r||!text||!s)return 1;
 regmatch_t *m=(regmatch_t*)calloc((size_t)r->captures+1,sizeof(*m));
 if(!m)return 2;
 int rc=regexec(&r->native,text,(size_t)r->captures+1,m,eflags);
 if(rc==0&&index<=(size_t)r->captures&&m[index].rm_so>=0){s->start=(size_t)m[index].rm_so;s->end=(size_t)m[index].rm_eo;s->matched=1;}else{s->start=s->end=0;s->matched=0;}free(m);return rc;
}
int sleela_regex_full_match(const sleela_regex *r,const char *text,sleela_regex_span *w){if(!r||!text||!w)return 1;size_t n=strlen(text);int rc=exec_span(r,text,0,0,w);return(rc==0&&w->start==0&&w->end==n)?0:1;}
int sleela_regex_search(const sleela_regex *r,const char *text,sleela_regex_span *w){return exec_span(r,text,0,0,w)==0?0:1;}
int sleela_regex_capture_count(const sleela_regex *r){return r?r->captures:0;}
int sleela_regex_capture(const sleela_regex *r,const char *text,size_t capture,sleela_regex_span *s){return exec_span(r,text,0,capture,s)==0?0:1;}
int sleela_regex_replace_first(const sleela_regex *r,const char *text,const char *replacement,char *out,size_t out_size){
 if(!r||!text||!replacement||!out||!out_size)return 1;
 sleela_regex_span s;
 if(sleela_regex_search(r,text,&s)!=0){size_t n=strlen(text);if(n+1>out_size)return 2;memcpy(out,text,n+1);return 0;}
 size_t a=s.start,b=s.end,tn=strlen(text),rn=strlen(replacement);if(a+rn+(tn-b)+1>out_size)return 2;memcpy(out,text,a);memcpy(out+a,replacement,rn);memcpy(out+a+rn,text+b,tn-b+1);return 0;
}
size_t sleela_regex_escape(const char *literal,char *out,size_t out_size){
 static const char meta[]=".^$|()[]*+?{}\\";
 size_t need=0,i;if(!literal)return 0;for(i=0;literal[i];++i)need+=strchr(meta,literal[i])?2:1;
 if(out&&out_size){size_t p=0;for(i=0;literal[i]&&p+1<out_size;++i){if(strchr(meta,literal[i])){if(p+2>=out_size)break;out[p++]='\\';}out[p++]=literal[i];}out[p]='\0';}return need;
}
