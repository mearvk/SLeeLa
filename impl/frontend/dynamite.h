// Dynamite Connector source marker discovery.
#ifndef SLEELA_DYNAMITE_H
#define SLEELA_DYNAMITE_H
#include <string>
#include <vector>
namespace sleela { namespace dynamite {
struct Property { std::string key; std::string value; };
struct Marker {
 bool present=false;
 std::string className;
 std::string configName;
 std::string sourcePath;
 size_t line=0;
 std::vector<Property> properties;
};
Marker discover(const std::string& source, const std::string& sourcePath="");
bool isImplicitLoadCandidate(const Marker&);
}}
#endif
