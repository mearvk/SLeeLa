#include "sleela_code_verifier.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Reserved keywords, from impl/frontend/lexer.cpp. Using one as an identifier
   (method/variable/parameter name) is a parse error. */
static const char *const kReserved[] = {
 "abstract","assert","boolean","break","case","catch","class","const","continue",
 "default","do","double","else","enum","extends","false","final","finally","for",
 "if","implements","import","instanceof","int","interface","long","native","new",
 "null","package","private","protected","public","record","return","sealed","static",
 "strictfp","struct","super","switch","synchronized","this","throw","throws",
 "transient","true","try","void","volatile","while","yield"
};
static const size_t kReservedCount = sizeof(kReserved)/sizeof(kReserved[0]);

/* Primitive/return types that precede a method/variable name, used to detect a
   "TYPE reserved (" pattern (reserved word as a method name). */
static const char *const kTypeLeads[] = {
 "void","int","long","double","boolean","String","float","char","byte","short"
};
static const size_t kTypeLeadsCount = sizeof(kTypeLeads)/sizeof(kTypeLeads[0]);

const char *sllmsl_rule_name(sllmsl_rule r){
 switch(r){
  case SLLMSL_OK: return "ok";
  case SLLMSL_RESERVED_IDENTIFIER: return "reserved-word-as-identifier";
  case SLLMSL_CLOSE_COLLISION: return "close()-munction-collision";
  case SLLMSL_CSTYLE_COMMENT: return "c-style-comment-unsupported";
  case SLLMSL_MULTILINE_ANGLE: return "multiline-single-angle-comment";
  case SLLMSL_INLINE_ANGLE: return "inline-trailing-angle-comment";
  case SLLMSL_APOSTROPHE_COMMENT: return "apostrophe-in-angle-comment";
  case SLLMSL_FIELD_CAP: return "class-field-cap-exceeded";
  case SLLMSL_MISSING_METADATA: return "missing-metadata-metapattern";
 }
 return "unknown";
}

int sllmsl_is_reserved_word(const char *w){
 if(!w) return 0;
 for(size_t i=0;i<kReservedCount;i++) if(strcmp(w,kReserved[i])==0) return 1;
 return 0;
}

void sllmsl_options_init(sllmsl_options *o){
 if(!o) return;
 o->abi_version=SLLMSL_ABI_VERSION;
 o->treat_missing_metadata_as_error=0;
}

static int is_ident_char(int c){ return isalnum(c) || c=='_'; }

/* Extract the next identifier starting at s; copies into buf; returns end ptr. */
static const char *read_ident(const char *s, char *buf, size_t cap){
 size_t n=0; while(*s && is_ident_char((unsigned char)*s)){ if(n+1<cap) buf[n++]=*s; s++; }
 buf[n]=0; return s;
}

static void set(sllmsl_finding *out, sllmsl_rule r, uint32_t line, const char *msg){
 if(!out) return;
 out->rule=r; out->line=line;
 snprintf(out->message,sizeof out->message,"%s",msg);
}

