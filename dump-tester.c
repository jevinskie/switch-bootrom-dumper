#include <stdio.h>
#include <stdint.h>

#include "dump.h"

#if defined(__APPLE__)
#define TEXT_SEC_NAME "__TEXT,__text"
extern void *__dso_handle;
#define HEADER_START (&__dso_handle)
extern void *__text_start __asm("section$start$__TEXT$__text");
#define EXE_START (&__text_start)
extern void *__data_start __asm("section$start$__DATA$__data");
#define DATA_START (&__data_start)
#elif defined(__unix__)
#define TEXT_SEC_NAME ".text"
extern void *__executable_start;
#define HEADER_START (&__executable_start)
extern void *__text_start;
#define EXE_START (&__text_start)
extern void *__data_start;
#define DATA_START (&__data_start)
#endif

__attribute__((section(TEXT_SEC_NAME)))
uint8_t tfill0[1*1024] = {0xc0, 0xfe, 0xd0, 0x0d};

uint8_t fill0[2*1024] = {0xde, 0xea, 0xbe, 0xef};
uint8_t fill1[] = {0xba, 0xad, 0xf0, 0x0d};

int main(int argc, const char **argv) {
	dump("hdr", HEADER_START, 1 * DUMP_DATA_SIZE + 128);
	dump("exe", EXE_START, 3 * DUMP_DATA_SIZE + 256);
	dump("data", DATA_START, 4 * DUMP_DATA_SIZE + 512);
	return 0;
}
