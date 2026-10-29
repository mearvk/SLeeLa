/* operating-system/tests/test_os_translate.c -- executable classification (manipulate). */
#include "sleela_os_translate.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
    /* guest shape by extension */
    assert(sleela_os_guest_shape_for_ext(".exe")  == SLEELA_OS_SHAPE_WINDOWS);
    assert(sleela_os_guest_shape_for_ext("exe")   == SLEELA_OS_SHAPE_WINDOWS); /* dot optional */
    assert(sleela_os_guest_shape_for_ext(".EXE")  == SLEELA_OS_SHAPE_WINDOWS); /* case-insensitive */
    assert(sleela_os_guest_shape_for_ext(".app")  == SLEELA_OS_SHAPE_MACOS);
    assert(sleela_os_guest_shape_for_ext(".sh")   == SLEELA_OS_SHAPE_LINUX);
    assert(sleela_os_guest_shape_for_ext("")      == SLEELA_OS_SHAPE_LINUX);

    /* native executability per host */
    assert(sleela_os_is_native_executable(SLEELA_OS_SHAPE_WINDOWS, ".exe") == 1);
    assert(sleela_os_is_native_executable(SLEELA_OS_SHAPE_WINDOWS, ".sh")  == 0);
    assert(sleela_os_is_native_executable(SLEELA_OS_SHAPE_LINUX, ".sh")    == 1);
    assert(sleela_os_is_native_executable(SLEELA_OS_SHAPE_LINUX, "")       == 1); /* ELF */
    assert(sleela_os_is_native_executable(SLEELA_OS_SHAPE_LINUX, ".exe")   == 0);
    assert(sleela_os_is_native_executable(SLEELA_OS_SHAPE_MACOS, ".app")   == 1);

    /* needs translation when guest shape != host */
    assert(sleela_os_needs_translation(SLEELA_OS_SHAPE_LINUX, ".exe") == 1);
    assert(sleela_os_needs_translation(SLEELA_OS_SHAPE_LINUX, ".sh")  == 0);
    assert(sleela_os_needs_translation(SLEELA_OS_SHAPE_WINDOWS, ".exe") == 0);

    printf("test_os_translate: ok\n");
    return 0;
}
