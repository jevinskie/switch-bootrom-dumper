#pragma once

#include <stdint.h>

#ifdef __cplusplus
#include "fmt/format.h"
#endif


#define DUMP_NAME_SIZE 32
#define DUMP_DATA_SIZE 1024
#define DUMP_TAG "DUMP: "

typedef struct dump_record {
	uint64_t addr;
	uint32_t len;
	char name[DUMP_NAME_SIZE];
	uint8_t data[DUMP_DATA_SIZE];

#ifdef __cplusplus
	friend std::ostream& operator<<(std::ostream &o, const dump_record &rec) {
		return o << fmt::format("{{ name: '{}' addr: {} len: {:x} }}",
			rec.name, (void *)rec.addr, rec.len);
	}
#endif

} __attribute__((packed)) dump_record_t;

void dump(const char name[DUMP_NAME_SIZE], void *addr, uint32_t len);

#ifdef __cplusplus
// }
#endif
