#ifndef SLEELA_AI_LLM_EXAMPLE_LOADER_SLNN_H
#define SLEELA_AI_LLM_EXAMPLE_LOADER_SLNN_H
/*
 * slnn training sink for the example loader: feed every encoded example
 * straight into a real slnn_* network via slnn_network_train_sample. This is
 * the bridge that lets a user "load words/maths/objects and train against known
 * weights" — the loader encodes, this sink trains the actual in-process net.
 */
#include "example_loader.h"
#include "../../neural-nets/neural_nets.h"
#ifdef __cplusplus
extern "C" {
#endif

struct sllme_slnn_sink {
 slnn_network *network;  /* borrowed; must match handler feature/target widths */
 double learning_rate;
 size_t trained;         /* count of successful train_sample calls */
 size_t errors;          /* count of train_sample failures (size/numeric) */
};

/* Initialize a sink over a network. The network's input_count must equal the
   handler feature_width and output_count the target_width (checked per call by
   slnn_network_train_sample). */
SLLME_API void sllme_slnn_sink_init(sllme_slnn_sink *sink,
                                    slnn_network *network,
                                    double learning_rate);

#ifdef __cplusplus
}
#endif
#endif
