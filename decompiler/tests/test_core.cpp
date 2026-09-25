#include "sleela/decompiler/decompiler.hpp"
#include <cassert>
int main() {
    const unsigned char bytes[] = {'M','Z',0,1,2,3};
    auto a = sleela::decompiler::Artifact::from_bytes(bytes);
    assert(a.format() == sleela::decompiler::Format::PE);
    assert(a.size() == 6);
    auto i = sleela::decompiler::Decoder{}.decode(a, 0, 4);
    assert(i.size() == 4);
    return 0;
}
