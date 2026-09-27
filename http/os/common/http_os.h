#ifndef SLEELA_HTTP_OS_H
#define SLEELA_HTTP_OS_H
#ifdef __cplusplus
extern "C" {
#endif
typedef enum sleela_http_os { SLEELA_HTTP_OS_UNKNOWN=0, SLEELA_HTTP_OS_LINUX, SLEELA_HTTP_OS_WINDOWS, SLEELA_HTTP_OS_MACOS } sleela_http_os_t;
sleela_http_os_t sleela_http_os_detect(void);
const char *sleela_http_os_name(sleela_http_os_t os);
#ifdef __cplusplus
}
#endif
#endif
