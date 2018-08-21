#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define Z85_ENC_BUF_SIZE(sz) ((sz) * 5 / 4 + 1)
#define Z85_DEC_BUF_SIZE(sz) ((sz) * 4 / 5)

char *Z85_encode(void *data, uint32_t size, char *buf, uint32_t buf_sz);

uint8_t *Z85_decode(const char *string, uint8_t *buf, uint32_t buf_sz);

#ifdef __cplusplus
}
#endif
