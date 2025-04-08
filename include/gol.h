#ifndef _GOL_H
#define _GOL_H

#define DEAD 0
#define LIVE 1

typedef struct {
	uint8_t *old_chunks; // old map buffer
	uint8_t *chunks; // current map buffer
	uint32_t size; // map size
	uint16_t bw; // number of bytes
	uint16_t w; // number of bits by width
	uint16_t h; // number of bits by height
} GOL_MAP;

typedef struct {
	// mechanics
	GOL_MAP map;
	uint32_t seed;
	uint8_t edge_wrapping;
	char *birth_rule;
	char *survival_rule;
	// view
	COLSTR live;
	COLSTR dead;
	// screen buffer
	uint8_t cell_bytes;
	char *screen_buffer;
	// switches
	uint8_t color;
	uint8_t rainbow;
} GOL_STATE;

// Old map buffer mark
#define OLD 0
// Current map buffer mark
#define CURRENT 1

// Current map functions
#define gol_map_get(MAP, X, Y) _gol_map_get(MAP, CURRENT, X, Y)
#define gol_map_set(MAP, X, Y) _gol_map_set(MAP, CURRENT, X, Y)
#define gol_near(MAP, X, Y) _gol_near(MAP, CURRENT, X, Y)

// Old map functions
#define gol_omap_get(MAP, X, Y) _gol_map_get(MAP, OLD, X, Y)
#define gol_omap_set(MAP, X, Y) _gol_map_set(MAP, OLD, X, Y)
#define gol_onear(MAP, X, Y) _gol_near(MAP, OLD, X, Y)

// map initialization
void gol_map_init(GOL_MAP*, uint16_t, uint16_t);
// cell insertion into map
uint8_t _gol_map_set(GOL_MAP, uint8_t, uint16_t, uint16_t);
// getting a cell state from a map
uint8_t _gol_map_get(GOL_MAP, uint8_t, uint16_t, uint16_t);
// getting number of neigbors of cell
uint8_t _gol_near(GOL_STATE, uint8_t, uint16_t, uint16_t);
// randomize map
void gol_map_rand(GOL_MAP, uint32_t);
// clean map
void gol_map_clean(GOL_MAP);
// render map
void gol_map_render(GOL_STATE);
// next step of map generation
void gol_next(GOL_STATE);

#endif
