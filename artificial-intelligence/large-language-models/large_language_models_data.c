#include "large_language_models_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct sllmd_file_source { FILE *file; };
sllmd_status sllmd_file_open(const char *path, sllmd_file_source **out) {
    sllmd_file_source *source;
    if (!path || !path[0] || !out) return SLLMD_INVALID_ARGUMENT;
    *out = NULL;
    source = (sllmd_file_source *)calloc(1, sizeof(*source));
    if (!source) return SLLMD_OUT_OF_MEMORY;
    source->file = fopen(path, "rb");
    if (!source->file) { free(source); return SLLMD_IO_ERROR; }
    *out = source;
    return SLLMD_OK;
}
sllmd_status sllmd_file_read(void *opaque, unsigned char *buffer,
                             size_t capacity, size_t *bytes_read) {
    sllmd_file_source *source = (sllmd_file_source *)opaque;
    size_t count;
    if (!source || !source->file || (!buffer && capacity) || !bytes_read)
        return SLLMD_INVALID_ARGUMENT;
    *bytes_read = 0;
    if (!capacity) return SLLMD_OK;
    count = fread(buffer, 1, capacity, source->file);
    *bytes_read = count;
    if (count < capacity && ferror(source->file)) return SLLMD_IO_ERROR;
    return SLLMD_OK;
}
void sllmd_file_close(sllmd_file_source *source) {
    if (!source) return;
    if (source->file) fclose(source->file);
    free(source);
}
sllmd_status sllmd_jsonl_read(sllmd_read_fn read_fn, void *connector_user_data,
                              size_t max_record_bytes, sllmd_record_fn record_fn,
                              void *record_user_data, size_t *records_read) {
    unsigned char input[8192];
    char *record;
    size_t used = 0, total = 0;
    int saw_any = 0;
    sllmd_status status;
    if (records_read) *records_read = 0;
    if (!read_fn || !record_fn || max_record_bytes == 0 ||
        max_record_bytes == (size_t)-1) return SLLMD_INVALID_ARGUMENT;
    record = (char *)malloc(max_record_bytes + 1);
    if (!record) return SLLMD_OUT_OF_MEMORY;
    for (;;) {
        size_t got = 0, i;
        status = read_fn(connector_user_data, input, sizeof(input), &got);
        if (status != SLLMD_OK) { free(record); return status; }
        if (got > sizeof(input)) { free(record); return SLLMD_IO_ERROR; }
        if (got == 0) break;
        for (i = 0; i < got; ++i) {
            unsigned char ch = input[i];
            saw_any = 1;
            if (ch == 0) { free(record); return SLLMD_INVALID_ARGUMENT; }
            if (ch == '\n') {
                if (used && record[used - 1] == '\r') --used;
                if (used) {
                    record[used] = '\0';
                    if (record_fn(record_user_data, record, used) != 0) {
                        free(record); return SLLMD_CALLBACK_STOP;
                    }
                    ++total;
                }
                used = 0;
            } else {
                if (used >= max_record_bytes) { free(record); return SLLMD_RECORD_TOO_LARGE; }
                record[used++] = (char)ch;
            }
        }
    }
    if (saw_any && used) {
        if (used && record[used - 1] == '\r') --used;
        if (used) {
            record[used] = '\0';
            if (record_fn(record_user_data, record, used) != 0) {
                free(record); return SLLMD_CALLBACK_STOP;
            }
            ++total;
        }
    }
    if (records_read) *records_read = total;
    free(record);
    return SLLMD_OK;
}
