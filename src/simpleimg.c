#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include "../include/simpleimg.h"
#define stbi_rgba STBI_rgb_alpha

static uint8_t c4to256(uint8_t* color) {
	uint8_t result = 0;
	for (int idx = 2; idx >= 0; idx -= 1) {
		result *= 6;
		uint16_t c = color[idx];
		result += (uint8_t)(c * 6 / 256);
	}
	return result + 16;
}

void simpleimg(print)(Simpleimg()* img) {
	for (size_t hh = 0; hh < img->height; hh += 2) {
		for (size_t ww = 0; ww < img->width; ww += 1) {
			uint8_t c = c4to256(
				&img->data[(hh * img->width + ww) * 4]);
			// printf("%d ", c);
			printf("[48;5;%dm ", c);
		}
		printf("[40m\n");
	}
}

void simpleimg(new)(Simpleimg()* result, uint32_t w, uint32_t h) {
	result->data = calloc(w * h * 4, sizeof(uint8_t));
	result->width = w;
	result->height = h;
}

static void bgra2rgba(uint8_t* data, size_t w, size_t h) {
	for (size_t i = 0; i < (size_t)h * (size_t)w; i += 1) {
		uint8_t tmp = data[i * 4];
		data[i * 4] = data[i * 4 + 2];
		data[i * 4 + 2] = tmp;
	}
}

void simpleimg(load)(Simpleimg()* result, char* path) {
	int w, h, c;
	result->data = stbi_load(
		path, &w, &h, &c, stbi_rgba
	);
	assert(result->data != NULL);
	result->width = (uint32_t)w;
	result->height = (uint32_t)h;
	bgra2rgba(result->data, (size_t)w, (size_t)h);
}

// save make the whole copy which is not efficient
// however stbi itself is already inefficient so we don't care about that
void simpleimg(save)(Simpleimg()* img, char* path) {
	uint8_t *buffer = malloc(img->width * img->height * 4);
	assert(buffer != NULL);
	memcpy(buffer, img->data, img->width * img->height * 4);
	bgra2rgba(buffer, img->width, img->height);
	stbi_write_png(path, (int)img->width, (int)img->height,
		4, buffer, (int)img->width * 4);
	free(buffer);
	printf("saved %s\n", path);
}

void simpleimg(deinit)(Simpleimg()* simpleimg) {
	free(simpleimg->data);
}

void simpleimg(pixelwise)(Simpleimg() *img,
	uint32_t x1, uint32_t y1, uint32_t w, uint32_t h,
	void (*op)(uint8_t*)
) {
	uint32_t x2 = x1 + w;
	uint32_t y2 = y1 + h;
	assert(x2 <= img->width);
	assert(y2 <= img->height);
	for (uint32_t y = y1; y < y2; y += 1) {
		for (uint32_t x = x1; x < x2; x += 1) {
			op(simpleimg(offset)(img, x, y));
		}
	}
}

void simpleimg(clear)(Simpleimg() *img,
	uint32_t x1, uint32_t y1, uint32_t w, uint32_t h
) {
	uint32_t x2 = x1 + w;
	uint32_t y2 = y1 + h;
	uint32_t offset = img->width * 4;
	uint32_t copywidth = w * 4;
	uint8_t *p = simpleimg(offset)(img, x1, y1);
	assert(x2 <= img->width);
	assert(y2 <= img->height);
	for (size_t i = y1; i < y2; i += 1) {
		memset(p, 0, copywidth);
		p += offset;
	}
}

void simpleimg(clearall)(Simpleimg() *img) {
	simpleimg(clear)(img, 0, 0, img->width, img->height);
}

void simpleimg(paste)(Simpleimg() *src, Simpleimg() *dst,
	uint32_t w, uint32_t h,
	uint32_t sx1, uint32_t sy1,
	uint32_t dx1, uint32_t dy1
) {
	size_t soffset = (size_t)src->width * 4;
	size_t doffset = (size_t)dst->width * 4;
	size_t copywidth = (size_t)w * 4;

	uint32_t sx2 = w + sx1; assert(sx2 <= src->width);
	uint32_t sy2 = h + sy1; assert(sy2 <= src->height);
	uint32_t dx2 = w + dx1; assert(dx2 <= dst->width);
	uint32_t dy2 = h + dy1; assert(dy2 <= dst->height);

	uint8_t *ps = simpleimg(offset)(src, sx1, sy1);
	uint8_t *pd = simpleimg(offset)(dst, dx1, dy1);
	for (uint32_t sy = sy1; sy < sy2; sy += 1) {
		memcpy(pd, ps, copywidth);
		ps += soffset;
		pd += doffset;
	}
}

uint8_t *simpleimg(offset)(Simpleimg() *img, uint32_t x, uint32_t y) {
	return img->data + (img->width * y + x) * 4;
}

#endif
