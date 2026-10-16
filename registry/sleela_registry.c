/*
 * registry/sleela_registry.c -- Sleela Registry (C implementation).
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * A bounded, deterministic key/value + provenance store. Keys are held in a
 * sorted array (so save/review are stable and idempotent). Each key owns a
 * small set of named values and, optionally, one provenance record carrying the
 * file identity + SHA-256 + originality seal. Pure C.
 */
#include "sleela_registry.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SL_REG_MAX_KEYS      4096
#define SL_REG_MAX_VALUES    16     /* values per key                      */
#define SL_REG_PATH_CAP      256
#define SL_REG_NAME_CAP      64
#define SL_REG_STR_CAP       256

typedef struct {
    char        name[SL_REG_NAME_CAP];
    sl_reg_kind kind;
    char        s[SL_REG_STR_CAP];
    long        i;
    int         b;
    int         used;
} RegValue;

typedef struct {
    char      path[SL_REG_PATH_CAP];
    RegValue  values[SL_REG_MAX_VALUES];
    int       nvalues;
    int       has_prov;
    sl_reg_provenance prov;
    int       used;
} RegKey;

struct sl_registry {
    RegKey *keys;
    int     nkeys;
    int     cap;
};

/* ---- SHA-256 (FIPS 180-4, self-contained) ----------------------------- */
static uint32_t rotr(uint32_t x, uint32_t n){ return (x>>n)|(x<<(32-n)); }
void sl_reg_sha256_hex(const void *data, size_t len, char *out){
    static const uint32_t K[64]={
      0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
      0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
      0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
      0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
      0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
      0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
      0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
      0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2};
    uint32_t h[8]={0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19};
    size_t padded=((len+8)/64+1)*64;
    unsigned char *m=(unsigned char*)calloc(1,padded);
    memcpy(m,data,len); m[len]=0x80;
    uint64_t bits=(uint64_t)len*8;
    for(int i=0;i<8;++i) m[padded-1-i]=(unsigned char)((bits>>(8*i))&0xff);
    for(size_t off=0;off<padded;off+=64){
        uint32_t w[64];
        for(int i=0;i<16;++i) w[i]=(m[off+i*4]<<24)|(m[off+i*4+1]<<16)|(m[off+i*4+2]<<8)|m[off+i*4+3];
        for(int i=16;i<64;++i){ uint32_t s0=rotr(w[i-15],7)^rotr(w[i-15],18)^(w[i-15]>>3); uint32_t s1=rotr(w[i-2],17)^rotr(w[i-2],19)^(w[i-2]>>10); w[i]=w[i-16]+s0+w[i-7]+s1; }
        uint32_t a=h[0],b=h[1],c=h[2],d=h[3],e=h[4],f=h[5],g=h[6],hh=h[7];
        for(int i=0;i<64;++i){ uint32_t S1=rotr(e,6)^rotr(e,11)^rotr(e,25); uint32_t ch=(e&f)^((~e)&g); uint32_t t1=hh+S1+ch+K[i]+w[i]; uint32_t S0=rotr(a,2)^rotr(a,13)^rotr(a,22); uint32_t maj=(a&b)^(a&c)^(b&c); uint32_t t2=S0+maj; hh=g;g=f;f=e;e=d+t1;d=c;c=b;b=a;a=t1+t2; }
        h[0]+=a;h[1]+=b;h[2]+=c;h[3]+=d;h[4]+=e;h[5]+=f;h[6]+=g;h[7]+=hh;
    }
    free(m);
    static const char *hx="0123456789abcdef";
    for(int i=0;i<8;++i) for(int j=7;j>=0;--j){ *out++=hx[(h[i]>>(j*4))&0xf]; }
    *out='\0';
}

/* ---- seal ------------------------------------------------------------- */
void sl_reg_seal(sl_reg_provenance *p){
    if(!p) return;
    char buf[17+64+65+128+8];
    int n=snprintf(buf,sizeof buf,"%s|%s|%s|%s",p->hex_id,p->version,p->hash,p->author);
    if(n<0) n=0;
    sl_reg_sha256_hex(buf,(size_t)n,p->seal);
}
int sl_reg_seal_verify(const sl_reg_provenance *p){
    if(!p) return 0;
    sl_reg_provenance tmp=*p; tmp.seal[0]='\0';
    sl_reg_seal(&tmp);
    return strncmp(tmp.seal,p->seal,64)==0 ? 1 : 0;
}

