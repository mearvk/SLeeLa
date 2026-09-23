#ifndef SLEELA_MEDIA_H
#define SLEELA_MEDIA_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum { SL_MEDIA_AUDIO=1, SL_MEDIA_VIDEO=2 } SLMediaKind;
typedef enum { SL_MEDIA_PCM_F32=1, SL_MEDIA_GRAY8=2, SL_MEDIA_RGB24=3, SL_MEDIA_RGBA32=4 } SLMediaFormat;
typedef struct { uint64_t sequence; uint64_t timestamp_ns; SLMediaKind kind; SLMediaFormat format; uint32_t channels; uint32_t sample_rate; uint32_t width; uint32_t height; uint32_t stride; size_t bytes; const void *data; } SLMediaFrame;
typedef struct { float level; float peak; float rms; float dominant_hz; uint32_t bins; const float *spectrum; } SLAudioAnalysis;
typedef struct { uint32_t width; uint32_t height; float motion; float luminance; float edge_density; } SLVideoAnalysis;
typedef enum { SL_MEDIA_OK=0, SL_MEDIA_INVALID=-1, SL_MEDIA_UNSUPPORTED=-2, SL_MEDIA_NO_PROVIDER=-3 } SLMediaStatus;
typedef struct { uint64_t id; SLMediaKind kind; int active; uint32_t rate_or_fps; uint32_t channels_or_planes; } SLMediaStream;
typedef int (*SLMediaFrameListener)(const SLMediaFrame *frame, void *context);
typedef int (*SLMediaTransform)(const SLMediaFrame *input, SLMediaFrame *output, void *context);
typedef int (*SLMediaAIHook)(const SLMediaFrame *input, SLMediaFrame *output, void *context);
int slmedia_validate_frame(const SLMediaFrame *frame);
int slmedia_audio_analyze(const SLMediaFrame *frame, SLAudioAnalysis *out);
int slmedia_video_analyze(const SLMediaFrame *frame, SLVideoAnalysis *out);
const char *slmedia_kind_name(SLMediaKind kind);
const char *slmedia_format_name(SLMediaFormat format);
#ifdef __cplusplus
}
#endif
#endif