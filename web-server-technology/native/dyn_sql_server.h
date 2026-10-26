#ifndef SLEELA_WST_DYN_SQL_SERVER_H
#define SLEELA_WST_DYN_SQL_SERVER_H
/*
 * web-server-technology/native/dyn_sql_server.h
 *
 * Public entry point for the SLeeLa dynamic, SQL-backed HTTP/1.1 server.
 *
 * This server extends the hardened Grade-1 file server model
 * (http-servers/common/http_server.cpp) with two capabilities:
 *
 *   1. IPv6 / dual-stack listening, so it is reachable from the full range of
 *      world IP addresses (IPv4-mapped v6 on a single AF_INET6 socket, with an
 *      automatic AF_INET fallback where dual-stack is unavailable).
 *
 *   2. A dynamic request path at /db/... that executes SQL against the
 *      CSV-backed SLeeLa SQL engine (lib/sleela-sql) and returns JSON. This is
 *      the dynamic (non-static) half: responses are computed per request from
 *      live table data rather than read from a file on disk.
 *
 * Static file serving, bounded worker pool, bounded headers/bodies, chunked
 * decoding limits, path-traversal containment, keep-alive, and graceful
 * shutdown are retained from the Grade-1 design.
 */
#ifdef __cplusplus
extern "C" {
#endif

/*
 * Run the dynamic SQL-backed HTTP server until SIGINT/SIGTERM (or one request
 * when --once is given). Returns 0 on clean shutdown, 2 on bad arguments,
 * 1 on a fatal startup error. argv is the usual argv tail after the program
 * name; see --help for options.
 */
int sleela_wst_dyn_sql_server_run(int argc, char **argv);

#ifdef __cplusplus
}
#endif
#endif /* SLEELA_WST_DYN_SQL_SERVER_H */
