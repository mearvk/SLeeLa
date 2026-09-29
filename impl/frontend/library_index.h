// Library discovery contract for the SLeeLa compiler and Nordshrift.
#ifndef SLEELA_LIBRARY_INDEX_H
#define SLEELA_LIBRARY_INDEX_H
#include <string>
#include <vector>
#include <cstddef>
namespace sleela { namespace library {
struct Symbol { std::string package; std::string name; std::string path; };
class Index { public: static Index discover(const std::string& preferredRoot = ""); bool empty() const; size_t symbolCount() const; bool hasPackage(const std::string&) const; size_t packageCount() const;
    size_t packageSymbolCount(const std::string&) const;
    const Symbol* findSymbol(const std::string&, const std::string&) const;
    const std::vector<Symbol>& symbols() const; const std::string& root() const; std::string resolveImport(const std::string&) const;
    std::string resolveSymbol(const std::string&, const std::string&) const; private: std::string root_; std::vector<Symbol> symbols_; };
void validateImports(const std::vector<std::string>&, const Index&);
}}
#endif
