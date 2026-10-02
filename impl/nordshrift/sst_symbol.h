// Formal SST symbol contract shared by Nordshrift and the SLeeLa library index.
#ifndef NORDSHRIFT_SST_SYMBOL_H
#define NORDSHRIFT_SST_SYMBOL_H
#include <string>
#include "../frontend/library_index.h"
namespace nordshrift {
enum class SSTSymbolKind { Class, Interface, Function, Field, Constant, Variable, Module, Package, Type };
struct SSTSymbol {
    std::string package;
    std::string name;
    std::string qualifiedName;
    std::string kind;
    std::string source;
    std::string signature;
    std::string status;
};
struct SSTSymbolResolution {
    SSTSymbol symbol;
    bool resolved = false;
    bool ambiguous = false;
    std::string resolvedPath;
    std::string diagnostic;
};
SSTSymbol makeSSTSymbol(const std::string& package,const std::string& name,const std::string& kind,const std::string& source);
SSTSymbolResolution resolveSSTSymbol(const SSTSymbol&, const sleela::library::Index&);
}
#endif
