/*
 * virtual-machine/vm-version/001/vm_status.h
 * VM v001 ground floor -- the "what am I running" readout.
 *
 * Idea System(TM) / Idea(TM).  From Author; distributive.
 *
 * One call that renders the user-facing status of the VM: version, the active
 * configuration (memory limit, thread cap, verbosity, enabled capabilities),
 * and -- when the memory manager is compiled in -- its live stats. This is the
 * banner a user sees to understand their running VM.
 */
#ifndef VM_STATUS_H
#define VM_STATUS_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define VM_VERSION_STRING "idea-vm v001 (ground floor)"

/* Render the status into `buf` (NUL-terminated, truncated to cap). Returns the
 * number of bytes that would be written (snprintf semantics). Reads the current
 * vm_config and, if available, the memory-manager stats. */
int vm_status_render(char *buf, size_t cap);

/* Print the status banner to stdout at NORMAL+ verbosity (no-op at QUIET). */
void vm_status_banner(void);

#ifdef __cplusplus
}
#endif

#endif /* VM_STATUS_H */
