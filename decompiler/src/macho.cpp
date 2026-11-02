#include "sleela/decompiler/macho.hpp"
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
// Mach-O parsing is isolated here. It reads the header and load commands to recover
// architecture, sections, dylib dependencies, and symbol-table evidence. FAT/universal
// containers are reported as a slice count with the first analyzable slice lifted.
namespace sleela::decompiler {
namespace {
std::uint32_t rd32(const std::uint8_t* p,bool be){return be?(std::uint32_t(p[0])<<24)|(std::uint32_t(p[1])<<16)|(std::uint32_t(p[2])<<8)|p[3]:(std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24));}
std::uint64_t rd64le(const std::uint8_t* p){std::uint64_t v=0;for(int i=0;i<8;++i)v|=std::uint64_t(p[i])<<(8*i);return v;}
bool range(std::size_t n,std::uint64_t o,std::uint64_t s){return o<=n&&s<=n-o;}
Architecture cpu_arch(std::uint32_t cputype){
    switch(cputype){case 7:return Architecture::X86;case 0x01000007:return Architecture::X86_64;
        case 12:return Architecture::ARM;case 0x0100000c:return Architecture::ARM64;default:return Architecture::Unknown;}
}
std::string cstr(const std::vector<std::uint8_t>& b,std::uint64_t off){
    if(off==0||off>=b.size())return{};auto e=off;while(e<b.size()&&b[e])++e;
    return std::string(reinterpret_cast<const char*>(b.data()+off),e-off);
}
// Analyze a single thin slice beginning at file offset `base`.
void analyze_slice(const std::vector<std::uint8_t>& b,std::uint64_t base,const std::string& name,MachoAnalysis& out){
    if(!range(b.size(),base,28))return; const std::uint8_t* h=b.data()+base;
    const std::uint32_t magic=rd32(h,false);
    const bool be=(magic==0xfeedface||magic==0xfeedfacf)&&(h[0]==0xfe);
    const bool is64=(h[0]==0xfe?h[3]==0xcf:h[0]==0xcf); // MH_MAGIC_64 variants
    const std::uint32_t cputype=rd32(h+4,be);
    const std::uint32_t filetype=rd32(h+12,be);
    const std::uint32_t ncmds=rd32(h+16,be);
    out.architecture=cpu_arch(cputype);
    switch(filetype){case 2:out.artifact_class=ArtifactClass::Executable;break;      // MH_EXECUTE
                      case 6:out.artifact_class=ArtifactClass::DynamicLibrary;break;  // MH_DYLIB
                      case 8:out.artifact_class=ArtifactClass::DynamicLibrary;break;  // MH_BUNDLE
                      case 1:out.artifact_class=ArtifactClass::RelocatableObject;break;// MH_OBJECT
                      case 11:out.artifact_class=ArtifactClass::KernelModule;break;}   // MH_KEXT_BUNDLE
    const std::uint64_t cmd0=base+(is64?32:28);
    std::uint64_t symoff=0,nsyms=0,stroff=0,strsize=0;
    std::uint64_t p=cmd0;
    for(std::uint32_t i=0;i<ncmds;++i){
        if(!range(b.size(),p,8))break; const std::uint32_t cmd=rd32(b.data()+p,be),sz=rd32(b.data()+p+4,be);
        if(sz<8||!range(b.size(),p,sz))break;
        if(cmd==0x19){ // LC_SEGMENT_64
            const std::uint32_t nsects=rd32(b.data()+p+64,be); std::uint64_t sp=p+72;
            for(std::uint32_t s=0;s<nsects&&range(b.size(),sp,80);++s){char nm[17]={0};std::memcpy(nm,b.data()+sp,16);
                const std::uint64_t addr=rd64le(b.data()+sp+32),size=rd64le(b.data()+sp+40);const std::uint32_t off=rd32(b.data()+sp+48,be);
                out.sections.push_back({std::string(nm),off,size,addr,size,0});sp+=80;}
        } else if(cmd==0x01){ // LC_SEGMENT (32-bit)
            const std::uint32_t nsects=rd32(b.data()+p+48,be); std::uint64_t sp=p+56;
            for(std::uint32_t s=0;s<nsects&&range(b.size(),sp,68);++s){char nm[17]={0};std::memcpy(nm,b.data()+sp,16);
                const std::uint32_t addr=rd32(b.data()+sp+32,be),size=rd32(b.data()+sp+36,be),off=rd32(b.data()+sp+40,be);
                out.sections.push_back({std::string(nm),off,size,addr,size,0});sp+=68;}
        } else if(cmd==0x0c||cmd==0x0d||cmd==0x1f){ // LC_LOAD_DYLIB / LC_ID_DYLIB / LC_REEXPORT_DYLIB
            const std::uint32_t nameoff=rd32(b.data()+p+8,be); if(nameoff<sz){std::string dl=cstr(b,p+nameoff);
                if(cmd==0x0d){out.library.soname=dl;} else if(!dl.empty())out.library.needed_libraries.push_back(dl);}
        } else if(cmd==0x02){ // LC_SYMTAB
            symoff=base+rd32(b.data()+p+8,be);nsyms=rd32(b.data()+p+12,be);stroff=base+rd32(b.data()+p+16,be);strsize=rd32(b.data()+p+20,be);
        }
        p+=sz;
    }
    if(symoff&&nsyms&&stroff){const std::uint64_t ent=is64?16:12;
        for(std::uint64_t i=0;i<nsyms&&range(b.size(),symoff+i*ent,ent);++i){const std::uint8_t* e=b.data()+symoff+i*ent;
            const std::uint32_t strx=rd32(e,be);const std::uint8_t type=e[4];
            const std::uint64_t val=is64?rd64le(e+8):rd32(e+8,be);
            std::string sn=(strx&&stroff+strx<stroff+strsize)?cstr(b,stroff+strx):"";
            if(sn.empty())continue; const bool ext=(type&0x01)!=0; const bool undef=(type&0x0e)==0;
            out.symbols.push_back({sn,val,undef,{0,val,name,0.9}});
            if(undef)out.imports.push_back({"",sn,0}); else if(ext)out.exports.push_back({sn,val});
        }
    }
    out.library.pie=out.library.position_independent=(filetype==2||filetype==6||filetype==8);
    out.valid=true;
}
}
MachoAnalysis analyze_macho(std::span<const std::uint8_t> in,const std::string& name){
    MachoAnalysis out; std::vector<std::uint8_t> b(in.begin(),in.end());
    if(b.size()<8)return out; const std::uint8_t* h=b.data();
    if(h[0]==0xca&&h[1]==0xfe&&h[2]==0xba&&(h[3]==0xbe||h[3]==0xbf)){ // FAT/universal (big-endian count)
        out.fat=true; const bool fat64=h[3]==0xbf; const std::uint32_t nfat=rd32(h+4,true); out.slice_count=nfat;
        const std::uint64_t ent=fat64?32:20,hdr=8;
        for(std::uint32_t i=0;i<nfat;++i){const std::uint64_t ap=hdr+i*ent; if(!range(b.size(),ap,ent))break;
            const std::uint64_t off=fat64?((std::uint64_t(rd32(b.data()+ap+8,true))<<32)|rd32(b.data()+ap+12,true)):rd32(b.data()+ap+8,true);
            if(range(b.size(),off,28)){analyze_slice(b,off,name,out); if(out.valid)break;} // lift first analyzable slice
        }
        out.valid=out.valid||nfat>0; return out;
    }
    analyze_slice(b,0,name,out); return out;
}
}
