#include "sleela/decompiler/pe.hpp"
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
// PE parsing is intentionally isolated here. It records the container boundary and then
// recovers sections, imports, exports, and relocation-directory evidence without mapping
// or executing the image. All directory walks are bounds-checked against the file size.
namespace sleela::decompiler {
namespace {
std::uint16_t rd16(const std::uint8_t* p){return std::uint16_t(p[0])|(std::uint16_t(p[1])<<8);}
std::uint32_t rd32(const std::uint8_t* p){return std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);}
std::uint64_t rd64(const std::uint8_t* p){return std::uint64_t(rd32(p))|(std::uint64_t(rd32(p+4))<<32);}
bool range(std::size_t n,std::uint64_t o,std::uint64_t s){return o<=n&&s<=n-o;}
struct Sec{std::string name;std::uint32_t vsize,vaddr,rawsize,rawptr,flags;};
// Translate a relative virtual address to a file offset through the section table.
std::uint64_t rva_to_off(const std::vector<Sec>& secs,std::uint64_t rva){
    for(const auto& s:secs){const std::uint64_t end=s.vaddr+std::max(s.vsize,s.rawsize);
        if(rva>=s.vaddr&&rva<end){const std::uint64_t d=rva-s.vaddr; if(d<s.rawsize) return std::uint64_t(s.rawptr)+d;}}
    return 0;
}
std::string cstr(const std::vector<std::uint8_t>& b,std::uint64_t off){
    if(off==0||off>=b.size()) return {}; auto e=off; while(e<b.size()&&b[e]) ++e;
    return std::string(reinterpret_cast<const char*>(b.data()+off),e-off);
}
}
PeAnalysis analyze_pe(std::span<const std::uint8_t> in,const std::string& name){
    PeAnalysis out; std::vector<std::uint8_t> b(in.begin(),in.end());
    if(b.size()<0x40||b[0]!='M'||b[1]!='Z') return out;
    const std::uint32_t pe=rd32(b.data()+0x3c);
    if(!range(b.size(),pe,24)||b[pe]!='P'||b[pe+1]!='E'||b[pe+2]||b[pe+3]) return out;
    const std::uint16_t machine=rd16(b.data()+pe+4);
    const std::uint16_t nsec=rd16(b.data()+pe+6);
    const std::uint16_t optsz=rd16(b.data()+pe+20);
    const std::uint16_t chars=rd16(b.data()+pe+22);
    if(machine==0x14c) out.architecture=Architecture::X86;
    else if(machine==0x8664) out.architecture=Architecture::X86_64;
    else if(machine==0xaa64) out.architecture=Architecture::ARM64;
    else if(machine==0x1c0||machine==0x1c4) out.architecture=Architecture::ARM;
    const std::uint64_t opt=pe+24;
    if(!range(b.size(),opt,optsz)||optsz<2) return out;
    const std::uint16_t magic=rd16(b.data()+opt); const bool pe32plus=(magic==0x20b);
    // Optional-header fields: entry point, image base, and the data-directory array.
    std::uint32_t entry=rd32(b.data()+opt+16);
    std::uint16_t subsystem=0; std::uint32_t ndir=0; std::uint64_t dir_base=0;
    if(pe32plus){ out.image_base=range(b.size(),opt+24,8)?rd64(b.data()+opt+24):0; if(range(b.size(),opt+68,2))subsystem=rd16(b.data()+opt+68); ndir=range(b.size(),opt+108,4)?rd32(b.data()+opt+108):0; dir_base=opt+112; }
    else { out.image_base=range(b.size(),opt+28,4)?rd32(b.data()+opt+28):0; if(range(b.size(),opt+68,2))subsystem=rd16(b.data()+opt+68); ndir=range(b.size(),opt+92,4)?rd32(b.data()+opt+92):0; dir_base=opt+96; }
    out.entry_point=out.image_base+entry;
    auto dir=[&](std::uint32_t i,std::uint32_t& rva,std::uint32_t& sz){rva=sz=0; if(i<ndir&&range(b.size(),dir_base+i*8,8)){rva=rd32(b.data()+dir_base+i*8);sz=rd32(b.data()+dir_base+i*8+4);}};
    out.is_driver=(subsystem==1)/*NATIVE*/||((chars&0x2000)==0&&(name.size()>=4&&name.compare(name.size()-4,4,".sys")==0));
    out.artifact_class=(chars&0x2000)?ArtifactClass::DynamicLibrary:(out.is_driver?ArtifactClass::Driver:ArtifactClass::Executable);
    // Section table follows the optional header.
    const std::uint64_t sectab=opt+optsz; std::vector<Sec> secs;
    for(std::uint16_t i=0;i<nsec;++i){const std::uint64_t p=sectab+std::uint64_t(i)*40; if(!range(b.size(),p,40)) break;
        char nm[9]={0}; std::memcpy(nm,b.data()+p,8); Sec s{std::string(nm),rd32(b.data()+p+8),rd32(b.data()+p+12),rd32(b.data()+p+16),rd32(b.data()+p+20),rd32(b.data()+p+36)};
        secs.push_back(s); out.sections.push_back({s.name,s.rawptr,s.rawsize,out.image_base+s.vaddr,s.vsize,s.flags});
    }
    // Import directory: each descriptor names a DLL and a thunk array of imported names.
    std::uint32_t irva,isz; dir(1,irva,isz);
    if(irva){std::uint64_t d=rva_to_off(secs,irva);
        for(std::uint64_t e=d;e&&range(b.size(),e,20);e+=20){const std::uint32_t oft=rd32(b.data()+e),nmr=rd32(b.data()+e+12),fta=rd32(b.data()+e+16);
            if(!oft&&!nmr&&!fta) break; const std::string dll=cstr(b,rva_to_off(secs,nmr)); if(dll.empty()&&!fta) break;
            const std::uint32_t thunk=oft?oft:fta; std::uint64_t t=rva_to_off(secs,thunk); const std::uint64_t step=pe32plus?8:4;
            for(;t&&range(b.size(),t,step);t+=step){const std::uint64_t v=pe32plus?rd64(b.data()+t):rd32(b.data()+t); if(!v) break;
                const std::uint64_t ord_bit=pe32plus?0x8000000000000000ull:0x80000000ull;
                if(v&ord_bit){out.imports.push_back({dll,"#ordinal:"+std::to_string(v&0xffff),0});}
                else{const std::uint64_t ho=rva_to_off(secs,std::uint32_t(v)); std::string fn=ho?cstr(b,ho+2):""; if(!fn.empty())out.imports.push_back({dll,fn,0});}
            }
            if(!dll.empty()&&std::find(out.library.needed_libraries.begin(),out.library.needed_libraries.end(),dll)==out.library.needed_libraries.end())
                out.library.needed_libraries.push_back(dll);
        }
    }
    // Export directory: the name-pointer table yields the public interface.
    std::uint32_t erva,esz; dir(0,erva,esz);
    if(erva){std::uint64_t d=rva_to_off(secs,erva);
        if(d&&range(b.size(),d,40)){const std::uint32_t nameptr=rd32(b.data()+d+12),nnames=rd32(b.data()+d+24),anames=rd32(b.data()+d+32);
            if(!out.library.soname.size()) out.library.soname=cstr(b,rva_to_off(secs,nameptr));
            std::uint64_t na=rva_to_off(secs,anames);
            for(std::uint32_t i=0;i<nnames&&na&&range(b.size(),na+std::uint64_t(i)*4,4);++i){const std::uint32_t r=rd32(b.data()+na+std::uint64_t(i)*4);const std::string en=cstr(b,rva_to_off(secs,r));if(!en.empty())out.exports.push_back({en,0});}
        }
    }
    // Base-relocation directory presence is recorded as evidence of a relocatable image.
    std::uint32_t rrva,rsz; dir(5,rrva,rsz);
    if(rrva&&rsz){out.library.position_independent=true; out.relocations.push_back({out.image_base+rrva,"IMAGE_BASE_RELOCATION","",".reloc"});}
    out.library.pie=out.library.position_independent;
    out.library.nx_stack=true; // DEP/NX is the default execution model for modern PE images.
    out.valid=true; return out;
}
}
