#include "PrintState.h"
const char*cr_print_state_name(CrPrintState s){switch(s){case CR_START:return "START";case CR_WORKING:return "WORKING";case CR_PROGRESS:return "PROGRESS";case CR_COMPLETE:return "COMPLETE";case CR_WARN:return "WARN";default:return "ERROR";}}
