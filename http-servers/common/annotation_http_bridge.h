#ifndef SLEELA_ANNOTATION_HTTP_BRIDGE_H
#define SLEELA_ANNOTATION_HTTP_BRIDGE_H
#ifdef __cplusplus
extern "C" {
#endif
typedef struct sleela_http_forwarding {
    const char *holding_document;
    const char *forwarding_annotation;
    const char *nexter_colony;
    unsigned http_grade;
    int validated;
} sleela_http_forwarding;
int sleela_http_forwarding_validate(const sleela_http_forwarding *f);
#ifdef __cplusplus
}
#endif
#endif
