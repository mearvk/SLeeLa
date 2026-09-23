#include <stdio.h>
#include <string.h>
#include "sleela_media.h"
int main(void){float a[8]={0,.5f,-.5f,.25f,-.25f,.1f,-.1f,0};unsigned char v[4]={128,128,128,128};SLMediaFrame af={1,1,SL_MEDIA_AUDIO,SL_MEDIA_PCM_F32,1,8000,0,0,0,sizeof(a),a};SLMediaFrame vf={2,2,SL_MEDIA_VIDEO,SL_MEDIA_GRAY8,0,0,2,2,2,sizeof(v),v};SLAudioAnalysis aa;SLVideoAnalysis vv;if(slmedia_audio_analyze(&af,&aa)!=SL_MEDIA_OK||aa.rms<=0)return 1;if(slmedia_video_analyze(&vf,&vv)!=SL_MEDIA_OK||vv.luminance<=0)return 2;puts("media: PASS");return 0;}