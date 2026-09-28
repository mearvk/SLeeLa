#pragma once
#include "AnnotationForwarder.hpp"
namespace sleela::annotation { class AnnotationInterpreter { AnnotationForwarder forwarder_; public: ForwardingResult interpret(const DocumentAnnotations& d) const { return forwarder_.forward(d); } }; }