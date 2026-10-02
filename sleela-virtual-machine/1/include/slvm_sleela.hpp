#ifndef SLEELA_SLVM_SLEELA_HPP
#define SLEELA_SLVM_SLEELA_HPP
#include <string>
namespace sleela::vm {
struct execution_result { int status=-1; std::string error; };
execution_result execute_source(const std::string& source);
execution_result execute_artifact(const std::string& path);
execution_result execute_source_file(const std::string& path);
}
#endif
