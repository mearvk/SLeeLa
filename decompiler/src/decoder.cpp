#include "sleela/decompiler/decompiler.hpp"
#include <algorithm>
namespace sleela::decompiler {
std::vector<Instruction> Decoder::decode(const Artifact&a,std::uint64_t address,std::size_t length)const{std::vector<Instruction>o;const auto&b=a.bytes();auto start=static_cast<std::size_t>(address);if(start>=b.size())return o;auto end=std::min(b.size(),start+length);for(std::size_t p=start;p<end;++p){Instruction i;i.address=p;i.bytes.push_back(b[p]);i.mnemonic="db";i.operands.push_back({Operand::Kind::Immediate,"0x"+std::to_string(b[p]),b[p]});i.provenance={p,p,"raw-byte",1.0};o.push_back(std::move(i));}return o;}
}