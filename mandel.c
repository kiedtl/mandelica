#include <stdio.h>

int
main(int argc, char **argv)
{
	float i, j, r;

	for (float y = -16; y < 15; ++y) {
		for(float x = 0, unsigned int c = 0; x < 84; ++x) {
			i = r = 0;
			for (;;) {
				j = r * r - i * i - 2 + x / 25;
				i = 2 * r * i + y / 10;

				r = j;
				++c;

				if (j * j + i * i >= 11 || k >= 30)
					break;
			}

			putchar(62 - c);
		}

		putchar(10); /* newline */
	}
}
