#include "sleela_media_formats.h"
#include <ctype.h>
#include <string.h>

typedef struct { SLMediaCodec id; const char *name; int audio; int video; int lossless; } CodecEntry;
static const CodecEntry CODECS[] = {
 {SL_MEDIA_CODEC_PCM,"pcm",1,0,1},{SL_MEDIA_CODEC_AAC,"aac",1,0,0},
 {SL_MEDIA_CODEC_ALAC,"alac",1,0,1},{SL_MEDIA_CODEC_AMR,"amr",1,0,0},
 {SL_MEDIA_CODEC_FLAC,"flac",1,0,1},{SL_MEDIA_CODEC_G711,"g711",1,0,0},
 {SL_MEDIA_CODEC_G722,"g722",1,0,0},{SL_MEDIA_CODEC_MP3,"mp3",1,0,0},
 {SL_MEDIA_CODEC_OPUS,"opus",1,0,0},{SL_MEDIA_CODEC_VORBIS,"vorbis",1,0,0},
 {SL_MEDIA_CODEC_H263,"h263",0,1,0},{SL_MEDIA_CODEC_H264,"h264",0,1,0},
 {SL_MEDIA_CODEC_HEVC,"hevc",0,1,0},{SL_MEDIA_CODEC_AV1,"av1",0,1,0},
 {SL_MEDIA_CODEC_MPEG1VIDEO,"mpeg1video",0,1,0},{SL_MEDIA_CODEC_MPEG2VIDEO,"mpeg2video",0,1,0},
 {SL_MEDIA_CODEC_MPEG4VIDEO,"mpeg4",0,1,0},{SL_MEDIA_CODEC_THEORA,"theora",0,1,0},
 {SL_MEDIA_CODEC_VP8,"vp8",0,1,0},{SL_MEDIA_CODEC_VP9,"vp9",0,1,0},
 {SL_MEDIA_CODEC_FFV1,"ffv1",0,1,1}
};

static int eq(const char *a,const char *b) {
 if(!a||!b)return 0;
 while(*a&&*b) { if(tolower((unsigned char)*a)!=tolower((unsigned char)*b))return 0; ++a;++b; }
 return *a==0&&*b==0;
}
const char *slmedia_codec_name(SLMediaCodec c){for(size_t i=0;i<sizeof(CODECS)/sizeof(CODECS[0]);++i)if(CODECS[i].id==c)return CODECS[i].name;return "unknown";}
SLMediaCodec slmedia_codec_from_name(const char *n){
 if(eq(n,"h.264")||eq(n,"avc"))return SL_MEDIA_CODEC_H264;
 if(eq(n,"h.265")||eq(n,"hevc"))return SL_MEDIA_CODEC_HEVC;
 if(eq(n,"avc1"))return SL_MEDIA_CODEC_H264;
 if(eq(n,"av01"))return SL_MEDIA_CODEC_AV1;
 for(size_t i=0;i<sizeof(CODECS)/sizeof(CODECS[0]);++i)if(eq(n,CODECS[i].name))return CODECS[i].id;
 return SL_MEDIA_CODEC_UNKNOWN;
}
int slmedia_codec_is_audio(SLMediaCodec c){for(size_t i=0;i<sizeof(CODECS)/sizeof(CODECS[0]);++i)if(CODECS[i].id==c)return CODECS[i].audio;return 0;}
int slmedia_codec_is_video(SLMediaCodec c){for(size_t i=0;i<sizeof(CODECS)/sizeof(CODECS[0]);++i)if(CODECS[i].id==c)return CODECS[i].video;return 0;}
int slmedia_codec_is_lossless(SLMediaCodec c){for(size_t i=0;i<sizeof(CODECS)/sizeof(CODECS[0]);++i)if(CODECS[i].id==c)return CODECS[i].lossless;return 0;}

