#include "sleela_server.hpp"
#include <iostream>
#include <string>
using namespace sleela::server;
int main(int argc,char** argv){
 ServerConfig cfg;
 for(int i=1;i<argc;++i){std::string a(argv[i]);
  if(a=="--port"&&i+1<argc)cfg.port=(std::uint16_t)std::stoul(argv[++i]);
  else if(a=="--bind"&&i+1<argc)cfg.bind_address=argv[++i];
  else if(a=="--log"&&i+1<argc)cfg.log_file=argv[++i];
  else if(a=="--log-payload")cfg.log_payload=true;
  else if(a=="--max-packet"&&i+1<argc)cfg.max_packet_bytes=std::stoull(argv[++i]);
  else if(a=="--help"){std::cout<<"SLeeLa Server Edition\n --bind ADDRESS\n --port PORT\n --log FILE\n --log-payload\n --max-packet BYTES\n";return 0;}
 }
 try{SleelaServer server(cfg);if(!server.start()){std::cerr<<"SLeeLa server: start failed\n";return 1;}std::cout<<"SLeeLa Server Edition listening on "<<cfg.bind_address<<":"<<cfg.port<<"\n";server.run();}
 catch(const std::exception&e){std::cerr<<"SLeeLa server fatal: "<<e.what()<<"\n";return 2;}
 return 0;
}
