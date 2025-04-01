#ifndef __MACRO_H__
#define __MACRO_H__

#include <stddef.h>
#include <stdlib.h>

#if defined(__clang__)
#define typeof __typeof__
#endif

#define container_of(ptr, type, member) ({                   \
	const typeof( ((type *)0)->member ) *__mptr = (ptr); \
	(type *)( (char *)__mptr - offsetof(type,member) );})



#define new(type) (type*)malloc(sizeof(type))

#endif