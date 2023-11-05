#include <stddef.h>
#include <stdint.h>

typedef struct {
	uint8_t* data;
	uint32_t width;
	uint32_t height;
} Simpleimg;

Simpleimg* simpleimg_load(char* path);
void simpleimg_print(Simpleimg* img);
void simpleimg_destroy(Simpleimg* simpleimg);
