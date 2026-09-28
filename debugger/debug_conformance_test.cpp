#include "debug_conformance.hpp"
#include <cassert>
int main(){using namespace sleela::debugger;ConformanceSuite s;s.add({"breakpoints","linux","ptrace","hash","rev",true,true,true,true,"fixture"});assert(s.releaseReady());ConformanceSuite f;f.add({"memory","linux","ptrace","","rev",true,true,true,false,""});assert(!f.releaseReady());return 0;}