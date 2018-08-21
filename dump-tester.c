#include <stdio.h>
#include <stdint.h>

#include "dump.h"

#if defined(__APPLE__)
extern void *__dso_handle;
#define EXE_START (&__dso_handle)
extern void *__data_start __asm("segment$start$__DATA");
#define DATA_START (&__data_start)
#elif defined(__unix__)
extern void *__executable_start;
#define EXE_START (&__executable_start)
extern void *__data_start;
#define DATA_START (&__data_start)
#endif

int main(int argc, const char **argv) {
	dump("exe", EXE_START, 2 * DUMP_DATA_SIZE + 128);
	dump("data", DATA_START, 3 * DUMP_DATA_SIZE + 256);
	return 0;
}
