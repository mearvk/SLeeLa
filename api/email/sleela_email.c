#define _POSIX_C_SOURCE 200112L
#include "sleela_email.h"
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

static void fail(char *e,size_t n,const char *s){if(e&&n)snprintf(e,n,"%s",s?s:"SMTP failure");}
static int field(const char*s,size_t n){return s&&*s&&strlen(s)<=n;}
static int header_field(const char*s,size_t n){
 if(!field(s,n))return 0;
 return !strchr(s,'\r')&&!strchr(s,'\n');
}
const char *sleela_email_tls_mode_name(sleela_email_tls_mode_t m){return m==SLEELA_EMAIL_TLS_NONE?"none":m==SLEELA_EMAIL_TLS_STARTTLS?"starttls":m==SLEELA_EMAIL_TLS_IMPLICIT?"implicit":"unknown";}

static int connect_to(const char*h,unsigned short p,const char*b,int*out){
 char service[16];struct addrinfo q,*r,*x;int fd=-1;
 snprintf(service,sizeof(service),"%u",p);memset(&q,0,sizeof(q));q.ai_socktype=SOCK_STREAM;q.ai_family=AF_UNSPEC;
 if(getaddrinfo(h,service,&q,&r)!=0)return 0;
 for(x=r;x;x=x->ai_next){fd=socket(x->ai_family,x->ai_socktype,x->ai_protocol);if(fd<0)continue;
  if(b&&*b){struct addrinfo z=q,*br=NULL;z.ai_family=x->ai_family;
   if(getaddrinfo(b,"0",&z,&br)!=0||!br||bind(fd,br->ai_addr,br->ai_addrlen)!=0){if(br)freeaddrinfo(br);close(fd);fd=-1;continue;}freeaddrinfo(br);}
  if(connect(fd,x->ai_addr,x->ai_addrlen)==0)break;close(fd);fd=-1;
 } freeaddrinfo(r);if(fd<0)return 0;*out=fd;return 1;
}
static int psend(int f,const char*s){size_t n=strlen(s),p=0;while(p<n){ssize_t z=send(f,s+p,n-p,0);if(z<=0)return 0;p+=(size_t)z;}return 1;}
static int pline(int f,char*b,size_t n){size_t p=0;while(p+1<n){char c;ssize_t z=recv(f,&c,1,0);if(z<=0)return 0;b[p++]=c;if(c=='\n')break;}b[p]=0;return p>0;}
static int tline(SSL*s,char*b,size_t n){size_t p=0;while(p+1<n){char c;int z=SSL_read(s,&c,1);if(z<=0)return 0;b[p++]=c;if(c=='\n')break;}b[p]=0;return p>0;}
static int code(const char*s){return s&&strlen(s)>=3?atoi(s):0;}

static int pexpect(int f,int want,char*e,size_t en){
 char b[2048];int c,last=0;do{if(!pline(f,b,sizeof(b))){fail(e,en,"SMTP read failed");return 0;}c=code(b);last=c;if(c/100!=want/100){fail(e,en,b);return 0;}}while(strlen(b)>=4&&b[3]=='-');
 if(want&&last!=want){fail(e,en,b);return 0;}return 1;
}
static int texpect(SSL*s,int want,char*e,size_t en){
 char b[2048];int c,last=0;do{if(!tline(s,b,sizeof(b))){fail(e,en,"SMTP TLS read failed");return 0;}c=code(b);last=c;if(c/100!=want/100){fail(e,en,b);return 0;}}while(strlen(b)>=4&&b[3]=='-');
 if(want&&last!=want){fail(e,en,b);return 0;}return 1;
}
static int tsend(SSL*s,const char*x){size_t n=strlen(x),p=0;while(p<n){int z=SSL_write(s,x+p,(int)(n-p));if(z<=0)return 0;p+=(size_t)z;}return 1;}

static int auth_plain(SSL*s,const char*u,const char*p,char*e,size_t en){
 size_t rawlen=strlen(u)+strlen(p)+2,b64len=4*((rawlen+2)/3);unsigned char*raw=calloc(1,rawlen);char*b64=calloc(1,b64len+1);int ok=0;
 if(!raw||!b64)goto done;memcpy(raw+1,u,strlen(u));memcpy(raw+strlen(u)+2,p,strlen(p));
 if(EVP_EncodeBlock((unsigned char*)b64,raw,(int)rawlen)<=0)goto done;
 {char cmd[8192];if(strlen(b64)+14>=sizeof(cmd))goto done;snprintf(cmd,sizeof(cmd),"AUTH PLAIN %s\r\n",b64);ok=tsend(s,cmd)&&texpect(s,235,e,en);}
done: if(raw){memset(raw,0,rawlen);free(raw);}if(b64){memset(b64,0,b64len+1);free(b64);}return ok;
}