/* ---- key store -------------------------------------------------------- */
sl_registry *sl_reg_create(void){
    sl_registry *r=(sl_registry*)calloc(1,sizeof(*r));
    if(!r) return NULL;
    r->cap=SL_REG_MAX_KEYS;
    r->keys=(RegKey*)calloc((size_t)r->cap,sizeof(RegKey));
    if(!r->keys){ free(r); return NULL; }
    return r;
}
void sl_reg_destroy(sl_registry *r){ if(r){ free(r->keys); free(r); } }

static RegKey *find_key(const sl_registry *r,const char *path){
    for(int i=0;i<r->nkeys;++i) if(r->keys[i].used && strcmp(r->keys[i].path,path)==0) return (RegKey*)&r->keys[i];
    return NULL;
}
static int key_cmp(const void*a,const void*b){ return strcmp(((const RegKey*)a)->path,((const RegKey*)b)->path); }

static RegKey *ensure_key(sl_registry *r,const char *path){
    RegKey *k=find_key(r,path);
    if(k) return k;
    if(r->nkeys>=r->cap) return NULL;
    k=&r->keys[r->nkeys++];
    memset(k,0,sizeof(*k));
    strncpy(k->path,path,SL_REG_PATH_CAP-1);
    k->used=1;
    qsort(r->keys,(size_t)r->nkeys,sizeof(RegKey),key_cmp);
    return find_key(r,path);
}

sl_reg_status sl_reg_key_create(sl_registry *r,const char *path){
    if(!r||!path) return SL_REG_INVALID;
    /* make parents: A, A/B, A/B/C */
    char buf[SL_REG_PATH_CAP]; strncpy(buf,path,sizeof buf-1); buf[sizeof buf-1]='\0';
    char acc[SL_REG_PATH_CAP]=""; char *tok=strtok(buf,"/");
    while(tok){ if(acc[0]) strncat(acc,"/",sizeof acc-strlen(acc)-1); strncat(acc,tok,sizeof acc-strlen(acc)-1);
        if(!ensure_key(r,acc)) return SL_REG_LIMIT; tok=strtok(NULL,"/"); }
    if(!path[0]) ensure_key(r,"");
    return SL_REG_OK;
}
int sl_reg_key_exists(const sl_registry *r,const char *path){ return (r&&path&&find_key(r,path))?1:0; }

int sl_reg_key_children(const sl_registry *r,const char *path,char **names,int max,size_t name_cap){
    if(!r||!path||!names) return 0;
    size_t plen=strlen(path);
    int count=0;
    for(int i=0;i<r->nkeys&&count<max;++i){
        const char *kp=r->keys[i].path;
        if(!r->keys[i].used) continue;
        if(plen>0){ if(strncmp(kp,path,plen)!=0||kp[plen]!='/') continue; kp+=plen+1; }
        if(!*kp) continue;
        if(strchr(kp,'/')) continue;  /* direct child only */
        strncpy(names[count],kp,name_cap-1); names[count][name_cap-1]='\0'; count++;
    }
    return count;
}

/* ---- values ----------------------------------------------------------- */
static RegValue *find_val(RegKey *k,const char *name){
    for(int i=0;i<k->nvalues;++i) if(k->values[i].used&&strcmp(k->values[i].name,name)==0) return &k->values[i];
    return NULL;
}
static RegValue *ensure_val(RegKey *k,const char *name){
    RegValue *v=find_val(k,name); if(v) return v;
    if(k->nvalues>=SL_REG_MAX_VALUES) return NULL;
    v=&k->values[k->nvalues++]; memset(v,0,sizeof(*v));
    strncpy(v->name,name,SL_REG_NAME_CAP-1); v->used=1; return v;
}

