#include <stdio.h>

int main(void) {
    int total = 0;
    int i;
    printf("%d\n", 40 + 2);
    for (i = 1; i <= 5; ++i) total += i;
    printf("%d\n", total);
    puts("hello");
    return 0;
}
