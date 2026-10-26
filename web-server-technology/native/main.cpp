/*
 * web-server-technology/native/main.cpp
 *
 * Entry point for the SLeeLa dynamic, SQL-backed HTTP/1.1 server.
 */
#include "dyn_sql_server.h"

int main(int argc, char **argv) {
  return sleela_wst_dyn_sql_server_run(argc, argv);
}
