#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <termios.h>

#include "../include/color.h"
#include "../include/tc.h"
#include "../include/gol.h"
#include "../include/setup.h"
#include "../include/extra.h"

uint8_t LOOP = 1, PAUSE = 0;
uint32_t DELAY = 50000;
uint16_t COLS, LINES;

struct termios original;

void end_loop() {
	LOOP = 0;
}

int main(int argc, char **argv) {
	term_resolution(&COLS, &LINES);
	// default setup
	GOL_STATE state = {
		.seed = (uint32_t)time(NULL),
		.birth_rule = "3",
		.survival_rule = "23",
		.edge_wrapping = 1,
		.live = COLSTR_C(
			COLORP_C(
				COLOR_C(0x000000), 
				COLOR_C(0xffffff)
			), 
			"[]"
		),
		.dead = COLSTR_C(
			COLORP_C(
				COLOR_C(0x000000), 
				COLOR_C(0xffffff)
			), 
			"  "
		),
		.cell_bytes = 3,
		.rainbow = 0,
		.color = 0,
	};
	gol_setup(&state, argc, argv); // setup game by shell arguments
	signal(SIGINT, end_loop); // interruption catching
	
	tcgetattr(STDIN_FILENO, &original);
	set_raw_mode(0, 0);
	uint16_t cur_map_x = state.map.w / 2;
	uint16_t cur_map_y = state.map.h / 2;
	uint16_t cur_x = cur_map_x * state.cell_scale + 1;
	uint16_t cur_y = cur_map_y;
	
	//HIDE_CUR();
	CLRSCR();
	char key;
	char info = 1;
	uint64_t gen = 0;
	while (LOOP) {
		//if (!PAUSE) {
			RESET_CUR();
			gol_map_render(state);
		//}
		if (read(STDIN_FILENO, &key, 1)) {
			switch (key) {
				// game edit
				case 'q':
					LOOP = 0;
					break;
				case 'p':
					PAUSE ^= 1;
					break;
				case 'i':
					info ^= 1;
					break;
				case '-':
					if (DELAY != 0)
						DELAY -= 5000;
					break;
				case '+':
					if (DELAY != 200000)
						DELAY += 5000;
					break;
				case 'r':
					gen = 0;
					state.seed = (uint32_t)time(NULL);
					gol_map_rand(state.map, state.seed);
					break;
				case 'c':
					gen = 0;
					state.seed = 0;
					gol_map_clean(state.map);
					break;
				// map edit
				case 'w':
					cur_map_y = MAX(cur_map_y - 1, 0);
					cur_y = cur_map_y + 1;
					break;
				case 's':
					cur_map_y = MIN(cur_map_y + 1, state.map.h - 1);
					cur_y = cur_map_y + 1;
					break;
				case 'a':
					cur_map_x = MAX(cur_map_x - 1, 0);
					cur_x = cur_map_x * state.cell_scale + 1;
					break;
				case 'd':
					cur_map_x = MIN(cur_map_x + 1, state.map.w - 1);
					cur_x = cur_map_x * state.cell_scale + 1;
					break;
				case ' ':
					gol_map_set(state.map, cur_map_x, cur_map_y);
					RESET_CUR();
					gol_map_render(state);
					break;
			}
		}
		RESET_CUR();
		if (info) {
			printcf(BG, COLOR_C(0x0),
			"SEED: %d\n"
			"GENERATION: %d\n"
			"STATE: %s\n"
			"CURSOR:\n"
			"X - %d\n"
			"Y - %d\n"
			"DELAY - %d sec",
			state.seed, gen,
			PAUSE ? "PAUSE" : "RUN",
			cur_map_x, cur_map_y, DELAY);
		}
		
		MOVE_CUR(cur_y, cur_x);
		fflush(stdout);
		
		if (!PAUSE) {
			gol_next(state);
			gen++;
		}
		usleep(DELAY);
	}
	//SHOW_CUR();
	CLRSCR();
	RESET_CUR();
	reset_mode(&original);

	// stats
	printf("seed: %d\n", state.seed);
	printf("resolution: %dx%d ch\n", COLS, LINES);
	printf("map size: %dx%d\n", state.map.w, state.map.h);
	printf("memory allocation:\n"
		   "- map (old and current buffer): \t%d bytes\n"
		   "- screen buffer: \t\t\t%ld bytes\n",
	state.map.size * 2, strlen(state.screen_buffer));

	// memory free
	free(state.screen_buffer);
	free(state.map.old_chunks);
	free(state.map.chunks);
	
	return 0;
}
