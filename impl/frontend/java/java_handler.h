// java_handler.h -- the Java handler module. Nothing in the compiler includes
// this header; the handler is reached only through frontend/language_handler.h.
// It is exposed for the unit test (impl/tests/java_references).
#ifndef SLEELA_JAVA_HANDLER_H
#define SLEELA_JAVA_HANDLER_H
#include "../language_handler.h"
namespace sleela {
namespace javahandler {
// Last segment of a qualified Java name ("java.lang.Throwable" -> "Throwable").
std::string simpleName(const std::string& qualified);
bool isQualified(const std::string& name);
}
}
#endif
