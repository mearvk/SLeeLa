#include "http3_port.h"
#include <stdio.h>
#include <string.h>
int main(void) {
    http3_port_t p;
    char text[64];
    http3_port_from_u64(&p, 65535u);
    if (http3_port_to_decimal(&p, text, sizeof(text)) != 0 || strcmp(text, "65535") != 0) return 1;
    if (http3_port_from_decimal(&p, "999999999999999999999999999999999999999999999999") != 0) return 2;
    if (http3_port_to_decimal(&p, text, sizeof(text)) != 0 ||
        strcmp(text, "999999999999999999999999999999999999999999999999") != 0) return 3;
    if (http3_port_from_decimal(&p, "1000000000000000000000000000000000000000000000000") == 0) return 4;
    if (http3_port_increment(&p) == 0) return 5;
    puts("HTTP 3.0 extended port test: PASS");
    return 0;
}
