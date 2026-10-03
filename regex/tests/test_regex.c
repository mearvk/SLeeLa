#include "../include/sleela_regex.h"
#include <assert.h>
#include <string.h>
int main(void){
    sleela_regex *r=NULL; sleela_regex_error e;
    /* Anchored pattern: used for whole-string match and capture checks. */
    assert(sleela_regex_compile(&r,"^(hello|world)[0-9]+$",0,&e)==0);
    sleela_regex_span s;
    assert(sleela_regex_full_match(r,"hello42",&s)==0);
    assert(s.start==0&&s.end==7);
    /* One parenthesized group -> one capture (POSIX re_nsub). */
    assert(sleela_regex_capture_count(r)==1);
    assert(sleela_regex_capture(r,"hello42",1,&s)==0&&s.start==0&&s.end==5);
    char out[64];
    assert(sleela_regex_replace_first(r,"hello42","X",out,sizeof(out))==0);
    assert(strcmp(out,"X")==0);
    sleela_regex_free(r);

    /* Unanchored pattern: a mid-string search must find the substring. The
     * previous version searched with the ^...$ anchored pattern above, which
     * cannot match a substring, so this now uses an unanchored pattern. */
    r=NULL;
    assert(sleela_regex_compile(&r,"(hello|world)[0-9]+",0,&e)==0);
    assert(sleela_regex_search(r,"xx world7 yy",&s)==0);
    assert(s.start==3&&s.end==9);
    sleela_regex_free(r);

    char esc[64];
    assert(sleela_regex_escape("a+b.c",esc,sizeof(esc))==7&&strcmp(esc,"a\\+b\\.c")==0);

    assert(sleela_regex_compile(&r,"(",0,&e)!=0);
    return 0;
}
