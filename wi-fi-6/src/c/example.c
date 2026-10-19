#include "wifi_profile.h"
#include <stdio.h>
int main(void){const wifi_profile_t *p=wifi_profile_get();if(!p)return 1;wifi_profile_print();printf("\n80 MHz profile support: %s\n",wifi_profile_supports_width_mhz(80u)?"yes":"no");puts("Actual adapter capabilities must be queried from the operating system/driver.");return 0;}
