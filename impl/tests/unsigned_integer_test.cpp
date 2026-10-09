#include "../unsigned_integer.h"
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

using sleela::UnsignedInteger;

int main() {
    assert(UnsignedInteger::fromDecimal(1, "1").toDecimal() == "1");
    assert(UnsignedInteger::fromDecimal(8, "255").toDecimal() == "255");
    assert(UnsignedInteger::fromDecimal(64, "18446744073709551615").toDecimal() == "18446744073709551615");
    assert(UnsignedInteger::fromDecimal(9, "511").toDecimal() == "511");
    assert(UnsignedInteger::fromDecimal(9, "511").compare(UnsignedInteger::fromDecimal(9, "510")) > 0);
    assert(UnsignedInteger::fromDecimal(8, "12").add(UnsignedInteger::fromDecimal(8, "20")).toDecimal() == "32");
    assert(UnsignedInteger::fromDecimal(8, "20").subtract(UnsignedInteger::fromDecimal(8, "12")).toDecimal() == "8");
    assert(UnsignedInteger::fromDecimal(1048576, "123456789012345678901234567890").toDecimal() == "123456789012345678901234567890");

    bool underflow = false;
    try { (void)UnsignedInteger::fromDecimal(8, "0").subtract(UnsignedInteger::fromDecimal(8, "1")); }
    catch (const std::underflow_error&) { underflow = true; }
    assert(underflow);

    bool overflow = false;
    try { (void)UnsignedInteger::fromDecimal(8, "256"); }
    catch (const std::out_of_range&) { overflow = true; }
    assert(overflow);

    bool badWidth = false;
    try { (void)UnsignedInteger(1048577); }
    catch (const std::out_of_range&) { badWidth = true; }
    assert(badWidth);

    std::cout << "unsigned-integer tests: PASS\n";
    return 0;
}
