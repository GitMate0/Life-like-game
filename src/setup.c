#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uniwidth.h>
#include <getopt.h>
#include <stdint.h>
#include <ctype.h>

#include "../include/extra.h"
#include "../include/color.h"
#include "../include/tc.h"
#include "../include/gol.h"
#include "../include/setup.h"

void gol_map_setup(GOL_STATE *state) {
	uint16_t cols, lines;
	term_resolution(&cols, &lines);
	size_t s1w = u8_strwidth((uint8_t*)state->live.str, ""); // true char len live cell string
	size_t s2w = u8_strwidth((uint8_t*)state->dead.str, ""); // true char len dead cell string
	state->cell_scale = MAX(s1w, s2w); // map width divider or max char len cell string
	gol_map_init(&state->map, cols / state->cell_scale, lines - 1);
	gol_map_rand(state->map, state->seed);
}

void state_opt_setcolor(char *optarg, GOL_STATE *state, uint8_t cell_state, uint8_t color_type) {
	state->color = 1;
	if (strstr(optarg, "rainbow")) {
		BIT_SET(state->rainbow, cell_state * 2 + color_type);
		return;
	}
	uint32_t code;
	uint8_t r, g, b;
	if (optarg[0] == '#' && strsat(optarg + 1, isxdigit)) {
		code = str_to_hex(optarg);
	} else if (str_to_rgb(optarg, ";", &r, &g, &b) != -1) {
		code = rgb_to_hex(r, g, b);
	} else {
		return;
	}
	
	COLSTR *cstrptr;
	if (cell_state == LIVE) {
		cstrptr = &state->live;
	} else if (cell_state == DEAD) {
		cstrptr = &state->dead;
	} else {
		return;
	}
	
	if (color_type == BG_T) {
		cstrptr->bg = COLOR_C(code);
	} else if (color_type == FG_T) {
		cstrptr->fg = COLOR_C(code);
	}
}

void gol_opt_parse(GOL_STATE *state, int argc, char **argv) {
	struct option options[] = {
		{"birth",         required_argument, NULL, 'b'},
		{"survival",      required_argument, NULL, 's'},
		{"edge-wrapping", required_argument, NULL, 'w'},
		{"live-string",   required_argument, NULL, 'l'},
		{"dead-string",   required_argument, NULL, 'd'},
		{"seed",          required_argument, NULL, 'S'},
		{"live-fg-color", required_argument, NULL, '1'},
		{"dead-fg-color", required_argument, NULL, '2'},
		{"live-bg-color", required_argument, NULL, '3'},
		{"dead-bg-color", required_argument, NULL, '4'},
		{"help",          no_argument,       NULL, 'h'},
		{0, 0, 0, 0}
	};
	
	int opt, optindex;
	while ((opt = getopt_long(argc, argv, "b:s:w:l:d:S:1:2:3:4:h", options, &optindex)) != -1) {
		switch (opt) {
			case 'b':
				state->birth_rule = optarg;
				break;
			case 's':
				state->survival_rule = optarg;
				break;
			case 'w':
				state->edge_wrapping = strtol(optarg, NULL, 10);
				break;
			case 'l':
				state->live.str = malloc(CELLSTRMAX);
				strncpy(state->live.str, optarg, CELLSTRMAX);
				state->live.str[CELLSTRMAX - 1] = '\0';
				break;
			case 'd':
				state->dead.str = malloc(CELLSTRMAX);
				strncpy(state->dead.str, optarg, CELLSTRMAX);
				state->dead.str[CELLSTRMAX - 1] = '\0';
				break;
			case 'S':
				state->seed = strtol(optarg, NULL, 10);
				break;
			case '1':
				state_opt_setcolor(optarg, state, LIVE, FG_T);
				break;
			case '2':
				state_opt_setcolor(optarg, state, DEAD, FG_T);
				break;
			case '3':
				state_opt_setcolor(optarg, state, LIVE, BG_T);
				break;
			case '4':
				state_opt_setcolor(optarg, state, DEAD, BG_T);
				break;
			case 'h':
				printf("Usage: ...\nOptions:\n"
				       "  -b, --birth RULE       Birth rule string\n"
				       "  -s, --survival RULE    Survival rule string\n"
				       "  -w, --edge-wrapping [0|1]\n"
				       "  -l, --live-string STR  Character(s) for live cell\n"
				       "  -d, --dead-string STR  Character(s) for dead cell\n"
				       "  -S, --seed NUM         Random seed\n"
				       "  -1  --live-fg-color    Live foreground color\n"
				       "  -2  --dead-fg-color    Dead foreground color\n"
				       "  -3  --live-bg-color    Live background color\n"
				       "  -4  --dead-bg-color    Dead background color\n"
				       "  color=[#hex|r;g;b|rainbow]\n");
				exit(0);
				break;
			default:
				fprintf(stderr, "Unknown option: %s\n", argv[optind - 1]);
				break;
		}
	}
}

void gol_setup(GOL_STATE *state, int argc, char **argv) {
	// setup game state from shell options
	gol_opt_parse(state, argc, argv);
	size_t s1l = strlen(state->live.str);
	size_t s2l = strlen(state->dead.str);
	state->cell_bytes = MAX(s1l, s2l) + 1; // max byte size of cell string
	gol_map_setup(state); // set map
	// set screen buffer
	size_t colored_cell_string_size = state->cell_bytes + 2 * C24_FMAX + ENDCMAX;
	size_t buffer_size;
	if (state->color) {
		buffer_size = (state->map.w * colored_cell_string_size + 1) * state->map.h + 1;
	} else {
		buffer_size = (state->map.w * state->cell_bytes + 1) * state->map.h + 1;
	}
	state->screen_buffer = (char*)malloc(buffer_size);
}

