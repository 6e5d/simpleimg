#ifndef INCLUDEGUARD_SIMPLEIMG
#define INCLUDEGUARD_SIMPLEIMG

typedef struct {
	uint8_t* data;
	uint32_t width;
	uint32_t height;
} Simpleimg;

void simpleimg_new(Simpleimg* result, uint32_t w, uint32_t h);
void simpleimg_save(Simpleimg* img, char* path);
void simpleimg_load(Simpleimg* result, char* path);
void simpleimg_print(Simpleimg* img);
void simpleimg_deinit(Simpleimg* simpleimg);
void simpleimg_paste(Simpleimg *src, Simpleimg *dst,
	uint32_t w, uint32_t h,
	uint32_t sx1, uint32_t sy1,
	uint32_t dx1, uint32_t dy1);
void simpleimg_pixelwise(Simpleimg *img,
	uint32_t x1, uint32_t y1, uint32_t w, uint32_t h,
	void (*op)(uint8_t*));
void simpleimg_clear(Simpleimg *img,
	uint32_t x1, uint32_t y1, uint32_t w, uint32_t h);
void simpleimg_clearall(Simpleimg *img);

static inline uint8_t *simpleimg_offset(Simpleimg *img,
	uint32_t x, uint32_t y
) {
	return img->data + (img->width * y + x) * 4;
}

#endif
