#include "sleela/decompiler/decompiler.hpp"
#include "sleela/decompiler/elf.hpp"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
namespace sleela::decompiler {
static std::string identity_digest(const std::vector<std::uint8_t>& b){std::uint64_t h=1469598103934665603ULL;for(auto x:b)h=(h^x)*1099511628211ULL;std::ostringstream s;s<<std::hex<<std::setw(16)<<std::setfill('0')<<h;return s.str();}
static std::uint16_t u16(const std::vector<std::uint8_t>&b,std::size_t p){return p+1<b.size()?std::uint16_t(b[p])|(std::uint16_t(b[p+1])<<8):0;}
static std::uint32_t u32(const std::vector<std::uint8_t>&b,std::size_t p){return p+3<b.size()?std::uint32_t(b[p])|(std::uint32_t(b[p+1])<<8)|(std::uint32_t(b[p+2])<<16)|(std::uint32_t(b[p+3])<<24):0;}
static bool suffix(const std::string& n,const std::string& s){return n.size()>=s.size()&&n.compare(n.size()-s.size(),s.size(),s)==0;}
Artifact Artifact::open(const std::string&p){std::ifstream f(p,std::ios::binary);if(!f)throw std::runtime_error("unable to open artifact: "+p);std::vector<std::uint8_t>b((std::istreambuf_iterator<char>(f)),{});return from_bytes(b,p);}
Artifact Artifact::from_bytes(std::span<const std::uint8_t>b,std::string n){
    Artifact a;a.name_=std::move(n);a.bytes_.assign(b.begin(),b.end());a.sha256_=identity_digest(a.bytes_);
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
    if(a.format_==Format::ELF){ auto e=analyze_elf(a.bytes_,a.name_); a.interfaces_.sections=e.sections; a.interfaces_.symbols=e.symbols; a.interfaces_.imports=e.imports; a.interfaces_.exports=e.exports; a.interfaces_.relocations=e.relocations; a.library_metadata_=e.library; a.kernel_module_metadata_=e.kernel_module; }
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