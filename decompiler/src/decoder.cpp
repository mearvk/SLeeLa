#include "sleela/decompiler/decompiler.hpp"
#include <algorithm>
#include <cstdint>
#include <string>

namespace sleela::decompiler {
namespace {
Instruction raw(const std::vector<std::uint8_t>& b,std::size_t p,const std::string& m){
    Instruction i; i.address=p; i.bytes.push_back(b[p]); i.mnemonic=m;
    i.provenance={p,p,"stage4-x86",0.70}; return i;
}
std::int64_t rel8(std::uint8_t x){return static_cast<std::int8_t>(x);}
std::int64_t rel32(const std::vector<std::uint8_t>& b,std::size_t p){
    return std::int64_t(std::int32_t(std::uint32_t(b[p])|std::uint32_t(b[p+1])<<8|std::uint32_t(b[p+2])<<16|std::uint32_t(b[p+3])<<24));
}
}
std::vector<Instruction> Decoder::decode(const Artifact& a,std::uint64_t address,std::size_t length) const {
    std::vector<Instruction> out; const auto& b=a.bytes(); std::size_t p=static_cast<std::size_t>(address);
    if(p>=b.size()) return out; const auto end=std::min(b.size(),p+length);
    if(a.architecture()!=Architecture::X86 && a.architecture()!=Architecture::X86_64) {
        for(;p<end;++p){auto i=raw(b,p,"db");i.operands.push_back({Operand::Kind::Immediate,"0x"+std::to_string(b[p]),b[p]});out.push_back(std::move(i));}
        return out;
    }
    while(p<end){
        const auto op=b[p]; Instruction i; std::size_t n=1;
        if(op==0xC3){i=raw(b,p,"ret");i.is_return=true;}
        else if(op==0xC2&&p+2<end){i=raw(b,p,"ret");n=3;i.is_return=true;i.bytes.assign(b.begin()+p,b.begin()+p+n);}
        else if(op==0xE8&&p+4<end){i=raw(b,p,"call");n=5;i.is_call=true;i.branch_targets.push_back(std::uint64_t(std::int64_t(p+n)+rel32(b,p+1)));i.bytes.assign(b.begin()+p,b.begin()+p+n);}
        else if(op==0xE9&&p+4<end){i=raw(b,p,"jmp");n=5;i.is_branch=true;i.branch_targets.push_back(std::uint64_t(std::int64_t(p+n)+rel32(b,p+1)));i.bytes.assign(b.begin()+p,b.begin()+p+n);}
        else if(op==0xEB&&p+1<end){i=raw(b,p,"jmp");n=2;i.is_branch=true;i.branch_targets.push_back(std::uint64_t(std::int64_t(p+n)+rel8(b[p+1])));i.bytes.assign(b.begin()+p,b.begin()+p+n);}
        else if(op>=0x70&&op<=0x7F&&p+1<end){i=raw(b,p,"jcc");n=2;i.is_branch=true;i.is_conditional=true;i.branch_targets.push_back(std::uint64_t(std::int64_t(p+n)+rel8(b[p+1])));i.bytes.assign(b.begin()+p,b.begin()+p+n);}
        else if(op==0x0F&&p+5<end&&b[p+1]>=0x80&&b[p+1]<=0x8F){i=raw(b,p,"jcc");n=6;i.is_branch=true;i.is_conditional=true;i.branch_targets.push_back(std::uint64_t(std::int64_t(p+n)+rel32(b,p+2)));i.bytes.assign(b.begin()+p,b.begin()+p+n);}
        else {i=raw(b,p,"db");i.operands.push_back({Operand::Kind::Immediate,"0x"+std::to_string(b[p]),b[p]});}
        out.push_back(std::move(i)); p+=n;
    }
    return out;
}
}
