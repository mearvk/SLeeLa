// ===========================================================================
// artifact.cpp -- Sleelvac compiler -> persistent .sleela artifact.
// ===========================================================================
#include "artifact.h"
#include "compiler.h"
#include "../core/sleela_core.h"

#include <stdexcept>
#include <string>

namespace sleela {

int compileToArtifact(const Program& prog,
                      const std::string& outputPath,
                      const catalog::Catalog* cat,
                      const SyntaxVersion& syntax) {
    if (outputPath.empty())
        throw std::runtime_error("artifact output path must not be empty");

    SLVM* vm = slvm_new();
    if (!vm) throw std::runtime_error("unable to allocate Sleela VM");

    try {
        compile(prog, vm, cat, syntax);
        int rc = slvm_save_file(vm, outputPath.c_str());
        if (rc != 0) {
            slvm_free(vm);
            throw std::runtime_error("unable to write runnable .sleela artifact '" +
                                     outputPath + "' (error " + std::to_string(rc) + ")");
        }
        slvm_free(vm);
        return 0;
    } catch (...) {
        slvm_free(vm);
        throw;
    }
}

} // namespace sleela
