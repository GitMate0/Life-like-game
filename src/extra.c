#include <stdint.h>
#include <string.h>
#include <uniwidth.h>

#include "../include/extra.h"

// character contain in string? char in string
int cins(char c, char *str) {
	for (uint32_t i = 0; str[i] != '\0'; i++) {
		if (str[i] == c) {
			return 1;
		}
	}
	return 0;
}

int strsat(char *str, int (*func)(int)) {
	int len = strlen(str);
	for (int i = 0; i < len; i++) {
		if (!func(str[i])) {
			return 0;
		}
	}
	return 1;
}
