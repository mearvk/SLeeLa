#include "Audio.h"
#include <stdio.h>
#include <string.h>
static void set_error(char *e,size_t n,const char *m){if(e&&n)snprintf(e,n,"%s",m);}
int sleela_audio_validate_input(const SleelaAudioInput *i){return i&&i->path&&i->path[0]&&i->start_seconds>=0.0;}
int sleela_audio_validate_controls(const SleelaAudioControls *c){return c&&c->pan>=-1.0&&c->pan<=1.0&&c->left_gain>=0.0&&c->right_gain>=0.0;}
int sleela_audio_validate_configuration(const SleelaAudioConfiguration *c){
 if(!c||c->sample_rate<=0||!c->output_path||!c->output_path[0]||!c->inputs||c->input_count==0||c->input_count>128)return 0;
 if(!sleela_audio_validate_controls(&c->controls))return 0;
 for(size_t i=0;i<c->input_count;i++)if(!sleela_audio_validate_input(&c->inputs[i]))return 0;
 return 1;
}
int sleela_audio_mix(const SleelaAudioConfiguration *c,char *e,size_t n){if(!sleela_audio_validate_configuration(c)){set_error(e,n,"invalid SLeeLa Audio configuration");return 0;}return 1;}
int sleela_audio_native_render(const char *x,const SleelaAudioConfiguration *c){return x&&x[0]&&sleela_audio_validate_configuration(c);}
void sleela_audio_native_interrupt(void){}
int sleela_audio_device_available(const SleelaAudioDevice *d){return d&&d->identifier[0]!='\0';}
int sleela_audio_stream_open(SleelaAudioStream *s){if(!s||!sleela_audio_device_available(&s->device)||s->sample_rate<=0||s->channels<=0)return 0;s->opened=1;s->running=0;return 1;}
int sleela_audio_stream_start(SleelaAudioStream *s){if(!s||!s->opened)return 0;s->running=1;return 1;}
void sleela_audio_stream_stop(SleelaAudioStream *s){if(s)s->running=0;}
void sleela_audio_stream_close(SleelaAudioStream *s){if(s){s->running=0;s->opened=0;}}
void sleela_audio_system_init(SleelaAudioSystem *s){
 if(!s)return;
#if defined(_WIN32)
 snprintf(s->platform,sizeof(s->platform),"windows");
#elif defined(__APPLE__)
 snprintf(s->platform,sizeof(s->platform),"macos");
#elif defined(__linux__)
 snprintf(s->platform,sizeof(s->platform),"linux");
#else
 snprintf(s->platform,sizeof(s->platform),"unknown");
#endif
}
size_t sleela_audio_system_enumerate(const SleelaAudioSystem *s,SleelaAudioDevice *d,size_t n){(void)s;(void)d;(void)n;return 0;}
const char *sleela_audio_system_platform(const SleelaAudioSystem *s){return s?s->platform:"unknown";}
