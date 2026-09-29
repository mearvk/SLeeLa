#ifndef SLEELA_VIDEO_CODECS_HPP
#define SLEELA_VIDEO_CODECS_HPP
#include "sleela_video_codecs.h"
#include <cstddef>
namespace sleela::video::codecs{using Id=sleela_video_codec_id;using Descriptor=sleela_video_codec;inline std::size_t count()noexcept{return sleela_video_codec_count();}inline const Descriptor*at(std::size_t i)noexcept{return sleela_video_codec_at(i);}inline const Descriptor*by_id(Id i)noexcept{return sleela_video_codec_by_id(i);}inline const Descriptor*by_extension(const char*e)noexcept{return sleela_video_codec_by_extension(e);}}
#endif
