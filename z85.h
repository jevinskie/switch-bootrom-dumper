#pragma once

#include <stdint.h>

#define Z85_BUF_SIZE(sz) ((sz) * 5 / 4 + 1)

char *Z85_encode(void *data, uint32_t size, char *buf, uint32_t buf_sz);
