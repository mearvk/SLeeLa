#include <stdio.h>
#include "sleela_media.h"
int main(void){
 float a[8]={0,.5f,-.5f,.25f,-.25f,.1f,-.1f,0};
 unsigned char v[4]={128,128,128,128};
 SLMediaFrame af={1,1,SL_MEDIA_AUDIO,SL_MEDIA_PCM_F32,1,8000,0,0,0,sizeof(a),a};
 SLMediaFrame vf={2,2,SL_MEDIA_VIDEO,SL_MEDIA_GRAY8,0,0,2,2,2,sizeof(v),v};
 SLAudioAnalysis aa; SLVideoAnalysis vv;
 SLMediaProviderInfo p={SL_MEDIA_CODEC_H264,SL_MEDIA_CONTAINER_MP4,SL_MEDIA_IMAGE_UNKNOWN,1,1};
 if(slmedia_audio_analyze(&af,&aa)!=SL_MEDIA_OK||aa.rms<=0)return 1;
 if(slmedia_video_analyze(&vf,&vv)!=SL_MEDIA_OK||vv.luminance<=0)return 2;
 if(slmedia_codec_from_name("H.264")!=SL_MEDIA_CODEC_H264)return 3;
 if(slmedia_codec_from_name("opus")!=SL_MEDIA_CODEC_OPUS)return 4;
 if(slmedia_image_format_from_extension(".jpeg")!=SL_MEDIA_IMAGE_JPEG)return 5;
 if(slmedia_container_from_extension(".webm")!=SL_MEDIA_CONTAINER_WEBM)return 6;
 if(!slmedia_provider_matches(&p,SL_MEDIA_CODEC_H264,SL_MEDIA_CONTAINER_MP4,SL_MEDIA_IMAGE_UNKNOWN,1))return 7;
 puts("media: PASS"); return 0;
}
