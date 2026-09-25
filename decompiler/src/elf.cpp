#include "sleela/decompiler/elf.hpp"
#include <algorithm>
#include <cstring>
namespace sleela::decompiler {
namespace {
std::uint16_t rd16(const std::uint8_t* p){return std::uint16_t(p[0])|(std::uint16_t(p[1])<<8);}
std::uint32_t rd32(const std::uint8_t* p){return std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|(std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);}
std::uint64_t rd64(const std::uint8_t* p){return std::uint64_t(rd32(p))|(std::uint64_t(rd32(p+4))<<32);}
bool range(std::size_t n,std::uint64_t o,std::uint64_t s){return o<=n&&s<=n-o;}
std::string str(const std::vector<std::uint8_t>& b,std::uint64_t off,std::uint64_t tab,std::uint64_t size){if(off>=size||tab>=b.size())return{};std::uint64_t p=tab+off;if(p>=b.size())return{};std::uint64_t e=std::min<std::uint64_t>(b.size(),tab+size);std::uint64_t q=p;while(q<e&&b[q])++q;return std::string(reinterpret_cast<const char*>(b.data()+p),q-p);}
}
ElfAnalysis analyze_elf(std::span<const std::uint8_t> in,const std::string& name){
 ElfAnalysis out;std::vector<std::uint8_t>b(in.begin(),in.end());if(b.size()<0x40||b[0]!=0x7f||b[1]!='E'||b[2]!='L'||b[3]!='F'||b[5]!=1)return out;
 const bool x64=b[4]==2; const std::uint16_t type=rd16(b.data()+16); const std::uint64_t shoff=x64?rd64(b.data()+40):rd32(b.data()+32); const std::uint16_t shentsz=x64?rd16(b.data()+58):rd16(b.data()+46); const std::uint16_t shnum=x64?rd16(b.data()+60):rd16(b.data()+48); const std::uint16_t shstr=x64?rd16(b.data()+62):rd16(b.data()+50);
 if(!shnum||shentsz< (x64?64:40)||!range(b.size(),shoff,std::uint64_t(shentsz)*shnum))return out;
 struct S{std::uint32_t name,type;std::uint64_t flags,addr,off,size,entsize;std::uint32_t link,info;};
 std::vector<S> ss(shnum);
 for(std::size_t i=0;i<shnum;i++){auto p=b.data()+shoff+i*shentsz;if(x64){ss[i]={rd32(p),rd32(p+4),rd64(p+8),rd64(p+16),rd64(p+24),rd64(p+32),rd64(p+56),rd32(p+40),rd32(p+44)};}else{ss[i]={rd32(p),rd32(p+4),rd32(p+8),rd32(p+12),rd32(p+16),rd32(p+20),rd32(p+36),rd32(p+24),rd32(p+28)};}}
 if(shstr>=shnum||!range(b.size(),ss[shstr].off,ss[shstr].size))return out;
 for(const auto&s:ss)if(range(b.size(),s.off,s.size))out.sections.push_back({str(b, s.name, ss[shstr].off, ss[shstr].size),s.off,s.size,s.addr,s.size,std::uint32_t(s.flags)});
 std::uint64_t dynstr_off=0,dynstr_size=0; std::uint64_t symoff=0,symsize=0,symentsz=0; std::uint64_t dynoff=0,dynsize=0,dynentsz=0; std::uint64_t relocs=0;
 for(const auto&s:ss){std::string n=str(b,s.name,ss[shstr].off,ss[shstr].size);if(n==".dynstr"){dynstr_off=s.off;dynstr_size=s.size;}else if(n==".dynsym"){symoff=s.off;symsize=s.size;symentsz=s.entsize? s.entsize:(x64?24:16);}else if(n==".dynamic"){dynoff=s.off;dynsize=s.size;dynentsz=s.entsize? s.entsize:(x64?16:8);}else if(n==".modinfo"&&range(b.size(),s.off,s.size)){std::size_t p=s.off,e=s.off+s.size;while(p<e){std::size_t q=p;while(q<e&&b[q])++q;std::string v(reinterpret_cast<const char*>(b.data()+p),q-p);auto eq=v.find('=');if(eq!=std::string::npos){auto k=v.substr(0,eq),val=v.substr(eq+1);if(k=="name")out.kernel_module.module_name=val;else if(k=="vermagic")out.kernel_module.vermagic=val;else if(k=="license")out.kernel_module.license=val;else if(k=="author")out.kernel_module.author=val;else if(k=="description")out.kernel_module.description=val;else if(k=="alias")out.kernel_module.aliases.push_back(val);else if(k=="depends")out.kernel_module.dependencies.push_back(val);}p=q+1;}}}
 if(dynoff&&dynstr_off&&range(b.size(),dynoff,dynsize)){for(std::uint64_t p=dynoff;p+dynentsz<=dynoff+dynsize;p+=dynentsz){std::int64_t tag=x64?std::int64_t(rd64(b.data()+p)):std::int32_t(rd32(b.data()+p));std::uint64_t val=x64?rd64(b.data()+p+8):rd32(b.data()+p+4);if(tag==0)break;if(tag==1)out.library.needed_libraries.push_back(str(b,val,dynstr_off,dynstr_size));else if(tag==14)out.library.soname=str(b,val,dynstr_off,dynstr_size);}}
 if(symoff&&symentsz&&range(b.size(),symoff,symsize)){for(std::uint64_t p=symoff;p+symentsz<=symoff+symsize;p+=symentsz){std::uint32_t no;std::uint8_t info;std::uint64_t val; if(x64){no=rd32(b.data()+p);info=b[p+4];val=rd64(b.data()+p+8);}else{no=rd32(b.data()+p);val=rd32(b.data()+p+4);info=b[p+12];}std::string sn=str(b,no,dynstr_off,dynstr_size);if(sn.empty())continue;bool ext=val==0;out.symbols.push_back({sn,val,ext,{0,val,name,0.9}});if(!ext){if((info&15)==2|| (info&15)==1)out.exports.push_back({sn,val});}else out.imports.push_back({"",sn,val});}}
 out.valid=true; return out;
}
}
