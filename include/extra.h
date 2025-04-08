#ifndef _EXTRA_H
#define _EXTRA_H

#define _MIN(A, B) ((A) < (B)) ? (A) : (B)
#define _MAX(A, B) ((A) > (B)) ? (A) : (B)
#define MAX(A, B) _MAX(A, B)
#define MIN(A, B) _MIN(A, B)

#define BIT_GET(bytes, n) (((bytes) >> (n)) & 1)
#define BIT_SET(bytes, n) ((bytes) |= (1 << (n)))
#define BIT_FLIP(bytes, n) ((bytes) ^= (1 << (n)))
#define BOOL(v) !!(v)

// character contain in string?
int cins(char, char *);
// string satisfies the condition?
int strsat(char *, int (*)(int));

size_t max_u8_strlen(char*, char*);

size_t max_strlen(char*, char*);

#endif
