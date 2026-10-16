// ===========================================================================
// xlt_model.h  --  The parsed model of an XLT (.xlt) CPU-model sheet.
//
// XLT is the CPU-model counterpart of the Nordshrift .sst system sheet: a
// declarative, brace-delimited document that names a CPU SKU and the parts that
// assemble it. Each part is one orthogonal slot declared at most once; the
// document's orthogonality is the integrity contract (no new keyword, no
// setters). This header holds the parse result the lexer/parser produce and the
// resolver/driver consume.
//
//   #xlt 1.0
//   model Name {
//     cpu      "SLAMDEPYC9575F";
//     board    "wrx90";
//     memory   "ddr2_800" 4096;
//     cadence  "DDR2";
//     storage  "nvmeGen4";
//     bus      "pcieGen5x16";
//     gpu;                         // optional capability directive
//     dma;                         // optional capability directive
//     nativeram;                   // optional capability directive
//     target   sleela;
//   }
// ===========================================================================
#ifndef SLEELA_XLT_MODEL_H
#define SLEELA_XLT_MODEL_H

#include <string>

namespace xlt {

// One declared part slot: its value, whether it was declared, and the source
// line (for diagnostics). Memory additionally carries an integer capacity.
struct Slot {
    std::string ref;        // the quoted ref, e.g. "ddr2_800"
    bool        present = false;
    int         line = 0;
};

struct XltModel {
    std::string name;                 // the `model Name` identifier
    std::string version = "1.0";      // from the `#xlt` pragma
    std::string target = "sleela";    // `target <id>;`
    int         targetLine = 0;

    Slot cpu;
    Slot board;
    Slot memory;
    int  memoryCapMiB = 0;            // the integer after the memory ref
    Slot cadence;
    Slot storage;
    Slot bus;

    // Bare capability directives (no quoted ref), each declared at most once.
    bool gpu = false;        // `gpu;`       -> enableGPU()
    bool dma = false;        // `dma;`       -> enableDMA()
    bool nativeRam = false;  // `nativeram;` -> NATIVE_RAM feature bit
    int  gpuLine = 0, dmaLine = 0, nativeRamLine = 0;

    bool headerSeen = false;         // `#xlt` pragma present
    bool modelSeen = false;          // a `model { }` block was parsed
};

} // namespace xlt

#endif // SLEELA_XLT_MODEL_H
