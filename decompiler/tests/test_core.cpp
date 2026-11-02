#include "sleela/decompiler/decompiler.hpp"
#include "sleela/decompiler/errors.hpp"
#include "sleela/decompiler/output.hpp"
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <string>
#include <vector>
static void put16(std::vector<std::uint8_t>& b,std::size_t p,std::uint16_t v){b[p]=std::uint8_t(v);b[p+1]=std::uint8_t(v>>8);}
static void put32(std::vector<std::uint8_t>& b,std::size_t p,std::uint32_t v){for(int i=0;i<4;++i)b[p+i]=std::uint8_t(v>>(8*i));}
static void put64(std::vector<std::uint8_t>& b,std::size_t p,std::uint64_t v){for(int i=0;i<8;++i)b[p+i]=std::uint8_t(v>>(8*i));}
int main(){
    {
        using namespace sleela::decompiler;
        DecompilerError e(ErrorCode::InvalidAddress, "bad address", 17, 0x401000, "decoder");
        assert(e.code() == ErrorCode::InvalidAddress);
        assert(e.file_offset() == 17);
        assert(e.virtual_address() == 0x401000);
        assert(e.stage() == "decoder");
        assert(std::string(error_code_name(e.code())) == "InvalidAddress");
    }

    {const unsigned char bytes[]={'M','Z',0,1,2,3};auto a=sleela::decompiler::Artifact::from_bytes(bytes,"sample.exe");assert(a.format()==sleela::decompiler::Format::PE);assert(a.size()==6);auto i=sleela::decompiler::Decoder{}.decode(a,0,4);assert(i.size()==4);}
    {std::vector<std::uint8_t> ar{'!','<','a','r','c','h','>','\n'};std::string h(60,' ');h.replace(0,16,"member.o/");h.replace(48,10,"2");h[58]=char(96);h[59]='\n';ar.insert(ar.end(),h.begin(),h.end());ar.insert(ar.end(),{0x7f,'E','L','F'});auto a=sleela::decompiler::Artifact::from_bytes(ar,"libsample.a");assert(a.format()==sleela::decompiler::Format::GNUArchive);assert(a.artifact_class()==sleela::decompiler::ArtifactClass::StaticArchive);assert(a.interfaces().archive_members.size()==1);assert(a.interfaces().archive_members[0].name=="member.o");assert(a.interfaces().archive_members[0].format==sleela::decompiler::Format::ELF);}
    {std::vector<std::uint8_t> elf(0x140,0);elf[0]=0x7f;elf[1]='E';elf[2]='L';elf[3]='F';elf[4]=2;elf[5]=1;elf[6]=1;put16(elf,16,2);put16(elf,18,62);put64(elf,24,0x100);put64(elf,32,0);put64(elf,40,0);put16(elf,52,64);put16(elf,54,56);put16(elf,56,0);
        const std::uint8_t code[]={0xe8,0x01,0x00,0x00,0x00,0x74,0x02,0xff,0xd0,0xc3};std::copy(std::begin(code),std::end(code),elf.begin()+0x100);
        auto a=sleela::decompiler::Artifact::from_bytes(elf,"fixture.elf");assert(a.format()==sleela::decompiler::Format::ELF);assert(a.architecture()==sleela::decompiler::Architecture::X86_64);
        auto ins=sleela::decompiler::Decoder{}.decode(a,0x100,sizeof(code));assert(ins.size()==4);assert(ins[0].is_call&&ins[0].branch_targets[0]==0x106);assert(ins[1].is_branch&&ins[1].is_conditional&&ins[1].branch_targets[0]==0x109);assert(ins[2].is_call&&ins[2].is_indirect&&ins[2].operands[0].kind==sleela::decompiler::Operand::Kind::Register);assert(ins[3].is_return);
        auto cfg=sleela::decompiler::Analyzer{}.build_cfg(ins);assert(cfg.blocks.size()>=3);assert(!cfg.edges.empty());auto funcs=sleela::decompiler::Analyzer{}.recover_function_candidates(a,cfg);assert(!funcs.empty());
    }
    // PE/COFF: real import-directory recovery (DLL dependency + imported function name).
    {
        using namespace sleela::decompiler;
        std::vector<std::uint8_t> pe(0x400,0);
        pe[0]='M';pe[1]='Z'; put32(pe,0x3c,0x80);              // e_lfanew -> PE header at 0x80
        const std::size_t h=0x80; pe[h]='P';pe[h+1]='E';pe[h+2]=0;pe[h+3]=0;
        put16(pe,h+4,0x8664);                                   // machine x86-64
        put16(pe,h+6,1);                                        // 1 section
        put16(pe,h+20,0xF0);                                    // SizeOfOptionalHeader
        put16(pe,h+22,0x0002);                                  // characteristics: EXECUTABLE_IMAGE
        const std::size_t opt=h+24; put16(pe,opt,0x20b);        // PE32+ magic
        put64(pe,opt+24,0x140000000ull);                        // ImageBase
        put32(pe,opt+108,16);                                   // NumberOfRvaAndSizes
        const std::size_t dirs=opt+112;
        // Section ".idata" maps RVA 0x1000 -> file 0x200.
        const std::size_t st=opt+0xF0; const char* sn=".idata";
        for(int i=0;i<6;++i)pe[st+i]=std::uint8_t(sn[i]);
        put32(pe,st+8,0x200);put32(pe,st+12,0x1000);put32(pe,st+16,0x200);put32(pe,st+20,0x200);
        // Import directory at RVA 0x1000 (file 0x200): one descriptor + terminator.
        put32(pe,dirs+1*8,0x1000);put32(pe,dirs+1*8+4,40);      // data directory [1] = IMPORT
        const std::size_t id=0x200;
        put32(pe,id+0,0x1040);                                   // OriginalFirstThunk RVA 0x1040 (file 0x240)
        put32(pe,id+12,0x1060);                                  // Name RVA 0x1060 (file 0x260)
        put32(pe,id+16,0x1040);                                  // FirstThunk
        // terminator descriptor is the zeroed 20 bytes that follow
        put64(pe,0x240,0x1050);                                  // thunk -> hint/name RVA 0x1050 (file 0x250), then 0 terminator
        const char* fn="CreateFileW"; for(std::size_t i=0;i<11;++i)pe[0x250+2+i]=std::uint8_t(fn[i]);
        const char* dll="KERNEL32.dll"; for(std::size_t i=0;i<12;++i)pe[0x260+i]=std::uint8_t(dll[i]);
        auto a=Artifact::from_bytes(pe,"sample.exe");
        assert(a.format()==Format::PE);
        assert(a.architecture()==Architecture::X86_64);
        assert(a.interfaces().sections.size()==1);
        bool saw_dll=false,saw_fn=false;
        for(const auto& im:a.interfaces().imports){if(im.library=="KERNEL32.dll")saw_dll=true;if(im.name=="CreateFileW")saw_fn=true;}
        assert(saw_dll&&saw_fn);
    }

    // Mach-O: thin 64-bit object header is recognized and classified.
    {
        using namespace sleela::decompiler;
        std::vector<std::uint8_t> mo(0x80,0);
        mo[0]=0xcf;mo[1]=0xfa;mo[2]=0xed;mo[3]=0xfe;                 // MH_MAGIC_64 (little-endian)
        put32(mo,4,0x01000007);                                      // cputype x86_64
        put32(mo,12,2);                                              // filetype MH_EXECUTE
        put32(mo,16,0);                                             // ncmds 0 (header-only fixture)
        auto a=Artifact::from_bytes(mo,"sample.macho");
        assert(a.format()==Format::MachO);
        assert(a.architecture()==Architecture::X86_64);
        assert(a.artifact_class()==ArtifactClass::Executable);
    }

    // ARM (A32): real control-flow decode (BL, B, BX lr=return) and CFG construction.
    {
        using namespace sleela::decompiler;
        std::vector<std::uint8_t> elf(0x140,0);
        elf[0]=0x7f;elf[1]='E';elf[2]='L';elf[3]='F';elf[4]=1;elf[5]=1;elf[6]=1;   // 32-bit ELF
        put16(elf,16,2);put16(elf,18,40);                                          // ET_EXEC, EM_ARM
        put32(elf,28,0);put32(elf,32,0);put16(elf,40,32);put16(elf,42,56);put16(elf,44,0);
        auto w=[&](std::size_t off,std::uint32_t v){for(int k=0;k<4;++k)elf[off+k]=std::uint8_t(v>>(8*k));};
        w(0x100,0xeb000000);   // bl  (pc+8 + 0)
        w(0x104,0xea000001);   // b   (pc+8 + 4)
        w(0x108,0xe12fff1e);   // bx lr  -> return
        auto a=Artifact::from_bytes(elf,"fixture.arm");
        assert(a.architecture()==Architecture::ARM);
        auto ins=Decoder{}.decode(a,0x100,12);
        assert(ins.size()==3);
        assert(ins[0].is_call&&ins[0].mnemonic=="bl");
        assert(ins[1].is_branch&&ins[1].mnemonic=="b");
        assert(ins[2].is_return&&ins[2].mnemonic=="bx");
        auto cfg=Analyzer{}.build_cfg(ins);
        assert(!cfg.blocks.empty());
    }

    // ARM Thumb (T32): variable-width control-flow decode, bytes from llvm-mc (thumbv7).
    // Layout: bl(4) b.w(4) beq(2) b(2) cbz(2) bx-lr(2) nop(2) nop(2) = 8 instructions.
    {
        using namespace sleela::decompiler;
        std::vector<std::uint8_t> elf(0x140,0);
        elf[0]=0x7f;elf[1]='E';elf[2]='L';elf[3]='F';elf[4]=1;elf[5]=1;elf[6]=1;   // 32-bit ELF
        put16(elf,16,2);put16(elf,18,40);                                          // ET_EXEC, EM_ARM
        put16(elf,40,32);put16(elf,42,56);put16(elf,44,0);
        const std::uint8_t code[]={0x00,0xf0,0x07,0xf8, 0x00,0xf0,0x05,0xb8, 0x03,0xd0, 0x02,0xe0, 0x08,0xb1, 0x70,0x47, 0x00,0xbf, 0x00,0xbf};
        std::copy(std::begin(code),std::end(code),elf.begin()+0x100);
        auto a=Artifact::from_bytes(elf,"fixture.thumb");
        assert(a.architecture()==Architecture::ARM);
        auto ins=Decoder{}.decode(a,0x100,sizeof(code),DecodeMode::ArmThumb);
        assert(ins.size()==8);                                   // widths resolved correctly
        assert(ins[0].mnemonic=="bl"&&ins[0].is_call&&ins[0].bytes.size()==4);
        assert(ins[1].mnemonic=="b.w"&&ins[1].is_branch&&ins[1].bytes.size()==4);
        assert(ins[2].mnemonic=="b"&&ins[2].is_conditional&&ins[2].bytes.size()==2);
        assert(ins[3].mnemonic=="b"&&ins[3].is_branch&&!ins[3].is_conditional);
        assert(ins[4].mnemonic=="cbz"&&ins[4].is_conditional);
        assert(ins[5].mnemonic=="bx"&&ins[5].is_return);
        assert(ins[6].mnemonic=="nop"&&ins[7].mnemonic=="nop");
        // All five branch/call instructions should resolve to the same target (0x112).
        assert(ins[0].branch_targets[0]==0x112);
        assert(ins[1].branch_targets[0]==0x112);
        assert(ins[2].branch_targets[0]==0x112);
        assert(ins[3].branch_targets[0]==0x112);
        assert(ins[4].branch_targets[0]==0x112);
        auto cfg=Analyzer{}.build_cfg(ins);
        assert(!cfg.blocks.empty());
        // POP {..,pc} must classify as a return and not be shadowed by the PUSH mask.
        // push {r4,lr}=10 b5 | pop {r4,pc}=10 bd | pop {r4,r5}=30 bc  (llvm-mc thumbv7)
        std::vector<std::uint8_t> pp(0x140,0);
        pp[0]=0x7f;pp[1]='E';pp[2]='L';pp[3]='F';pp[4]=1;pp[5]=1;pp[6]=1;put16(pp,16,2);put16(pp,18,40);
        const std::uint8_t ppc[]={0x10,0xb5, 0x10,0xbd, 0x30,0xbc};
        std::copy(std::begin(ppc),std::end(ppc),pp.begin()+0x100);
        auto pa=Artifact::from_bytes(pp,"fixture.thumbpp");
        auto pi=Decoder{}.decode(pa,0x100,sizeof(ppc),DecodeMode::ArmThumb);
        assert(pi.size()==3);
        assert(pi[0].mnemonic=="push"&&!pi[0].is_return);
        assert(pi[1].mnemonic=="pop"&&pi[1].is_return);
        assert(pi[2].mnemonic=="pop"&&!pi[2].is_return);
    }

    // x86-64 extended opcodes: REX.W prefix, ModR/M mov, leave, int3, ret.
    {
        using namespace sleela::decompiler;
        std::vector<std::uint8_t> elf(0x140,0);
        elf[0]=0x7f;elf[1]='E';elf[2]='L';elf[3]='F';elf[4]=2;elf[5]=1;elf[6]=1;
        put16(elf,16,2);put16(elf,18,62);put64(elf,24,0x100);put16(elf,52,64);put16(elf,54,56);put16(elf,56,0);
        // 48 89 e5  mov rbp,rsp (REX.W + mov r/m,r)  |  c9 leave  |  cc int3  |  c3 ret
        const std::uint8_t code[]={0x48,0x89,0xe5, 0xc9, 0xcc, 0xc3};
        std::copy(std::begin(code),std::end(code),elf.begin()+0x100);
        auto a=Artifact::from_bytes(elf,"fixture.x64");
        auto ins=Decoder{}.decode(a,0x100,sizeof(code));
        assert(ins.size()==4);                           // prefix folds into the mov
        assert(ins[0].mnemonic=="mov"&&ins[0].bytes.size()==3);
        assert(ins[1].mnemonic=="leave");
        assert(ins[2].mnemonic=="int3");
        assert(ins[3].mnemonic=="ret"&&ins[3].is_return);
    }

    // x86-64 immediate-group ALU + mov-imm, bytes from llvm-mc (x86_64).
    // sub rsp,0x10 (48 83 ec 10) | cmp [rbp-4],0x7f (83 7d fc 7f) | mov [rbp-8],0xdeadbeef (c7 45 f8 ..)
    {
        using namespace sleela::decompiler;
        std::vector<std::uint8_t> elf(0x140,0);
        elf[0]=0x7f;elf[1]='E';elf[2]='L';elf[3]='F';elf[4]=2;elf[5]=1;elf[6]=1;
        put16(elf,16,2);put16(elf,18,62);put64(elf,24,0x100);put16(elf,52,64);put16(elf,54,56);put16(elf,56,0);
        const std::uint8_t code[]={0x48,0x83,0xec,0x10, 0x83,0x7d,0xfc,0x7f, 0xc7,0x45,0xf8,0xef,0xbe,0xad,0xde};
        std::copy(std::begin(code),std::end(code),elf.begin()+0x100);
        auto a=Artifact::from_bytes(elf,"fixture.imm");
        auto ins=Decoder{}.decode(a,0x100,sizeof(code));
        assert(ins.size()==3);                                   // all three lengths resolved exactly
        assert(ins[0].mnemonic=="sub"&&ins[0].bytes.size()==4);
        assert(ins[1].mnemonic=="cmp"&&ins[1].bytes.size()==4);
        assert(ins[2].mnemonic=="mov"&&ins[2].bytes.size()==7);
        assert(ins[2].operands.size()>=2&&ins[2].operands.back().value==std::int64_t(0xdeadbeef));
    }

    // ARM64: real A64 control-flow decode (BL, B.cond, CBZ, RET) and CFG construction.
    {
        using namespace sleela::decompiler;
        std::vector<std::uint8_t> elf(0x140,0);
        elf[0]=0x7f;elf[1]='E';elf[2]='L';elf[3]='F';elf[4]=2;elf[5]=1;elf[6]=1;
        put16(elf,16,2);put16(elf,18,183);                          // ET_EXEC, EM_AARCH64
        put64(elf,24,0x100);put64(elf,32,0);put64(elf,40,0);put16(elf,52,64);put16(elf,54,56);put16(elf,56,0);
        auto w=[&](std::size_t off,std::uint32_t v){elf[off]=std::uint8_t(v);elf[off+1]=std::uint8_t(v>>8);elf[off+2]=std::uint8_t(v>>16);elf[off+3]=std::uint8_t(v>>24);};
        w(0x100,0x94000002);                                        // bl  +8
        w(0x104,0x54000040);                                        // b.cond +8
        w(0x108,0xb4000040);                                        // cbz  +8
        w(0x10c,0xd65f03c0);                                        // ret
        auto a=Artifact::from_bytes(elf,"fixture.arm64");
        assert(a.architecture()==Architecture::ARM64);
        auto ins=Decoder{}.decode(a,0x100,16);
        assert(ins.size()==4);
        assert(ins[0].is_call&&ins[0].mnemonic=="bl"&&ins[0].branch_targets[0]==0x108);
        assert(ins[1].is_branch&&ins[1].is_conditional&&ins[1].mnemonic=="b.cond");
        assert(ins[2].is_branch&&ins[2].is_conditional&&ins[2].mnemonic=="cbz");
        assert(ins[3].is_return);
        auto cfg=Analyzer{}.build_cfg(ins);
        assert(!cfg.blocks.empty());
    }

    return 0;
}
