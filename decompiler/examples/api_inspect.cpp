#include "sleela/decompiler/decompiler.hpp"
#include <iostream>

using namespace sleela::decompiler;

static const char* format_name(Format f) {
    switch (f) {
        case Format::ELF: return "ELF";
        case Format::PE: return "PE";
        case Format::MachO: return "Mach-O";
        case Format::GNUArchive: return "GNU archive";
        case Format::Raw: return "raw";
        default: return "unknown";
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: slecompiler-api-inspect <artifact>\n";
        return 2;
    }

    try {
        const auto artifact = Artifact::open(argv[1]);
        std::cout << "name: " << artifact.name() << "\n"
                  << "size: " << artifact.size() << "\n"
                  << "format: " << format_name(artifact.format()) << "\n"
                  << "sha256: " << artifact.sha256() << "\n"
                  << "sections: " << artifact.interfaces().sections.size() << "\n"
                  << "symbols: " << artifact.interfaces().symbols.size() << "\n"
                  << "imports: " << artifact.interfaces().imports.size() << "\n"
                  << "exports: " << artifact.interfaces().exports.size() << "\n"
                  << "relocations: " << artifact.interfaces().relocations.size() << "\n";
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Slecompiler error: " << e.what() << "\n";
        return 1;
    }
}
