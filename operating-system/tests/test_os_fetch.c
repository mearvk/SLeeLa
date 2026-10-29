/* operating-system/tests/test_os_fetch.c -- fetch command + path composition (download). */
#include "sleela_os_fetch.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

int main(void) {
    sleela_os_source src;
    char buf[512];

    sleela_os_source_default(&src);
    assert(strcmp(src.owner, "mearvk") == 0);
    assert(strcmp(src.repo, "OS.Sensitivity.III") == 0);
    assert(strcmp(src.branch, "main") == 0);

    /* component map mirrors SLOSSensitivitySource */
    assert(strcmp(sleela_os_component_dir("os-core"), "os-core") == 0);
    assert(strcmp(sleela_os_component_dir("base-drivers"), "base-drivers") == 0);
    assert(sleela_os_component_dir("nope") == 0);
    assert(sleela_os_source_provides_component("bootloader") == 1);
    assert(sleela_os_source_provides_component("bootloader-x") == 0);

    /* shallow clone command */
    sleela_os_fetch_clone_command(&src, SLEELA_OS_FETCH_SHALLOW, "/tmp/w", buf, sizeof buf);
    assert(strstr(buf, "git ") == buf);
    assert(strstr(buf, "--depth 1 --single-branch --branch main") != NULL);
    assert(strstr(buf, "OS.Sensitivity.III.git /tmp/w") != NULL);

    /* full clone command has no --depth */
    sleela_os_fetch_clone_command(&src, SLEELA_OS_FETCH_FULL, "/tmp/w", buf, sizeof buf);
    assert(strstr(buf, "--depth 1") == NULL);

    /* pin command: empty for shallow, present for full */
    sleela_os_fetch_pin_command(&src, SLEELA_OS_FETCH_SHALLOW, "/tmp/w", buf, sizeof buf);
    assert(buf[0] == '\0');
    sleela_os_fetch_pin_command(&src, SLEELA_OS_FETCH_FULL, "/tmp/w", buf, sizeof buf);
    assert(strstr(buf, "checkout f619c8f") != NULL);

    /* shape + component paths */
    sleela_os_fetch_shape_path(&src, "/out", SLEELA_OS_SHAPE_LINUX, buf, sizeof buf);
    assert(strcmp(buf, "/out/OS.Sensitivity.III/linux") == 0);
    sleela_os_fetch_component_path(&src, "/out", SLEELA_OS_SHAPE_WINDOWS, "os-core", buf, sizeof buf);
    assert(strcmp(buf, "/out/OS.Sensitivity.III/windows/os-core") == 0);
    assert(sleela_os_fetch_component_path(&src, "/out", SLEELA_OS_SHAPE_LINUX, "bad", buf, sizeof buf) < 0);

    printf("test_os_fetch: ok\n");
    return 0;
}
