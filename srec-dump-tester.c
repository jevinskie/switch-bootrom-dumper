#include <stdio.h>
#include <stdint.h>

#include "srec-dump.h"

#if defined(__APPLE__)
extern uint8_t __dso_handle;
#define EXE_START (&__dso_handle)
#elif defined(__unix__)
extern uint8_t __executable_start;
#define EXE_START (&__executable_start)
#endif

int main(int argc, const char **argv) {
	srec_dump(EXE_START, 4096);
	return 0;
}