int sllmsl_check_snippet(const char *src, const sllmsl_options *opt, sllmsl_finding *out){
 sllmsl_options def; if(!opt){ sllmsl_options_init(&def); opt=&def; }
 if(out){ out->rule=SLLMSL_OK; out->line=0; out->message[0]=0; }
 if(!src || !*src) return 0;

 int has_class=0, has_metadata=0;
 int field_count=0;         /* rough: declarations at class scope */
 uint32_t line=1;
 const char *p=src;

 while(*p){
  /* track line */
  const char *line_start=p;
  /* find end of this logical line */
  const char *eol=p; while(*eol && *eol!='\n') eol++;

  /* leading whitespace */
  const char *t=line_start; while(*t==' '||*t=='\t') t++;

  /* --- comment-form checks --- */
  /* C-style block comment opener (slash-star): unsupported in SLeeLa. */
  for(const char *q=t; q<eol-1; q++){
   if(q[0]=='/' && q[1]=='*'){ set(out,SLLMSL_CSTYLE_COMMENT,line,
       "C-style /* ... */ is not a SLeeLa comment; use << ... >> (angle block)"); return 1; }
  }
  /* angle comment handling: a line beginning with '<' (not "<<") */
  if(t[0]=='<' && t[1]!='<'){
   /* multi-line single '<': no closing '>' on this same line */
   const char *gt=t; int closed=0;
   while(gt<eol){ if(*gt=='>'){ closed=1; break; } gt++; }
   if(!closed){ set(out,SLLMSL_MULTILINE_ANGLE,line,
      "a '<' comment is single-line; a comment spanning multiple lines must use << ... >>"); return 1; }
   /* apostrophe inside the angle comment */
   for(const char *q=t; q<=gt; q++) if(*q=='\''){ set(out,SLLMSL_APOSTROPHE_COMMENT,line,
      "apostrophe (') inside an angle comment is a lexical error; reword to avoid it"); return 1; }
  } else {
   /* inline trailing '<' comment: code, then a '<' that is not '<<' and not in a string */
   int in_str=0; char sc=0;
   for(const char *q=t; q<eol; q++){
    if(in_str){ if(*q==sc && q[-1]!='\\') in_str=0; continue; }
    if(*q=='"'||*q=='\''){ in_str=1; sc=*q; continue; }
    if(*q=='<' && q!=t && q[1]!='<' && q[-1]!='<'){
     /* require some non-space code before it on the line */
     const char *b=t; int code=0; for(; b<q; b++) if(!isspace((unsigned char)*b)){ code=1; break; }
     if(code){ set(out,SLLMSL_INLINE_ANGLE,line,
        "inline trailing '< comment >' after code is a parse error; put the comment on its own line"); return 1; }
    }
   }
  }

  /* --- class presence + metadata header --- */
  {
   char w[64]; const char *q=t;
   /* skip modifiers to find 'class' */
   while(*q && q<eol){
    q=read_ident(q,w,sizeof w);
    if(w[0]){
     if(strcmp(w,"class")==0) has_class=1;
    } else { q++; }
    if(*q==' '||*q=='\t') { q++; continue; }
    if(!is_ident_char((unsigned char)*q)) break;
   }
   if(strstr(t,"Descriptive metadata")) has_metadata=1;
  }

  /* --- reserved word as identifier: scan the whole line for "TYPE reserved"
         (a primitive/return type immediately followed by a reserved word used
         as the method/variable name), e.g. "void record(" or "int enum". --- */
  {
   int in_str=0; char sc=0;
   for(const char *r=t; r<eol; r++){
    if(in_str){ if(*r==sc && r[-1]!='\\') in_str=0; continue; }
    if(*r=='"'||*r=='\''){ in_str=1; sc=*r; continue; }
    /* a word boundary start */
    int boundary = (r==t) || !is_ident_char((unsigned char)r[-1]);
    if(!boundary || !isalpha((unsigned char)*r)) continue;
    char w1[64]; const char *q=read_ident(r,w1,sizeof w1);
    int is_type=0; for(size_t i=0;i<kTypeLeadsCount;i++) if(strcmp(w1,kTypeLeads[i])==0){ is_type=1; break; }
    if(!is_type) continue;
    const char *a=q; while(*a==' '||*a=='\t') a++;
    char w2[64]; read_ident(a,w2,sizeof w2);
    if(w2[0] && sllmsl_is_reserved_word(w2) && strcmp(w2,"close")!=0){
     char m[200]; snprintf(m,sizeof m,
      "'%s' is a reserved SLeeLa keyword and cannot be used as an identifier (method/variable name)", w2);
     set(out,SLLMSL_RESERVED_IDENTIFIER,line,m); return 1;
    }
    /* crude field counter: "TYPE name" at class scope that is not a method call */
    if(has_class && w2[0]){
     const char *b=a; while(*b && is_ident_char((unsigned char)*b)) b++;
     while(*b==' '||*b=='\t') b++;
     if(*b!='(' ) field_count++;
    }
   }
  }

  /* --- no-arg close() method/collision --- */
  {
   const char *c=strstr(t,"close");
   if(c){
    const char *a=c+5; while(*a==' '||*a=='\t') a++;
    if(*a=='('){ const char *b=a+1; while(*b==' '||*b=='\t') b++;
     if(*b==')'){ set(out,SLLMSL_CLOSE_COLLISION,line,
        "no-arg close() collides with the reserved Munction reach-handle builtin at runtime; rename (e.g. sealMoment())"); return 1; }
    }
   }
  }

  if(*eol=='\n'){ p=eol+1; line++; } else { p=eol; }
 }

 if(has_class && field_count>SLLMSL_MAX_FIELDS){
  char m[160]; snprintf(m,sizeof m,
   "class declares %d named fields, exceeding the %d-field cap; pack handles into an array reached via accessors or split the class",
   field_count, SLLMSL_MAX_FIELDS);
  set(out,SLLMSL_FIELD_CAP,0,m); return 1;
 }
 if(has_class && !has_metadata){
  set(out,SLLMSL_MISSING_METADATA,0,
   "new SLeeLa class is missing the Metadata Class metapattern header '// ---- Descriptive metadata ... ----'");
  if(opt->treat_missing_metadata_as_error) return 1;
  /* advisory: report in 'out' but do not fail the check */
 }
 return 0;
}

/* ---- reasoning-layer adapter ---- */
static sllmr_verdict verifier_check(void *ud, sllmr_subject subject,
                                    const char *question,
                                    const sllmr_step *prior, size_t pn,
                                    const sllmr_step *cand,
                                    char *note, size_t ncap){
 (void)question;(void)prior;(void)pn;
 const sllmsl_options *opt=(const sllmsl_options*)ud;
 /* Only judge code-subject proposals that look like SLeeLa source. */
 if(subject!=SLLMR_SUBJECT_CODE) return SLLMR_VERDICT_UNKNOWN;
 const char *text=cand->text? cand->text : "";
 /* Heuristic: judge snippets that contain SLeeLa structure. */
 int looks_sleela = strstr(text,"class")||strstr(text,"void ")||strstr(text,"#sleela")||
                    strstr(text,"int ")||strstr(text,"<<")||strstr(text,"close(");
 if(!looks_sleela) return SLLMR_VERDICT_UNKNOWN;
 sllmsl_finding f;
 if(sllmsl_check_snippet(text,opt,&f) && f.rule!=SLLMSL_OK){
  if(note&&ncap) snprintf(note,ncap,"%s: %s", sllmsl_rule_name(f.rule), f.message);
  return SLLMR_VERDICT_REFUTED;
 }
 if(note&&ncap) snprintf(note,ncap,"no SLeeLa rule violation found (static check)");
 return SLLMR_VERDICT_SUPPORTED;
}

sllmr_verifier sllmsl_as_reasoning_verifier(const sllmsl_options *opt){
 sllmr_verifier v;
 v.abi_version=SLLMR_ABI_VERSION;
 v.name="sleela-static-verifier";
 v.user_data=(void*)opt;  /* borrowed; may be NULL for defaults */
 v.check=verifier_check;
 return v;
}
