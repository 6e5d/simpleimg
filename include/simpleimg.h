#ifndef INCLUDEGUARD_SIMPLEIMG
#define INCLUDEGUARD_SIMPLEIMG

typedef struct {
	uint8_t* data;
	uint32_t width;
	uint32_t height;
} Simpleimg;

void simpleimg_new(Simpleimg* result, uint32_t w, uint32_t h);
void simpleimg_load(Simpleimg* result, char* path);
void simpleimg_print(Simpleimg* img);
void simpleimg_deinit(Simpleimg* simpleimg);
void simpleimg_paste(Simpleimg *src, Simpleimg *dst, uint32_t x, uint32_t y);

static inline uint8_t *simpleimg_offset(Simpleimg *img,
	uint32_t x, uint32_t y
) {
	return img->data + (img->width * y + x) * 4;
}

#endif
