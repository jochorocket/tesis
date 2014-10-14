#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdlib.h>

#define HALF_PI 1.5707963267948966

typedef unsigned char byte;

typedef struct _byteList
{
	byte *list;
	int length;
} byteList;

#endif