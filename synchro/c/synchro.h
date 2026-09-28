#ifndef SLEELA_SYNCHRO_H
#define SLEELA_SYNCHRO_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#define SYNCHRO_MAGIC 0x53594e43u
typedef struct{double*rtts_ms;size_t count,capacity,window;uint64_t sent,acked,lost;double sum,sumsq;} synchro_stats;
int synchro_stats_init(synchro_stats*s,size_t window); void synchro_stats_free(synchro_stats*s);
int synchro_stats_record(synchro_stats*s,int success,double rtt_ms); double synchro_percentile(const synchro_stats*s,double p);
double synchro_loss_rate(const synchro_stats*s); double synchro_delivery_rate(const synchro_stats*s);
int synchro_packet_encode(uint8_t*buf,size_t len,uint32_t seq,uint64_t sent_ns);
int synchro_packet_decode(const uint8_t*buf,size_t len,uint32_t*seq,uint64_t*sent_ns);
#ifdef __cplusplus
}
#endif
#endif
