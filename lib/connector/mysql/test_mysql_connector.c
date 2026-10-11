#include "mysql_connector.h"
#include <stdio.h>
#include <stdlib.h>
int main(void) {
    const char *host = getenv("SLEELA_MYSQL_HOST");
    const char *user = getenv("SLEELA_MYSQL_USER");
    const char *password = getenv("SLEELA_MYSQL_PASSWORD");
    const char *database = getenv("SLEELA_MYSQL_DATABASE");
    const char *port_text = getenv("SLEELA_MYSQL_PORT");
    sleela_mysql_config cfg = {0};
    char error[512] = {0};
    sleela_mysql_connection *c;
    if (!user || !password) {
        fprintf(stderr, "Set SLEELA_MYSQL_USER and SLEELA_MYSQL_PASSWORD for the integration test.\n");
        return 2;
    }
    cfg.host = host; cfg.user = user; cfg.password = password; cfg.database = database;
    cfg.port = port_text ? (unsigned int)strtoul(port_text, NULL, 10) : 0;
    cfg.connect_timeout_seconds = 5;
    c = sleela_mysql_open(&cfg, error, sizeof(error));
    if (!c) { fprintf(stderr, "connect failed: %s\n", error); return 1; }
    if (sleela_mysql_ping(c) != SLEELA_MYSQL_OK) {
        sleela_mysql_last_error(c, error, sizeof(error));
        fprintf(stderr, "ping failed: %s\n", error); sleela_mysql_close(c); return 1;
    }
    puts("SLeeLa MySQL connector: connection and ping passed.");
    sleela_mysql_close(c); return 0;
}
