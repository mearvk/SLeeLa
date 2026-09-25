#include "sleela/decompiler/decompiler.hpp"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
namespace sleela::decompiler {
static std::string identity_digest(const std::vector<std::uint8_t>& b){std::uint64_t h=1469598103934665603ULL;for(auto x:b)h=(h^x)*1099511628211ULL;std::ostringstream s;s<<std::hex<<std::setw(16)<<std::setfill('0')<<h;return s.str();}
static std::uint16_t u16(const std::vector<std::uint8_t>&b,std::size_t p){return p+1<b.size()?std::uint16_t(b[p])|(std::uint16_t(b[p+1])<<8):0;}
static std::uint32_t u32(const std::vector<std::uint8_t>&b,std::size_t p){return p+3<b.size()?std::uint32_t(b[p])|(std::uint32_t(b[p+1])<<8)|(std::uint32_t(b[p+2])<<16)|(std::uint32_t(b[p+3])<<24):0;}
Artifact Artifact::open(const std::string&p){std::ifstream f(p,std::ios::binary);if(!f)throw std::runtime_error("unable to open artifact: "+p);std::vector<std::uint8_t>b((std::istreambuf_iterator<char>(f)),{});return from_bytes(b,p);}
Artifact Artifact::from_bytes(std::span<const std::uint8_t>b,std::string n){Artifact a;a.name_=std::move(n);a.bytes_.assign(b.begin(),b.end());a.sha256_=identity_digest(a.bytes_);
if(a.bytes_.size()>=2&&a.bytes_[0]=='M'&&a.bytes_[1]=='Z'){a.format_=Format::PE;if(a.bytes_.size()>=0x40){auto pe=u32(a.bytes_,0x3c);if(pe+6<=a.bytes_.size()&&a.bytes_[pe]=='P'&&a.bytes_[pe+1]=='E'&&a.bytes_[pe+2]==0&&a.bytes_[pe+3]==0){auto m=u16(a.bytes_,pe+4);if(m==0x14c)a.architecture_=Architecture::X86;else if(m==0x8664)a.architecture_=Architecture::X86_64;else if(m==0xaa64)a.architecture_=Architecture::ARM64;}}}
else if(a.bytes_.size()>=20&&a.bytes_[0]==0x7f&&a.bytes_[1]=='E'&&a.bytes_[2]=='L'&&a.bytes_[3]=='F'){a.format_=Format::ELF;auto m=u16(a.bytes_,18);if(m==3)a.architecture_=Architecture::X86;else if(m==62)a.architecture_=Architecture::X86_64;else if(m==40)a.architecture_=Architecture::ARM;else if(m==183)a.architecture_=Architecture::ARM64;}
else a.format_=Format::Raw;return a;}
Format Artifact::format()const noexcept{return format_;} Architecture Artifact::architecture()const noexcept{return architecture_;}
std::uint64_t Artifact::size()const noexcept{return bytes_.size();} const std::vector<std::uint8_t>& Artifact::bytes()const noexcept{return bytes_;}
const std::string& Artifact::sha256()const noexcept{return sha256_;} const std::string& Artifact::name()const noexcept{return name_;}
}