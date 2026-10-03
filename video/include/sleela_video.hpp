#ifndef SLEELA_VIDEO_HPP
#define SLEELA_VIDEO_HPP
#include "sleela_video.h"
#include <cstdint>
namespace sleela::video { enum class PixelFormat{Gray8=0,RGB24,RGBA32,YUV420P,YUV422P,YUV444P,NV12,P010}; struct Limits{sleela_video_limits raw{3840,2160,4,64U*1024U*1024U};}; struct Frame{std::uint32_t width{},height{};PixelFormat format{PixelFormat::Gray8};sleela_video_frame raw{};Frame(std::uint32_t w,std::uint32_t h,PixelFormat f):width(w),height(h),format(f){raw.width=w;raw.height=h;raw.format=static_cast<sleela_video_pixel_format>(f);} bool valid(const Limits&l)const noexcept{return sleela_video_frame_validate(&raw,&l.raw)!=0;}}; inline const char*pixel_format_name(PixelFormat f)noexcept{return sleela_video_pixel_format_name(static_cast<sleela_video_pixel_format>(f));} }
#endif
