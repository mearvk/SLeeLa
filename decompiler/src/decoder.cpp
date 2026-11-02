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
// --- ARM (A32) fixed-width decoder -------------------------------------------------------
// A32 instructions are 4 bytes, little-endian, with a 4-bit condition code in the top
// nibble (0xE = AL/always; 0xF is reserved for unconditional-encoding space). This
// recovers the control-flow-relevant subset (B/BL, BX/BLX register, pc-writing returns,
// nop) for a CFG; other encodings are represented conservatively as a ".inst" word.
// Thumb (T32) is variable-width and is intentionally out of scope for this increment.
namespace {
Instruction a32_base(std::size_t p,const std::vector<std::uint8_t>& b,const std::string& mnem,double conf=0.80){
    Instruction i;i.address=p;i.bytes.assign(b.begin()+p,b.begin()+p+4);i.mnemonic=mnem;i.provenance={p,p,"stage4-arm",conf};return i;
}
Instruction decode_a32(const std::vector<std::uint8_t>& b,std::size_t p){
    const std::uint32_t w=a64_word(b,p);                 // same little-endian word reader
    const std::uint32_t cond=w>>28;
    const bool always=(cond==0xE);
    if(w==0xe320f000) return a32_base(p,b,"nop");          // NOP (hint) on ARMv6K+
    if((w&0x0e000000)==0x0a000000){                        // B/BL: cccc 101L imm24
        const bool link=(w&0x01000000)!=0;
        const std::int64_t off=sign_extend(w&0x00ffffff,24)*4;
        const std::uint64_t t=std::uint64_t(std::int64_t(p)+8+off); // ARM PC reads as insn+8
        auto i=a32_base(p,b,link?"bl":"b");
        if(link)i.is_call=true;else i.is_branch=true;
        if(!always)i.is_conditional=true;
        i.branch_targets.push_back(t);i.operands.push_back({Operand::Kind::Relative,hex(t),std::int64_t(t)});return i;
    }
    if((w&0x0ffffff0)==0x012fff10){auto i=a32_base(p,b,"bx");i.is_branch=true;i.is_indirect=true;if((w&0xf)==14)i.is_return=true;if(!always)i.is_conditional=true;return i;}  // BX Rm (BX lr == return)
    if((w&0x0ffffff0)==0x012fff30){auto i=a32_base(p,b,"blx");i.is_call=true;i.is_indirect=true;if(!always)i.is_conditional=true;return i;}                                    // BLX Rm
    // Returns that write PC directly: LDM/POP with the PC bit set, or MOV pc,lr.
    if((w&0x0e108000)==0x08108000){auto i=a32_base(p,b,"ldm");i.is_return=true;i.is_indirect=true;if(!always)i.is_conditional=true;return i;}  // LDM.. {..,pc}
    if((w&0x0fffffff)==0x01a0f00e){auto i=a32_base(p,b,"mov");i.is_return=true;i.is_indirect=true;return i;}                                   // MOV pc, lr
    auto i=a32_base(p,b,".inst",0.30);i.operands.push_back({Operand::Kind::Immediate,hex(w),std::int64_t(w)});return i;
}
}
// --- ARM Thumb (T32) variable-width decoder ----------------------------------------------
// Thumb is 16-bit by default; a first halfword with top-5-bits in {11101,11110,11111}
// introduces a 32-bit instruction that consumes a second halfword. This recovers the
// control-flow subset -- conditional/unconditional B, BL/BLX(imm), BX/BLX(reg), CBZ/CBNZ,
// pc-writing POP, and NOP -- which is what the CFG stage needs. Each decoded instruction
// reports its own byte length so the caller advances correctly; unknown encodings become
// ".inst" of their detected width rather than being guessed.
namespace {
std::uint16_t thumb_hw(const std::vector<std::uint8_t>& b,std::size_t p){
    return std::uint16_t(b[p])|(std::uint16_t(b[p+1])<<8);
}
bool thumb_is_32bit(std::uint16_t hw1){ const std::uint16_t top=hw1&0xf800; return top==0xe800||top==0xf000||top==0xf800; }
Instruction thumb_base(std::size_t p,const std::vector<std::uint8_t>& b,std::size_t len,const std::string& mnem,double conf=0.80){
    Instruction i;i.address=p;i.bytes.assign(b.begin()+p,b.begin()+p+len);i.mnemonic=mnem;i.provenance={p,p,"stage4-thumb",conf};return i;
}
// Decode one Thumb instruction at p (p+2 <= end guaranteed by caller). out_len is set to 2 or 4.
Instruction decode_thumb(const std::vector<std::uint8_t>& b,std::size_t p,std::size_t end,std::size_t& out_len){
    const std::uint16_t hw1=thumb_hw(b,p);
    auto rel=[&](std::int64_t imm){return std::uint64_t(std::int64_t(p)+4+imm);}; // Thumb PC reads as insn+4
    if(thumb_is_32bit(hw1)&&p+4<=end){
        out_len=4; const std::uint16_t hw2=thumb_hw(b,p+2);
        // 32-bit B/BL/BLX family: hw1[15:11]=11110, hw2[15]=1, hw2[14]=1 selects branch class.
        if((hw1&0xf800)==0xf000&&(hw2&0x8000)==0x8000){
            const std::uint32_t s=(hw1>>10)&1, imm10=hw1&0x3ff, j1=(hw2>>13)&1, j2=(hw2>>11)&1, imm11=hw2&0x7ff;
            const std::uint32_t op2=(hw2>>12)&0x7; // bit14..12
            const std::uint32_t i1=~(j1^s)&1, i2=~(j2^s)&1;
            if(op2==0x5||op2==0x7){ // BL (op2 bit14=1,bit12=1) / BLX(imm) (bit12=0) -> both are calls
                const std::uint32_t imm=(s<<24)|(i1<<23)|(i2<<22)|(imm10<<12)|(imm11<<1);
                auto t=rel(sign_extend(imm,25)); auto in=thumb_base(p,b,4,(op2&0x1)?"bl":"blx");
                in.is_call=true; in.branch_targets.push_back(t); in.operands.push_back({Operand::Kind::Relative,hex(t),std::int64_t(t)}); return in;
            }
            if(op2==0x1||op2==0x3){ // B.W unconditional (T4)
                const std::uint32_t imm=(s<<24)|(i1<<23)|(i2<<22)|(imm10<<12)|(imm11<<1);
                auto t=rel(sign_extend(imm,25)); auto in=thumb_base(p,b,4,"b.w"); in.is_branch=true;
                in.branch_targets.push_back(t); in.operands.push_back({Operand::Kind::Relative,hex(t),std::int64_t(t)}); return in;
            }
            if(op2==0x0||op2==0x2){ // B<cond>.W (T3): cond in hw1[9:6]
                const std::uint32_t cond=(hw1>>6)&0xf;
                if(cond<0xe){ const std::uint32_t imm11b=hw2&0x7ff, imm6=hw1&0x3f, j1b=(hw2>>13)&1, j2b=(hw2>>11)&1, sb=(hw1>>10)&1;
                    const std::uint32_t imm=(sb<<20)|(j2b<<19)|(j1b<<18)|(imm6<<12)|(imm11b<<1);
                    auto t=rel(sign_extend(imm,21)); auto in=thumb_base(p,b,4,"b.w"); in.is_branch=true; in.is_conditional=true;
                    in.branch_targets.push_back(t); in.operands.push_back({Operand::Kind::Relative,hex(t),std::int64_t(t)}); return in;
                }
            }
        }
        auto in=thumb_base(p,b,4,".inst",0.30); in.operands.push_back({Operand::Kind::Immediate,hex((std::uint32_t(hw1)<<16)|hw2),std::int64_t((std::uint32_t(hw1)<<16)|hw2)}); return in;
    }
    out_len=2;
    if(hw1==0xbf00) return thumb_base(p,b,2,"nop");
    if((hw1&0xff00)==0x4700){ const bool link=(hw1&0x0080)!=0; const std::uint32_t rm=(hw1>>3)&0xf; // BX/BLX Rm
        auto in=thumb_base(p,b,2,link?"blx":"bx"); if(link)in.is_call=true;else in.is_branch=true; in.is_indirect=true; if(!link&&rm==14)in.is_return=true; return in; }
    if((hw1&0xfe00)==0xbc00&&(hw1&0x0100)){auto in=thumb_base(p,b,2,"pop");in.is_return=true;in.is_indirect=true;return in;} // POP {..,pc} -> return (check before PUSH mask)
    if((hw1&0xfe00)==0xb400){auto in=thumb_base(p,b,2,"push");return in;}      // PUSH {reglist}/{..,lr}
    if((hw1&0xfe00)==0xbc00){auto in=thumb_base(p,b,2,"pop");return in;}       // POP {reglist} (no pc)
    if((hw1&0xf000)==0xd000){ const std::uint32_t cond=(hw1>>8)&0xf;           // B<cond> (T1), imm8
        if(cond==0xe){auto in=thumb_base(p,b,2,".inst",0.30);return in;}         // 0xDE = permanently UNDEFINED
        if(cond==0xf){auto in=thumb_base(p,b,2,"svc");return in;}                // 0xDF = SVC, not a branch
        auto t=rel(sign_extend(hw1&0xff,8)*2); auto in=thumb_base(p,b,2,"b"); in.is_branch=true; in.is_conditional=true;
        in.branch_targets.push_back(t); in.operands.push_back({Operand::Kind::Relative,hex(t),std::int64_t(t)}); return in; }
    if((hw1&0xf800)==0xe000){ auto t=rel(sign_extend(hw1&0x7ff,11)*2);          // B (T2) unconditional, imm11
        auto in=thumb_base(p,b,2,"b"); in.is_branch=true; in.branch_targets.push_back(t); in.operands.push_back({Operand::Kind::Relative,hex(t),std::int64_t(t)}); return in; }
    if((hw1&0xf500)==0xb100){ const std::uint32_t i5=((hw1>>9)&1), imm5=(hw1>>3)&0x1f; // CBZ/CBNZ, positive imm only
        auto t=std::uint64_t(std::int64_t(p)+4+((i5<<6)|(imm5<<1))); auto in=thumb_base(p,b,2,(hw1&0x0800)?"cbnz":"cbz");
        in.is_branch=true; in.is_conditional=true; in.branch_targets.push_back(t); in.operands.push_back({Operand::Kind::Relative,hex(t),std::int64_t(t)}); return in; }
    auto in=thumb_base(p,b,2,".inst",0.30); in.operands.push_back({Operand::Kind::Immediate,hex(hw1),hw1}); return in;
}
}

