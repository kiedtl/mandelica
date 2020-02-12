#include <arpa/inet.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"

int
main(int argc, char **argv)
{
	u32 height = 600;
	u32 width  = 800;
	usize iter =  30;

	double minre   = -2.0;
	double maxre   =  1.0;
	double minim   = -1.2;
	double maxim   = minim + (maxre - minre) * height / width;
	double re_fact = (maxre - minre) / (width - 1);
	double im_fact = (maxim - maxim) / (height - 1);

	/* initialize farbfeld stuff */
	u32 buf;
	fputs("farbfeld", stdout);                   /* magic value */
	buf = htonl(width);
	fwrite(&buf, sizeof(buf), 1, stdout);        /* width */
	buf = htonl(height);
	fwrite(&buf, sizeof(buf), 1, stdout);        /* height */

	u16 *data = malloc(width * ((sizeof("RGBA") - 1) * sizeof(u16)));
	if (data == NULL) {
		fprintf(stderr, "error: malloc: ");
		perror(NULL);
		return 1;
	}

	for (usize y = 0; y < height; ++y) {
		double c_im = (y - height / 1.8) * 3.5 / width;

		for (usize x = 0; x < width; ++x) {
			double c_re = (x - width / 1.8) * 3.5 / width;
			double Z_re = c_re, Z_im = c_im;

        		/* number of iterations */
			usize ctr;

			for (ctr = 0; ctr < iter; ++ctr) {
				double Z_re2 = Z_re * Z_re, Z_im2 = Z_im * Z_im;

				if (Z_re2 + Z_im2 > 4) break;

				Z_im = 2 * Z_re * Z_im + c_im;
				Z_re = Z_re2 - Z_im2 + c_re;
			}

			/* set pixel */
			u16 color = 0xffff;// - (ctr * 5000);
			if (ctr == iter) color = 0x0000;

			data[4 * x + 0] = htons(color);
			data[4 * x + 1] = htons(color);
			data[4 * x + 2] = htons(color);
			data[4 * x + 3] = htons(0xffff);
			//fprintf(stdout, "%c[%i;%iH%c", 27, y, x, 62 - ctr);
		}

		/* print row for image */
		fwrite(data, sizeof(u16), width * 4, stdout);
	}
}
