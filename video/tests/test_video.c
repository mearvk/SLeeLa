#include "sleela_video.h"
#include "sleela_video_codecs.h"
#include <assert.h>
#include <stdint.h>
int main(void){uint8_t p=0;sleela_video_limits l={64,64,4,1024};sleela_video_frame f={1,1,SLEELA_VIDEO_PIXFMT_GRAY8,1,{&p},{1},{1},0,1,30};assert(sleela_video_frame_validate(&f,&l));assert(sleela_video_codec_count()==SLEELA_VIDEO_CODEC_COUNT);assert(sleela_video_codec_by_extension(".H264")->id==SLEELA_VIDEO_CODEC_H264);return 0;}
