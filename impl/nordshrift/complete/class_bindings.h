#pragma once
#include <cstddef>
#include <string>
#include <vector>
namespace nordshrift::complete {
struct ClassBinding { const char* module; const char* className; const char* canonicalPath; };
const std::vector<ClassBinding>& classBindings();
const ClassBinding* findClass(const std::string&, const std::string&);
bool canonicalPathExists(const ClassBinding&, const std::string&);
std::size_t moduleCount();
std::size_t classCount();
}
