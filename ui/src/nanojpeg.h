/* NanoJPEG 1.3.5 API declarations.
 * Implementation copyright (c) 2009-2016 Martin J. Fiedler, MIT license.
 * The matching implementation source carries the complete MIT license. */
#ifndef SLEELA_NANOJPEG_H
#define SLEELA_NANOJPEG_H
#ifdef __cplusplus
extern "C" {
#endif

typedef enum _nj_result {
    NJ_OK = 0,
    NJ_NO_JPEG,
    NJ_UNSUPPORTED,
    NJ_OUT_OF_MEM,
    NJ_INTERNAL_ERR,
    NJ_SYNTAX_ERROR,
    __NJ_FINISHED
} nj_result_t;

void njInit(void);
nj_result_t njDecode(const void* jpeg, const int size);
int njGetWidth(void);
int njGetHeight(void);
int njIsColor(void);
unsigned char* njGetImage(void);
int njGetImageSize(void);
int njGetPrecision(void);
void njDone(void);

#ifdef __cplusplus
}
#endif
#endif
