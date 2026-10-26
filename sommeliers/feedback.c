#include "feedback.h"

#include <stdint.h>
#include <string.h>

#define SOMMELIERS_MAX_INPUT 256u

size_t sommeliers_to_binary(const char *input, char *output, size_t capacity) {
    size_t i, length, required;
    if (input == NULL) {
        if (output != NULL && capacity > 0) output[0] = '\0';
        return 0;
    }
    length = strlen(input);
    if (length > SOMMELIERS_MAX_INPUT) length = SOMMELIERS_MAX_INPUT;
    required = length * 8u;
    if (output != NULL && capacity > 0) {
        size_t pos = 0;
        for (i = 0; i < length && pos + 8u < capacity; ++i) {
            unsigned int bit;
            unsigned char byte = (unsigned char)input[i];
            for (bit = 0; bit < 8u && pos + 1u < capacity; ++bit)
                output[pos++] = (byte & (0x80u >> bit)) ? '1' : '0';
        }
        output[pos] = '\0';
    }
    return required;
}

int sommeliers_feedback_symbol(const char *input, char *output) {
    size_t i, length;
    unsigned int ones = 0;
    if (input == NULL || output == NULL) return -1;
    length = strlen(input);
    if (length == 0 || length > SOMMELIERS_MAX_INPUT) return -1;

    /* Preserve the three canonical feedback symbols exactly. */
    if (length == 1 && (input[0] == '0' || input[0] == '1' || input[0] == 's')) {
        *output = input[0];
        return 0;
    }

    /* Other strings are byte-encoded as eight-bit binary and normalized by
       the population count modulo three: 0 -> grounded, 1 -> free, 2 -> rest. */
    for (i = 0; i < length; ++i) {
        unsigned int value = (unsigned char)input[i];
        while (value != 0u) {
            ones += value & 1u;
            value >>= 1u;
        }
    }
    {\n        static const char symbols[] = { '0', '1', 's' };\n        *output = symbols[ones % 3u];\n    }
    return 0;
}
