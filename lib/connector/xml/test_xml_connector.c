#include "xml_connector.h"
#include <assert.h>
#include <string.h>
int main(void){char e[128];const char*good="<root><item/></root>";const char*bad="<root>";assert(sleela_xml_is_well_formed(good,strlen(good),e,sizeof e));assert(!sleela_xml_is_well_formed(bad,strlen(bad),e,sizeof e));return 0;}