static int message_tls(SSL*s,const sleela_email_message_t*m,char*e,size_t en){
 char b[4096];if(!texpect(s,220,e,en))return 0;snprintf(b,sizeof(b),"EHLO %s\r\n",m->helo_name);
 if(!tsend(s,b)||!texpect(s,250,e,en))return 0;
 if(m->username&&*m->username&&m->password&&!auth_plain(s,m->username,m->password,e,en))return 0;
 snprintf(b,sizeof(b),"MAIL FROM:<%s>\r\n",m->from);if(!tsend(s,b)||!texpect(s,250,e,en))return 0;
 snprintf(b,sizeof(b),"RCPT TO:<%s>\r\n",m->to);if(!tsend(s,b)||!texpect(s,250,e,en))return 0;
 if(!tsend(s,"DATA\r\n")||!texpect(s,354,e,en))return 0;
 snprintf(b,sizeof(b),"From: <%s>\r\nTo: <%s>\r\nSubject: %s\r\nMIME-Version: 1.0\r\nContent-Type: text/plain; charset=utf-8\r\n\r\n",m->from,m->to,m->subject);
 if(!tsend(s,b)||!tsend(s,m->body)||!tsend(s,"\r\n.\r\n")||!texpect(s,250,e,en))return 0;
 tsend(s,"QUIT\r\n");return 1;
}

int sleela_email_send(const sleela_email_message_t*m,char*e,size_t en){
 int fd=-1,ok=0;SSL_CTX*ctx=NULL;SSL*ssl=NULL;char b[4096];
 if(!m||!field(m->smtp_host,253)||!header_field(m->helo_name,253)||!header_field(m->from,320)||!header_field(m->to,320)||
    !header_field(m->subject,998)||!m->body||strlen(m->body)>16*1024*1024||!m->smtp_port){fail(e,en,"invalid email configuration");return 0;}
 if(m->tls_mode==SLEELA_EMAIL_TLS_NONE && m->username&&*m->username){fail(e,en,"SMTP credentials require TLS");return 0;}
 if(!connect_to(m->smtp_host,m->smtp_port,m->local_bind_host,&fd)){fail(e,en,"SMTP connection failed");return 0;}
 if(m->tls_mode==SLEELA_EMAIL_TLS_NONE){
  if(!pexpect(fd,220,e,en)||!psend(fd,"EHLO sleela\r\n")||!pexpect(fd,250,e,en))goto done;
  snprintf(b,sizeof(b),"MAIL FROM:<%s>\r\n",m->from);if(!psend(fd,b)||!pexpect(fd,250,e,en))goto done;
  snprintf(b,sizeof(b),"RCPT TO:<%s>\r\n",m->to);if(!psend(fd,b)||!pexpect(fd,250,e,en))goto done;
  if(!psend(fd,"DATA\r\n")||!pexpect(fd,354,e,en))goto done;
  snprintf(b,sizeof(b),"From: <%s>\r\nTo: <%s>\r\nSubject: %s\r\nContent-Type: text/plain; charset=utf-8\r\n\r\n",m->from,m->to,m->subject);
  if(!psend(fd,b)||!psend(fd,m->body)||!psend(fd,"\r\n.\r\n")||!pexpect(fd,250,e,en))goto done;psend(fd,"QUIT\r\n");ok=1;goto done;
 }
 if(m->tls_mode==SLEELA_EMAIL_TLS_STARTTLS){
  if(!pexpect(fd,220,e,en)||!psend(fd,"EHLO sleela\r\n")||!pexpect(fd,250,e,en)||!psend(fd,"STARTTLS\r\n")||!pexpect(fd,220,e,en))goto done;
 }
 ctx=SSL_CTX_new(TLS_client_method());if(!ctx){fail(e,en,"TLS context creation failed");goto done;}
 SSL_CTX_set_min_proto_version(ctx,TLS1_2_VERSION);SSL_CTX_set_verify(ctx,SSL_VERIFY_PEER,NULL);
 if(SSL_CTX_set_default_verify_paths(ctx)!=1){fail(e,en,"TLS trust store unavailable");goto done;}
 ssl=SSL_new(ctx);if(!ssl){fail(e,en,"TLS session creation failed");goto done;}SSL_set_fd(ssl,fd);
 {X509_VERIFY_PARAM*v=SSL_get0_param(ssl);struct in_addr ia; if(inet_pton(AF_INET,m->smtp_host,&ia)==1)X509_VERIFY_PARAM_set1_ip_asc(v,m->smtp_host);else if(inet_pton(AF_INET6,m->smtp_host,&ia)==1)X509_VERIFY_PARAM_set1_ip_asc(v,m->smtp_host);else X509_VERIFY_PARAM_set1_host(v,m->smtp_host,0);}
 if(SSL_connect(ssl)!=1){fail(e,en,"SMTP TLS certificate/handshake failed");goto done;}
 ok=message_tls(ssl,m,e,en);
done:if(ssl){SSL_shutdown(ssl);SSL_free(ssl);}if(ctx)SSL_CTX_free(ctx);if(fd>=0)close(fd);return ok;
}
