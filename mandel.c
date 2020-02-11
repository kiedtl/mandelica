#include <stdio.h>
#include "types.h"

int
main(int argc, char **argv)
{
	usize ctr = 0;
	float i, j, r;

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

			putchar(62 - ctr);
		}

		putchar(10); /* newline */
	}
}
