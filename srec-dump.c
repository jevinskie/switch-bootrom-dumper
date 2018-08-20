#include "srec-dump.h"

#include <stdio.h>

void srec_dump(void *addr, size_t len) {
	printf("dumping %p size %zu\n", addr, len);
}