sl_reg_status sl_reg_set_string(sl_registry *r,const char *path,const char *name,const char *value){
    if(!r||!path||!name||!value) return SL_REG_INVALID;
    sl_reg_key_create(r,path); RegKey *k=find_key(r,path); if(!k) return SL_REG_LIMIT;
    RegValue *v=ensure_val(k,name); if(!v) return SL_REG_LIMIT;
    v->kind=SL_REG_STRING; strncpy(v->s,value,SL_REG_STR_CAP-1); v->s[SL_REG_STR_CAP-1]='\0';
    return SL_REG_OK;
}
sl_reg_status sl_reg_set_int(sl_registry *r,const char *path,const char *name,long value){
    if(!r||!path||!name) return SL_REG_INVALID;
    sl_reg_key_create(r,path); RegKey *k=find_key(r,path); if(!k) return SL_REG_LIMIT;
    RegValue *v=ensure_val(k,name); if(!v) return SL_REG_LIMIT; v->kind=SL_REG_INT; v->i=value; return SL_REG_OK;
}
sl_reg_status sl_reg_set_bool(sl_registry *r,const char *path,const char *name,int value){
    if(!r||!path||!name) return SL_REG_INVALID;
    sl_reg_key_create(r,path); RegKey *k=find_key(r,path); if(!k) return SL_REG_LIMIT;
    RegValue *v=ensure_val(k,name); if(!v) return SL_REG_LIMIT; v->kind=SL_REG_BOOL; v->b=value?1:0; return SL_REG_OK;
}
sl_reg_status sl_reg_get_string(const sl_registry *r,const char *path,const char *name,char *buf,size_t cap){
    if(!r||!path||!name||!buf) return SL_REG_INVALID; RegKey *k=find_key(r,path); if(!k) return SL_REG_NOT_FOUND;
    RegValue *v=find_val(k,name); if(!v) return SL_REG_NOT_FOUND; if(v->kind!=SL_REG_STRING) return SL_REG_INVALID;
    strncpy(buf,v->s,cap-1); buf[cap-1]='\0'; return SL_REG_OK;
}
sl_reg_status sl_reg_get_int(const sl_registry *r,const char *path,const char *name,long *out){
    if(!r||!path||!name||!out) return SL_REG_INVALID; RegKey *k=find_key(r,path); if(!k) return SL_REG_NOT_FOUND;
    RegValue *v=find_val(k,name); if(!v) return SL_REG_NOT_FOUND; if(v->kind!=SL_REG_INT) return SL_REG_INVALID; *out=v->i; return SL_REG_OK;
}
sl_reg_status sl_reg_get_bool(const sl_registry *r,const char *path,const char *name,int *out){
    if(!r||!path||!name||!out) return SL_REG_INVALID; RegKey *k=find_key(r,path); if(!k) return SL_REG_NOT_FOUND;
    RegValue *v=find_val(k,name); if(!v) return SL_REG_NOT_FOUND; if(v->kind!=SL_REG_BOOL) return SL_REG_INVALID; *out=v->b; return SL_REG_OK;
}
sl_reg_status sl_reg_value_kind(const sl_registry *r,const char *path,const char *name,sl_reg_kind *kind){
    if(!r||!path||!name||!kind) return SL_REG_INVALID; RegKey *k=find_key(r,path); if(!k) return SL_REG_NOT_FOUND;
    RegValue *v=find_val(k,name); if(!v) return SL_REG_NOT_FOUND; *kind=v->kind; return SL_REG_OK;
}
sl_reg_status sl_reg_feature_set(sl_registry *r,const char *name,int enabled){
    char p[SL_REG_PATH_CAP]; snprintf(p,sizeof p,"Features"); (void)name; return sl_reg_set_bool(r,p,name,enabled);
}
int sl_reg_feature_enabled(const sl_registry *r,const char *name){
    int out=0; if(sl_reg_get_bool(r,"Features",name,&out)==SL_REG_OK) return out; return 0;
}

