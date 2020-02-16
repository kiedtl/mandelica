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
version(void *data, char **pars, const int pars_count)
{
	fprintf(stdout, "mandelica v0.0.1\n");
	exit(0);
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

void
help(void *data, char **pars, const int pars_count)
{
	printf("usage: mandelica [FLAGS] [OPTIONS]\n");
	printf("\nFLAGS:\n");
	printf("    -v    --verbose            print verbose information.\n");
	printf("    -V    --version            print version and exit.\n");
	printf("          --help               print this help message and exit.\n");
	printf("\nOPTIONS:\n");
	printf("    -i    --iters    [ITERS]   level of detail of resulting set. (default: 100)\n");
	printf("    -w    --width    [WIDTH]   width of resulting image. (default: 800)\n");
	printf("    -h    --height   [HEIGHT]  height of resulting image. (default: 650)\n");
	printf("          --leftbdr  [VALUE]   minimum real of resulting set. (default: -2.0)\n");
	printf("          --rightbdr [VALUE]   maximum real of resulting set. (default: 1.0)\n");
	printf("          --downbdr  [VALUE]   minimum imaginary of resulting set. (default: -1.2)\n");
	printf("          --topbdr   [VALUE]   maximum imaginary of resulting set. (default: 1.2375)\n");
	printf("\nReport bugs to https://github.com/lptstr/mandelica.\n");
	exit(0);
}
