#ifndef SLEELA_AI_LLM_DATA_H
#define SLEELA_AI_LLM_DATA_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
#if defined(_WIN32) && defined(SLEELA_AI_SHARED)
# if defined(SLEELA_AI_BUILD)
#  define SLLMD_API __declspec(dllexport)
# else
#  define SLLMD_API __declspec(dllimport)
# endif
#elif defined(__GNUC__) && defined(SLEELA_AI_SHARED)
# define SLLMD_API __attribute__((visibility("default")))
#else
# define SLLMD_API
#endif
#define SLLMD_ABI_VERSION 1u
typedef enum sllmd_status {
 SLLMD_OK=0, SLLMD_INVALID_ARGUMENT=1, SLLMD_OUT_OF_MEMORY=2,
 SLLMD_IO_ERROR=3, SLLMD_RECORD_TOO_LARGE=4, SLLMD_CALLBACK_STOP=5
} sllmd_status;
/* A connector read callback returns SLLMD_OK and sets *bytes_read to zero at EOF.
   On error it returns a non-zero status. The caller owns connector_user_data. */
typedef sllmd_status (*sllmd_read_fn)(void *connector_user_data,
                                     unsigned char *buffer, size_t capacity,
                                     size_t *bytes_read);
/* Receives one complete non-empty JSONL record as bytes, without its newline.
   The reader rejects embedded NUL but does not validate UTF-8 or JSON syntax.
   Return non-zero to stop iteration; the reader reports SLLMD_CALLBACK_STOP. */
typedef int (*sllmd_record_fn)(void *user_data, const char *record, size_t length);
/* Reads a stream in bounded memory. max_record_bytes must be non-zero.
   records_read may be NULL. CRLF is normalized by removing the trailing CR.
   Blank lines are ignored. A final record without a newline is delivered. */
SLLMD_API sllmd_status sllmd_jsonl_read(sllmd_read_fn read_fn,
                                       void *connector_user_data,
                                       size_t max_record_bytes,
                                       sllmd_record_fn record_fn,
                                       void *record_user_data,
                                       size_t *records_read);
/* Convenience local-file connector. The returned handle is owned by the caller
   and must be closed with sllmd_file_close after reading. */
typedef struct sllmd_file_source sllmd_file_source;
SLLMD_API sllmd_status sllmd_file_open(const char *path, sllmd_file_source **out);
SLLMD_API sllmd_status sllmd_file_read(void *source, unsigned char *buffer,
                                      size_t capacity, size_t *bytes_read);
SLLMD_API void sllmd_file_close(sllmd_file_source *source);
#ifdef __cplusplus
}
#endif
#endif
