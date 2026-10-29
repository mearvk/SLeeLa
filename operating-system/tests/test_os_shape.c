/* operating-system/tests/test_os_shape.c -- shape model conventions. */
#include "sleela_os_shape.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

int main(void) {
    assert(sleela_os_shape_by_name("Linux")   == SLEELA_OS_SHAPE_LINUX);
    assert(sleela_os_shape_by_name("Windows")  == SLEELA_OS_SHAPE_WINDOWS);
    assert(sleela_os_shape_by_name("macOS")    == SLEELA_OS_SHAPE_MACOS);
    assert(sleela_os_shape_by_name("nonsense") == SLEELA_OS_SHAPE_LINUX); /* defaults */

    assert(strcmp(sleela_os_shape_name(SLEELA_OS_SHAPE_MACOS), "macOS") == 0);
    assert(strcmp(sleela_os_shape_path_separator(SLEELA_OS_SHAPE_WINDOWS), "\\") == 0);
    assert(strcmp(sleela_os_shape_path_separator(SLEELA_OS_SHAPE_LINUX), "/") == 0);
    assert(strcmp(sleela_os_shape_list_separator(SLEELA_OS_SHAPE_WINDOWS), ";") == 0);
    assert(strcmp(sleela_os_shape_init_manager(SLEELA_OS_SHAPE_LINUX), "systemd") == 0);
    assert(strcmp(sleela_os_shape_init_manager(SLEELA_OS_SHAPE_MACOS), "launchd") == 0);
    assert(strcmp(sleela_os_shape_boot_style(SLEELA_OS_SHAPE_WINDOWS), "UEFI+BOOTMGR") == 0);
    assert(strcmp(sleela_os_shape_root_filesystem(SLEELA_OS_SHAPE_MACOS), "APFS") == 0);
    assert(strcmp(sleela_os_shape_system_root(SLEELA_OS_SHAPE_WINDOWS), "C:\\Windows") == 0);
    assert(strcmp(sleela_os_shape_executable_extension(SLEELA_OS_SHAPE_WINDOWS), ".exe") == 0);
    assert(strcmp(sleela_os_shape_executable_extension(SLEELA_OS_SHAPE_LINUX), "") == 0);

    assert(strcmp(sleela_os_shape_warehouse_dir(SLEELA_OS_SHAPE_LINUX),   "linux")   == 0);
    assert(strcmp(sleela_os_shape_warehouse_dir(SLEELA_OS_SHAPE_WINDOWS), "windows") == 0);
    assert(strcmp(sleela_os_shape_warehouse_dir(SLEELA_OS_SHAPE_MACOS),   "macos")   == 0);

    printf("test_os_shape: ok\n");
    return 0;
}