/* ---- provenance ------------------------------------------------------- */
sl_reg_status sl_reg_provenance_set(sl_registry *r,const char *path,const sl_reg_provenance *p){
    if(!r||!path||!p) return SL_REG_INVALID;
    sl_reg_key_create(r,path); RegKey *k=find_key(r,path); if(!k) return SL_REG_LIMIT;
    k->prov=*p;
    if(k->prov.seal[0]=='\0') sl_reg_seal(&k->prov);
    k->has_prov=1; return SL_REG_OK;
}
sl_reg_status sl_reg_provenance_get(const sl_registry *r,const char *path,sl_reg_provenance *out){
    if(!r||!path||!out) return SL_REG_INVALID; RegKey *k=find_key(r,path); if(!k||!k->has_prov) return SL_REG_NOT_FOUND;
    *out=k->prov; return SL_REG_OK;
}

/* ---- seed ------------------------------------------------------------- */
sl_reg_status sl_reg_seed_defaults(sl_registry *r){
    if(!r) return SL_REG_INVALID;
    sl_reg_key_create(r,"System");
    sl_reg_set_string(r,"System","name","Idea System(TM) / SLeeLa");
    sl_reg_set_string(r,"System","registryVersion",SLEELA_REGISTRY_VERSION);
    sl_reg_key_create(r,"System/VM");
    sl_reg_set_string(r,"System/VM","default","native");
    sl_reg_key_create(r,"Settings");
    sl_reg_set_int(r,"Settings","memoryLimitMiB",2048);
    sl_reg_set_int(r,"Settings","threadCap",128);
    sl_reg_set_string(r,"Settings","verbosity","normal");
    sl_reg_key_create(r,"Features");
    sl_reg_set_bool(r,"Features","modium",1);
    sl_reg_set_bool(r,"Features","vmSelect",1);
    sl_reg_set_bool(r,"Features","premiumVms",1);
    sl_reg_key_create(r,"Artifacts");
    return SL_REG_OK;
}

/* ---- persistence (.sleelareg text format) ----------------------------- */
sl_reg_status sl_reg_save(const sl_registry *r,const char *path){
    if(!r||!path) return SL_REG_INVALID;
    FILE *f=fopen(path,"wb"); if(!f) return SL_REG_INVALID;
    fprintf(f,"%s %d\n",SLEELA_REGISTRY_MAGIC,SLEELA_REGISTRY_FORMAT);
    for(int i=0;i<r->nkeys;++i){ const RegKey *k=&r->keys[i]; if(!k->used) continue;
        fprintf(f,"[%s]\n",k->path);
        for(int j=0;j<k->nvalues;++j){ const RegValue *v=&k->values[j]; if(!v->used) continue;
            if(v->kind==SL_REG_STRING) fprintf(f,"= s %s %s\n",v->name,v->s);
            else if(v->kind==SL_REG_INT) fprintf(f,"= i %s %ld\n",v->name,v->i);
            else fprintf(f,"= b %s %d\n",v->name,v->b);
        }
        if(k->has_prov){ const sl_reg_provenance *p=&k->prov;
            fprintf(f,"@ %s | %s | %s | %s | %s | %s | %s\n",
                    p->name,p->date,p->author,p->version,p->hex_id,p->hash,p->seal);
        }
    }
    fclose(f); return SL_REG_OK;
}

