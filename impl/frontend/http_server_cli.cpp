#include "http_server_cli.h"
#include "../../http-servers/common/http_server.h"
#include <iostream>
#include <string>
int sleelaHttpServerCommand(int argc,char**argv){
  if(argc<3){std::cerr<<"sleela http-server: expected grade 1, 2, or 3\n";return 2;}
  std::string g=argv[2];
  if(g!="1"&&g!="2"&&g!="3"){std::cerr<<"sleela http-server: grade must be 1, 2, or 3\n";return 2;}
  return sleela_http_server_run(g.c_str(),argc-2,argv+2);
}
