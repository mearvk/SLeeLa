#include "sleela/decompiler/decompiler.hpp"
#include "sleela/decompiler/elf.hpp"
#include "sleela/decompiler/archive.hpp"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
namespace sleela::decompiler {
static std::string sha256_digest(const std::vector<std::uint8_t>& input){
    std::uint32_t h[8]={0x6a09e667u,0xbb67ae85u,0x3c6ef372u,0xa54ff53au,0x510e527fu,0x9b05688cu,0x1f83d9abu,0x5be0cd19u};
    static const std::uint32_t k[64]={
        0x428a2f98u,0x71374491u,0xb5c0fbcfu,0xe9b5dba5u,0x3956c25bu,0x59f111f1u,0x923f82a4u,0xab1c5ed5u,
        0xd807aa98u,0x12835b01u,0x243185beu,0x550c7dc3u,0x72be5d74u,0x80deb1feu,0x9bdc06a7u,0xc19bf174u,
        0xe49b69c1u,0xefbe4786u,0x0fc19dc6u,0x240ca1ccu,0x2de92c6fu,0x4a7484aau,0x5cb0a9dcu,0x76f988dau,
        0x983e5152u,0xa831c66du,0xb00327c8u,0xbf597fc7u,0xc6e00bf3u,0xd5a79147u,0x06ca6351u,0x14292967u,
        0x27b70a85u,0x2e1b2138u,0x4d2c6dfcu,0x53380d13u,0x650a7354u,0x766a0abbu,0x81c2c92eu,0x92722c85u,
        0xa2bfe8a1u,0xa81a664bu,0xc24b8b70u,0xc76c51a3u,0xd192e819u,0xd6990624u,0xf40e3585u,0x106aa070u,
        0x19a4c116u,0x1e376c08u,0x2748774cu,0x34b0bcb5u,0x391c0cb3u,0x4ed8aa4au,0x5b9cca4fu,0x682e6ff3u,
        0x748f82eeu,0x78a5636fu,0x84c87814u,0x8cc70208u,0x90befffau,0xa4506cebu,0xbef9a3f7u,0xc67178f2u};
    auto rotr=[](std::uint32_t x,unsigned n){return (x>>n)|(x<<(32-n));};
    std::vector<std::uint8_t> b=input;b.push_back(0x80);while((b.size()%64)!=56)b.push_back(0);
    const std::uint64_t bits=std::uint64_t(input.size())*8;for(int i=7;i>=0;--i)b.push_back(std::uint8_t(bits>>(8*i)));
    for(std::size_t off=0;off<b.size();off+=64){
        std::uint32_t w[64]{};for(int i=0;i<16;++i)w[i]=std::uint32_t(b[off+i*4])<<24|std::uint32_t(b[off+i*4+1])<<16|std::uint32_t(b[off+i*4+2])<<8|b[off+i*4+3];
        for(int i=16;i<64;++i){auto s0=rotr(w[i-15],7)^rotr(w[i-15],18)^(w[i-15]>>3);auto s1=rotr(w[i-2],17)^rotr(w[i-2],19)^(w[i-2]>>10);w[i]=w[i-16]+s0+w[i-7]+s1;}
        auto a0=h[0],b0=h[1],c0=h[2],d0=h[3],e0=h[4],f0=h[5],g0=h[6],hh=h[7];
        for(int i=0;i<64;++i){auto S1=rotr(e0,6)^rotr(e0,11)^rotr(e0,25);auto ch=(e0&f0)^((~e0)&g0);auto t1=hh+S1+ch+k[i]+w[i];auto S0=rotr(a0,2)^rotr(a0,13)^rotr(a0,22);auto maj=(a0&b0)^(a0&c0)^(b0&c0);auto t2=S0+maj;hh=g0;g0=f0;f0=e0;e0=d0+t1;d0=c0;c0=b0;b0=a0;a0=t1+t2;}
        h[0]+=a0;h[1]+=b0;h[2]+=c0;h[3]+=d0;h[4]+=e0;h[5]+=f0;h[6]+=g0;h[7]+=hh;
    }
    std::ostringstream s;s<<std::hex<<std::setfill('0');for(auto x:h)s<<std::setw(8)<<x;return s.str();
}
static std::uint16_t u16(const std::vector<std::uint8_t>&b,std::size_t p){return p+1<b.size()?std::uint16_t(b[p])|(std::uint16_t(b[p+1])<<8):0;}
static std::uint32_t u32(const std::vector<std::uint8_t>&b,std::size_t p){return p+3<b.size()?std::uint32_t(b[p])|(std::uint32_t(b[p+1])<<8)|(std::uint32_t(b[p+2])<<16)|(std::uint32_t(b[p+3])<<24):0;}
static bool suffix(const std::string& n,const std::string& s){return n.size()>=s.size()&&n.compare(n.size()-s.size(),s.size(),s)==0;}
Artifact Artifact::open(const std::string&p){std::ifstream f(p,std::ios::binary);if(!f)throw std::runtime_error("unable to open artifact: "+p);std::vector<std::uint8_t>b((std::istreambuf_iterator<char>(f)),{});return from_bytes(b,p);}
Artifact Artifact::from_bytes(std::span<const std::uint8_t>b,std::string n){
    Artifact a;a.name_=std::move(n);a.bytes_.assign(b.begin(),b.end());a.sha256_=sha256_digest(a.bytes_);
    const bool named_ko=suffix(a.name_,".ko")||suffix(a.name_,".ko.xz")||suffix(a.name_,".ko.zst")||suffix(a.name_,".ko.gz");
    const bool named_so=suffix(a.name_,".so")||a.name_.find(".so.")!=std::string::npos;
    const bool named_a=suffix(a.name_,".a");
    const bool named_o=suffix(a.name_,".o");
    if(a.bytes_.size()>=2&&a.bytes_[0]=='M'&&a.bytes_[1]=='Z'){
        a.format_=Format::PE;
        a.artifact_class_=suffix(a.name_,".sys")?ArtifactClass::Driver:ArtifactClass::Executable;
        if(a.bytes_.size()>=0x40){auto pe=u32(a.bytes_,0x3c);if(pe+24<=a.bytes_.size()&&a.bytes_[pe]=='P'&&a.bytes_[pe+1]=='E'&&a.bytes_[pe+2]==0&&a.bytes_[pe+3]==0){
            auto m=u16(a.bytes_,pe+4);if(m==0x14c)a.architecture_=Architecture::X86;else if(m==0x8664)a.architecture_=Architecture::X86_64;else if(m==0xaa64)a.architecture_=Architecture::ARM64;
            const auto characteristics=u16(a.bytes_,pe+22); if((characteristics&0x2000)!=0)a.artifact_class_=ArtifactClass::DynamicLibrary;
        }}
    } else if(a.bytes_.size()>=20&&a.bytes_[0]==0x7f&&a.bytes_[1]=='E'&&a.bytes_[2]=='L'&&a.bytes_[3]=='F'){
        a.format_=Format::ELF; const auto type=u16(a.bytes_,16); const auto m=u16(a.bytes_,18);
        if(m==3)a.architecture_=Architecture::X86;else if(m==62)a.architecture_=Architecture::X86_64;else if(m==40)a.architecture_=Architecture::ARM;else if(m==183)a.architecture_=Architecture::ARM64;
        if(type==2)a.artifact_class_=ArtifactClass::Executable;
        else if(type==3)a.artifact_class_=ArtifactClass::DynamicLibrary;
        else if(type==1)a.artifact_class_=ArtifactClass::RelocatableObject;
        if(named_ko)a.artifact_class_=ArtifactClass::KernelModule;
        if(named_so)a.artifact_class_=ArtifactClass::DynamicLibrary;
        if(named_o)a.artifact_class_=ArtifactClass::RelocatableObject;
    } else if(a.bytes_.size()>=8&&a.bytes_[0]=='!'&&a.bytes_[1]=='<'&&a.bytes_[2]=='a'&&a.bytes_[3]=='r'&&a.bytes_[4]=='c'&&a.bytes_[5]=='h'){
        a.format_=Format::GNUArchive;a.artifact_class_=ArtifactClass::StaticArchive;
    } else a.format_=Format::Raw;
    if(named_a)a.artifact_class_=ArtifactClass::StaticArchive;
    if(a.format_==Format::GNUArchive){ analyze_archive(a.bytes_,a.interfaces_); }
    if(a.format_==Format::ELF){ auto e=analyze_elf(a.bytes_,a.name_); a.interfaces_.segments=e.segments; a.interfaces_.sections=e.sections; a.interfaces_.symbols=e.symbols; a.interfaces_.imports=e.imports; a.interfaces_.exports=e.exports; a.interfaces_.relocations=e.relocations; a.library_metadata_=e.library; a.kernel_module_metadata_=e.kernel_module; }
    if(a.format_==Format::Raw&&named_ko)a.artifact_class_=ArtifactClass::KernelModule;
    return a;
}
Format Artifact::format()const noexcept{return format_;}
Architecture Artifact::architecture()const noexcept{return architecture_;}
ArtifactClass Artifact::artifact_class()const noexcept{return artifact_class_;}
std::uint64_t Artifact::size()const noexcept{return bytes_.size();}
const std::vector<std::uint8_t>& Artifact::bytes()const noexcept{return bytes_;}
const std::string& Artifact::sha256()const noexcept{return sha256_;}
const std::string& Artifact::name()const noexcept{return name_;}
const LibraryMetadata& Artifact::library_metadata()const noexcept{return library_metadata_;}
const KernelModuleMetadata& Artifact::kernel_module_metadata()const noexcept{return kernel_module_metadata_;}
const NativeInterfaces& Artifact::interfaces()const noexcept{return interfaces_;}
}