#include "slvm9_native.h"
#include <assert.h>
#include <stdio.h>
int main(void){slvm9_native_filesystem_t fs;slvm9_native_file_t file;assert(slvm9_native_probe_filesystem(".",&fs)==SLVM9_OK);assert(slvm9_filesystem_validate(&fs.filesystem)==SLVM9_OK);assert(slvm9_native_validate_generation(&fs,&fs.filesystem)==SLVM9_OK);assert(slvm9_native_probe_file(__FILE__,&file)==SLVM9_OK);assert(slvm9_file_validate(&file.file)==SLVM9_OK);puts("SLVM/9 native adapter: PASS");return 0;}
