#include "dump.h"

#include <stdio.h>

void dump(void *addr, size_t len) {
	printf("dumping %p size %zu\n", addr, len);
}
