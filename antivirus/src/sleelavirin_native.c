#include "sleelavirin_native.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SHA256_BLOCK_SIZE 64u

typedef struct {
    uint32_t state[8];
    uint64_t bit_count;
    uint8_t block[SHA256_BLOCK_SIZE];
    size_t used;
} sha256_ctx;

static const uint32_t k[64] = {
    0x428a2f98u,0x71374491u,0xb5c0fbcfu,0xe9b5dba5u,0x3956c25bu,0x59f111f1u,0x923f82a4u,0xab1c5ed5u,
    0xd807aa98u,0x12835b01u,0x243185beu,0x550c7dc3u,0x72be5d74u,0x80deb1feu,0x9bdc06a7u,0xc19bf174u,
    0xe49b69c1u,0xefbe4786u,0x0fc19dc6u,0x240ca1ccu,0x2de92c6fu,0x4a7484aau,0x5cb0a9dcu,0x76f988dau,
    0x983e5152u,0xa831c66du,0xb00327c8u,0xbf597fc7u,0xc6e00bf3u,0xd5a79147u,0x06ca6351u,0x14292967u,
    0x27b70a85u,0x2e1b2138u,0x4d2c6dfcu,0x53380d13u,0x650a7354u,0x766a0abbu,0x81c2c92eu,0x92722c85u,
    0xa2bfe8a1u,0xa81a664bu,0xc24b8b70u,0xc76c51a3u,0xd192e819u,0xd6990624u,0xf40e3585u,0x106aa070u,
    0x19a4c116u,0x1e376c08u,0x2748774cu,0x34b0bcb5u,0x391c0cb3u,0x4ed8aa4au,0x5b9cca4fu,0x682e6ff3u,
    0x748f82eeu,0x78a5636fu,0x84c87814u,0x8cc70208u,0x90befffau,0xa4506cebu,0xbef9a3f7u,0xc67178f2u
};

static uint32_t rotr(uint32_t x, unsigned n) { return (x >> n) | (x << (32u - n)); }
static uint32_t load_be32(const uint8_t *p)
{ return ((uint32_t)p[0] << 24) | ((uint32_t)p[1] << 16) | ((uint32_t)p[2] << 8) | (uint32_t)p[3]; }
static void store_be32(uint8_t *p, uint32_t x)
{ p[0]=(uint8_t)(x>>24); p[1]=(uint8_t)(x>>16); p[2]=(uint8_t)(x>>8); p[3]=(uint8_t)x; }

