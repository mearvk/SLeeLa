#define _POSIX_C_SOURCE 200809L
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>

#define N 4096
#define ITERS 2000000ULL
static uint32_t seed = 0x53435350U;
static uint32_t next_u32(void) { seed ^= seed << 13; seed ^= seed >> 17; seed ^= seed << 5; return seed; }
static uint64_t now_ns(void) { struct timespec t; if (clock_gettime(CLOCK_MONOTONIC, &t)) return 0; return (uint64_t)t.tv_sec*1000000000ULL+(uint64_t)t.tv_nsec; }

#define BENCH_INT(T, TAG, MASK, SIGNED_CAST) \
static void bench_##TAG(const char *op, T *a, T *b) { \
  uint64_t start=now_ns(), sum=0; \
  for (uint64_t i=0;i<ITERS;i++) { size_t k=(size_t)(i & (N-1)); T x; \
    if (op[0]=='+') x=(T)(a[k]+b[k]); \
    else if (op[0]=='-') x=(T)(a[k]-b[k]); \
    else if (op[0]=='*') x=(T)(a[k]*b[k]); \
    else x=(T)(a[k]/b[k]); \
    sum ^= (uint64_t)(SIGNED_CAST); \
  } \
  uint64_t elapsed=now_ns()-start; \
  printf("%s,%s,%" PRIu64 ",%.3f,%" PRIu64 "\n", #TAG, op, ITERS, (double)elapsed/(double)ITERS, sum); \
}

#define RUN_TYPE(T, TAG, CAST) do { \
  static T a[N], b[N]; seed = 0x53435350U; \
  for(size_t i=0;i<N;i++){ a[i]=(T)(5+(next_u32()%6)); b[i]=(T)(1+(next_u32()%5)); } \
  bench_##TAG("+",a,b); bench_##TAG("-",a,b); bench_##TAG("*",a,b); bench_##TAG("/",a,b); \
} while(0)

BENCH_INT(int8_t, i8, 0, x)
BENCH_INT(uint8_t, u8, 0, x)
BENCH_INT(int16_t, i16, 0, x)
BENCH_INT(uint16_t, u16, 0, x)
BENCH_INT(int32_t, i32, 0, x)
BENCH_INT(uint32_t, u32, 0, x)
BENCH_INT(int64_t, i64, 0, x)
BENCH_INT(uint64_t, u64, 0, x)
#if defined(__SIZEOF_INT128__)
BENCH_INT(__int128, i128, 0, (uint64_t)x)
#endif

#define BENCH_FLOAT(T, TAG, SCALE) \
static void bench_##TAG(const char *op, T *a, T *b) { \
  uint64_t start=now_ns(), sum=0; \
  for(uint64_t i=0;i<ITERS;i++){size_t k=(size_t)(i&(N-1)); T x; \
    if(op[0]=='+') x=(T)(a[k]+b[k]); else if(op[0]=='-') x=(T)(a[k]-b[k]); \
    else if(op[0]=='*') x=(T)(a[k]*b[k]); else x=(T)(a[k]/b[k]); \
    sum ^= (uint64_t)(x*(SCALE)); \
  } \
  uint64_t elapsed=now_ns()-start; \
  printf("%s,%s,%" PRIu64 ",%.3f,%" PRIu64 "\n", #TAG, op, ITERS, (double)elapsed/(double)ITERS, sum); \
}
BENCH_FLOAT(float, f32, 1000000.0)
BENCH_FLOAT(double, f64, 1000000.0)

int main(void) {
  puts("type,operator,operations,ns_per_operation,checksum");
  RUN_TYPE(int8_t,i8,0); RUN_TYPE(uint8_t,u8,0);
  RUN_TYPE(int16_t,i16,0); RUN_TYPE(uint16_t,u16,0);
  RUN_TYPE(int32_t,i32,0); RUN_TYPE(uint32_t,u32,0);
  RUN_TYPE(int64_t,i64,0); RUN_TYPE(uint64_t,u64,0);
#if defined(__SIZEOF_INT128__)
  RUN_TYPE(__int128,i128,0);
#endif
  RUN_TYPE(float,f32,0); RUN_TYPE(double,f64,0);
  return 0;
}