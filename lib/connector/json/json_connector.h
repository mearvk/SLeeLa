#ifndef SLEELA_JSON_CONNECTOR_H
#define SLEELA_JSON_CONNECTOR_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum sleela_json_type { SLEELA_JSON_OBJECT,SLEELA_JSON_ARRAY,SLEELA_JSON_STRING,SLEELA_JSON_PRIMITIVE } sleela_json_type;
typedef struct sleela_json_token { sleela_json_type type; size_t start,end,parent; } sleela_json_token;
size_t sleela_json_tokenize(const char *json,sleela_json_token *tokens,size_t capacity);
#ifdef __cplusplus
}
#endif
#endif
