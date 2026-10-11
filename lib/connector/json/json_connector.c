#include "json_connector.h"
#include <ctype.h>
static int hexch(char c){return (c>='0'&&c<='9')||(c>='a'&&c<='f')||(c>='A'&&c<='F');}
size_t sleela_json_tokenize(const char*s,sleela_json_token*t,size_t cap){
 if(!s||!t||!cap)return 0;
 size_t i=0,n=0,stack[256],depth=0,roots=0;
 while(s[i]){
  while(s[i]&&isspace((unsigned char)s[i]))i++;
  char c=s[i];if(!c)break;
  if(c==','||c==':'){i++;continue;}
  if(c=='}'||c==']'){
   if(!depth)return 0;
   size_t k=stack[depth-1];
   if((c=='}'&&t[k].type!=SLEELA_JSON_OBJECT)||(c==']'&&t[k].type!=SLEELA_JSON_ARRAY))return 0;
   depth--;t[k].end=i+1;i++;continue;
  }
  if(n>=cap)return 0;
  size_t parent=depth?stack[depth-1]:(size_t)-1,start=i;
  if(parent==(size_t)-1&&++roots>1)return 0;
  sleela_json_type ty;
  if(c=='{'||c=='['){
   ty=c=='{'?SLEELA_JSON_OBJECT:SLEELA_JSON_ARRAY;
   t[n]=(sleela_json_token){ty,i,0,parent};
   if(depth>=256)return 0;
   stack[depth++]=n++;i++;continue;
  }
  if(c=='"'){
   i++;int closed=0;
   while(s[i]){
    if(s[i]=='"'){i++;closed=1;break;}
    if((unsigned char)s[i]<0x20)return 0;
    if(s[i]=='\\'){
     i++;if(!s[i])return 0;
     if(s[i]=='u'){for(int k=1;k<=4;k++)if(!hexch(s[i+k]))return 0;i+=5;continue;}
     if(s[i]!='"'&&s[i]!='\\'&&s[i]!='/'&&s[i]!='b'&&s[i]!='f'&&s[i]!='n'&&s[i]!='r'&&s[i]!='t')return 0;
     i++;continue;
    }
    i++;
   }
   if(!closed)return 0;ty=SLEELA_JSON_STRING;
  }else{
   while(s[i]&&s[i]!=','&&s[i]!=']'&&s[i]!='}'&&!isspace((unsigned char)s[i]))i++;
   if(i==start)return 0;ty=SLEELA_JSON_PRIMITIVE;
  }
  t[n++]=(sleela_json_token){ty,start,i,parent};
 }
 if(depth||roots!=1)return 0;
 return n;
}
