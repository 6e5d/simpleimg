#include <stb/stb_image.h>

#include "../include/simpleimg.h"

static uint8_t c4to256(uint8_t* color) {
	uint8_t result = 0;
	for (int idx = 2; idx >= 0; idx -= 1) {
		result *= 6;
		uint16_t c = color[idx];
		result += (uint8_t)(c * 6 / 256);
	}
	return result + 16;
}

void simpleimg_print(Simpleimg* img) {
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

void simpleimg_new(Simpleimg* result, uint32_t w, uint32_t h) {
	result->data = calloc(w * h * 4, sizeof(uint8_t));
	result->width = w;
	result->height = h;
}

void simpleimg_load(Simpleimg* result, char* path) {
	int w, h, c;
	result->data = stbi_load(
		path, &w, &h, &c, STBI_rgb_alpha
	);
	assert(result->data != NULL);
	// RGBA -> BGRA
	for (size_t i = 0; i < (size_t)h * (size_t)w; i++) {
		uint8_t tmp = result->data[i * 4];
		result->data[i * 4] = result->data[i * 4 + 2];
		result->data[i * 4 + 2] = tmp;
	}
	result->width = (uint32_t)w;
	result->height = (uint32_t)h;
}

void simpleimg_deinit(Simpleimg* simpleimg) {
	free(simpleimg->data);
}

void simpleimg_clear(Simpleimg *img,
	uint32_t x1, uint32_t y1, uint32_t w, uint32_t h
) {
	uint32_t x2 = x1 + w;
	uint32_t y2 = y1 + h;
	uint32_t offset = img->width * 4;
	uint32_t copywidth = w * 4;
	uint8_t *p = simpleimg_offset(img, x1, y1);
	assert(x2 <= img->width);
	assert(y2 <= img->height);
	for (size_t i = y1; i < y2; i += 1) {
		memset(p, 0, copywidth);
		p += offset;
	}
}

void simpleimg_paste(Simpleimg *src, Simpleimg *dst,
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

	uint8_t *ps = simpleimg_offset(src, sx1, sy1);
	uint8_t *pd = simpleimg_offset(dst, dx1, dy1);
	for (uint32_t sy = sy1; sy < sy2; sy += 1) {
		memcpy(pd, ps, copywidth);
		*ps += soffset;
		*pd += doffset;
	}
}
