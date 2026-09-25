#include "../api/native/reflection/sleela_reflection.hpp"
#include <cassert>
struct Example{int value;};
int main(){sleela::reflection::Registry r;auto t=sleela::reflection::describe<Example>("Example");t.typeId=sleela::reflection::stableTypeId("Example","1");assert(r.add(t));assert(r.find("Example"));assert(!r.add(t));assert(r.remove("Example"));return 0;}
