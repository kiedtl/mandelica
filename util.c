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

void
handle_uint(void *data, char **pars, const int pars_count)
{
	u32 buf = 0;
	char *p = pars[0];

	while (*p) {
		if (*p >= '0' && *p <= '9')
			buf = (buf * 10) + (*p - '0');
		++p;
	}

	*((u32*) data) = buf;
}

void
handle_bool(void *data, char **pars, const int pars_count)
{
	*((bool*) data) = TRUE;
}
