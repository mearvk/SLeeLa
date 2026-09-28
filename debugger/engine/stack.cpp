#include "stack.hpp"
namespace sleela::debugger::engine {
void StackEngine::replace(std::vector<Frame> f){frames_=std::move(f);selected_=0;}
bool StackEngine::select(std::size_t i){if(i>=frames_.size())return false;selected_=i;return true;}
const Frame* StackEngine::selected()const noexcept{return frames_.empty()?nullptr:&frames_[selected_];}
}