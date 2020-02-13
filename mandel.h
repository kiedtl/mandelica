#ifndef MANDEL_H
#define MANDEL_H

#include "types.h"

typedef struct Options {
	u32 height;
	u32 width;
	u32 iter;

	bool verbose;
} Options;

struct Options *opts;

#endif
