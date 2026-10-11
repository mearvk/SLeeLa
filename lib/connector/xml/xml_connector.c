#include "xml_connector.h"
#include <libxml/parser.h>
#include <stdio.h>
int sleela_xml_is_well_formed(const char*x,size_t n,char*e,size_t es){if(e&&es)e[0]=0;if(!x||n>2147483647U){if(e&&es)snprintf(e,es,"invalid XML input or input too large");return 0;}xmlDocPtr d=xmlReadMemory(x,(int)n,"sleela-input.xml",NULL,XML_PARSE_NONET|XML_PARSE_NOERROR|XML_PARSE_NOWARNING);if(!d){if(e&&es)snprintf(e,es,"XML is not well-formed (parser details suppressed)");return 0;}xmlFreeDoc(d);return 1;}
