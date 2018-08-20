#include <stdio.h>

#include "srec-dump.h"

#ifdef __APPLE__
extern void *__dso_handle;
#define EXE_START (&__dso_handle)
#endif

int main(int argc, const char **argv) {
	srec_dump(EXE_START, 4096);
	return 0;
}
