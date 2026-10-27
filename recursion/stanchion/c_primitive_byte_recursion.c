/* c_primitive_byte_recursion.c
 * C typed recursion prototype for the SLeeLa byte primitive.
 * Patterns: linear recursion, divide-and-conquer, structural tree recursion.
 * Build: cc -std=c11 -Wall -Wextra -Wpedantic c_primitive_byte_recursion.c -o c_primitive_byte_recursion
 * Reference prototype; align type width, GC, and overflow rules with the VM.
 */
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
typedef uint8_t SLStanchion_byte;
typedef struct SLStanchion_byteNode {
    SLStanchion_byte value;
    struct SLStanchion_byteNode *left, *right;
} SLStanchion_byteNode;

size_t sl_stanchion_byte_count_linear(const SLStanchion_byte *v, size_t n, size_t i, SLStanchion_byte target, size_t depth, size_t limit) {
    if (v == NULL || i >= n || depth >= limit) return 0;
    return (v[i] == target ? 1u : 0u) + sl_stanchion_byte_count_linear(v, n, i + 1, target, depth + 1, limit);
}
size_t sl_stanchion_byte_count_divide(const SLStanchion_byte *v, size_t first, size_t last, SLStanchion_byte target, size_t depth, size_t limit) {
    if (v == NULL || first >= last || depth >= limit) return 0;
    if (last - first == 1) return v[first] == target ? 1u : 0u;
    size_t mid = first + (last - first) / 2;
    return sl_stanchion_byte_count_divide(v, first, mid, target, depth + 1, limit)
         + sl_stanchion_byte_count_divide(v, mid, last, target, depth + 1, limit);
}
size_t sl_stanchion_byte_count_tree(const SLStanchion_byteNode *node, SLStanchion_byte target, size_t depth, size_t limit) {
    if (node == NULL || depth >= limit) return 0;
    return (node->value == target ? 1u : 0u)
         + sl_stanchion_byte_count_tree(node->left, target, depth + 1, limit)
         + sl_stanchion_byte_count_tree(node->right, target, depth + 1, limit);
}
int main(void) {
    const SLStanchion_byte values[] = { (uint8_t)7, (uint8_t)7, (uint8_t)0 };
    const size_t n = sizeof(values) / sizeof(values[0]), limit = 64;
    printf("byte: linear=%zu divide=%zu\n",
        sl_stanchion_byte_count_linear(values, n, 0, (uint8_t)7, 0, limit),
        sl_stanchion_byte_count_divide(values, 0, n, (uint8_t)7, 0, limit));
    return 0;
}
