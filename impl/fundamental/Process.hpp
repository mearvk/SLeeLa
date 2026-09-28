#pragma once
#include <string>
namespace sleela::fundamental { class Process {
public: explicit Process(std::string command={}); int run(); bool running()const noexcept; int exit_code()const noexcept;
private: std::string command_; int exit_code_{-1}; bool running_{false}; }; }