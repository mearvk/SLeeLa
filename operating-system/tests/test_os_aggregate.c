/* operating-system/tests/test_os_aggregate.c -- build-report aggregation. */
#include "sleela_os_aggregate.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

int main(void) {
    sleela_os_report r;
    char buf[1024];

    sleela_os_report_init(&r, "Determinant 1.0 (Linux, amd64)");
    assert(sleela_os_report_count(&r) == 0);
    assert(sleela_os_report_succeeded(&r) == 1);

    assert(sleela_os_report_record(&r, SLEELA_OS_ART_SOURCE, 489, "os/os_boot.c") == 1);
    assert(sleela_os_report_record(&r, SLEELA_OS_ART_SOURCE, 689, "os/os_image.cpp") == 2);
    assert(sleela_os_report_record(&r, SLEELA_OS_ART_CONFIG, 343, "config/os.conf") == 3);
    assert(sleela_os_report_record(&r, SLEELA_OS_ART_SCRIPT, 380, "tools/gen-iso.sh") == 4);
    assert(sleela_os_report_record(&r, SLEELA_OS_ART_ISO, 0, "tools/gen-iso.sh (recipe)") == 5);

    assert(sleela_os_report_count(&r) == 5);
    assert(sleela_os_report_total_bytes(&r) == 489 + 689 + 343 + 380);

    assert(strcmp(sleela_os_artifact_kind_name(SLEELA_OS_ART_SOURCE), "source") == 0);
    assert(strcmp(sleela_os_artifact_kind_name(SLEELA_OS_ART_ISO), "iso") == 0);

    sleela_os_report_index(&r, buf, sizeof buf);
    assert(strstr(buf, "source 489 os/os_boot.c") != NULL);
    assert(strstr(buf, "config 343 config/os.conf") != NULL);
    /* newline-separated: first line has no leading newline */
    assert(buf[0] == 's');

    sleela_os_report_summary(&r, buf, sizeof buf);
    assert(strcmp(buf, "OK: Determinant 1.0 (Linux, amd64) -> 5 artifacts") == 0);

    sleela_os_report_fail(&r);
    assert(sleela_os_report_succeeded(&r) == 0);
    sleela_os_report_summary(&r, buf, sizeof buf);
    assert(strstr(buf, "FAILED:") == buf);

    /* bad args */
    assert(sleela_os_report_record(&r, SLEELA_OS_ART_OTHER, 1, 0) == -1);

    printf("test_os_aggregate: ok\n");
    return 0;
}