std::vector<Instruction> Decoder::decode(const Artifact& a,std::uint64_t address,std::size_t length) const {
    return decode(a,address,length,DecodeMode::Auto);
}

std::vector<Instruction> Decoder::decode(const Artifact& a,std::uint64_t address,std::size_t length,DecodeMode mode) const {
    std::vector<Instruction> out;const auto& b=a.bytes();std::size_t p=static_cast<std::size_t>(address);
    if(p>=b.size())return out;const auto end=std::min(b.size(),p+length);
    if(a.architecture()==Architecture::ARM64){
        for(;p+4<=end;p+=4) out.push_back(decode_a64(b,p));
        for(;p<end;++p){auto i=base(b,p,"db",0.20);i.operands.push_back({Operand::Kind::Immediate,hex(b[p]),b[p]});i.provenance.source="stage4-arm64";out.push_back(std::move(i));}
        return out;
    }
    if(a.architecture()==Architecture::ARM&&mode==DecodeMode::ArmThumb){
        while(p+2<=end){std::size_t len=2; out.push_back(decode_thumb(b,p,end,len)); if(len==0)len=2; p+=len;}
        for(;p<end;++p){auto i=base(b,p,"db",0.20);i.operands.push_back({Operand::Kind::Immediate,hex(b[p]),b[p]});i.provenance.source="stage4-thumb";out.push_back(std::move(i));}
        return out;
    }
    if(a.architecture()==Architecture::ARM){
        for(;p+4<=end;p+=4) out.push_back(decode_a32(b,p));
        for(;p<end;++p){auto i=base(b,p,"db",0.20);i.operands.push_back({Operand::Kind::Immediate,hex(b[p]),b[p]});i.provenance.source="stage4-arm";out.push_back(std::move(i));}
        return out;
    }
    if(a.architecture()!=Architecture::X86&&a.architecture()!=Architecture::X86_64){
        for(;p<end;++p){auto i=base(b,p,"db",0.25);i.operands.push_back({Operand::Kind::Immediate,hex(b[p]),b[p]});out.push_back(std::move(i));}return out;
    }
    const bool mode64=a.architecture()==Architecture::X86_64;
    // Common names for the ModR/M arithmetic/logic group keyed by the primary opcode.
    auto alu_name=[](std::uint8_t op)->const char*{
        switch(op&0x38){case 0x00:return "add";case 0x08:return "or";case 0x10:return "adc";case 0x18:return "sbb";
            case 0x20:return "and";case 0x28:return "sub";case 0x30:return "xor";case 0x38:return "cmp";}return "alu";};
    while(p<end){
        const std::size_t start=p; Instruction i; std::size_t n=1;
        // --- Prefix pre-pass: consume a legacy-prefix run and, in 64-bit mode, one REX byte.
        // Zero prefixes leaves the opcode position and length identical to the original decoder.
        std::size_t pfx=0; bool rex_w=false; bool have_prefix=false; bool opsize16=false;
        while(start+pfx<end){
            const std::uint8_t c=b[start+pfx];
            const bool legacy=(c==0x66||c==0x67||c==0x2e||c==0x36||c==0x3e||c==0x26||c==0x64||c==0x65||c==0xf0||c==0xf2||c==0xf3);
            if(legacy){have_prefix=true;if(c==0x66)opsize16=true;++pfx;continue;}
            if(mode64&&c>=0x40&&c<=0x4f){have_prefix=true;rex_w=(c&0x08)!=0;++pfx; /*REX is the last prefix before the opcode*/ break;}
            break;
        }
        const std::size_t o=start+pfx;            // opcode offset after any prefixes
        if(o>=end){ i=base(b,start,"db",0.25); i.operands.push_back({Operand::Kind::Immediate,hex(b[start]),b[start]}); i.bytes.assign(b.begin()+start,b.begin()+std::min(end,start+1)); mark_relocation(a,i); out.push_back(std::move(i)); p=start+1; continue; }
        const std::uint8_t op=b[o];
        const bool wide=mode64&&rex_w;            // REX.W selects 64-bit operand size for reg names
        auto emit=[&](const std::string& m,double c=0.80){Instruction x;x.address=start;x.bytes.push_back(b[start]);x.mnemonic=m;x.provenance={start,start,"stage4-x86",c};return x;};
        auto consume=[&](std::size_t op_len){ n=pfx+op_len; }; // total length = prefixes + opcode-relative length
        if(op==0x90){i=emit("nop");consume(1);}
        else if(op==0xcc){i=emit("int3");consume(1);}
        else if(op==0xf4){i=emit("hlt");i.is_return=false;consume(1);}
        else if(op==0xc9){i=emit("leave");consume(1);}
        else if(op==0x98){i=emit(wide?"cdqe":"cwde");consume(1);}
        else if(op==0x99){i=emit(wide?"cqo":"cdq");consume(1);}
        else if(op>=0x50&&op<=0x5f){i=emit(op<0x58?"push":"pop");i.operands.push_back({Operand::Kind::Register,reg_name(op&7,mode64),0});consume(1);}
        else if(op==0x68&&o+4<end){i=emit("push");std::uint32_t v=std::uint32_t(rel32(b,o+1));i.operands.push_back({Operand::Kind::Immediate,hex(v),std::int64_t(std::int32_t(v))});consume(5);}
        else if(op==0x6a&&o+1<end){i=emit("push");i.operands.push_back({Operand::Kind::Immediate,hex(std::uint8_t(b[o+1])),std::int64_t(rel8(b[o+1]))});consume(2);}
        else if(op==0xc3){i=emit("ret");i.is_return=true;consume(1);}
        else if(op==0xc2&&o+2<end){i=emit("ret");i.is_return=true;i.operands.push_back({Operand::Kind::Immediate,"imm16",std::int64_t(b[o+1]|(std::uint16_t(b[o+2])<<8))});consume(3);}
        else if(op==0xe8&&o+4<end){i=emit("call");i.is_call=true;consume(5);auto target=std::uint64_t(std::int64_t(start+n)+rel32(b,o+1));i.branch_targets.push_back(target);i.operands.push_back({Operand::Kind::Relative,hex(target),std::int64_t(target)});}
        else if(op==0xe9&&o+4<end){i=emit("jmp");i.is_branch=true;consume(5);auto target=std::uint64_t(std::int64_t(start+n)+rel32(b,o+1));i.branch_targets.push_back(target);i.operands.push_back({Operand::Kind::Relative,hex(target),std::int64_t(target)});}
        else if(op==0xeb&&o+1<end){i=emit("jmp");i.is_branch=true;consume(2);auto target=std::uint64_t(std::int64_t(start+n)+rel8(b[o+1]));i.branch_targets.push_back(target);i.operands.push_back({Operand::Kind::Relative,hex(target),std::int64_t(target)});}
        else if(op>=0x70&&op<=0x7f&&o+1<end){i=emit("jcc");i.is_branch=true;i.is_conditional=true;consume(2);auto target=std::uint64_t(std::int64_t(start+n)+rel8(b[o+1]));i.branch_targets.push_back(target);i.operands.push_back({Operand::Kind::Relative,hex(target),std::int64_t(target)});}
        else if(op==0x0f&&o+5<end&&b[o+1]>=0x80&&b[o+1]<=0x8f){i=emit("jcc");i.is_branch=true;i.is_conditional=true;consume(6);auto target=std::uint64_t(std::int64_t(start+n)+rel32(b,o+2));i.branch_targets.push_back(target);i.operands.push_back({Operand::Kind::Relative,hex(target),std::int64_t(target)});}
        else if(op==0xff&&o+1<end){
            ModRm mr;if(parse_modrm(b,o+1,end,mode64,mr)){consume(1+mr.length);i=emit((mr.reg&7)==2?"call":((mr.reg&7)==4?"jmp":((mr.reg&7)==6?"push":"inc")));i.operands.push_back({mr.kind,mr.operand,0});if((mr.reg&7)==2)i.is_call=true;if((mr.reg&7)==4)i.is_branch=true;if((mr.reg&7)==2||(mr.reg&7)==4)i.is_indirect=true;}else{i=emit("db",0.35);consume(1);}
        }
        // ModR/M arithmetic/logic group: add/or/adc/sbb/and/sub/xor/cmp (opcodes x0/x1/x8/x9), mov (88/89/8a/8b), test (84/85), lea (8d), xchg (86/87).
        else if(((op&0xc4)==0x00&&(op&0x07)<=0x03) || op==0x88||op==0x89||op==0x8a||op==0x8b||op==0x84||op==0x85||op==0x8d||op==0x86||op==0x87){
            ModRm mr;if(o+1<end&&parse_modrm(b,o+1,end,mode64,mr)){consume(1+mr.length);
                const char* nm = op==0x8d?"lea":((op==0x88||op==0x89||op==0x8a||op==0x8b)?"mov":((op==0x84||op==0x85)?"test":((op==0x86||op==0x87)?"xchg":alu_name(op))));
                i=emit(nm);i.operands.push_back({mr.kind,mr.operand,0});
            } else {i=emit("db",0.35);consume(1);}
        }
        // Immediate ALU group (80 /r ib, 81 /r iz, 83 /r ib): the /digit field selects the operation.
        else if(op==0x80||op==0x81||op==0x83){
            ModRm mr;if(o+1<end&&parse_modrm(b,o+1,end,mode64,mr)){
                const std::size_t immlen = op==0x81 ? (opsize16?2:4) : 1;   // 81 = full-width imm (16 under 0x66); 80/83 = imm8
                if(o+1+mr.length+immlen<=end){consume(1+mr.length+immlen);
                    // The /digit (ModR/M reg field) selects the ALU operation for the 80/81/83 group.
                    const char* nm=[&]{switch(mr.reg&7){case 0:return "add";case 1:return "or";case 2:return "adc";case 3:return "sbb";case 4:return "and";case 5:return "sub";case 6:return "xor";default:return "cmp";}}();
                    i=emit(nm);
                    i.operands.push_back({mr.kind,mr.operand,0});
                    std::int64_t imm=0; for(std::size_t k=0;k<immlen;++k)imm|=std::int64_t(b[o+1+mr.length+k])<<(8*k);
                    i.operands.push_back({Operand::Kind::Immediate,hex(std::uint64_t(imm)),imm});
                } else {i=emit("db",0.35);consume(1);}
            } else {i=emit("db",0.35);consume(1);}
        }
        // mov r/m, imm (C6 /0 ib, C7 /0 iz).
        else if(op==0xc6||op==0xc7){
            ModRm mr;if(o+1<end&&parse_modrm(b,o+1,end,mode64,mr)){
                const std::size_t immlen = op==0xc6 ? 1 : (opsize16?2:4);
                if(o+1+mr.length+immlen<=end){consume(1+mr.length+immlen);i=emit("mov");i.operands.push_back({mr.kind,mr.operand,0});
                    std::uint64_t v=0; for(std::size_t k=0;k<immlen;++k)v|=std::uint64_t(b[o+1+mr.length+k])<<(8*k);
                    i.operands.push_back({Operand::Kind::Immediate,hex(v),std::int64_t(v)});
                } else {i=emit("db",0.35);consume(1);}
            } else {i=emit("db",0.35);consume(1);}
        }
        else if(op>=0xb8&&op<=0xbf){
            const std::size_t imm=wide?8:4;
            if(o+1+imm<=end){i=emit("mov");consume(1+imm);std::uint64_t v=0;for(std::size_t k=0;k<imm;++k)v|=std::uint64_t(b[o+1+k])<<(8*k);i.operands.push_back({Operand::Kind::Register,reg_name(op&7,wide),0});i.operands.push_back({Operand::Kind::Immediate,hex(v),std::int64_t(v)});}else{i=emit("db",0.35);consume(1);}
        } else {i=emit("db",0.25);i.operands.push_back({Operand::Kind::Immediate,hex(op),op});consume(1);(void)have_prefix;}
        if(n==0)n=1;                                // defensive: never zero-advance
        i.bytes.assign(b.begin()+start,b.begin()+std::min(end,start+n));mark_relocation(a,i);out.push_back(std::move(i));p=start+n;
    }
    return out;
}
} // namespace sleela::decompiler
