#ifndef SLEELA_MYSQL_CONNECTOR_H
#define SLEELA_MYSQL_CONNECTOR_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
#if defined(_WIN32) && defined(SLEELA_MYSQL_CONNECTOR_SHARED)
# if defined(SLEELA_MYSQL_CONNECTOR_BUILD)
#  define SLEELA_MYSQL_API __declspec(dllexport)
# else
#  define SLEELA_MYSQL_API __declspec(dllimport)
# endif
#else
# define SLEELA_MYSQL_API
#endif

typedef struct sleela_mysql_connection sleela_mysql_connection;
typedef struct sleela_mysql_config {
    const char *host;
    const char *user;
    const char *password;
    const char *database;
    unsigned int port;
    const char *unix_socket;
    unsigned long client_flags;
    unsigned int connect_timeout_seconds;
} sleela_mysql_config;

typedef enum sleela_mysql_status {
    SLEELA_MYSQL_OK = 0,
    SLEELA_MYSQL_INVALID_ARGUMENT = 1,
    SLEELA_MYSQL_OUT_OF_MEMORY = 2,
    SLEELA_MYSQL_CLIENT_ERROR = 3,
    SLEELA_MYSQL_NOT_CONNECTED = 4
} sleela_mysql_status;

SLEELA_MYSQL_API sleela_mysql_connection *sleela_mysql_open(const sleela_mysql_config *, char *, size_t);
SLEELA_MYSQL_API void sleela_mysql_close(sleela_mysql_connection *);
SLEELA_MYSQL_API sleela_mysql_status sleela_mysql_ping(sleela_mysql_connection *);
/* Executes one SQL string; never concatenate untrusted input. SELECT rows are discarded. */
SLEELA_MYSQL_API sleela_mysql_status sleela_mysql_execute(sleela_mysql_connection *, const char *);
SLEELA_MYSQL_API unsigned long long sleela_mysql_affected_rows(const sleela_mysql_connection *);
SLEELA_MYSQL_API size_t sleela_mysql_last_error(const sleela_mysql_connection *, char *, size_t);
SLEELA_MYSQL_API const char *sleela_mysql_client_version(void);
#ifdef __cplusplus
}
#endif
#endif