sl_reg_status sl_reg_load(sl_registry *r,const char *path){
    if(!r||!path) return SL_REG_INVALID;
    FILE *f=fopen(path,"rb"); if(!f) return SL_REG_NOT_FOUND;
    char line[1024]; char cur[SL_REG_PATH_CAP]="";
    if(!fgets(line,sizeof line,f)){ fclose(f); return SL_REG_MALFORMED; }
    if(strncmp(line,SLEELA_REGISTRY_MAGIC,5)!=0){ fclose(f); return SL_REG_MALFORMED; }
    r->nkeys=0;
    while(fgets(line,sizeof line,f)){
        size_t L=strlen(line); while(L&&(line[L-1]=='\n'||line[L-1]=='\r')) line[--L]='\0';
        if(line[0]=='['){ char *e=strchr(line,']'); if(e){ *e='\0'; strncpy(cur,line+1,sizeof cur-1); cur[sizeof cur-1]='\0'; sl_reg_key_create(r,cur); } }
        else if(line[0]=='='&&line[2]){ char t=line[2]; char nm[SL_REG_NAME_CAP]; char rest[SL_REG_STR_CAP];
            if(sscanf(line+4,"%63s %255[^\n]",nm,rest)>=1){
                if(t=='s') sl_reg_set_string(r,cur,nm,rest);
                else if(t=='i') sl_reg_set_int(r,cur,nm,strtol(rest,NULL,10));
                else if(t=='b') sl_reg_set_bool(r,cur,nm,atoi(rest));
            }
        }
        else if(line[0]=='@'){ sl_reg_provenance p; memset(&p,0,sizeof p);
            /* name | date | author | version | hexid | hash | seal */
            sscanf(line+2,"%255[^|] | %31[^|] | %127[^|] | %63[^|] | %16[^|] | %64[^|] | %64[^\n]",
                   p.name,p.date,p.author,p.version,p.hex_id,p.hash,p.seal);
            /* trim trailing spaces from fields */
            for(char*s=p.name+strlen(p.name)-1;s>=p.name&&*s==' ';--s)*s='\0';
            for(char*s=p.hash+strlen(p.hash)-1;s>=p.hash&&*s==' ';--s)*s='\0';
            for(char*s=p.seal+strlen(p.seal)-1;s>=p.seal&&*s==' ';--s)*s='\0';
            sl_reg_provenance_set(r,cur,&p);
        }
    }
    fclose(f); return SL_REG_OK;
}

/* ---- SQL export ------------------------------------------------------- */
static void sql_esc(const char*s,char*out,size_t cap){ size_t j=0; for(size_t i=0;s&&s[i]&&j+2<cap;++i){ if(s[i]=='\''||s[i]=='\\'){ out[j++]='\\'; } out[j++]=s[i]; } out[j]='\0'; }
sl_reg_status sl_reg_export_sql(const sl_registry *r,const char *path){
    if(!r||!path) return SL_REG_INVALID;
    FILE *f=fopen(path,"wb"); if(!f) return SL_REG_INVALID;
    fprintf(f,"-- sleela registry SQL export. Idea System(TM) / Idea(TM). From Author; distributive.\n");
    fprintf(f,"-- Portable MySQL/MariaDB: load with `mysql <db> < this_file`. Deterministic (sorted).\n\n");
    fprintf(f,"CREATE TABLE IF NOT EXISTS registry_entries (\n"
              "  id INT AUTO_INCREMENT PRIMARY KEY,\n"
              "  key_path VARCHAR(256) NOT NULL,\n"
              "  name VARCHAR(64) NOT NULL,\n"
              "  kind VARCHAR(8) NOT NULL,\n"
              "  value TEXT NOT NULL,\n"
              "  UNIQUE KEY uq_entry (key_path, name)\n);\n\n");
    fprintf(f,"CREATE TABLE IF NOT EXISTS registry_provenance (\n"
              "  id INT AUTO_INCREMENT PRIMARY KEY,\n"
              "  key_path VARCHAR(256) NOT NULL,\n"
              "  file VARCHAR(256) NOT NULL, date VARCHAR(32) NOT NULL,\n"
              "  author VARCHAR(128) NOT NULL, version VARCHAR(64) NOT NULL,\n"
              "  hex_id CHAR(16) NOT NULL, hash CHAR(64) NOT NULL, seal CHAR(64) NOT NULL,\n"
              "  UNIQUE KEY uq_prov (key_path)\n);\n\n");
    char eb[1024];
    for(int i=0;i<r->nkeys;++i){ const RegKey *k=&r->keys[i]; if(!k->used) continue;
        for(int j=0;j<k->nvalues;++j){ const RegValue *v=&k->values[j]; if(!v->used) continue;
            const char *kind = v->kind==SL_REG_STRING?"string":(v->kind==SL_REG_INT?"int":"bool");
            char val[SL_REG_STR_CAP+8];
            if(v->kind==SL_REG_STRING) sql_esc(v->s,val,sizeof val);
            else if(v->kind==SL_REG_INT) snprintf(val,sizeof val,"%ld",v->i);
            else snprintf(val,sizeof val,"%d",v->b);
            char ekp[520]; sql_esc(k->path,ekp,sizeof ekp);
            fprintf(f,"INSERT INTO registry_entries (key_path,name,kind,value) VALUES ('%s','%s','%s','%s');\n",
                    ekp,v->name,kind,val);
        }
        if(k->has_prov){ const sl_reg_provenance *p=&k->prov; char ekp[520]; sql_esc(k->path,ekp,sizeof ekp);
            char ef[520],ea[260]; sql_esc(p->name,ef,sizeof ef); sql_esc(p->author,ea,sizeof ea);
            fprintf(f,"INSERT INTO registry_provenance (key_path,file,date,author,version,hex_id,hash,seal) VALUES "
                      "('%s','%s','%s','%s','%s','%s','%s','%s');\n",
                    ekp,ef,p->date,ea,p->version,p->hex_id,p->hash,p->seal);
        }
    }
    (void)eb;
    fclose(f); return SL_REG_OK;
}

