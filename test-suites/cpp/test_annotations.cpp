#include <cassert>
#include <iostream>
#include "../../impl/annotation/Annotation.hpp"
#include "../../impl/annotation/AnnotationForwarder.hpp"
#include "../../impl/annotation/AnnotationInterpreter.hpp"
#include "../../impl/annotation/ForwardingAnnotation.hpp"
using namespace sleela::annotation;
int main(){
 DocumentAnnotations d; d.add({"scope","system"}); d.add({"next","system/network/transport"});
 assert(d.has("scope")); assert(d.has("next")); assert(d.all().size()==2);
 AnnotationForwarder forwarder;
 auto r=forwarder.forward(d); assert(r.forwarded&&!r.rejected&&r.next=="system/network/transport");
 DocumentAnnotations bad; bad.add({"next","../escape"}); auto b=forwarder.forward(bad); assert(!b.forwarded&&b.rejected);
 assert(ForwardingPolicy::valid({"system/network/transport"})); assert(!ForwardingPolicy::valid({"../escape"}));
 assert(AnnotationInterpreter{}.interpret(d).forwarded);
 std::cout<<"PASS: annotation runtime\n"; return 0;
}
