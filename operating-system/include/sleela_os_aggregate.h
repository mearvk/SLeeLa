/*
 * operating-system/include/sleela_os_aggregate.h
 * SLeeLa OS build aggregation -- the C backing of lib/os/SLOSBuildReport.sleela
 * (the AGGREGATION capability).
 *
 * As the OS Creator emits artifacts (C/C++ sources, configs, scripts, the ISO
 * recipe), each is recorded here with its kind, byte size, and path, building a
 * printable index of everything the pipeline produced, plus the model identity
 * and an overall success flag. A caller aggregates the whole build into one
 * report and can print a single summary and enumerate exactly which files to
 * persist. Mirrors SLOSBuildReport field-for-field.
 *
 * Bounded, fixed-capacity storage (no dynamic allocation) in the honest,
 * enumerable-output style the library favours.
 */
#ifndef SLEELA_OS_AGGREGATE_H
#define SLEELA_OS_AGGREGATE_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SLEELA_OS_MAX_ARTIFACTS 256
#define SLEELA_OS_KIND_CAP       32
#define SLEELA_OS_PATH_CAP      256
#define SLEELA_OS_IDENTITY_CAP  256

/* The kinds of artifact a build emits (matches the OS Creator vocabulary). */
typedef enum sleela_os_artifact_kind {
    SLEELA_OS_ART_SOURCE = 0,  /* a C/C++/header source file */
    SLEELA_OS_ART_CONFIG,      /* a config file */
    SLEELA_OS_ART_SCRIPT,      /* a build/install/fetch script */
    SLEELA_OS_ART_ISO,         /* an ISO recipe or image */
    SLEELA_OS_ART_OTHER
} sleela_os_artifact_kind;

typedef struct sleela_os_artifact {
    sleela_os_artifact_kind kind;
    int64_t                 bytes;
    char                    path[SLEELA_OS_PATH_CAP];
} sleela_os_artifact;

typedef struct sleela_os_report {
    char               identity[SLEELA_OS_IDENTITY_CAP];
    sleela_os_artifact items[SLEELA_OS_MAX_ARTIFACTS];
    int                count;
    int                ok;      /* 1 unless failed */
} sleela_os_report;

/* Start a report for a given model identity (ok=1, empty index). */
void sleela_os_report_init(sleela_os_report *r, const char *model_identity);

/* Record one produced artifact. Returns the new count, or -1 if the report is
 * full or an argument is bad. */
int sleela_os_report_record(sleela_os_report *r,
                            sleela_os_artifact_kind kind,
                            int64_t bytes,
                            const char *path);

/* Mark the build failed (e.g. an unbuildable model). */
void sleela_os_report_fail(sleela_os_report *r);

/* The canonical kind label ("source"/"config"/"script"/"iso"/"other"). */
const char *sleela_os_artifact_kind_name(sleela_os_artifact_kind kind);

int sleela_os_report_count(const sleela_os_report *r);
int sleela_os_report_succeeded(const sleela_os_report *r);
int64_t sleela_os_report_total_bytes(const sleela_os_report *r);

/* Write the newline-separated "kind size path" index into `buf`. Returns the
 * would-be length (like snprintf). */
int sleela_os_report_index(const sleela_os_report *r, char *buf, size_t cap);

/* Write the one-line summary ("OK|FAILED: <identity> -> N artifacts") into
 * `buf`. Returns the would-be length. */
int sleela_os_report_summary(const sleela_os_report *r, char *buf, size_t cap);

#ifdef __cplusplus
}
#endif

#endif /* SLEELA_OS_AGGREGATE_H */
