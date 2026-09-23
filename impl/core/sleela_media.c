#include "sleela_media.h"
#include <math.h>
int slmedia_validate_frame(const SLMediaFrame *f) {
 if(!f||!f->data||!f->bytes)return 0;
 if(f->kind==SL_MEDIA_AUDIO)return f->format==SL_MEDIA_PCM_F32&&f->channels&&f->sample_rate;
 if(f->kind==SL_MEDIA_VIDEO)return (f->format==SL_MEDIA_GRAY8||f->format==SL_MEDIA_RGB24||f->format==SL_MEDIA_RGBA32)&&f->width&&f->height&&f->stride;
 return 0;
}
int slmedia_audio_analyze(const SLMediaFrame *f,SLAudioAnalysis *o){
 if(!o||!slmedia_validate_frame(f)||f->kind!=SL_MEDIA_AUDIO)return SL_MEDIA_INVALID;
 size_t n=f->bytes/(sizeof(float)*f->channels);if(!n)return SL_MEDIA_INVALID;
 const float*x=(const float*)f->data;double sum=0,peak=0;
 for(size_t i=0;i<n*f->channels;i++){double v=fabs((double)x[i]);sum+=(double)x[i]*x[i];if(v>peak)peak=v;}
 o->peak=(float)peak;o->rms=(float)sqrt(sum/(double)(n*f->channels));o->level=o->rms;o->dominant_hz=0;o->bins=0;o->spectrum=0;return SL_MEDIA_OK;
}
int slmedia_video_analyze(const SLMediaFrame *f,SLVideoAnalysis *o){
 if(!o||!slmedia_validate_frame(f)||f->kind!=SL_MEDIA_VIDEO)return SL_MEDIA_INVALID;
 const unsigned char*p=(const unsigned char*)f->data;uint64_t sum=0,edges=0;size_t bpp=f->format==SL_MEDIA_GRAY8?1:(f->format==SL_MEDIA_RGB24?3:4);
 for(uint32_t y=0;y<f->height;y++){const unsigned char*r=p+(size_t)y*f->stride;for(uint32_t x=0;x<f->width;x++){unsigned char v=r[(size_t)x*bpp];sum+=v;if(x&&v>r[(size_t)(x-1)*bpp]+16)edges++;}}
 o->width=f->width;o->height=f->height;o->luminance=(float)sum/(255.0f*f->width*f->height);o->motion=0;o->edge_density=(float)edges/(float)(f->width*f->height);return SL_MEDIA_OK;
}
int slmedia_provider_matches(const SLMediaProviderInfo *p,SLMediaCodec c,SLMediaContainer k,SLMediaImageFormat i,int encode){
 if(!p)return 0;if(encode&&!p->encode)return 0;if(!encode&&!p->decode)return 0;
 if(c!=SL_MEDIA_CODEC_UNKNOWN&&p->codec!=c)return 0;
 if(k!=SL_MEDIA_CONTAINER_UNKNOWN&&p->container!=k)return 0;
 if(i!=SL_MEDIA_IMAGE_UNKNOWN&&p->image_format!=i)return 0;
 return 1;
}
const char*slmedia_kind_name(SLMediaKind k){return k==SL_MEDIA_AUDIO?"audio":k==SL_MEDIA_VIDEO?"video":"unknown";}
const char*slmedia_format_name(SLMediaFormat f){switch(f){case SL_MEDIA_PCM_F32:return "pcm-f32";case SL_MEDIA_GRAY8:return "gray8";case SL_MEDIA_RGB24:return "rgb24";case SL_MEDIA_RGBA32:return "rgba32";default:return "unknown";}}
