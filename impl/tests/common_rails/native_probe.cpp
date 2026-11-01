// ===========================================================================
// native_probe.cpp -- emit the CommonRails heritage (C++) rendering for the
// conformance fixtures. Prints a small, deterministic set of cases so the
// conformance test can diff this native output against (a) pinned fixtures and
// (b) the Sleela PrintRail rendering of the SAME cases.
//
// Keep the case list in lockstep with sleela_probe.sleela.
// ===========================================================================
#include "CommonRailsPrinting.hpp"
#include <iostream>
#include <string>

using namespace commonrails::printing;

int main(int argc, char** argv) {
    std::string section = (argc > 1) ? argv[1] : "all";

    if (section == "square" || section == "all") {
        // The core 441-cell fill contract: this MUST be byte-identical between
        // the native engine and the Sleela PrintRail.
        PrintRenderer::square(std::cout, 0);
        PrintRenderer::square(std::cout, 1);
        PrintRenderer::square(std::cout, 22);
        PrintRenderer::square(std::cout, 220);
        PrintRenderer::square(std::cout, 441);
    }

    if (section == "lines" || section == "all") {
        // Single-line primitives. These are pinned to a fixture so any change
        // to the native formatting is caught; the conformance test documents
        // where the native and Sleela forms legitimately differ today.
        PrintComponent::write(std::cout, "Sleelvac", 1, 1, "compiling Hello.sleela");
        PrintFormatter::status(std::cout, PrintState::START, "semantic analysis");
        PrintFormatter::status(std::cout, PrintState::WORKING, "lowering subject");
        PrintFormatter::status(std::cout, PrintState::COMPLETE, "runnable Core artifact written");
        PrintFormatter::status(std::cout, PrintState::WARN, "deprecated pragma");
        PrintFormatter::status(std::cout, PrintState::ERROR, "unknown type");
    }

    return 0;
}
