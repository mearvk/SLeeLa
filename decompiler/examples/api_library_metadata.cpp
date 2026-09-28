#include "sleela/decompiler/decompiler.hpp"
#include <iostream>

using namespace sleela::decompiler;

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: slecompiler-api-library <library-or-module>\n";
        return 2;
    }

    try {
        const auto artifact = Artifact::open(argv[1]);
        const auto& lib = artifact.library_metadata();
        const auto& mod = artifact.kernel_module_metadata();

        std::cout << "artifact: " << artifact.name() << "\n";
        if (!lib.soname.empty()) std::cout << "soname: " << lib.soname << "\n";
        std::cout << "needed libraries: " << lib.needed_libraries.size() << "\n";
        for (const auto& name : lib.needed_libraries)
            std::cout << "  " << name << "\n";
        if (!lib.build_id.empty()) std::cout << "build-id: " << lib.build_id << "\n";

        if (!mod.module_name.empty()) {
            std::cout << "kernel module: " << mod.module_name << "\n";
            std::cout << "vermagic: " << mod.vermagic << "\n";
            std::cout << "license: " << mod.license << "\n";
            std::cout << "dependencies: " << mod.dependencies.size() << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Slecompiler error: " << e.what() << "\n";
        return 1;
    }
}
