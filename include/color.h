#ifndef _COLOR_H
#define _COLOR_H

typedef union {
	struct {
		uint8_t b, g, r;
	};
	uint32_t code;
} COLOR;

#define COLOR_RGB(R, G, B) (COLOR){.r=R, .g=G, .b=B}
#define COLOR_C(HEX) (COLOR){.code=HEX}

// Convert RGB to hex color format
uint32_t rgb_to_hex(uint8_t, uint8_t, uint8_t);

// Convert hex color to RGB
void hex_to_rgb(uint32_t, uint8_t*, uint8_t*, uint8_t*);

int str_to_rgb(char*, char*, uint8_t*, uint8_t*, uint8_t*);

uint32_t str_to_hex(char*);

// Linear interpolation between two colors
COLOR interpolate_color(COLOR, COLOR, float);

COLOR shimmer_effect(COLOR, COLOR, float, uint32_t, uint32_t);

void rainbow_rgb(float, uint8_t*, uint8_t*, uint8_t*);

COLOR rainbow_color(float);

COLOR rainbow_effect(float, uint32_t, float);

#endif
