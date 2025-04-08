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

size_t max_u8_strlen(char *s1, char *s2) {
	size_t s1w = u8_strwidth((uint8_t*)s1, "");
	size_t s2w = u8_strwidth((uint8_t*)s2, "");
	return MAX(s1w, s2w);
}

size_t max_strlen(char *s1, char *s2) {
	size_t s1l = strlen(s1);
	size_t s2l = strlen(s2);
	return MAX(s1l, s2l);
}
