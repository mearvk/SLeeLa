#include "sleela_registry_v3.hpp"
#include <cassert>
int main(){
 reg3::Registry r("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee",8);
 slr3_record x;
 assert(r.valid() && r.count()==0);
 slr3_record_init(&x,"88888888888888888888888888888888","sleela.test","fixture","C++ facade");
 assert(r.add(x)==SLR3_OK);
 assert(r.count()==1 && r.find(x.id)!=nullptr);
 assert(r.identify("sleela.test","C++ facade")!=nullptr);
 return 0;
}
