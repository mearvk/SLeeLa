#include "sleela_audio.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
namespace {
thread_local std::string error;
struct Wav { uint32_t rate=0; uint16_t channels=0; std::vector<int16_t> pcm; };
uint16_t u16(const unsigned char*p){return uint16_t(p[0])|(uint16_t(p[1])<<8);}
uint32_t u32(const unsigned char*p){return uint32_t(p[0])|(uint32_t(p[1])<<8)|(uint32_t(p[2])<<16)|(uint32_t(p[3])<<24);}
bool read_wav(const char*path,Wav&w){
 std::ifstream f(path,std::ios::binary); if(!f){error="cannot open input";return false;}
 std::vector<unsigned char>b((std::istreambuf_iterator<char>(f)),{});
 if(b.size()<44||std::memcmp(b.data(),"RIFF",4)||std::memcmp(b.data()+8,"WAVE",4)){error="unsupported WAV";return false;}
 size_t p=12; uint16_t bits=0; bool fmt=false,data=false;
 while(p+8<=b.size()){uint32_t n=u32(b.data()+p+4);if(p+8+n>b.size())break;
  if(!std::memcmp(b.data()+p,"fmt ",4)&&n>=16){w.channels=u16(b.data()+p+10);w.rate=u32(b.data()+p+12);bits=u16(b.data()+p+22);fmt=true;}
  if(!std::memcmp(b.data()+p,"data",4)&&bits==16){w.pcm.resize(n/2);std::memcpy(w.pcm.data(),b.data()+p+8,n);data=true;}
  p+=8+n+(n&1);
 }
 if(!fmt||!data||w.channels<1||w.channels>2||bits!=16||!w.rate){error="only PCM16 mono/stereo WAV is supported";return false;} return true;
}
double db(double x){return std::pow(10.0,x/20.0);}
void put16(std::ofstream&f,uint16_t x){char b[2]={char(x),char(x>>8)};f.write(b,2);}
void put32(std::ofstream&f,uint32_t x){char b[4]={char(x),char(x>>8),char(x>>16),char(x>>24)};f.write(b,4);}
}
extern "C" int sleela_audio_validate(const sleela_audio_config*c){
 if(!c||!c->output_path||!*c->output_path||!c->sample_rate||!c->inputs||!c->input_count||c->input_count>128){error="invalid configuration";return 0;}
 if(!std::isfinite(c->controls.pan)||c->controls.pan<-1||c->controls.pan>1){error="pan out of range";return 0;}
 for(uint32_t i=0;i<c->input_count;i++)if(!c->inputs[i].path||!*c->inputs[i].path||c->inputs[i].start_seconds<0||!std::isfinite(c->inputs[i].gain_db)){error="invalid input";return 0;}
 return 1;
}
extern "C" int sleela_audio_mix_wav(const sleela_audio_config*c){
 if(!sleela_audio_validate(c))return 0;
 std::vector<Wav>w(c->input_count);size_t frames=0;
 for(uint32_t i=0;i<c->input_count;i++){if(!read_wav(c->inputs[i].path,w[i]))return 0;if(w[i].rate!=c->sample_rate){error="sample-rate mismatch";return 0;}frames=std::max(frames,size_t(std::llround(c->inputs[i].start_seconds*c->sample_rate))+w[i].pcm.size()/w[i].channels);}
 std::vector<int16_t>out(frames*2);double lg=db(c->controls.master_gain_db)*c->controls.left_gain*(c->controls.pan>0?1-c->controls.pan:1),rg=db(c->controls.master_gain_db)*c->controls.right_gain*(c->controls.pan<0?1+c->controls.pan:1);
 for(uint32_t i=0;i<c->input_count;i++){size_t off=size_t(std::llround(c->inputs[i].start_seconds*c->sample_rate));double g=db(c->inputs[i].gain_db);size_t n=w[i].pcm.size()/w[i].channels;
  for(size_t f=0;f<n;f++){double l=w[i].pcm[f*w[i].channels]*g,r=w[i].channels==2?w[i].pcm[f*2+1]*g:l;size_t j=(off+f)*2;if(j+1<out.size()){out[j]=int16_t(std::clamp(double(out[j])+l*lg,-32768.0,32767.0));out[j+1]=int16_t(std::clamp(double(out[j+1])+r*rg,-32768.0,32767.0));}}}
 std::ofstream f(c->output_path,std::ios::binary);if(!f){error="cannot open output";return 0;}uint32_t bytes=uint32_t(out.size()*2);
 f.write("RIFF",4);put32(f,36+bytes);f.write("WAVEfmt ",8);put32(f,16);put16(f,1);put16(f,2);put32(f,c->sample_rate);put32(f,c->sample_rate*4);put16(f,4);put16(f,16);f.write("data",4);put32(f,bytes);f.write(reinterpret_cast<const char*>(out.data()),bytes);return f.good();
}
extern "C" const char* sleela_audio_last_error(void){return error.c_str();}
