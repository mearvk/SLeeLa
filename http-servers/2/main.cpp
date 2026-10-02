#include "http2/http2_server.h"
#include "../../preferred-routers/preferred_router.h"
int main(int argc,char**argv){sleela_preferred_router_startup(nullptr,"HTTP/2");return sleela_http2_server_run(argc,argv);}
