#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../include/extra.h"
#include "../include/color.h"
#include "../include/tc.h"
#include "../include/gol.h"

// map initialization
void gol_map_init(GOL_MAP *map, uint16_t w, uint16_t h) {
	map->bw = w / 8 + BOOL(w % 8); // number of bytes by width
	map->w = w; // number of bits by width
	map->h = h; // number of bytes by height
	map->size = map->bw * h; // total number of bytes
	map->chunks = (uint8_t*)calloc(map->size, sizeof(uint8_t)); // current map chunks
	map->old_chunks = (uint8_t*)calloc(map->size, sizeof(uint8_t)); // old map chunks
}

// cell insertion into map
uint8_t _gol_map_set(GOL_MAP map, uint8_t v, uint16_t x, uint16_t y) {
    if (x >= map.w || y >= map.h) return 1; // border check
    uint32_t index = y * map.bw + x / 8; // byte index in array
    uint8_t *chunks = (v) ? map.chunks : map.old_chunks;
    BIT_FLIP(chunks[index], x % 8);
    return 0;
}

// getting a cell state from a map
uint8_t _gol_map_get(GOL_MAP map, uint8_t v, uint16_t x, uint16_t y) {
    if (x >= map.w || y >= map.h) return 0; // border check
    uint32_t index = y * map.bw + x / 8; // byte index in array
    uint8_t *chunks = (v) ? map.chunks : map.old_chunks;
    return BIT_GET(chunks[index], x % 8);
}

// getting number of neigbors of cell
uint8_t _gol_near(GOL_STATE state, uint8_t v, uint16_t x, uint16_t y) {
	uint8_t nn = 0; // number of neighbors
	for (char vo = -1; vo <= 1; vo++) { // vertical offset
		for (char ho = -1; ho <= 1; ho++) { // horisontal offset
			if (vo == 0 && ho == 0) continue; // skip main cell
			
			uint16_t nx, ny;
			if (state.edge_wrapping) { // map edge wrap around
				nx = (x + ho + state.map.w) % state.map.w; // neighbor x position
				ny = (y + vo + state.map.h) % state.map.h; // neighbor y position
			}
			else {
				nx = x + ho; // neighbor x position
				ny = y + vo; // neighbor y position
			}
			// if neighbor position goes beyond the map - skip
			if (nx >= state.map.w || ny >= state.map.h) continue;
			// add the bit value to the list of the number of neighbors
			nn += _gol_map_get(state.map, v, nx, ny);
		}
	}
	return nn;
}

// randomize map
void gol_map_rand(GOL_MAP map, uint32_t seed) {
	srand(seed);
	for (size_t i = 0; i < map.size; i++) {
		map.chunks[i] = rand() % 256;
	}
}

// clean map
void gol_map_clean(GOL_MAP map) {
	for (size_t i = 0; i < map.size; i++) {
		map.chunks[i] = 0;
	}
}

static float phase = 0; // phase of color animation
static float speed = 0.1;

// render map
void gol_map_render(GOL_STATE state) {
	char *ptr = state.screen_buffer;
	for (uint16_t i = 0; i < state.map.h; i++) {
		for (uint16_t j = 0; j < state.map.w; j++) {
			
			uint8_t cstate = gol_map_get(state.map, j, i); // cell state
			char *cell = (cstate) ? state.live.str : state.dead.str; // cell string
			
			int rbit_bg = cstate * 2; // bit index for background cell rainbow color
			int rbit_fg = cstate * 2 + 1; // bit index for foreground cell rainbow color
			int pos = i + j; // row color position
			
			if (state.color) { // if color enable
				COLOR bg = (BIT_GET(state.rainbow, rbit_bg)) ? // if rainbow background color enable
					rainbow_effect(phase, pos, 0.1) : 
					(cstate == LIVE) ? // else if cell alive
						state.live.bg : 
						state.dead.bg;
				
				COLOR fg = (BIT_GET(state.rainbow, rbit_fg)) ? // if rainbow background color enable
					rainbow_effect(phase, pos, 0.1) : 
					(cstate == LIVE) ? // else if cell alive
						state.live.fg : 
						state.dead.fg;
						
				size_t remain = state.cell_bytes + 2 * C24_FMAX + ENDCMAX; // size of memory for one colored cell string element
				ptr += snprintcpf(ptr, remain, COLORP_C(bg, fg), "%s", cell); // write colored cell string element to screen buffer
			} else {
				ptr += snprintf(ptr, state.cell_bytes, "%s", cell); // write uncolored cell string element to screen buffer
			}
		}
		*ptr++ = '\n'; // write row carring
	}
	*ptr++ = '\0'; // write end of string

	if (state.rainbow) phase += speed; // color animation step
	printf("%s", state.screen_buffer); // print screen buffer
	fflush(stdout);
}

// next step of cellular automaton
void gol_next(GOL_STATE state) {
	memcpy(state.map.old_chunks, state.map.chunks, state.map.size); // copy current map chunks to old map buffer
	memset(state.map.chunks, 0, state.map.size); // clean current map

	for (uint16_t i = 0; i < state.map.h; i++) {
		for (uint16_t j = 0; j < state.map.w; j++) {
			uint8_t nn = gol_onear(state, j, i); // number of neighbors
			uint8_t cell = gol_omap_get(state.map, j, i); // main cell bit value

			// if the cell is alive and number of neighbors satisfies survival rules
			if (cell && cins(nn + '0', state.survival_rule)) {
				gol_map_set(state.map, j, i);
			}
			// if the cell is dead and number of neighbors satisfies birth rules
			else if (!cell && cins(nn + '0', state.birth_rule)) {
				gol_map_set(state.map, j, i);
			}
		}
	}
}
