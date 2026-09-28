#include "annotation_http_bridge.h"
#include <string>
static bool safe_destination(const char *s) {
    if (!s || !*s) return false;
    std::string v(s);
    return v.front()!='/' && v.find("..") == std::string::npos;
}
extern "C" int sleela_http_forwarding_validate(const sleela_http_forwarding *f) {
    if (!f || !f->holding_document || !f->forwarding_annotation) return 0;
    if (f->http_grade < 1 || f->http_grade > 9) return 0;
    if (!safe_destination(f->nexter_colony)) return 0;
    return 1;
}