static void sha256_transform(sha256_ctx *c, const uint8_t block[64])
{
    uint32_t w[64], a,b,d,e,f,g,h,cc,t1,t2;
    size_t i;
    for (i=0;i<16;i++) w[i]=load_be32(block+4*i);
    for (i=16;i<64;i++) {
        uint32_t s0=rotr(w[i-15],7)^rotr(w[i-15],18)^(w[i-15]>>3);
        uint32_t s1=rotr(w[i-2],17)^rotr(w[i-2],19)^(w[i-2]>>10);
        w[i]=w[i-16]+s0+w[i-7]+s1;
    }
    a=c->state[0]; b=c->state[1]; cc=c->state[2]; d=c->state[3];
    e=c->state[4]; f=c->state[5]; g=c->state[6]; h=c->state[7];
    for (i=0;i<64;i++) {
        uint32_t s1=rotr(e,6)^rotr(e,11)^rotr(e,25);
        uint32_t ch=(e&f)^((~e)&g);
        uint32_t s0=rotr(a,2)^rotr(a,13)^rotr(a,22);
        uint32_t maj=(a&b)^(a&cc)^(b&cc);
        t1=h+s1+ch+k[i]+w[i]; t2=s0+maj;
        h=g; g=f; f=e; e=d+t1; d=cc; cc=b; b=a; a=t1+t2;
    }
    c->state[0]+=a; c->state[1]+=b; c->state[2]+=cc; c->state[3]+=d;
    c->state[4]+=e; c->state[5]+=f; c->state[6]+=g; c->state[7]+=h;
}
static void sha256_init(sha256_ctx *c)
{
    static const uint32_t initial[8]={0x6a09e667u,0xbb67ae85u,0x3c6ef372u,0xa54ff53au,0x510e527fu,0x9b05688cu,0x1f83d9abu,0x5be0cd19u};
    memcpy(c->state,initial,sizeof(initial)); c->bit_count=0; c->used=0;
}
static void sha256_update(sha256_ctx *c, const uint8_t *data, size_t size)
{
    while (size) {
        size_t n=SHA256_BLOCK_SIZE-c->used;
        if (n>size) n=size;
        memcpy(c->block+c->used,data,n); c->used+=n; data+=n; size-=n;
        c->bit_count+=(uint64_t)n*8u;
        if (c->used==SHA256_BLOCK_SIZE) { sha256_transform(c,c->block); c->used=0; }
    }
}
static void sha256_final(sha256_ctx *c, uint8_t out[32])
{
    uint64_t bits=c->bit_count;
    size_t i;
    c->block[c->used++]=0x80;
    if (c->used>56) { while(c->used<64)c->block[c->used++]=0; sha256_transform(c,c->block); c->used=0; }
    while(c->used<56)c->block[c->used++]=0;
    for(i=0;i<8;i++) c->block[63-i]=(uint8_t)(bits>>(8*i));
    sha256_transform(c,c->block);
    for(i=0;i<8;i++)store_be32(out+4*i,c->state[i]);
    memset(c,0,sizeof(*c));
}
int sleelavirin_sha256_file(const char *path, uint8_t digest[32])
{
    FILE *f; uint8_t buffer[16384]; size_t n; sha256_ctx c;
    if(!path||!digest)return -1;
    f=fopen(path,"rb"); if(!f)return -1;
    sha256_init(&c);
    while((n=fread(buffer,1,sizeof(buffer),f))>0)sha256_update(&c,buffer,n);
    if(ferror(f)){fclose(f);return -1;}
    if(fclose(f)!=0)return -1;
    sha256_final(&c,digest); return 0;
}
int sleelavirin_sha256_buffer(const uint8_t *data, size_t length, uint8_t digest[32])
{
    sha256_ctx c;
    if(!digest)return -1;
    if(!data&&length)return -1;
    sha256_init(&c);
    if(length)sha256_update(&c,data,length);
    sha256_final(&c,digest);
    return 0;
}
static void digest_hex(const uint8_t digest[32], char out[65])
{
    static const char hex[]="0123456789abcdef"; size_t i;
    for(i=0;i<32;i++){out[2*i]=hex[digest[i]>>4];out[2*i+1]=hex[digest[i]&15];}
    out[64]='\0';
}
static int valid_hex_digest(const char *s)
{
    size_t i; if(strlen(s)<64)return 0;
    for(i=0;i<64;i++) if(!((s[i]>='0'&&s[i]<='9')||(s[i]>='a'&&s[i]<='f')||(s[i]>='A'&&s[i]<='F')))return 0;
    return 1;
}
int sleelavirin_native_scan_file(const char *path, const char *database, char *matched_name, size_t matched_name_size)
{
    uint8_t digest[32]; char target[65], line[1024]; FILE *db;
    if(!path||!database)return -1;
    if(sleelavirin_sha256_file(path,digest)!=0)return -1;
    digest_hex(digest,target);
    db=fopen(database,"r"); if(!db)return -1;
    if(matched_name&&matched_name_size)matched_name[0]='\0';
    while(fgets(line,sizeof(line),db)){
        char *p=line,*end; size_t i;
        while(*p==' '||*p=='\t')p++;
        if(*p=='#'||*p=='\n'||*p=='\r'||*p=='\0')continue;
        end=p; while(*end&&*end!=' '&&*end!='\t'&&*end!='\r'&&*end!='\n')end++;
        if((size_t)(end-p)!=64||!valid_hex_digest(p))continue;
        for(i=0;i<64;i++) if((char)((p[i]>='A'&&p[i]<='F')?p[i]+('a'-'A'):p[i])!=target[i])break;
        if(i==64){
            while(*end==' '||*end=='\t')end++;
            if(matched_name&&matched_name_size){
                size_t n=strcspn(end,"\r\n"); if(n>=matched_name_size)n=matched_name_size-1;
                memcpy(matched_name,end,n); matched_name[n]='\0';
            }
            fclose(db); return 1;
        }
    }
    if(ferror(db)){fclose(db);return -1;}
    fclose(db); return 0;
}