/* ---- review ----------------------------------------------------------- */
long sl_reg_review(const sl_registry *r,const char *path,char *buf,size_t cap){
    if(!r||!buf||cap==0) return SL_REG_INVALID;
    char *p=buf; char *end=buf+cap; int w;
    size_t plen=path?strlen(path):0;
    w=snprintf(p,(size_t)(end-p),"Sleela Registry %s  (keys=%d values=%d provenance=%d)\n",
               SLEELA_REGISTRY_VERSION,sl_reg_key_count(r),sl_reg_value_count(r),sl_reg_provenance_count(r));
    if(w>0&&p+w<end) p+=w;
    for(int i=0;i<r->nkeys&&p<end;++i){ const RegKey *k=&r->keys[i]; if(!k->used) continue;
        if(path&&plen>0&&!(strcmp(k->path,path)==0||(strncmp(k->path,path,plen)==0&&k->path[plen]=='/'))) continue;
        w=snprintf(p,(size_t)(end-p),"[%s]\n",k->path[0]?k->path:"(root)"); if(w>0&&p+w<end)p+=w;
        for(int j=0;j<k->nvalues&&p<end;++j){ const RegValue *v=&k->values[j]; if(!v->used) continue;
            if(v->kind==SL_REG_STRING) w=snprintf(p,(size_t)(end-p),"    %s = \"%s\"\n",v->name,v->s);
            else if(v->kind==SL_REG_INT) w=snprintf(p,(size_t)(end-p),"    %s = %ld\n",v->name,v->i);
            else w=snprintf(p,(size_t)(end-p),"    %s = %s\n",v->name,v->b?"true":"false");
            if(w>0&&p+w<end)p+=w;
        }
        if(k->has_prov&&p<end){ const sl_reg_provenance *pr=&k->prov;
            w=snprintf(p,(size_t)(end-p),
                "    <provenance> file=%s author=%s version=%s hexid=%s\n"
                "                 hash=%s seal=%s [%s]\n",
                pr->name,pr->author,pr->version,pr->hex_id,pr->hash,pr->seal,
                sl_reg_seal_verify(pr)?"SEALED":"BROKEN");
            if(w>0&&p+w<end)p+=w;
        }
    }
    return (long)(p-buf);
}

int sl_reg_key_count(const sl_registry *r){ int c=0; for(int i=0;i<r->nkeys;++i) if(r->keys[i].used)c++; return c; }
int sl_reg_value_count(const sl_registry *r){ int c=0; for(int i=0;i<r->nkeys;++i){ if(!r->keys[i].used)continue; for(int j=0;j<r->keys[i].nvalues;++j) if(r->keys[i].values[j].used)c++; } return c; }
int sl_reg_provenance_count(const sl_registry *r){ int c=0; for(int i=0;i<r->nkeys;++i) if(r->keys[i].used&&r->keys[i].has_prov)c++; return c; }

const char *sl_reg_status_name(sl_reg_status s){
    switch(s){
        case SL_REG_OK: return "OK"; case SL_REG_INVALID: return "INVALID";
        case SL_REG_NOT_FOUND: return "NOT_FOUND"; case SL_REG_EXISTS: return "EXISTS";
        case SL_REG_LIMIT: return "LIMIT"; case SL_REG_MALFORMED: return "MALFORMED";
        case SL_REG_SEAL_BROKEN: return "SEAL_BROKEN"; default: return "UNKNOWN";
    }
}
