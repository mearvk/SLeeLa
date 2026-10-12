#include "character_runtime.hpp"
#include <cassert>

int main() {
    using namespace sleela::alphabet;
    auto ascii = decode_utf8("ABC");
    assert(ascii && ascii.codepoints.size() == 3);
    auto multibyte = decode_utf8("A\xC3\xA9\xF0\x9F\x8C\x8D");
    assert(multibyte && multibyte.codepoints.size() == 3);
    assert(codepoint_count_utf8("A\xC3\xA9") == 2);
    assert(!decode_utf8("\xC0\xAF"));
    assert(!decode_utf8("\xED\xA0\x80"));
    assert(!decode_utf8("\xF4\x90\x80\x80"));
    assert(!decode_utf8("\xE2\x82"));
    assert(!encode_utf8(static_cast<char32_t>(0xD800)));
    assert(encode_utf8(static_cast<char32_t>(0x1F30D)).has_value());
    assert(parse_capacity_signature("C1") == 1);
    assert(parse_capacity_signature("C1048576") == 1048576);
    assert(!parse_capacity_signature("C0"));
    assert(!parse_capacity_signature("C01"));
    assert(!parse_capacity_signature("C1048577"));
    assert(!parse_capacity_signature("C-1"));
    CharacterSet set(2);
    assert(set.add("A"));
    assert(set.add("\xC3\xA9"));
    assert(set.contains("\xC3\xA9"));
    assert(!set.add("A"));
    assert(!set.add("B"));
    assert(!set.add("\xC0\xAF"));
    assert(set.size() == 2);
}
