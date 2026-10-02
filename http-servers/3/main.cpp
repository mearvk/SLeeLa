#include "http3/http3_server.h"
#include "../../preferred-routers/preferred_router.h"

int main(int argc, char **argv) {
  sleela_preferred_router_startup(nullptr, "HTTP/3");
  return sleela_http3_server_run(argc, argv);
}
