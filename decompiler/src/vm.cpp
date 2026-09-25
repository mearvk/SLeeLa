#include "sleela/decompiler/vm.hpp"
#include <algorithm>
#include <cstring>
namespace sleela::decompiler {
Vm::Vm(VmConfig c):config_(c),memory_(c.memory_size){}
void Vm::reset(){registers_.fill(0);std::fill(memory_.begin(),memory_.end(),0);trap_={};steps_=0;}
void Vm::load(const Module&m){module_=&m;reset();}
static std::uint64_t val(const Value&v,const std::array<std::uint64_t,32>&r){return r[v.id%r.size()];}
bool Vm::run(){
 if(!module_){trap_={VmTrap::Code::InvalidOperation,"no module loaded"};return false;}
 for(const auto&fn:module_->functions) for(const auto&blk:fn.blocks) for(const auto&op:blk.operations){
  if(++steps_>config_.max_steps){trap_={VmTrap::Code::StepLimit,"step limit"};return false;}
  auto out=[&](std::size_t n)->std::uint64_t&{return registers_[op.outputs.at(n).id%registers_.size()];};
  switch(op.opcode){
   case OpCode::Nop: break;
   case OpCode::Const: if(!op.outputs.empty())out(0)=static_cast<std::uint64_t>(op.immediate); break;
   case OpCode::Move: if(!op.outputs.empty()&&!op.inputs.empty())out(0)=val(op.inputs[0],registers_); break;
   case OpCode::Add: if(!op.outputs.empty()&&op.inputs.size()>=2)out(0)=val(op.inputs[0],registers_)+val(op.inputs[1],registers_); break;
   case OpCode::Sub: if(!op.outputs.empty()&&op.inputs.size()>=2)out(0)=val(op.inputs[0],registers_)-val(op.inputs[1],registers_); break;
   case OpCode::Mul: if(!op.outputs.empty()&&op.inputs.size()>=2)out(0)=val(op.inputs[0],registers_)*val(op.inputs[1],registers_); break;
   case OpCode::Div: if(!op.outputs.empty()&&op.inputs.size()>=2){auto d=val(op.inputs[1],registers_);if(!d){trap_={VmTrap::Code::DivisionByZero,"division by zero"};return false;}out(0)=val(op.inputs[0],registers_)/d;} break;
   case OpCode::Load: if(!op.outputs.empty()&&!op.inputs.empty()){auto p=val(op.inputs[0],registers_);if(p+sizeof(std::uint64_t)>memory_.size()){trap_={VmTrap::Code::MemoryFault,"load outside VM memory"};return false;}std::memcpy(&out(0),memory_.data()+p,sizeof(std::uint64_t));} break;
   case OpCode::Store: if(op.inputs.size()>=2){auto p=val(op.inputs[0],registers_);if(p+sizeof(std::uint64_t)>memory_.size()){trap_={VmTrap::Code::MemoryFault,"store outside VM memory"};return false;}auto x=val(op.inputs[1],registers_);std::memcpy(memory_.data()+p,&x,sizeof(x));} break;
   case OpCode::Trap: trap_={VmTrap::Code::InvalidOperation,"explicit SLIR trap"};return false;
   case OpCode::Return: return true;
   default: break;
  }
 }
 return trap_.code==VmTrap::Code::None;
}
std::uint64_t Vm::register_value(std::size_t i)const{return i<registers_.size()?registers_[i]:0;}
const std::vector<std::uint8_t>& Vm::memory()const noexcept{return memory_;}
const VmTrap& Vm::trap()const noexcept{return trap_;}
std::size_t Vm::steps()const noexcept{return steps_;}
}