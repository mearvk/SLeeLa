#include "CommonRailsPrinting.hpp"
#include <cassert>
int main(){using namespace commonrails::printing;assert(PrintLayout::width==80);assert(PrintProgress::clamp(-1)==0);assert(PrintProgress::clamp(101)==100);assert(PrintProgress::cells(50)==220);assert(PrintProgress::cells(100)==441);return 0;}
