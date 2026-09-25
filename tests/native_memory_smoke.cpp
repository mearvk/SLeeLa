#include "../api/native/memory/sleela_memory_manager.h"
#include <cassert>
#include <cstring>
int main(){sleela_mm* mm=sleela_mm_create();assert(mm);char* p=(char*)sleela_mm_alloc(mm,64);assert(p);std::memset(p,0x5a,64);assert(sleela_mm_validate(mm));p=(char*)sleela_mm_realloc(mm,p,128);assert(p);assert(sleela_mm_free(mm,p)==SLEELA_MM_OK);assert(sleela_mm_validate(mm));sleela_mm_destroy(mm);return 0;}
