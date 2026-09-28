#pragma once
#include "Annotation.hpp"
#include "ForwardingAnnotation.hpp"
namespace sleela::annotation {
struct ForwardingResult { bool forwarded=false; bool rejected=false; std::string next; };
class AnnotationForwarder { public: ForwardingResult forward(const DocumentAnnotations& d) const { ForwardingResult r; for(const auto& a:d.all()){ if(a.name!="next") continue; ForwardingAnnotation f{a.value}; if(!ForwardingPolicy::valid(f)){r.rejected=true; return r;} r.forwarded=true; r.next=f.next; } return r; } };
}