#include "sleela/api.hpp"
namespace sleela::api {
static_assert(sizeof(EvidenceKind) >= sizeof(int), "EvidenceKind must remain an enum-like API type");
}
