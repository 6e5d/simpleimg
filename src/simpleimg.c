#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
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

// paste whole src to offset of dst
void simpleimg_paste(Simpleimg *src, Simpleimg *dst, uint32_t x, uint32_t y) {
	uint32_t x2 = x + src->width;
	uint32_t y2 = y + src->height;
	assert(x2 <= dst->width);
	assert(y2 <= dst->height);
	for (uint32_t iy = 0; iy < src->height; iy += 1) {
		uint8_t *psrc, *pdst;
		psrc = simpleimg_offset(src, 0, iy);
		pdst = simpleimg_offset(dst, 0, iy + y);
		memcpy(pdst, psrc, 4 * src->width);
	}
}
