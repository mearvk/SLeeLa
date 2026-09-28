#ifndef SLEELA_MEDIA_FORMATS_H
#define SLEELA_MEDIA_FORMATS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SL_MEDIA_CODEC_UNKNOWN = 0,
    SL_MEDIA_CODEC_PCM,
    SL_MEDIA_CODEC_AAC,
    SL_MEDIA_CODEC_ALAC,
    SL_MEDIA_CODEC_AMR,
    SL_MEDIA_CODEC_FLAC,
    SL_MEDIA_CODEC_G711,
    SL_MEDIA_CODEC_G722,
    SL_MEDIA_CODEC_MP3,
    SL_MEDIA_CODEC_OPUS,
    SL_MEDIA_CODEC_VORBIS,
    SL_MEDIA_CODEC_H263,
    SL_MEDIA_CODEC_H264,
    SL_MEDIA_CODEC_HEVC,
    SL_MEDIA_CODEC_AV1,
    SL_MEDIA_CODEC_MPEG1VIDEO,
    SL_MEDIA_CODEC_MPEG2VIDEO,
    SL_MEDIA_CODEC_MPEG4VIDEO,
    SL_MEDIA_CODEC_THEORA,
    SL_MEDIA_CODEC_VP8,
    SL_MEDIA_CODEC_VP9,
    SL_MEDIA_CODEC_FFV1
} SLMediaCodec;

typedef enum {
    SL_MEDIA_CONTAINER_UNKNOWN = 0,
    SL_MEDIA_CONTAINER_WAV,
    SL_MEDIA_CONTAINER_AIFF,
    SL_MEDIA_CONTAINER_FLAC,
    SL_MEDIA_CONTAINER_MP3,
    SL_MEDIA_CONTAINER_MP4,
    SL_MEDIA_CONTAINER_MKV,
    SL_MEDIA_CONTAINER_WEBM,
    SL_MEDIA_CONTAINER_OGG,
    SL_MEDIA_CONTAINER_MPEGTS,
    SL_MEDIA_CONTAINER_MOV,
    SL_MEDIA_CONTAINER_AVI,
    SL_MEDIA_CONTAINER_3GP,
    SL_MEDIA_CONTAINER_ADTS
} SLMediaContainer;

typedef enum {
    SL_MEDIA_IMAGE_UNKNOWN = 0,
    SL_MEDIA_IMAGE_BMP,
    SL_MEDIA_IMAGE_JPEG,
    SL_MEDIA_IMAGE_PNG,
    SL_MEDIA_IMAGE_GIF,
    SL_MEDIA_IMAGE_APNG,
    SL_MEDIA_IMAGE_WEBP,
    SL_MEDIA_IMAGE_TIFF,
    SL_MEDIA_IMAGE_JPEG2000,
    SL_MEDIA_IMAGE_JPEGLS,
    SL_MEDIA_IMAGE_AVIF,
    SL_MEDIA_IMAGE_HEIF,
    SL_MEDIA_IMAGE_EXR,
    SL_MEDIA_IMAGE_HDR,
    SL_MEDIA_IMAGE_PAM,
    SL_MEDIA_IMAGE_PNM,
    SL_MEDIA_IMAGE_DPX
} SLMediaImageFormat;

const char *slmedia_codec_name(SLMediaCodec codec);
const char *slmedia_container_name(SLMediaContainer container);
const char *slmedia_image_format_name(SLMediaImageFormat format);

SLMediaCodec slmedia_codec_from_name(const char *name);
SLMediaContainer slmedia_container_from_extension(const char *extension);
SLMediaImageFormat slmedia_image_format_from_extension(const char *extension);

int slmedia_codec_is_audio(SLMediaCodec codec);
int slmedia_codec_is_video(SLMediaCodec codec);
int slmedia_codec_is_lossless(SLMediaCodec codec);

#ifdef __cplusplus
}
#endif

#endif
