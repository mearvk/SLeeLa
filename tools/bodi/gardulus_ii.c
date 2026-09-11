#define _GNU_SOURCE
#include "gardulus_ii.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

static uint64_t now_ms(void) { struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts); return (uint64_t)ts.tv_sec*1000ULL+(uint64_t)ts.tv_nsec/1000000ULL; }
static void read_memory(unsigned long *total, unsigned long *avail) {
    FILE *f=fopen("/proc/meminfo","r"); char key[64], unit[16]; unsigned long value; *total=0; *avail=0; if(!f)return;
    while(fscanf(f,"%63s %lu %15s",key,&value,unit)==3){if(strcmp(key,"MemTotal:")==0)*total=value;else if(strcmp(key,"MemAvailable:")==0)*avail=value;} fclose(f);
}
static unsigned long count_lines(const char *path){FILE*f=fopen(path,"r");char line[512];unsigned long n=0;if(!f)return 0;while(fgets(line,sizeof(line),f))n++;fclose(f);return n?n-1:0;}
int gardulus_emit_json(const gardulus_sample*s,char*b,size_t z){if(!s||!b||!z)return-1;return snprintf(b,z,"{\"iteration\":%llu,\"timestamp_ms\":%llu,\"input\":%d,\"p_phase\":%.6f,\"b_phase\":%.6f,\"memory_total_kb\":%lu,\"memory_available_kb\":%lu,\"connections\":%lu,\"relations\":%lu,\"items\":%lu,\"status\":%lu,\"containment_count\":%lu}\n",(unsigned long long)s->iteration,(unsigned long long)s->timestamp_ms,s->input_integer,s->p_phase,s->b_phase,s->memory_total_kb,s->memory_available_kb,s->connections,s->relations,s->items,s->status,s->containment_count);}
int gardulus_run(int interval_ms,int iterations,gardulus_listener listener,void*user_data){if(interval_ms<1||!listener)return-1;for(int i=0;iterations<=0||i<iterations;i++){gardulus_sample s;memset(&s,0,sizeof(s));s.iteration=(uint64_t)i;s.timestamp_ms=now_ms();s.input_integer=interval_ms;double t=(double)s.timestamp_ms/1000.0;s.p_phase=fmod(t*(double)interval_ms,360.0);s.b_phase=fmod(t*(double)(interval_ms+1),360.0);read_memory(&s.memory_total_kb,&s.memory_available_kb);s.connections=count_lines("/proc/net/tcp");s.relations=count_lines("/proc/net/tcp6");s.items=s.memory_total_kb?(s.memory_total_kb-s.memory_available_kb):0;s.status=s.memory_available_kb?1UL:0UL;s.containment_count=(unsigned long)(i+1);if(listener(&s,user_data)!=0)return 1;struct timespec req={interval_ms/1000,(long)(interval_ms%1000)*1000000L};nanosleep(&req,NULL);}return 0;}
