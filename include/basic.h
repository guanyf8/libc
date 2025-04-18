#ifndef __MACRO_H__
#define __MACRO_H__

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#if defined(__clang__)
	#define typeof __typeof__
#endif

union key_type {
	int num;
	char* str;
};

extern int mystrcmp(union key_type a, union key_type b);
extern int numcmp(union key_type a, union key_type b);

#define container_of(ptr, type, member) ({                   \
	const typeof( ((type *)0)->member ) *__mptr = (ptr); \
	(type *)( (char *)__mptr - offsetof(type,member) );})

static inline void prefetch(const void *x) {;}

#define new(type) (type*)malloc(sizeof(type))
#define new_array(type,size) (type*)malloc(sizeof(type)*size)
#define deep_copy(src) ({strcpy(malloc(strlen(src)+1),src);})

#endif