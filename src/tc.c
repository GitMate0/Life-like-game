#include <stdio.h>
#include <stdarg.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <stdint.h>

//#include "xmalloc.h"
#include "../include/extra.h"
#include "../include/color.h"
#include "../include/tc.h"

// UNIX terminal resolution
void term_resolution(uint16_t *cols, uint16_t *lines) {
	struct winsize ws;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws)) {
		perror("ioctl");
		return;
	}
	*cols = ws.ws_col;
	*lines = ws.ws_row;
}

void set_raw_mode(uint8_t minc, uint8_t delay) {
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~(ICANON | ECHO);
    t.c_cc[VMIN] = minc;
    t.c_cc[VTIME] = delay;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void reset_mode(struct termios *original) {
	if (original == NULL) return;
    tcsetattr(STDIN_FILENO, TCSANOW, original);
}

void printcf(uint8_t mode, COLOR color, const char *format, ...) {
    if (mode != BG && mode != FG) mode = FG;
    printf(C24_F, mode, color.r, color.g, color.b);
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf(ENDC);
}

int snprintcf(char *buf, size_t n, uint8_t mode, COLOR color, const char *format, ...) {
    if (mode != BG && mode != FG) mode = FG;
    int written = snprintf(buf, n, C24_F, mode, color.r, color.g, color.b);
    va_list args;
    va_start(args, format);
    written += vsnprintf(buf + written, n - written, format, args);
    va_end(args);
    written += snprintf(buf + written, n - written, ENDC);
    return written;
}

void printcpf(COLORP tcpair, const char *format, ...) {    
    printf(C24_F, BG, tcpair.bg.r, tcpair.bg.g, tcpair.bg.b);
    printf(C24_F, FG, tcpair.fg.r, tcpair.fg.g, tcpair.fg.b);
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf(ENDC);
}

int sprintcpf(char *buf,  COLORP tcpair, const char *format, ...) {    
    int written = sprintf(buf, C24_F, BG, tcpair.bg.r, tcpair.bg.g, tcpair.bg.b);
    written += sprintf(buf + written, C24_F, FG, tcpair.fg.r, tcpair.fg.g, tcpair.fg.b);
    va_list args;
    va_start(args, format);
    written += vsprintf(buf + written, format, args);
    va_end(args);
    written += sprintf(buf + written, ENDC);
    return written;
}

int snprintcpf(char *buf, size_t n, COLORP tcpair, const char *format, ...) {    
    int written = snprintf(buf, n, C24_F, BG, tcpair.bg.r, tcpair.bg.g, tcpair.bg.b);
    written += snprintf(buf + written, n - written, C24_F, FG, tcpair.fg.r, tcpair.fg.g, tcpair.fg.b);
    va_list args;
    va_start(args, format);
    written += vsnprintf(buf + written, n - written, format, args);
    va_end(args);
    written += snprintf(buf + written, n - written, ENDC);
    return written;
}
