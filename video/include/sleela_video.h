#ifndef SLEELA_VIDEO_H
#define SLEELA_VIDEO_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum sleela_video_pixel_format { SLEELA_VIDEO_PIXFMT_GRAY8=0,SLEELA_VIDEO_PIXFMT_RGB24,SLEELA_VIDEO_PIXFMT_RGBA32,SLEELA_VIDEO_PIXFMT_YUV420P,SLEELA_VIDEO_PIXFMT_YUV422P,SLEELA_VIDEO_PIXFMT_YUV444P,SLEELA_VIDEO_PIXFMT_NV12,SLEELA_VIDEO_PIXFMT_P010,SLEELA_VIDEO_PIXFMT_COUNT } sleela_video_pixel_format;
typedef struct sleela_video_limits { uint32_t max_width,max_height,max_planes; size_t max_bytes; } sleela_video_limits;
typedef struct sleela_video_frame { uint32_t width,height; sleela_video_pixel_format format; uint32_t plane_count; const uint8_t *planes[4]; size_t strides[4],sizes[4]; int64_t pts; uint32_t timebase_num,timebase_den; } sleela_video_frame;
int sleela_video_frame_validate(const sleela_video_frame*,const sleela_video_limits*);
const char *sleela_video_pixel_format_name(sleela_video_pixel_format);
size_t sleela_video_required_plane_count(sleela_video_pixel_format);
#ifdef __cplusplus
}
#endif
#endif
