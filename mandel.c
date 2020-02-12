#include <arpa/inet.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"

int
main(int argc, char **argv)
{
	u32 height = 31;
	u32 width  = 80;

	usize ctr = 0;
	float i, j, r;

	/* initialize farbfeld stuff */
	u32 buf;
	fputs("farbfeld", stdout);                   /* magic value */
	buf = htonl(width);
	fwrite(&buf, sizeof(buf), 1, stdout);        /* width */
	buf = htonl(height);
	fwrite(&buf, sizeof(buf), 1, stdout);        /* height */

	u16 *data = realloc(NULL, width * ((sizeof("RGBA") - 1) * sizeof(u16)));
	if (data == NULL) {
		fprintf(stderr, "error: realloc: ");
		perror(NULL);
		return 1;
	}

	for (float y = -16; y < 15; ++y) {
		for (float x = 0; x < 84; ++x) {
			for (i = r = ctr = 0;;) {
				j = r * r - i * i - 2 + x / 25;
				i = 2 * r * i + y / 10;

				r = j;
				++ctr;

				if (j * j + i * i >= 11 || ctr >= 30)
					break;
			}

			//putchar(62 - ctr);
			u16 color = 0xffff;
			if (ctr == 30) color = 0x0000;
			data[4 * (size_t) x + 0] = htons(color);
			data[4 * (size_t) x + 1] = htons(color);
			data[4 * (size_t) x + 2] = htons(color);
			data[4 * (size_t) x + 3] = htons(0xffff);
		}

		fwrite(data, sizeof(u16), width * 4, stdout);
	}
}
