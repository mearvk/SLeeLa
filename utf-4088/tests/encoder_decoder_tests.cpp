#ifdef NDEBUG
#undef NDEBUG
#endif

#include "encoder_decoder.hpp"
#include <cassert>
#include <cstdint>

int main() {
    using namespace utf4088;
    for (unsigned y = 0; y <= 11; ++y) {
        for (unsigned x = 0; x <= 7; ++x) {
            for (unsigned p = 1; p <= 50; ++p) {
                for (unsigned v : {0U, 1U, 127U, 255U}) {
                    const InputFields fields{static_cast<std::uint8_t>(x),
                        static_cast<std::uint8_t>(y), static_cast<std::uint8_t>(p),
                        static_cast<std::uint8_t>(v)};
                    const auto packed = pack_input(fields);
                    assert(packed);
                    const auto unpacked = unpack_input(*packed);
                    assert(unpacked);
                    assert(unpacked->x == fields.x && unpacked->y == fields.y);
                    assert(unpacked->pressure == fields.pressure && unpacked->voltage == fields.voltage);
                    const auto id = encode_input(fields);
                    assert(id && *id == kInputIdentifierBase + *packed);
                    const auto decoded = decode_input(*id);
                    assert(decoded && decoded->x == fields.x && decoded->y == fields.y);
                    assert(decoded->pressure == fields.pressure && decoded->voltage == fields.voltage);
                    assert(symbol_from_input(*packed) == id);
                }
            }
        }
    }
    assert(!pack_input(InputFields{8, 0, 1, 0}));
    assert(!pack_input(InputFields{0, 12, 1, 0}));
    assert(!pack_input(InputFields{0, 0, 0, 0}));
    assert(!pack_input(InputFields{0, 0, 51, 0}));
    assert(!unpack_input(1U << 21));
    assert(!decode_input(0x110000));
    assert(!decode_input(kInputIdentifierBase - 1));
    assert(!decode_input(kInputIdentifierBase + (1U << 21)));
    assert(!symbol_from_input(1U << 21));
    return 0;
}
