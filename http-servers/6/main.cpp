#include "server.hpp"
#include <iostream>
#include <string>
int main(int argc,char**v){sleela::http6::Config c;for(int i=1;i<argc;i++){std::string a=v[i];if(a=="--addr"&&i+1<argc)c.address=v[++i];else if(a=="--port"&&i+1<argc)c.port=static_cast<std::uint16_t>(std::stoul(v[++i]));else if(a=="--log"&&i+1<argc)c.log=v[++i];else if(a=="--once")c.once=true;else if(a=="--help"){std::cout<<"http-server-6 [--addr IPv4] [--port N] [--log FILE] [--once]\n";return 0;}else return 2;}return sleela::http6::run(c);}