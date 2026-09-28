#include <stdio.h>
#include <string.h>
#include "sleela_audio_mixer.h"
int main(void){
 SLAudioMixer m; slaudio_mixer_init(&m,48000,2);
 SLAudioMixerControls c={3.0f,-1.0f,2.0f,0.0f,0,{1,1,1,1,1,1,1,1}}; if(slaudio_mixer_set_controls(&m,&c))return 1;
 float a[8]={1,1,.5f,.5f,.25f,.25f,.1f,.1f},out[8]={0};
 SLAudioTrackSpec t={1,SL_AUDIO_ROLE_MASTER,SL_AUDIO_SOURCE_FILE,"master.wav",0,1,0,2,48000};
 SLAudioMixOutput o={0,0,0,0,0,out}; if(slaudio_mixer_process(&m,&t,a,4,0,&o))return 2;
 if(o.frames!=4||o.output_sequence!=1)return 3; puts("audio mixer: PASS"); return 0;
}
