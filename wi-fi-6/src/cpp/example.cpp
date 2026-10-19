#include "wifi_profile.hpp"
#include <iostream>
int main(){const auto p=sleela::wifi::profile();p.print();std::cout<<"\n80 MHz profile support: "<<(p.supportsWidth(80)?"yes":"no")<<"\nActual adapter capabilities must be queried from the operating system/driver.\n";return 0;}
