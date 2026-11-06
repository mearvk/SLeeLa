#include "example_loader_slnn.h"

void sllme_slnn_sink_init(sllme_slnn_sink *sink, slnn_network *net, double lr){
 if(!sink) return;
 sink->network=net;
 sink->learning_rate=lr;
 sink->trained=0;
 sink->errors=0;
}

/* The sink callback: train the real network on one encoded example. Returns 0
   to continue (always, even on a per-sample numeric error, which is counted)
   so one bad sample does not abort a load; the caller inspects sink->errors. */
int sllme_slnn_sink_train(void *sink_ud,
                          const double *features, size_t fw,
                          const double *target, size_t tw){
 sllme_slnn_sink *sink=(sllme_slnn_sink*)sink_ud;
 if(!sink || !sink->network) return 1; /* no network: stop */
 slnn_status st=slnn_network_train_sample(sink->network, features, fw, target, tw,
                                          sink->learning_rate);
 if(st==SLNN_OK) sink->trained++; else sink->errors++;
 return 0;
}