const char *slmedia_container_name(SLMediaContainer c){switch(c){
case SL_MEDIA_CONTAINER_WAV:return "wav";case SL_MEDIA_CONTAINER_AIFF:return "aiff";case SL_MEDIA_CONTAINER_FLAC:return "flac";
case SL_MEDIA_CONTAINER_MP3:return "mp3";case SL_MEDIA_CONTAINER_MP4:return "mp4";case SL_MEDIA_CONTAINER_MKV:return "mkv";
case SL_MEDIA_CONTAINER_WEBM:return "webm";case SL_MEDIA_CONTAINER_OGG:return "ogg";case SL_MEDIA_CONTAINER_MPEGTS:return "mpeg-ts";
case SL_MEDIA_CONTAINER_MOV:return "mov";case SL_MEDIA_CONTAINER_AVI:return "avi";case SL_MEDIA_CONTAINER_3GP:return "3gp";
case SL_MEDIA_CONTAINER_ADTS:return "adts";default:return "unknown";}}
SLMediaContainer slmedia_container_from_extension(const char *e){
 if(!e)return SL_MEDIA_CONTAINER_UNKNOWN;
 if(e[0]=='.')++e;
 if(eq(e,"wav"))return SL_MEDIA_CONTAINER_WAV;if(eq(e,"aif")||eq(e,"aiff"))return SL_MEDIA_CONTAINER_AIFF;
 if(eq(e,"flac"))return SL_MEDIA_CONTAINER_FLAC;if(eq(e,"mp3"))return SL_MEDIA_CONTAINER_MP3;
 if(eq(e,"mp4")||eq(e,"m4v"))return SL_MEDIA_CONTAINER_MP4;if(eq(e,"mkv"))return SL_MEDIA_CONTAINER_MKV;
 if(eq(e,"webm"))return SL_MEDIA_CONTAINER_WEBM;if(eq(e,"ogg")||eq(e,"ogv"))return SL_MEDIA_CONTAINER_OGG;
 if(eq(e,"ts")||eq(e,"m2ts"))return SL_MEDIA_CONTAINER_MPEGTS;if(eq(e,"mov"))return SL_MEDIA_CONTAINER_MOV;
 if(eq(e,"avi"))return SL_MEDIA_CONTAINER_AVI;if(eq(e,"3gp"))return SL_MEDIA_CONTAINER_3GP;
 if(eq(e,"aac")||eq(e,"adts"))return SL_MEDIA_CONTAINER_ADTS;return SL_MEDIA_CONTAINER_UNKNOWN;
}

const char *slmedia_image_format_name(SLMediaImageFormat f){switch(f){
case SL_MEDIA_IMAGE_BMP:return "bmp";case SL_MEDIA_IMAGE_JPEG:return "jpeg";case SL_MEDIA_IMAGE_PNG:return "png";
case SL_MEDIA_IMAGE_GIF:return "gif";case SL_MEDIA_IMAGE_APNG:return "apng";case SL_MEDIA_IMAGE_WEBP:return "webp";
case SL_MEDIA_IMAGE_TIFF:return "tiff";case SL_MEDIA_IMAGE_JPEG2000:return "jpeg2000";case SL_MEDIA_IMAGE_JPEGLS:return "jpeg-ls";
case SL_MEDIA_IMAGE_AVIF:return "avif";case SL_MEDIA_IMAGE_HEIF:return "heif";case SL_MEDIA_IMAGE_EXR:return "exr";
case SL_MEDIA_IMAGE_HDR:return "hdr";case SL_MEDIA_IMAGE_PAM:return "pam";case SL_MEDIA_IMAGE_PNM:return "pnm";
case SL_MEDIA_IMAGE_DPX:return "dpx";default:return "unknown";}}
SLMediaImageFormat slmedia_image_format_from_extension(const char *e){
 if(!e)return SL_MEDIA_IMAGE_UNKNOWN;if(e[0]=='.')++e;
 if(eq(e,"bmp"))return SL_MEDIA_IMAGE_BMP;if(eq(e,"jpg")||eq(e,"jpeg"))return SL_MEDIA_IMAGE_JPEG;
 if(eq(e,"png"))return SL_MEDIA_IMAGE_PNG;if(eq(e,"gif"))return SL_MEDIA_IMAGE_GIF;if(eq(e,"apng"))return SL_MEDIA_IMAGE_APNG;
 if(eq(e,"webp"))return SL_MEDIA_IMAGE_WEBP;if(eq(e,"tif")||eq(e,"tiff"))return SL_MEDIA_IMAGE_TIFF;
 if(eq(e,"jp2")||eq(e,"j2k"))return SL_MEDIA_IMAGE_JPEG2000;if(eq(e,"jls"))return SL_MEDIA_IMAGE_JPEGLS;
 if(eq(e,"avif"))return SL_MEDIA_IMAGE_AVIF;if(eq(e,"heif")||eq(e,"heic"))return SL_MEDIA_IMAGE_HEIF;
 if(eq(e,"exr"))return SL_MEDIA_IMAGE_EXR;if(eq(e,"hdr"))return SL_MEDIA_IMAGE_HDR;
 if(eq(e,"pam"))return SL_MEDIA_IMAGE_PAM;if(eq(e,"ppm")||eq(e,"pgm")||eq(e,"pbm")||eq(e,"pnm"))return SL_MEDIA_IMAGE_PNM;
 if(eq(e,"dpx"))return SL_MEDIA_IMAGE_DPX;return SL_MEDIA_IMAGE_UNKNOWN;
}
