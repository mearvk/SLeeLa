#include "Process.hpp"
#include <cstdlib>
namespace sleela::fundamental { Process::Process(std::string c):command_(std::move(c)){} int Process::run(){running_=true;exit_code_=std::system(command_.c_str());running_=false;return exit_code_;} bool Process::running()const noexcept{return running_;} int Process::exit_code()const noexcept{return exit_code_;} }