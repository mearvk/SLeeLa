#include "sleela/decompiler/elf.hpp"
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

namespace sleela::decompiler {
namespace {
std::uint16_t rd16(const std::uint8_t* p){return std::uint16_t(p[0])|(std::uint16_t(p[1])<<8);}
std::uint32_t rd32(const std::uint8_t* p){return std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);}
std::uint64_t rd64(const std::uint8_t* p){return std::uint64_t(rd32(p))|(std::uint64_t(rd32(p+4))<<32);}
bool range(std::size_t n,std::uint64_t o,std::uint64_t s){return o<=n&&s<=n-o;}
std::string str(const std::vector<std::uint8_t>& b,std::uint64_t off,std::uint64_t tab,std::uint64_t size){
    if(off>=size||tab>=b.size()) return {};
    const auto p=tab+off; if(p>=b.size()) return {};
    const auto e=std::min<std::uint64_t>(b.size(),tab+size);
    auto q=p; while(q<e&&b[q]) ++q;
    return std::string(reinterpret_cast<const char*>(b.data()+p),q-p);
}
std::string hex(const std::uint8_t* p,std::size_t n){
    static constexpr char d[]="0123456789abcdef"; std::string s; s.reserve(n*2);
    for(std::size_t i=0;i<n;++i){s.push_back(d[p[i]>>4]);s.push_back(d[p[i]&15]);} return s;
}
const char* reloc_name(std::uint16_t machine,std::uint64_t type){
    if(machine==62){
        switch(type){case 1:return "R_X86_64_64";case 2:return "R_X86_64_PC32";case 6:return "R_X86_64_GLOB_DAT";case 7:return "R_X86_64_JUMP_SLOT";case 8:return "R_X86_64_RELATIVE";case 10:return "R_X86_64_32";case 11:return "R_X86_64_32S";case 37:return "R_X86_64_IRELATIVE";default:return "R_X86_64_UNKNOWN";}
    }
    if(machine==3){
        switch(type){case 1:return "R_386_32";case 2:return "R_386_PC32";case 6:return "R_386_GLOB_DAT";case 7:return "R_386_JMP_SLOT";case 8:return "R_386_RELATIVE";default:return "R_386_UNKNOWN";}
    }
    if(machine==183){
        switch(type){case 257:return "R_AARCH64_ABS64";case 258:return "R_AARCH64_ABS32";case 275:return "R_AARCH64_JUMP26";case 282:return "R_AARCH64_JUMP_SLOT";case 283:return "R_AARCH64_RELATIVE";default:return "R_AARCH64_UNKNOWN";}
    }
    if(machine==40){
        switch(type){case 2:return "R_ARM_ABS32";case 3:return "R_ARM_REL32";case 21:return "R_ARM_GLOB_DAT";case 22:return "R_ARM_JUMP_SLOT";case 23:return "R_ARM_RELATIVE";default:return "R_ARM_UNKNOWN";}
    }
    return "ELF_RELOC_UNKNOWN";
}
std::string dyn_tag_name(std::int64_t tag){
    switch(tag){case 3:return "DT_PLTGOT";case 12:return "DT_INIT";case 13:return "DT_FINI";case 23:return "DT_JMPREL";case 25:return "DT_INIT_ARRAY";case 26:return "DT_FINI_ARRAY";case 27:return "DT_INIT_ARRAYSZ";case 28:return "DT_FINI_ARRAYSZ";case 30:return "DT_FLAGS";case 0x6ffffffb:return "DT_FLAGS_1";default:return {};}
}
}
ElfAnalysis analyze_elf(std::span<const std::uint8_t> in,const std::string& name){
    ElfAnalysis out; std::vector<std::uint8_t> b(in.begin(),in.end());
    if(b.size()<0x40||b[0]!=0x7f||b[1]!='E'||b[2]!='L'||b[3]!='F'||b[5]!=1||(b[4]!=1&&b[4]!=2)) return out;
    const bool x64=b[4]==2;
    const std::uint16_t type=rd16(b.data()+16), machine=rd16(b.data()+18);
    const std::uint64_t entry=x64?rd64(b.data()+24):rd32(b.data()+24);
    const std::uint64_t phoff=x64?rd64(b.data()+32):rd32(b.data()+28);
    const std::uint16_t phentsz=x64?rd16(b.data()+54):rd16(b.data()+42), phnum=x64?rd16(b.data()+56):rd16(b.data()+44);
    const std::uint64_t shoff=x64?rd64(b.data()+40):rd32(b.data()+32);
    const std::uint16_t shentsz=x64?rd16(b.data()+58):rd16(b.data()+46), shnum=x64?rd16(b.data()+60):rd16(b.data()+48), shstr=x64?rd16(b.data()+62):rd16(b.data()+50);
    if(phnum&&(!range(b.size(),phoff,std::uint64_t(phentsz)*phnum)||phentsz<(x64?56:32))) return out;
    struct S{std::uint32_t name,type;std::uint64_t flags,addr,off,size,entsize;std::uint32_t link,info;};
    std::vector<S> ss;
    if(shnum){
        if(shentsz<(x64?64:40)||!range(b.size(),shoff,std::uint64_t(shentsz)*shnum)) return out;
        ss.resize(shnum);
        for(std::size_t i=0;i<shnum;++i){auto p=b.data()+shoff+i*shentsz;
            if(x64) ss[i]={rd32(p),rd32(p+4),rd64(p+8),rd64(p+16),rd64(p+24),rd64(p+32),rd64(p+56),rd32(p+40),rd32(p+44)};
            else ss[i]={rd32(p),rd32(p+4),rd32(p+8),rd32(p+12),rd32(p+16),rd32(p+20),rd32(p+36),rd32(p+24),rd32(p+28)};
        }
        if(shstr>=shnum||!range(b.size(),ss[shstr].off,ss[shstr].size)) return out;
    }
    // Program headers provide the load/permission model and are also used for GNU build-id notes.
    constexpr std::uint32_t PT_LOAD=1,PT_DYNAMIC=2,PT_NOTE=4,PT_GNU_STACK=0x6474e551,PT_GNU_RELRO=0x6474e552;
    std::uint32_t dynamic_flags=0, dynamic_flags1=0;
    std::uint64_t dynamic_file=0,dynamic_size=0,dynamic_ent=0;
    for(std::size_t i=0;i<phnum;++i){auto p=b.data()+phoff+i*phentsz;ProgramSegment seg{};
        if(x64){seg.type=rd32(p);seg.flags=rd32(p+4);seg.file_offset=rd64(p+8);seg.virtual_address=rd64(p+16);seg.file_size=rd64(p+32);seg.memory_size=rd64(p+40);seg.alignment=rd64(p+48);}
        else{seg.type=rd32(p);seg.file_offset=rd32(p+4);seg.virtual_address=rd32(p+8);seg.file_size=rd32(p+16);seg.memory_size=rd32(p+20);seg.flags=rd32(p+24);seg.alignment=rd32(p+28);}
        if(!range(b.size(),seg.file_offset,seg.file_size)) continue;
        out.segments.push_back(seg);
        if(seg.type==PT_DYNAMIC){dynamic_file=seg.file_offset;dynamic_size=seg.file_size;dynamic_ent=x64?16:8;}
        if(seg.type==PT_GNU_RELRO) out.library.relro_present=true;
        if(seg.type==PT_GNU_STACK) out.library.nx_stack=(seg.flags&1)==0;
        if(seg.type==PT_NOTE){
            std::uint64_t q=seg.file_offset,end=seg.file_offset+seg.file_size;
            while(q+12<=end){auto namesz=rd32(b.data()+q),descsz=rd32(b.data()+q+4),ntype=rd32(b.data()+q+8);q+=12;
                auto np=(namesz+3)&~std::uint32_t(3), dp=(descsz+3)&~std::uint32_t(3);
                if(q+np+dp>end) break;
                std::string owner(reinterpret_cast<const char*>(b.data()+q),std::min<std::uint32_t>(namesz,4));
                if(ntype==3&&owner.rfind("GNU",0)==0&&descsz) out.library.build_id=hex(b.data()+q+np,descsz);
                q+=np+dp;
            }
        }
    }
    if(ss.empty()){out.valid=true; return out;}
    for(const auto&s:ss) if(range(b.size(),s.off,s.size))
        out.sections.push_back({str(b,s.name,ss[shstr].off,ss[shstr].size),s.off,s.size,s.addr,s.size,std::uint32_t(s.flags)});
    std::uint64_t dynstr_off=0,dynstr_size=0,symoff=0,symsize=0,symentsz=0,modinfo_off=0,modinfo_size=0;
    std::unordered_map<std::string,std::size_t> section_index;
    for(std::size_t i=0;i<ss.size();++i){auto n=str(b,ss[i].name,ss[shstr].off,ss[shstr].size);section_index[n]=i;
        if(n==".dynstr"){dynstr_off=ss[i].off;dynstr_size=ss[i].size;}
        else if(n==".dynsym"){symoff=ss[i].off;symsize=ss[i].size;symentsz=ss[i].entsize?ss[i].entsize:(x64?24:16);}
        else if(n==".modinfo"){modinfo_off=ss[i].off;modinfo_size=ss[i].size;}
        if(n==".tdata"||n==".tbss") out.library.tls_present=true;
    }
    std::vector<std::string> symbol_names; symbol_names.reserve(1024);
    std::vector<std::uint64_t> symbol_values;
    std::vector<std::uint8_t> symbol_info;
    if(symoff&&symentsz&&range(b.size(),symoff,symsize)){
        for(std::uint64_t p=symoff;p+symentsz<=symoff+symsize;p+=symentsz){
            std::uint32_t no;std::uint8_t info;std::uint64_t val;
            if(x64){no=rd32(b.data()+p);info=b[p+4];val=rd64(b.data()+p+8);}else{no=rd32(b.data()+p);val=rd32(b.data()+p+4);info=b[p+12];}
            auto sn=str(b,no,dynstr_off,dynstr_size); symbol_names.push_back(sn);symbol_values.push_back(val);symbol_info.push_back(info);
            if(sn.empty()) continue;
            const bool ext=val==0; out.symbols.push_back({sn,val,ext,{0,val,name,0.9}});
            const auto kind=info&15; if(!ext&&(kind==2||kind==1)) out.exports.push_back({sn,val}); else if(ext) out.imports.push_back({"",sn,val});
        }
    }
    // Dynamic table metadata. Virtual addresses are translated to file offsets through PT_LOAD.
    auto vaddr_to_file=[&](std::uint64_t va)->std::uint64_t{
        for(const auto&seg:out.segments) if(seg.type==PT_LOAD&&va>=seg.virtual_address&&va-seg.virtual_address<seg.file_size) return seg.file_offset+(va-seg.virtual_address);
        return 0;
    };
    std::uint64_t pltgot=0,jmprel=0,pltrelsz=0,initarr=0,finiarr=0,initsz=0,finisz=0;
    if(dynamic_file&&range(b.size(),dynamic_file,dynamic_size)){
        for(std::uint64_t p=dynamic_file;p+dynamic_ent<=dynamic_file+dynamic_size;p+=dynamic_ent){
            const std::int64_t tag=x64?std::int64_t(rd64(b.data()+p)):std::int32_t(rd32(b.data()+p));
            const std::uint64_t val=x64?rd64(b.data()+p+8):rd32(b.data()+p+4); if(tag==0) break;
            if(tag==1&&dynstr_off) out.library.needed_libraries.push_back(str(b,val,dynstr_off,dynstr_size));
            else if(tag==14&&dynstr_off) out.library.soname=str(b,val,dynstr_off,dynstr_size);
            else if(tag==3) pltgot=val;
            else if(tag==23) jmprel=val;
            else if(tag==2) pltrelsz=val;
            else if(tag==12) out.library.init_functions.push_back("0x"+[] (std::uint64_t x){std::ostringstream s;s<<std::hex<<x;return s.str();}(val));
            else if(tag==13) out.library.fini_functions.push_back("0x"+[] (std::uint64_t x){std::ostringstream s;s<<std::hex<<x;return s.str();}(val));
            else if(tag==25) initarr=val; else if(tag==26) finiarr=val; else if(tag==27) initsz=val; else if(tag==28) finisz=val;
            else if(tag==30) dynamic_flags=std::uint32_t(val); else if(tag==0x6ffffffb) dynamic_flags1=std::uint32_t(val);
        }
    }
    out.library.got_address=pltgot; out.library.plt_address=jmprel;
    out.library.bind_now=(dynamic_flags&8)!=0||(dynamic_flags1&1)!=0;
    out.library.position_independent=(type==3)||type==1||type==2;
    out.library.pie=(type==3?false:(dynamic_flags1&0x08000000u)!=0)||(type==3);
    auto parse_array=[&](std::uint64_t va,std::uint64_t sz,std::vector<std::string>& dst){
        auto off=vaddr_to_file(va); auto word=x64?8u:4u; if(!off||!sz||!range(b.size(),off,sz)) return;
        for(std::uint64_t p=off;p+word<=off+sz;p+=word){auto x=x64?rd64(b.data()+p):rd32(b.data()+p); if(x) dst.push_back("0x"+[] (std::uint64_t y){std::ostringstream s;s<<std::hex<<y;return s.str();}(x));}
    };
    parse_array(initarr,initsz,out.library.init_functions); parse_array(finiarr,finisz,out.library.fini_functions);
    // GNU symbol version tables.
    std::vector<std::uint16_t> versym; std::unordered_map<std::uint16_t,std::string> verdef,verneed;
    auto secdata=[&](const std::string& n)->std::pair<std::uint64_t,std::uint64_t>{auto it=section_index.find(n);return it==section_index.end()?std::pair<std::uint64_t,std::uint64_t>{}:std::pair<std::uint64_t,std::uint64_t>{ss[it->second].off,ss[it->second].size};};
    auto [vo,vs]=secdata(".gnu.version"); if(vo&&vs&&range(b.size(),vo,vs)) for(std::uint64_t p=vo;p+2<=vo+vs;p+=2) versym.push_back(rd16(b.data()+p));
    auto [vdo,vds]=secdata(".gnu.version_d"); if(vdo&&vds&&range(b.size(),vdo,vds)){
        std::uint64_t p=vdo; while(p+20<=vdo+vds){auto ndx=rd16(b.data()+p+4),cnt=rd16(b.data()+p+6),aux=rd32(b.data()+p+12),next=rd32(b.data()+p+16);if(aux&&dynstr_off){auto ap=p+aux; if(ap+8<=vdo+vds){auto no=rd32(b.data()+ap+4);verdef[ndx]=str(b,no,dynstr_off,dynstr_size);}}if(!next)break;p+=next;if(!cnt)break;}
    }
    auto [vro,vrs]=secdata(".gnu.version_r"); if(vro&&vrs&&range(b.size(),vro,vrs)){
        std::uint64_t p=vro; while(p+16<=vro+vrs){auto cnt=rd16(b.data()+p+2),aux=rd32(b.data()+p+8),next=rd32(b.data()+p+12);auto ap=p+aux;for(std::uint16_t i=0;i<cnt&&ap+16<=vro+vrs;++i){auto ndx=rd16(b.data()+ap+6),no=rd32(b.data()+ap+8);auto vn=str(b,no,dynstr_off,dynstr_size);if(!vn.empty()){verneed[ndx]=vn;out.library.required_symbol_versions.push_back(vn);}auto an=rd32(b.data()+ap+12);if(!an)break;ap+=an;}if(!next)break;p+=next;}
    }
    for(std::size_t i=0;i<versym.size()&&i<symbol_names.size();++i){auto v=versym[i]&0x7fff;auto it=verdef.find(v);if(it!=verdef.end()&&!it->second.empty())out.library.symbol_versions.push_back(it->second);auto ir=verneed.find(v);if(ir!=verneed.end()&&!ir->second.empty())out.library.required_symbol_versions.push_back(ir->second);}
    std::sort(out.library.symbol_versions.begin(),out.library.symbol_versions.end());out.library.symbol_versions.erase(std::unique(out.library.symbol_versions.begin(),out.library.symbol_versions.end()),out.library.symbol_versions.end());
    std::sort(out.library.required_symbol_versions.begin(),out.library.required_symbol_versions.end());out.library.required_symbol_versions.erase(std::unique(out.library.required_symbol_versions.begin(),out.library.required_symbol_versions.end()),out.library.required_symbol_versions.end());
    // Relocations, including PLT relocations, are mapped back to dynamic symbols.
    for(std::size_t si=0;si<ss.size();++si){auto n=str(b,ss[si].name,ss[shstr].off,ss[shstr].size);const auto t=ss[si].type;
        if(t!=4&&t!=9) continue; // SHT_RELA / SHT_REL
        const auto es=ss[si].entsize?ss[si].entsize:(x64?(t==4?24:16):(t==4?12:8)); if(!es||!range(b.size(),ss[si].off,ss[si].size)) continue;
        const auto count=ss[si].size/es;
        for(std::uint64_t j=0;j<count;++j){auto p=ss[si].off+j*es;std::uint64_t addr=0,info=0;if(x64){addr=rd64(b.data()+p);info=rd64(b.data()+p+8);}else{addr=rd32(b.data()+p);info=rd32(b.data()+p+4);}
            const auto sym=x64?info>>32:info>>8;const auto rt=x64?info&0xffffffffu:info&0xffu;std::string sn=sym<symbol_names.size()?symbol_names[sym]:"";
            out.relocations.push_back({addr,reloc_name(machine,rt),sn,n});
        }
    }
    // Relocation-backed imports now retain their relocation address, which is the key evidence
    // needed later for PLT/GOT and external-call recovery.
    for(const auto& r : out.relocations) {
        if(r.symbol.empty()) continue;
        for(auto& imp : out.imports)
            if(imp.name==r.symbol && imp.address==0) imp.address=r.address;
    }
    // Kernel module exports/imports are represented by ksymtab sections and ordinary undefined symbols.
    if(section_index.count("__ksymtab")||section_index.count("__ksymtab_gpl")) out.kernel_module.module_name=out.kernel_module.module_name.empty()?name:out.kernel_module.module_name;
    if(modinfo_off&&range(b.size(),modinfo_off,modinfo_size)) for(std::uint64_t p=modinfo_off;p<modinfo_off+modinfo_size;){auto q=p;while(q<modinfo_off+modinfo_size&&b[q])++q;std::string v(reinterpret_cast<const char*>(b.data()+p),q-p);auto eq=v.find('=');if(eq!=std::string::npos){auto k=v.substr(0,eq),val=v.substr(eq+1);if(k=="name")out.kernel_module.module_name=val;else if(k=="vermagic")out.kernel_module.vermagic=val;else if(k=="license")out.kernel_module.license=val;else if(k=="author")out.kernel_module.author=val;else if(k=="description")out.kernel_module.description=val;else if(k=="alias")out.kernel_module.aliases.push_back(val);else if(k=="depends"&&!val.empty()){std::size_t s=0;while(s<val.size()){auto e=val.find(',',s);out.kernel_module.dependencies.push_back(val.substr(s,e==std::string::npos?val.size()-s:e-s));if(e==std::string::npos)break;s=e+1;}}}p=q+1;}
    out.valid=true; return out;
}
}
