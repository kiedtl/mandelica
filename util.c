#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "util.h"

float
clamp(float value)
{
	if (value >= 1.f) return 1.f;
	else if (value <= 0.f) return 0.f;
	else return value;
}

u32
parse_uint(char *src)
{
	u32 buf = 0;

	for (; *src; ++src) {
		if (*src >= '0' && *src <= '9')
			buf = (buf * 10) + (*src - '0');
	}

	return buf;
}

void
handle_u32(void *data, char **pars, const int pars_count)
{
	*((u32*) data) = parse_uint(pars[0]);
}

void
handle_float(void *data, char **pars, const int pars_count)
{
	*((float*) data) = strtof(pars[0], NULL);
}

void
handle_bool(void *data, char **pars, const int pars_count)
{
	*((bool*) data) = TRUE;
}
