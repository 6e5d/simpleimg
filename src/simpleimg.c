#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stb/stb_image.h>

#include "../include/simpleimg.h"
#include "../../wholefile/include/wholefile.h"

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

Simpleimg* simpleimg_load(char* path) {
	Simpleimg* result = malloc(sizeof(Simpleimg));
	uint8_t *buf;
	size_t len = wholefile_read(path, &buf);
	int w, h, c;
	result->data = stbi_load_from_memory(
		buf, (int)len, &w, &h, &c, STBI_rgb_alpha
	);
	// RGBA -> BGRA
	for (size_t i = 0; i < h * w; i++) {
		uint8_t tmp = result->data[i * 4];
		result->data[i * 4] = result->data[i * 4 + 2];
		result->data[i * 4 + 2] = tmp;
	}
	result->width = (uint32_t)w;
	result->height = (uint32_t)h;
	return result;
}


void simpleimg_destroy(Simpleimg* simpleimg) {
	free(simpleimg->data);
	free(simpleimg);
}
