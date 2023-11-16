#include <stddef.h>
#include <stdint.h>

typedef struct {
	uint8_t* data;
	uint32_t width;
	uint32_t height;
} Simpleimg;

void simpleimg_load(Simpleimg* result, char* path);
void simpleimg_print(Simpleimg* img);
void simpleimg_deinit(Simpleimg* simpleimg);
uint8_t *simpleimg_offset(Simpleimg *img, uint32_t x, uint32_t y);
void simpleimg_paste(Simpleimg *src, Simpleimg *dst, uint32_t x, uint32_t y);
