#ifndef INCLUDEGUARD_SIMPLEIMG
#define INCLUDEGUARD_SIMPLEIMG

typedef struct {
	uint8_t* data;
	uint32_t width;
	uint32_t height;
} Simpleimg();

void simpleimg(new)(Simpleimg()* result, uint32_t w, uint32_t h);
void simpleimg(save)(Simpleimg()* img, char* path);
void simpleimg(load)(Simpleimg()* result, char* path);
void simpleimg(print)(Simpleimg()* img);
void simpleimg(deinit)(Simpleimg()* simpleimg);
void simpleimg(paste)(Simpleimg() *src, Simpleimg() *dst,
	uint32_t w, uint32_t h,
	uint32_t sx1, uint32_t sy1,
	uint32_t dx1, uint32_t dy1);
void simpleimg(pixelwise)(Simpleimg() *img,
	uint32_t x1, uint32_t y1, uint32_t w, uint32_t h,
	void (*op)(uint8_t*));
void simpleimg(clear)(Simpleimg() *img,
	uint32_t x1, uint32_t y1, uint32_t w, uint32_t h);
void simpleimg(clearall)(Simpleimg() *img);
uint8_t *simpleimg(offset)(Simpleimg() *img, uint32_t x, uint32_t y);

#endif
