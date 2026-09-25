#include "sleela/decompiler/decompiler.hpp"
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <string>
#include <vector>
static void put16(std::vector<std::uint8_t>& b,std::size_t p,std::uint16_t v){b[p]=std::uint8_t(v);b[p+1]=std::uint8_t(v>>8);}
static void put64(std::vector<std::uint8_t>& b,std::size_t p,std::uint64_t v){for(int i=0;i<8;++i)b[p+i]=std::uint8_t(v>>(8*i));}
int main(){
    {const unsigned char bytes[]={'M','Z',0,1,2,3};auto a=sleela::decompiler::Artifact::from_bytes(bytes,"sample.exe");assert(a.format()==sleela::decompiler::Format::PE);assert(a.size()==6);auto i=sleela::decompiler::Decoder{}.decode(a,0,4);assert(i.size()==4);}
    {std::vector<std::uint8_t> ar{'!','<','a','r','c','h','>','\n'};std::string h(60,' ');h.replace(0,16,"member.o/");h.replace(48,10,"2");h[58]=char(96);h[59]='\n';ar.insert(ar.end(),h.begin(),h.end());ar.insert(ar.end(),{0x7f,'E','L','F'});auto a=sleela::decompiler::Artifact::from_bytes(ar,"libsample.a");assert(a.format()==sleela::decompiler::Format::GNUArchive);assert(a.artifact_class()==sleela::decompiler::ArtifactClass::StaticArchive);assert(a.interfaces().archive_members.size()==1);assert(a.interfaces().archive_members[0].name=="member.o");assert(a.interfaces().archive_members[0].format==sleela::decompiler::Format::ELF);}
    {std::vector<std::uint8_t> elf(0x140,0);elf[0]=0x7f;elf[1]='E';elf[2]='L';elf[3]='F';elf[4]=2;elf[5]=1;elf[6]=1;put16(elf,16,2);put16(elf,18,62);put64(elf,24,0x100);put64(elf,32,0);put64(elf,40,0);put16(elf,52,64);put16(elf,54,56);put16(elf,56,0);
        const std::uint8_t code[]={0xe8,0x01,0x00,0x00,0x00,0x74,0x02,0xff,0xd0,0xc3};std::copy(std::begin(code),std::end(code),elf.begin()+0x100);
        auto a=sleela::decompiler::Artifact::from_bytes(elf,"fixture.elf");assert(a.format()==sleela::decompiler::Format::ELF);assert(a.architecture()==sleela::decompiler::Architecture::X86_64);
        auto ins=sleela::decompiler::Decoder{}.decode(a,0x100,sizeof(code));assert(ins.size()==4);assert(ins[0].is_call&&ins[0].branch_targets[0]==0x106);assert(ins[1].is_branch&&ins[1].is_conditional&&ins[1].branch_targets[0]==0x109);assert(ins[2].is_call&&ins[2].is_indirect&&ins[2].operands[0].kind==sleela::decompiler::Operand::Kind::Register);assert(ins[3].is_return);
        auto cfg=sleela::decompiler::Analyzer{}.build_cfg(ins);assert(cfg.blocks.size()>=3);assert(!cfg.edges.empty());auto funcs=sleela::decompiler::Analyzer{}.recover_function_candidates(a,cfg);assert(!funcs.empty());
    }
    return 0;
}
