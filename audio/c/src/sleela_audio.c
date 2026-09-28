#include "sleela_audio.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static char error_text[256];
static void err(const char*s){snprintf(error_text,sizeof error_text,"%s",s);}
static unsigned short u16(const unsigned char*p){return (unsigned short)(p[0]|(p[1]<<8));}
static unsigned long u32(const unsigned char*p){return (unsigned long)p[0]|((unsigned long)p[1]<<8)|((unsigned long)p[2]<<16)|((unsigned long)p[3]<<24);}
static double gain(double db){return pow(10.0,db/20.0);}
int sleela_audio_validate(const sleela_audio_config*c){
 if(!c||!c->output_path||!*c->output_path||!c->sample_rate||!c->inputs||!c->input_count||c->input_count>128){err("invalid configuration");return 0;}
 if(!isfinite(c->controls.pan)||c->controls.pan<-1||c->controls.pan>1){err("pan out of range");return 0;}
 for(uint32_t i=0;i<c->input_count;i++)if(!c->inputs[i].path||!*c->inputs[i].path||c->inputs[i].start_seconds<0||!isfinite(c->inputs[i].gain_db)){err("invalid input");return 0;}
 return 1;
}
int sleela_audio_mix_wav(const sleela_audio_config*c){
 if(!sleela_audio_validate(c))return 0;
 size_t max_frames=0; unsigned char **data=calloc(c->input_count,sizeof(*data)); size_t *bytes=calloc(c->input_count,sizeof(*bytes)); unsigned short *channels=calloc(c->input_count,sizeof(*channels));
 if(!data||!bytes||!channels){err("allocation failure");goto fail;}
 for(uint32_t i=0;i<c->input_count;i++){
  FILE*f=fopen(c->inputs[i].path,"rb"); if(!f){err("cannot open input");goto fail;} fseek(f,0,SEEK_END);long sz=ftell(f);fseek(f,0,SEEK_SET);
  if(sz<44){fclose(f);err("unsupported WAV");goto fail;} data[i]=malloc((size_t)sz); if(!data[i]){fclose(f);err("allocation failure");goto fail;}
  if(fread(data[i],1,(size_t)sz,f)!=(size_t)sz){fclose(f);err("read failure");goto fail;} fclose(f);
  if(memcmp(data[i],"RIFF",4)||memcmp(data[i]+8,"WAVE",4)){err("unsupported WAV");goto fail;}
  bytes[i]=(size_t)sz; size_t p=12; unsigned long rate=0,ds=0; unsigned short bits=0; int fmt=0,dat=0;
  while(p+8<=(size_t)sz){unsigned long n=u32(data[i]+p+4);if(p+8+n>(size_t)sz)break;if(!memcmp(data[i]+p,"fmt ",4)&&n>=16){channels[i]=u16(data[i]+p+10);rate=u32(data[i]+p+12);bits=u16(data[i]+p+22);fmt=1;}if(!memcmp(data[i]+p,"data",4)){ds=n;dat=1;}p+=8+n+(n&1);}
  if(!fmt||!dat||bits!=16||channels[i]<1||channels[i]>2||rate!=c->sample_rate){err("unsupported format or sample-rate mismatch");goto fail;}
  size_t start=(size_t)llround(c->inputs[i].start_seconds*c->sample_rate), frames=ds/(channels[i]*2);if(frames>SIZE_MAX-start){err("audio length overflow");goto fail;}if(start+frames>max_frames)max_frames=start+frames;
 }
 {
  if(max_frames>SIZE_MAX/2 || max_frames*2>SIZE_MAX/sizeof(int16_t)){err("output too large");goto fail;} size_t out_samples=max_frames*2; int16_t*out=calloc(out_samples,sizeof(*out)); if(!out){err("allocation failure");goto fail;}
  double lg=gain(c->controls.master_gain_db)*c->controls.left_gain*(c->controls.pan>0?1-c->controls.pan:1),rg=gain(c->controls.master_gain_db)*c->controls.right_gain*(c->controls.pan<0?1+c->controls.pan:1);
  for(uint32_t i=0;i<c->input_count;i++){size_t p=12,ds=0,doff=0;while(p+8<=bytes[i]){unsigned long n=u32(data[i]+p+4);if(!memcmp(data[i]+p,"data",4)){ds=n;doff=p+8;break;}p+=8+n+(n&1);}size_t frames=ds/(channels[i]*2),start=(size_t)llround(c->inputs[i].start_seconds*c->sample_rate);double g=gain(c->inputs[i].gain_db);for(size_t f=0;f<frames;f++){const unsigned char*q=data[i]+doff+f*channels[i]*2;double l=(int16_t)u16(q)*g,r=channels[i]==2?(int16_t)u16(q+2)*g:l;size_t j=(start+f)*2;double a=out[j]+l*lg,b=out[j+1]+r*rg;out[j]=(int16_t)(a>32767?32767:a<-32768?-32768:a);out[j+1]=(int16_t)(b>32767?32767:b<-32768?-32768:b);}}
  FILE*f=fopen(c->output_path,"wb");if(!f){free(out);err("cannot open output");goto fail;}unsigned long data_bytes=(unsigned long)(out_samples*2);unsigned char h[44]={'R','I','F','F',(unsigned char)((36+data_bytes)&255),(unsigned char)((36+data_bytes)>>8),(unsigned char)((36+data_bytes)>>16),(unsigned char)((36+data_bytes)>>24),'W','A','V','E','f','m','t',' ',16,0,0,0,1,0,2,0,(unsigned char)(c->sample_rate),(unsigned char)(c->sample_rate>>8),(unsigned char)(c->sample_rate>>16),(unsigned char)(c->sample_rate>>24),(unsigned char)(c->sample_rate*4),(unsigned char)(c->sample_rate*4>>8),(unsigned char)(c->sample_rate*4>>16),(unsigned char)(c->sample_rate*4>>24),4,0,16,0,'d','a','t','a',(unsigned char)data_bytes,(unsigned char)(data_bytes>>8),(unsigned char)(data_bytes>>16),(unsigned char)(data_bytes>>24)};fwrite(h,1,44,f);fwrite(out,1,data_bytes,f);int ok=!fclose(f);free(out);if(!ok){err("write failure");goto fail;}
 }
 for(uint32_t i=0;i<c->input_count;i++) free(data[i]);
 free(data);
 free(bytes);
 free(channels);
 return 1;
fail: if(data){for(uint32_t i=0;i<c->input_count;i++)free(data[i]);}free(data);free(bytes);free(channels);return 0;
}
const char*sleela_audio_last_error(void){return error_text;}
