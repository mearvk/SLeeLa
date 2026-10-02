#ifndef SLEELA_PERMISSIBLE_H
#define SLEELA_PERMISSIBLE_H
#include <string>
#include <vector>
namespace sleela { namespace permissible {
enum class Domain { Sight, Name, Travel, Process, Memory, System, LinearGraph };
enum class Safety { AdminSafe, JourneySafe, SecureFuture };
struct Property { std::string key; std::string value; };
struct Import { bool present=false; std::string referenceName; std::string sourcePath; std::string configName; size_t line=0; Domain domain=Domain::System; Safety safety=Safety::AdminSafe; std::vector<Property> properties; };
std::vector<Import> discoverAll(const std::string&,const std::string& sourcePath="");
Import discover(const std::string&,const std::string& sourcePath="");
bool isAdmissible(const Import&);
}}
#endif
