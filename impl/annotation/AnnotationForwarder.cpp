#include "AnnotationForwarder.hpp"
namespace sleela::annotation {
ForwardingResult AnnotationForwarder::forward(const DocumentAnnotations& d) const noexcept {
    ForwardingResult r;
    for(const auto& a:d.all()){
        if(a.name!="next") continue;
        ++r.candidates;
        if(!ForwardingPolicy::valid(ForwardingAnnotation{a.value})){r.rejected=true;r.status=ForwardingStatus::Rejected;return r;}
        if(r.candidates>1){r.rejected=true;r.status=ForwardingStatus::Multiple;r.next.clear();return r;}
        r.next=a.value;
    }
    if(r.candidates==1){r.forwarded=true;r.status=ForwardingStatus::Forwarded;}
    return r;
}
}