/*
 * lib/web-server-technology/cpp/src/slweb.cpp
 *
 * Translation unit for the C++17 facade. The facade is header-only inline over
 * the C ABI, so this unit mainly gives the facade a compiled object/library
 * (mirroring lib/sleela-sql/cpp) and exposes the facade version.
 */
#include "slweb.hpp"

namespace sleela {
namespace web {

const char *facade_version() { return "1.0.0"; }

}  // namespace web
}  // namespace sleela
