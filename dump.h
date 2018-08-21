#pragma once

#include <stdint.h>

#define DUMP_NAME_SIZE 32
#define DUMP_DATA_SIZE 1024
#define DUMP_TAG "DUMP: "

typedef
__attribute__((packed))
struct dump_record {
	uint64_t addr;
	uint32_t len;
	char name[DUMP_NAME_SIZE];
	uint8_t data[DUMP_DATA_SIZE];
} dump_record_t;

void dump(const char name[DUMP_NAME_SIZE], void *addr, uint32_t len);
