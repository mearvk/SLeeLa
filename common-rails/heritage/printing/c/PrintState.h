#ifndef CR_PRINT_STATE_H
#define CR_PRINT_STATE_H
typedef enum{CR_START,CR_WORKING,CR_PROGRESS,CR_COMPLETE,CR_WARN,CR_ERROR}CrPrintState;const char*cr_print_state_name(CrPrintState);
#endif
