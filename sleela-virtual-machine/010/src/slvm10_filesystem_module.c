#include "slvm10_filesystem_module.h"
#include <string.h>
int slvm10_filesystem_module_validate(const slvm10_filesystem_module_t *m){if(!m||!m->module_id||!m->schema||!m->name||!m->format_major||!m->block_size||!m->verified)return -1;return strcmp(m->schema,SLVM10_FS_MODULE_SCHEMA)==0?0:-1;}
int slvm10_filesystem_module_is_tac3(const slvm10_filesystem_module_t *m){return slvm10_filesystem_module_validate(m)==0&&strcmp(m->module_id,SLVM10_FS_MODULE_TAC3_ID)==0;}
