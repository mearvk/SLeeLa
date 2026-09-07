// ===========================================================================
// mem_model.h -- derive a Sleela memory-model summary from a parsed Program,
// for submission to the SecureJDK 28 supervisor over STP-0001.
// ===========================================================================
#ifndef SLEELA_MEM_MODEL_H
#define SLEELA_MEM_MODEL_H

#include "../frontend/ast.h"
extern "C" {
#include "stp_client.h"
}

namespace sleela {

// Analyze the AST and populate an stp_mem_model:
//   globals     = total class fields (shared state -> core globals)
//   functions   = total methods
//   code_len    = a conservative instruction-count estimate
//   max_threads = 1 + number of distinct spawn() targets (bounded to 128 cap)
//   locks       = count of distinct lock() slot arguments used
//   mailboxes   = count of distinct send()/recv() slot arguments used
//   est_heap    = a small per-global/per-function heap estimate
// sysdepth / degreemax carry the Constitution values (3024 / 4).
stp_mem_model analyze_mem_model(const Program& prog, long long sysdepth, long long degreemax);

} // namespace sleela

#endif // SLEELA_MEM_MODEL_H
