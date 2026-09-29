#include "sleela_video.hpp"
#include "sleela_video_codecs.hpp"
#include <cassert>
#include <cstdint>
int main(){std::uint8_t p=0;sleela::video::Frame f(16,16,sleela::video::PixelFormat::Gray8);f.raw.plane_count=1;f.raw.planes[0]=&p;f.raw.strides[0]=1;f.raw.sizes[0]=1;f.raw.timebase_num=1;f.raw.timebase_den=30;assert(f.valid({{64,64,4,1024}}));assert(sleela::video::codecs::by_extension(".av1")->id==SLEELA_VIDEO_CODEC_AV1);return 0;}
