// Dynamite Connector import discovery.
#ifndef SLEELA_DYNAMITE_H
#define SLEELA_DYNAMITE_H
#include <string>
#include <vector>
namespace sleela { namespace dynamite {
struct Property { std::string key; std::string value; };
struct Import {
    bool present=false;
    std::string referenceName;
    std::string sourcePath;
    std::string configName;
    size_t line=0;
    std::vector<Property> properties;
};
std::vector<Import> discoverAll(const std::string& source, const std::string& sourcePath="");
Import discover(const std::string& source, const std::string& sourcePath="");
bool isImplicitLoadCandidate(const Import&);
}}
#endif