#ifndef _TC_H
#define _TC_H

typedef struct {
	COLOR bg;
	COLOR fg;
} COLORP;

#define COLORP_C(BG, FG) (COLORP){.bg = BG, .fg = FG}

typedef struct {
	union {
		COLORP p;
		struct {
			COLOR bg;
			COLOR fg;
		};	
	};
	char *str;
} COLSTR;

#define COLSTR_C(P, STR) (COLSTR){.p = P, .str = STR}

// UNIX 16 colors
typedef enum {
	NONE, RED, GREEN, YELLOW,
	BLUE, MAGENTA, CYAN, WHITE,
	GRAY, BRIGHT_RED, BRIGHT_GREEN,
	BRIGHT_YELLOW, BRIGHT_BLUE,
	BRIGHT_MAGENTA, BRIGHT_CYAN,
	BRIGHT_WHITE, BLACK
} COLOR4;

#define FG 38
#define BG 48

#define C8_F "\x1b[%d;5;%dm"
#define C8_FMAX 11
#define C24_F "\x1b[%d;2;%d;%d;%dm"
#define C24_FMAX 19
#define ENDC "\x1b[0m"
#define ENDCMAX 4

#define HIDE_CUR() printf("\x1b[?25l"); // hide cursor
#define SHOW_CUR() printf("\x1b[?25h"); // show cursor
#define CLRSCR() printf("\x1b[2J"); // erase screen
#define RESET_CUR() printf("\x1b[H"); // move cursor to 0 0
#define MOVE_CUR(LINE, COL) printf("\x1b[%d;%dH", LINE, COL); // move cursor

#define printcbf(COL, FORMAT, ...) printf(BG, COL, FORMAT, __VA_ARGS__)
#define printcff(COL, FORMAT, ...) printf(FG, COL, FORMAT, __VA_ARGS__)

// UNIX terminal resolution
void term_resolution(uint16_t*, uint16_t*);

#ifdef _TERMIOS_H
void set_raw_mode(uint8_t, uint8_t);
void reset_mode(struct termios*);
#endif

#ifdef _COLOR_H
// PRINT COLORED FORMAT
void printcf(uint8_t, COLOR, const char*, ...);
int sprintcf(char *, uint8_t, COLOR, const char *, ...);
int snprintcf(char*, size_t, uint8_t, COLOR, const char*, ...);
// PRINT COLOR PAIR
void printcpf(COLORP, const char *,  ...);
int sprintcpf(char *, COLORP, const char *, ...);
int snprintcpf(char *, size_t, COLORP, const char *, ...);
#endif

#endif
