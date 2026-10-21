#include "sleelavirin_cpp.hpp"

#include <cassert>
#include <iostream>

int main() {
    sleelavirin::HashMap map(16);

    sleelavirin::KnownFile known;
    known.name = "native_sample";
    known.provider = "native";
    known.rule_id = "EICAR-001";
    known.severity = 8;

    auto file = sleelavirin::FileFingerprint::from_file("tests/native_sample.txt");
    if (!file) {
        std::cerr << "failed to read tests/native_sample.txt\n";
        return 2;
    }

    known.fingerprint = *file;
    if (!map.add(known)) {
        std::cerr << "failed to add known sample\n";
        return 3;
    }

    auto match = map.scan(*file);
    if (!match) {
        std::cerr << "scan did not return a match\n";
        return 4;
    }

    assert(match->kind == sleelavirin::MatchKind::kExactMatch);
    assert(match->known_name == "native_sample");
    assert(match->provider == "native");
    assert(match->rule_id == "EICAR-001");

    std::cout << "Exact match: " << match->known_name << " (confidence="
              << static_cast<int>(match->confidence) << ")\n";
    return 0;
}
