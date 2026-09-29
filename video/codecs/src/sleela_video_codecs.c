#include "sleela_video_codecs.h"
#include <ctype.h>
#define C(i,n,m,e,s,d,x){i,n,m,e,s,d,x}
static const sleela_video_codec r[]={
C(SLEELA_VIDEO_CODEC_RAW,"Raw Video","video/raw",".yuv,.rgb",SLEELA_VIDEO_CODEC_NATIVE,1,1),
C(SLEELA_VIDEO_CODEC_H264,"H.264/AVC","video/h264",".h264,.264",SLEELA_VIDEO_CODEC_BACKEND,1,1),
C(SLEELA_VIDEO_CODEC_HEVC,"H.265/HEVC","video/hevc",".h265,.hevc",SLEELA_VIDEO_CODEC_BACKEND,1,1),
C(SLEELA_VIDEO_CODEC_AV1,"AV1","video/AV1",".av1",SLEELA_VIDEO_CODEC_BACKEND,1,1),
C(SLEELA_VIDEO_CODEC_VP8,"VP8","video/vp8",".ivf,.webm",SLEELA_VIDEO_CODEC_BACKEND,1,1),
C(SLEELA_VIDEO_CODEC_VP9,"VP9","video/vp9",".ivf,.webm",SLEELA_VIDEO_CODEC_BACKEND,1,1),
C(SLEELA_VIDEO_CODEC_MPEG2,"MPEG-2 Video","video/mpeg",".mpg,.mpeg,.m2v",SLEELA_VIDEO_CODEC_BACKEND,1,1),
C(SLEELA_VIDEO_CODEC_THEORA,"Theora","video/ogg",".ogv",SLEELA_VIDEO_CODEC_BACKEND,1,1),
C(SLEELA_VIDEO_CODEC_MJPEG,"Motion JPEG","video/mjpeg",".mjpg,.mjpeg",SLEELA_VIDEO_CODEC_BACKEND,1,1),
C(SLEELA_VIDEO_CODEC_FFV1,"FFV1","video/x-ffv1",".ffv1",SLEELA_VIDEO_CODEC_BACKEND,1,1),
C(SLEELA_VIDEO_CODEC_PRORES,"Apple ProRes","video/x-prores",".mov",SLEELA_VIDEO_CODEC_QUALIFIED_BACKEND,1,1),
C(SLEELA_VIDEO_CODEC_DNX,"DNxHD/DNxHR","video/x-dnx",".mxf,.mov",SLEELA_VIDEO_CODEC_QUALIFIED_BACKEND,1,1)};
size_t sleela_video_codec_count(void){return sizeof(r)/sizeof(r[0]);}
const sleela_video_codec*sleela_video_codec_at(size_t i){return i<sleela_video_codec_count()?&r[i]:0;}
const sleela_video_codec*sleela_video_codec_by_id(sleela_video_codec_id i){return i>=0&&i<SLEELA_VIDEO_CODEC_COUNT?&r[i]:0;}
static int eq(const char*a,const char*b){while(*a&&*b){if(tolower((unsigned char)*a)!=tolower((unsigned char)*b))return 0;a++;b++;}return!*a&&! *b;}
const sleela_video_codec*sleela_video_codec_by_extension(const char*e){if(!e||!*e)return 0;for(size_t i=0;i<sleela_video_codec_count();i++){const char*p=r[i].extensions;while(*p){char t[16];size_t n=0;while(*p&&*p!=','&&n<15)t[n++]=*p++;t[n]=0;if(eq(e,t))return&r[i];if(*p==',')p++;}}return 0;}
