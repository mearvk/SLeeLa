#include "sleela_audio_mixer.h"
#include <math.h>
#include <string.h>

static float db_gain(float db){ return powf(10.0f, db/20.0f); }
static float clampf(float x,float lo,float hi){return x<lo?lo:(x>hi?hi:x);}
void slaudio_mixer_init(SLAudioMixer *m,uint32_t rate,uint32_t ch){
 if(!m)return; memset(m,0,sizeof(*m)); m->sample_rate=rate; m->channels=ch>8?8:ch;
 for(unsigned i=0;i<8;i++)m->controls.channel_gain[i]=1.0f;
}
int slaudio_mixer_set_controls(SLAudioMixer *m,const SLAudioMixerControls *c){
 if(!m||!c||!m->channels||m->channels>8)return -1;
 m->controls=*c; m->controls.pan=clampf(m->controls.pan,-1.0f,1.0f); return 0;
}
int slaudio_mixer_apply_eq(SLAudioMixer *m,float *s,size_t frames){
 if(!m||!s||!m->sample_rate||m->channels==0)return -1;
 /* Three deliberately simple real-time shelves. Providers may replace this
    processor with higher-order filters without changing the mixer ABI. */
 const float blp=expf(-2.0f*(float)M_PI*180.0f/(float)m->sample_rate);
 const float thp=expf(-2.0f*(float)M_PI*4200.0f/(float)m->sample_rate);
 float bg=db_gain(m->controls.bass_db), mg=db_gain(m->controls.mid_db), tg=db_gain(m->controls.treble_db);
 for(size_t f=0;f<frames;f++)for(uint32_t ch=0;ch<m->channels;ch++){
   size_t i=f*m->channels+ch; float x=s[i];
   float low=(1.0f-blp)*x+blp*m->bass_state[ch]; m->bass_state[ch]=low;
   float high=x-((1.0f-thp)*x+thp*m->treble_state[ch]); m->treble_state[ch]=(1.0f-thp)*x+thp*m->treble_state[ch];
   float mid=x-low-high;
   s[i]=(low*bg+mid*mg+high*tg)*db_gain(m->controls.gain_db);
   if(ch<8)s[i]*=m->controls.channel_gain[ch];
 }
 return 0;
}
int slaudio_mixer_process(SLAudioMixer *m,const SLAudioTrackSpec *t,const float *in,size_t frames,uint64_t ts,SLAudioMixOutput *o){
 if(!m||!t||!in||!o||!o->samples||!m->channels||t->channels!=m->channels||t->sample_rate!=m->sample_rate)return -1;
 if(t->start_offset_ns>0 && ts<(uint64_t)t->start_offset_ns){o->frames=0;return 0;}
 size_t n=frames*m->channels; memcpy(o->samples,in,n*sizeof(float));
 if(t->gain_db!=0){float g=db_gain(t->gain_db);for(size_t i=0;i<n;i++)o->samples[i]*=g;}
 if(slaudio_mixer_apply_eq(m,o->samples,frames)!=0)return -1;
 o->timestamp_ns=ts;o->sample_rate=m->sample_rate;o->channels=m->channels;o->frames=frames;o->output_sequence=++m->sequence;return 0;
}
int slaudio_mixer_mix(SLAudioMixer *m,const SLAudioTrackSpec *t,size_t count,const float *const *in,const size_t *fc,uint64_t timeline,SLAudioMixOutput *o){
 if(!m||!t||!in||!fc||!o||!o->samples||!count)return -1;
 size_t max=0;for(size_t i=0;i<count;i++)if(fc[i]>max)max=fc[i];
 memset(o->samples,0,max*m->channels*sizeof(float));
 for(size_t k=0;k<count;k++){
   if(!in[k]||!fc[k]||t[k].sample_rate!=m->sample_rate||t[k].channels!=m->channels)continue;
   int64_t off=t[k].start_offset_ns; if(off<0)off=0;
   uint64_t start=(uint64_t)off;
   size_t skip=start>timeline?(size_t)(((start-timeline)*(uint64_t)m->sample_rate)/1000000000ULL):0;
   size_t usable=fc[k]>skip?fc[k]-skip:0;
   size_t frames=usable<max?usable:max;
   float g=db_gain(t[k].gain_db)*t[k].quality;
   if(g<0)g=0;
   for(size_t f=0;f<frames;f++)for(uint32_t ch=0;ch<m->channels;ch++)o->samples[f*m->channels+ch]+=in[k][(f+skip)*m->channels+ch]*g;
 }
 o->timestamp_ns=timeline;o->sample_rate=m->sample_rate;o->channels=m->channels;o->frames=max;o->output_sequence=++m->sequence;
 if(slaudio_mixer_apply_eq(m,o->samples,max)!=0)return -1; return 0;
}
int slaudio_mixer_role_name(SLAudioTrackRole r,const char **n){if(!n)return -1;*n=r==SL_AUDIO_ROLE_MASTER?"master":r==SL_AUDIO_ROLE_SECONDARY?"second":r==SL_AUDIO_ROLE_INPUT?"input":"unknown";return 0;}
int slaudio_mixer_source_name(SLAudioSourceKind k,const char **n){if(!n)return -1;*n=k==SL_AUDIO_SOURCE_LIVE?"live":k==SL_AUDIO_SOURCE_FILE?"file":"unknown";return 0;}
