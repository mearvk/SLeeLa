#include "slvm_capability.hpp"
#include <cassert>
int main(){using namespace sleela::vm; capability c{1,capability_domain::files,3,1024};assert(c.id==1);assert(c.domain==capability_domain::files);return 0;}
