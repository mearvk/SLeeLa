#include "sleela_audio.h"
#include <iostream>
int main(int argc,char**argv){
 if(argc<3){std::cerr<<"usage: sleela-audio-native OUTPUT INPUT.wav [INPUT.wav...]\n";return 2;}
 int n=argc-2;if(n>128)return 2; sleela_audio_input inputs[128]{};
 for(int i=0;i<n;i++){inputs[i].id=argv[i+2];inputs[i].path=argv[i+2];inputs[i].gain_db=0;}
 sleela_audio_config c{48000,inputs,(uint32_t)n,{0,0,0,0,0,1,1},argv[1]};
 if(!sleela_audio_mix_wav(&c)){std::cerr<<sleela_audio_last_error()<<"\n";return 1;} return 0;
}
