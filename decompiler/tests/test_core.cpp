#include "sleela/decompiler/decompiler.hpp"
#include <cassert>
#include <cstdint>
#include <string>
#include <vector>

int main() {
    {
        const unsigned char bytes[] = {'M','Z',0,1,2,3};
        auto a = sleela::decompiler::Artifact::from_bytes(bytes, "sample.exe");
        assert(a.format() == sleela::decompiler::Format::PE);
        assert(a.size() == 6);
        auto i = sleela::decompiler::Decoder{}.decode(a, 0, 4);
        assert(i.size() == 4);
    }
    {
        std::vector<std::uint8_t> ar{'!','<','a','r','c','h','>','\n'};
        std::string h(60, ' ');
        h.replace(0, 16, "member.o/");
        h.replace(48, 10, "2");
        h[58] = char(96);
        h[59] = '\n';
        ar.insert(ar.end(), h.begin(), h.end());
        ar.insert(ar.end(), {0x7f, 'E', 'L', 'F'});
        auto a = sleela::decompiler::Artifact::from_bytes(ar, "libsample.a");
        assert(a.format() == sleela::decompiler::Format::GNUArchive);
        assert(a.artifact_class() == sleela::decompiler::ArtifactClass::StaticArchive);
        assert(a.interfaces().archive_members.size() == 1);
        assert(a.interfaces().archive_members[0].name == "member.o");
        assert(a.interfaces().archive_members[0].format == sleela::decompiler::Format::ELF);
    }
    {
        const unsigned char code[] = {0xE8,0x01,0x00,0x00,0x00,0xC3};
        auto a = sleela::decompiler::Artifact::from_bytes(code, "code.bin");
        // Raw artifacts intentionally lack architecture information; use a minimal ELF-shaped
        // path in future fixtures for architecture-specific decoding.
        assert(a.format() == sleela::decompiler::Format::Raw);
    }
    return 0;
}
