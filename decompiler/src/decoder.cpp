#include "sleela/decompiler/decompiler.hpp"
#include <algorithm>
#include <cstdint>
#include <sstream>
#include <string>

namespace sleela::decompiler {
namespace {
Instruction base(const std::vector<std::uint8_t>& b, std::size_t p, const std::string& mnemonic, double confidence=0.80) {
    Instruction i; i.address=p; i.bytes.push_back(b[p]); i.mnemonic=mnemonic;
    i.provenance={p,p,"stage4-x86",confidence}; return i;
}
std::int64_t rel8(std::uint8_t x){return static_cast<std::int8_t>(x);}
std::int64_t rel32(const std::vector<std::uint8_t>& b,std::size_t p){
    return std::int64_t(std::int32_t(std::uint32_t(b[p])|(std::uint32_t(b[p+1])<<8)|
        (std::uint32_t(b[p+2])<<16)|(std::uint32_t(b[p+3])<<24)));
}
std::string hex(std::uint64_t v){std::ostringstream s;s<<"0x"<<std::hex<<v;return s.str();}
std::string reg_name(std::uint8_t r,bool wide64){
    static const char* r64[]={"rax","rcx","rdx","rbx","rsp","rbp","rsi","rdi","r8","r9","r10","r11","r12","r13","r14","r15"};
    static const char* r32[]={"eax","ecx","edx","ebx","esp","ebp","esi","edi","r8d","r9d","r10d","r11d","r12d","r13d","r14d","r15d"};
    return wide64?r64[r&15]:r32[r&15];
}
struct ModRm { std::size_t length{1}; std::string operand; Operand::Kind kind{Operand::Kind::Register}; std::uint8_t reg{}; };
bool parse_modrm(const std::vector<std::uint8_t>& b,std::size_t p,std::size_t end,bool wide64,ModRm& out){
    if(p>=end)return false;
    const std::uint8_t m=b[p]; const unsigned mod=m>>6u,reg=(m>>3u)&7u,rm=m&7u; out.reg=static_cast<std::uint8_t>(reg); std::size_t n=1; std::ostringstream t; 
    if(mod==3){out.kind=Operand::Kind::Register;out.operand=reg_name(rm,wide64);return true;}
    out.kind=Operand::Kind::Memory;t<<"[";
    if(rm==4){
        if(p+1>=end)return false; const std::uint8_t sib=b[p+1]; const unsigned scale=1u<<(sib>>6),index=(sib>>3)&7u,base_r=sib&7u; n++;
        if(index!=4)t<<reg_name(index,wide64)<<"*"<<unsigned(scale);
        if(index!=4&&base_r!=5)t<<"+";
        if(mod==0&&base_r==5){
            if(p+n+4>end)return false; const auto d=std::uint32_t(b[p+n])|(std::uint32_t(b[p+n+1])<<8)|(std::uint32_t(b[p+n+2])<<16)|(std::uint32_t(b[p+n+3])<<24);
            t<<"0x"<<std::hex<<d;n+=4;
        } else {
            t<<reg_name(base_r,wide64);
            if(mod==1){if(p+n>=end)return false;t<<"+"<<int(static_cast<std::int8_t>(b[p+n]));n++;}
            else if(mod==2){if(p+n+4>end)return false; t<<"+0x"<<std::hex<<std::uint32_t(b[p+n])<<"..." ;n+=4;}
        }
    } else if(mod==0&&rm==5){
        if(p+n+4>end)return false; const auto d=std::uint32_t(b[p+n])|(std::uint32_t(b[p+n+1])<<8)|(std::uint32_t(b[p+n+2])<<16)|(std::uint32_t(b[p+n+3])<<24);
        t<<(wide64?"rip+":"disp32")<<"0x"<<std::hex<<d;n+=4;
    } else {
        t<<reg_name(rm,wide64);
        if(mod==1){if(p+n>=end)return false;t<<"+"<<int(static_cast<std::int8_t>(b[p+n]));n++;}
        else if(mod==2){if(p+n+4>end)return false;t<<"+disp32";n+=4;}
    }
    t<<"]";out.operand=t.str();out.length=n;return true;
}
void mark_relocation(const Artifact& a,Instruction& i){
    for(const auto& r:a.interfaces().relocations) if(r.address==i.address){
        i.is_relocated=true;i.relocation_type=r.type;i.relocation_symbol=r.symbol;i.provenance.source+="+reloc";i.provenance.confidence=0.95;
        if(!r.symbol.empty())i.operands.push_back({Operand::Kind::Symbol,r.symbol,0}); return;
    }
}
}
// --- AArch64 (ARM64) fixed-width decoder -------------------------------------------------
// A64 instructions are 4 bytes, little-endian. This recovers the control-flow-relevant
// subset (branches, compares, returns, indirect transfers, nop) needed to build a CFG;
// other encodings are represented conservatively as a ".inst" word rather than guessed.
namespace {
std::uint32_t a64_word(const std::vector<std::uint8_t>& b,std::size_t p){
    return std::uint32_t(b[p])|(std::uint32_t(b[p+1])<<8)|(std::uint32_t(b[p+2])<<16)|(std::uint32_t(b[p+3])<<24);
}
std::int64_t sign_extend(std::uint64_t v,unsigned bits){
    const std::uint64_t m=std::uint64_t(1)<<(bits-1); return std::int64_t((v^m)-m);
}
Instruction a64_base(std::size_t p,const std::vector<std::uint8_t>& b,const std::string& mnem,double conf=0.85){
    Instruction i;i.address=p;i.bytes.assign(b.begin()+p,b.begin()+p+4);i.mnemonic=mnem;i.provenance={p,p,"stage4-arm64",conf};return i;
}
Instruction decode_a64(const std::vector<std::uint8_t>& b,std::size_t p){
    const std::uint32_t w=a64_word(b,p);
    auto rel=[&](std::int64_t imm){return std::uint64_t(std::int64_t(p)+imm);};
    if(w==0xd503201f) return a64_base(p,b,"nop");
    if(w==0xd65f03c0){auto i=a64_base(p,b,"ret");i.is_return=true;return i;}            // RET x30
    if((w&0xfffffc1f)==0xd65f0000){auto i=a64_base(p,b,"ret");i.is_return=true;i.is_indirect=true;return i;} // RET Xn
    if((w&0xfc000000)==0x14000000||(w&0xfc000000)==0x94000000){                          // B / BL imm26
        const bool link=(w&0x80000000)!=0; auto t=rel(sign_extend((w&0x03ffffff),26)*4);
        auto i=a64_base(p,b,link?"bl":"b"); if(link)i.is_call=true;else i.is_branch=true;
        i.branch_targets.push_back(t); i.operands.push_back({Operand::Kind::Relative,hex(t),std::int64_t(t)}); return i;
    }
    if((w&0xff000010)==0x54000000){                                                      // B.cond imm19
        auto t=rel(sign_extend((w>>5)&0x7ffff,19)*4); auto i=a64_base(p,b,"b.cond");
        i.is_branch=true;i.is_conditional=true;i.branch_targets.push_back(t);i.operands.push_back({Operand::Kind::Relative,hex(t),std::int64_t(t)});return i;
    }
    if((w&0x7e000000)==0x34000000){                                                      // CBZ/CBNZ imm19
        auto t=rel(sign_extend((w>>5)&0x7ffff,19)*4); auto i=a64_base(p,b,(w&0x01000000)?"cbnz":"cbz");
        i.is_branch=true;i.is_conditional=true;i.branch_targets.push_back(t);i.operands.push_back({Operand::Kind::Relative,hex(t),std::int64_t(t)});return i;
    }
    if((w&0x7e000000)==0x36000000){                                                      // TBZ/TBNZ imm14
        auto t=rel(sign_extend((w>>5)&0x3fff,14)*4); auto i=a64_base(p,b,(w&0x01000000)?"tbnz":"tbz");
        i.is_branch=true;i.is_conditional=true;i.branch_targets.push_back(t);i.operands.push_back({Operand::Kind::Relative,hex(t),std::int64_t(t)});return i;
    }
    if((w&0xfffffc1f)==0xd61f0000){auto i=a64_base(p,b,"br");i.is_branch=true;i.is_indirect=true;return i;}   // BR Xn
    if((w&0xfffffc1f)==0xd63f0000){auto i=a64_base(p,b,"blr");i.is_call=true;i.is_indirect=true;return i;}    // BLR Xn
    auto i=a64_base(p,b,".inst",0.30); i.operands.push_back({Operand::Kind::Immediate,hex(w),std::int64_t(w)}); return i;
}
}

std::vector<Instruction> Decoder::decode(const Artifact& a,std::uint64_t address,std::size_t length) const {
    std::vector<Instruction> out;const auto& b=a.bytes();std::size_t p=static_cast<std::size_t>(address);
    if(p>=b.size())return out;const auto end=std::min(b.size(),p+length);
    if(a.architecture()==Architecture::ARM64){
        for(;p+4<=end;p+=4) out.push_back(decode_a64(b,p));
        for(;p<end;++p){auto i=base(b,p,"db",0.20);i.operands.push_back({Operand::Kind::Immediate,hex(b[p]),b[p]});i.provenance.source="stage4-arm64";out.push_back(std::move(i));}
        return out;
    }
    if(a.architecture()!=Architecture::X86&&a.architecture()!=Architecture::X86_64){
        for(;p<end;++p){auto i=base(b,p,"db",0.25);i.operands.push_back({Operand::Kind::Immediate,hex(b[p]),b[p]});out.push_back(std::move(i));}return out;
    }
    while(p<end){
        const std::size_t start=p; const std::uint8_t op=b[p]; Instruction i; std::size_t n=1;
        if(op==0x90)i=base(b,p,"nop");
        else if(op>=0x50&&op<=0x5f){i=base(b,p,op<0x58?"push":"pop");i.operands.push_back({Operand::Kind::Register,reg_name(op&7,a.architecture()==Architecture::X86_64),0});}
        else if(op==0xc3){i=base(b,p,"ret");i.is_return=true;}
        else if(op==0xc2&&p+2<end){i=base(b,p,"ret");n=3;i.is_return=true;i.operands.push_back({Operand::Kind::Immediate,"imm16",std::int64_t(b[p+1]|(std::uint16_t(b[p+2])<<8))});}
        else if(op==0xe8&&p+4<end){i=base(b,p,"call");n=5;i.is_call=true;auto target=std::uint64_t(std::int64_t(p+n)+rel32(b,p+1));i.branch_targets.push_back(target);i.operands.push_back({Operand::Kind::Relative,hex(target),std::int64_t(target)});}
        else if(op==0xe9&&p+4<end){i=base(b,p,"jmp");n=5;i.is_branch=true;auto target=std::uint64_t(std::int64_t(p+n)+rel32(b,p+1));i.branch_targets.push_back(target);i.operands.push_back({Operand::Kind::Relative,hex(target),std::int64_t(target)});}
        else if(op==0xeb&&p+1<end){i=base(b,p,"jmp");n=2;i.is_branch=true;auto target=std::uint64_t(std::int64_t(p+n)+rel8(b[p+1]));i.branch_targets.push_back(target);i.operands.push_back({Operand::Kind::Relative,hex(target),std::int64_t(target)});}
        else if(op>=0x70&&op<=0x7f&&p+1<end){i=base(b,p,"jcc");n=2;i.is_branch=true;i.is_conditional=true;auto target=std::uint64_t(std::int64_t(p+n)+rel8(b[p+1]));i.branch_targets.push_back(target);i.operands.push_back({Operand::Kind::Relative,hex(target),std::int64_t(target)});}
        else if(op==0x0f&&p+5<end&&b[p+1]>=0x80&&b[p+1]<=0x8f){i=base(b,p,"jcc");n=6;i.is_branch=true;i.is_conditional=true;auto target=std::uint64_t(std::int64_t(p+n)+rel32(b,p+2));i.branch_targets.push_back(target);i.operands.push_back({Operand::Kind::Relative,hex(target),std::int64_t(target)});}
        else if(op==0xff&&p+1<end){
            ModRm mr;if(parse_modrm(b,p+1,end,a.architecture()==Architecture::X86_64,mr)){n=1+mr.length;i=base(b,p,(mr.reg&7)==2?"call":((mr.reg&7)==4?"jmp":"ff"));i.bytes.assign(b.begin()+p,b.begin()+std::min(end,p+n));i.operands.push_back({mr.kind,mr.operand,0});if((mr.reg&7)==2)i.is_call=true;if((mr.reg&7)==4)i.is_branch=true;if((mr.reg&7)==2||(mr.reg&7)==4)i.is_indirect=true;}else i=base(b,p,"db",0.35);
        } else if(op>=0xb8&&op<=0xbf){
            const bool wide=a.architecture()==Architecture::X86_64;const std::size_t imm=wide?8:4;
            if(p+1+imm<=end){i=base(b,p,"mov");n=1+imm;std::uint64_t v=0;for(std::size_t k=0;k<imm;++k)v|=std::uint64_t(b[p+1+k])<<(8*k);i.operands.push_back({Operand::Kind::Register,reg_name(op&7,wide),0});i.operands.push_back({Operand::Kind::Immediate,hex(v),std::int64_t(v)});}else i=base(b,p,"db",0.35);
        } else {i=base(b,p,"db",0.25);i.operands.push_back({Operand::Kind::Immediate,hex(b[p]),b[p]});}
        i.bytes.assign(b.begin()+start,b.begin()+std::min(end,start+n));mark_relocation(a,i);out.push_back(std::move(i));p+=n;
    }
    return out;
}
} // namespace sleela::decompiler
