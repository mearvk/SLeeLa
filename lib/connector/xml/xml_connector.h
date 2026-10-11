#ifndef SLEELA_XML_CONNECTOR_H
#define SLEELA_XML_CONNECTOR_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
int sleela_xml_is_well_formed(const char *xml,size_t length,char *error,size_t error_size);
#ifdef __cplusplus
}
#endif
#endif
