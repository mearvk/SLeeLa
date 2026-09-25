#ifndef SKYA_HARDWARE_IO_H
#define SKYA_HARDWARE_IO_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum { SKYA_HW_AUDIO_CAPTURE=1,SKYA_HW_AUDIO_PLAYBACK=2,SKYA_HW_HID_INPUT=3,SKYA_HW_HID_OUTPUT=4,SKYA_HW_CONTROL=5 } skya_hardware_channel;
typedef struct { int(*open)(void*); int(*close)(void*); int(*read)(void*,skya_hardware_channel,void*,size_t,size_t*); int(*write)(void*,skya_hardware_channel,const void*,size_t); int(*control)(void*,uint32_t,const void*,size_t,void*,size_t,size_t*); void*ctx; } skya_hardware_io;
typedef struct { uint64_t reads,writes,bytes_in,bytes_out,errors,overruns,underruns; } skya_hardware_stats;
int skya_hardware_open(skya_hardware_io*); int skya_hardware_close(skya_hardware_io*); int skya_hardware_read(skya_hardware_io*,skya_hardware_channel,void*,size_t,size_t*); int skya_hardware_write(skya_hardware_io*,skya_hardware_channel,const void*,size_t); int skya_hardware_control(skya_hardware_io*,uint32_t,const void*,size_t,void*,size_t,size_t*);
#ifdef __cplusplus
}
#endif
#endif
