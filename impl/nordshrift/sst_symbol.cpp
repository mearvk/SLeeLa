// SST symbol resolution against the canonical recursive /lib index.
#include "sst_symbol.h"
namespace nordshrift {
SSTSymbol makeSSTSymbol(const std::string& package,const std::string& name,const std::string& kind,const std::string& source) {
    SSTSymbol s;
    s.package=package; s.name=name; s.qualifiedName=package.empty()?name:package+"."+name;
    s.kind=kind; s.source=source; s.status="declared";
    return s;
}
SSTSymbolResolution resolveSSTSymbol(const SSTSymbol& input,const sleela::library::Index& index) {
    SSTSymbolResolution r; r.symbol=input;
    const auto* found=index.findSymbol(input.package,input.name);
    if(!found) { r.symbol.status="unresolved"; r.diagnostic="SST symbol '"+input.qualifiedName+"' is not present under canonical /lib"; return r; }
    r.resolved=true; r.symbol.status="resolved"; r.resolvedPath=index.resolveSymbol(input.package,input.name);
    return r;
}
}
