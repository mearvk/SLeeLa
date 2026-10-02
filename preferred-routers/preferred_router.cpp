#include "preferred_router.h"
#include <string>
#include <sstream>
namespace sleela_preferred_routers {
std::string describe(const sleela_preferred_router& r){
 std::ostringstream s;s<<"country="<<r.country<<" role="<<r.role<<" preference="<<r.routing_preference<<" candidate="<<r.candidate;return s.str();
}
}
