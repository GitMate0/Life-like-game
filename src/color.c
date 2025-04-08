#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <inttypes.h>
#include <ctype.h>

#include "../include/extra.h"
#include "../include/color.h"

// Convert RGB to hex color format
uint32_t rgb_to_hex(uint8_t r, uint8_t g, uint8_t b) {
	uint32_t hex = 0;
	hex |= (r << 16);
	hex |= (g << 8 );
	hex |= (b << 0 );
	return hex;
}

// Convert hex color to RGB
void hex_to_rgb(uint32_t color, uint8_t *r, uint8_t *g, uint8_t *b) {
	*r = (color >> 16) & 0xFF;
	*g = (color >> 8 ) & 0xFF;
	*b = (color >> 0 ) & 0xFF;
}

int str_to_rgb(char *str, char *sep, uint8_t *r, uint8_t *g, uint8_t *b) {
	char buf[32];
	strncpy(buf, str, sizeof(buf));
	buf[sizeof(buf) - 1] = '\0';

	char *token = strtok(buf, sep);
	if (!token || sscanf(token, SCNu8, r) != 1) return -1;

	token = strtok(NULL, sep);
	if (!token || sscanf(token, SCNu8, g) != 1) return -1;

	token = strtok(NULL, sep);
	if (!token || sscanf(token, SCNu8, b) != 1) return -1;

	return 0;
}

uint32_t str_to_hex(char *str) {
	if (str[0] == '#' && strlen(str + 1) == 6 && strsat(str + 1, isxdigit)) {
		return (uint32_t)strtol(str + 1, NULL, 16);
	}
	return 0;
}

// Linear interpolation between two colors
COLOR interpolate_color(COLOR start, COLOR end, float factor) {
    COLOR result;
    result.r = (uint8_t)(start.r + factor * (end.r - start.r));
    result.g = (uint8_t)(start.g + factor * (end.g - start.g));
    result.b = (uint8_t)(start.b + factor * (end.b - start.b));
    return result;
}

COLOR shimmer_effect(COLOR start, COLOR stop, float offset, uint32_t wave, uint32_t step) {
	float factor = (1 + sin((float)step / wave * 2 * M_PI + offset)) / 2;
	COLOR color = interpolate_color(start, stop, factor);
	return color;
}

void rainbow_rgb(float pos, uint8_t *r, uint8_t *g, uint8_t *b) {
    *r = (uint8_t)(sin(pos) * 127 + 128);
    *g = (uint8_t)(sin(pos + 2 * M_PI / 3) * 127 + 128);
    *b = (uint8_t)(sin(pos + 4 * M_PI / 3) * 127 + 128);
}

COLOR rainbow_color(float pos) {
	COLOR color;
    rainbow_rgb(pos, &color.r, &color.g, &color.b);
    return color;
}

COLOR rainbow_effect(float phase, uint32_t step, float offset) {
	return rainbow_color(phase + step * offset);
}
