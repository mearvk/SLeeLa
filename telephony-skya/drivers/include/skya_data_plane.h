#ifndef SKYA_DATA_PLANE_H
#define SKYA_DATA_PLANE_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
#define SKYA_DATA_MAGIC 0x534B5941u
#define SKYA_DATA_MAX_PAYLOAD 65536u
typedef enum { SKYA_DATA_AUDIO_IN=1,SKYA_DATA_AUDIO_OUT,SKYA_DATA_HID_IN,SKYA_DATA_HID_OUT,SKYA_DATA_CONTROL_IN,SKYA_DATA_CONTROL_OUT,SKYA_DATA_TELEMETRY,SKYA_DATA_FIRMWARE } skya_data_channel;
typedef struct { uint32_t magic; uint16_t version,flags; uint32_t channel; uint64_t sequence,timestamp_ns; uint32_t length,crc32; } skya_data_frame;
typedef struct skya_data_plane skya_data_plane;
int skya_data_plane_init(skya_data_plane**,size_t);
void skya_data_plane_destroy(skya_data_plane*);
int skya_data_plane_start(skya_data_plane*);
int skya_data_plane_stop(skya_data_plane*);
int skya_data_plane_send(skya_data_plane*,skya_data_channel,uint16_t,const void*,size_t,uint64_t*);
int skya_data_plane_receive(skya_data_plane*,skya_data_frame*,void*,size_t,size_t*,int);
size_t skya_data_plane_depth(const skya_data_plane*);
#ifdef __cplusplus
}
#endif
#endif
