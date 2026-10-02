#include "slvm_capability.h"
#include <stddef.h>
int slvm_capability_domain_valid(slvm_capability_domain_t domain){return domain>=SLVM_CAP_FILES&&domain<=SLVM_CAP_GUI_MEDIA;}
int slvm_capability_allows(const slvm_capability_t *cap,uint64_t rights){return cap && slvm_capability_domain_valid(cap->domain) && (cap->rights & rights)==rights;}
