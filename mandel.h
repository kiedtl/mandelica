#ifndef MANDEL_H
#define MANDEL_H

#include "types.h"

typedef struct Options {
	u32 height;
	u32 width;
	u32 iter;

	float minre;
	float maxre;
	float minim;
	float maxim;

	bool verbose;
} Options;

struct Options *opts;

#endif
