/* SLeeLa recursion examples — linear recursion over an array.
 * Build: cc -std=c11 -Wall -Wextra -Wpedantic linear_array_sum.c -o linear_array_sum
 */
#include <stddef.h>
#include <stdio.h>

static long long sum_from(const int *values, size_t length, size_t index) {
    if (index >= length) return 0;                 /* base case */
    return values[index] + sum_from(values, length, index + 1);
}

int main(void) {
    const int values[] = {3, 1, 4, 1, 5, 9};
    const size_t length = sizeof values / sizeof values[0];
    printf("array sum = %lld\n", sum_from(values, length, 0));
    return 0;
}
