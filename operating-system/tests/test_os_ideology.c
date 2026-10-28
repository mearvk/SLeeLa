/* operating-system/tests/test_os_ideology.c -- origin-scope inspection + shape conversion. */
#include "sleela_os_ideology.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

int main(void) {
    /* origin classification (Metadata metaOrigin scale) */
    assert(sleela_os_origin_classify("International") == SLEELA_OS_SCOPE_INTERNATIONAL);
    assert(sleela_os_origin_classify("Galactic")      == SLEELA_OS_SCOPE_GALACTIC);
    assert(sleela_os_origin_classify("National")      == SLEELA_OS_SCOPE_NATIONAL);
    assert(sleela_os_origin_classify("Finland")       == SLEELA_OS_SCOPE_NATIONAL); /* a nation name */
    assert(sleela_os_origin_classify("none")          == SLEELA_OS_SCOPE_UNKNOWN);
    assert(sleela_os_origin_classify("")              == SLEELA_OS_SCOPE_UNKNOWN);
    assert(sleela_os_origin_classify(0)               == SLEELA_OS_SCOPE_UNKNOWN);

    assert(sleela_os_origin_is_known("International") == 1);
    assert(sleela_os_origin_is_known("none") == 0);
    assert(sleela_os_origin_is_known("unknown") == 0);

    assert(strcmp(sleela_os_origin_scope_name(SLEELA_OS_SCOPE_GALACTIC), "Galactic") == 0);

    /* ordered by breadth: National < International < Galactic */
    assert(sleela_os_origin_scope_cmp(SLEELA_OS_SCOPE_NATIONAL, SLEELA_OS_SCOPE_INTERNATIONAL) < 0);
    assert(sleela_os_origin_scope_cmp(SLEELA_OS_SCOPE_GALACTIC, SLEELA_OS_SCOPE_NATIONAL) > 0);
    assert(sleela_os_origin_scope_cmp(SLEELA_OS_SCOPE_NATIONAL, SLEELA_OS_SCOPE_NATIONAL) == 0);

    /* conversion matrix (mirrors SLCompatLayer) */
    assert(sleela_os_convert_is_native(SLEELA_OS_SHAPE_LINUX, SLEELA_OS_SHAPE_LINUX) == 1);
    assert(strcmp(sleela_os_convert_runtime(SLEELA_OS_SHAPE_LINUX, SLEELA_OS_SHAPE_LINUX), "") == 0);
    assert(strcmp(sleela_os_convert_runtime(SLEELA_OS_SHAPE_LINUX, SLEELA_OS_SHAPE_WINDOWS), "wine") == 0);
    assert(strcmp(sleela_os_convert_runtime(SLEELA_OS_SHAPE_WINDOWS, SLEELA_OS_SHAPE_LINUX), "wsl2") == 0);
    assert(strcmp(sleela_os_convert_runtime(SLEELA_OS_SHAPE_MACOS, SLEELA_OS_SHAPE_WINDOWS), "wine") == 0);
    assert(strcmp(sleela_os_convert_runtime(SLEELA_OS_SHAPE_LINUX, SLEELA_OS_SHAPE_MACOS), "darling") == 0);

    assert(sleela_os_convert_component_count(SLEELA_OS_SHAPE_LINUX, SLEELA_OS_SHAPE_LINUX) == 0);
    assert(sleela_os_convert_component_count(SLEELA_OS_SHAPE_LINUX, SLEELA_OS_SHAPE_WINDOWS) == 3); /* binfmt_misc */
    assert(sleela_os_convert_component_count(SLEELA_OS_SHAPE_WINDOWS, SLEELA_OS_SHAPE_LINUX) == 2);

    printf("test_os_ideology: ok\n");
    return 0;
}
