/*
 * winsock.h on windows,
 * arpa/inet.h on unix
 * for htons()
 */
#if defined(_WIN32) || defined(__WIN32__)
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif

#include <math.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"

float clamp(float value);

int
main(int argc, char **argv)
{
	u32 height = 1000;
	u32 width  = 2000;
	u64 total  = height * width;
	u32 iter   = 100;

	/* column (x) counter */
	usize pxctr = 0;

	//double minre   = -2.0;
	//double maxre   =  1.0;
	//double minim   = -1.2;
	//double maxim   = minim + (maxre - minre) * height / width;
	//double re_fact = (maxre - minre) / (width - 1);
	//double im_fact = (maxim - maxim) / (height - 1);

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

		for (usize x = 0; x < width; ++x, ++pxctr) {
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

			/* get color based on iterations */
			float quotient = ((float) ctr) / ((float) iter);
			u16 color = (u16) (quotient * (float) 0xffff);
			
			//DEBUG("quotient = %f\tcolor = %i\n", quotient, color);

			/* set pixel */
			if (ctr != iter) {
				if (ctr > (iter / 2)) {
					data[4 * x + 0] = htons(color);     /* red */
					data[4 * x + 1] = htons(0xffff);    /* green */
					data[4 * x + 2] = htons(color);     /* blue */
				} else {
					data[4 * x + 0] = htons(0x0000);    /* red */
					data[4 * x + 1] = htons(color);     /* green */
					data[4 * x + 2] = htons(0x0000);    /* blue */
				}
			} else {
				data[4 * x + 0] = htons(0x0000);    /* red */
				data[4 * x + 1] = htons(0x0000);    /* green */
				data[4 * x + 2] = htons(0x0000);    /* blue */
			}

			data[4 * x + 3] = htons(0xffff);   /* alpha */
			
			//fprintf(stdout, "%c[%i;%iH%c", 27, y, x, 62 - ctr);

			if (pxctr % 2048 == 0) {
				fprintf(stderr,
					"%c[smandel: on pixel %i/%i, row %i/%i, %f%% done.%c[u",
					27, pxctr, total, y, height,
					((double) pxctr) / ((double) total) * 100, 27);
			}
		}

		/* print row for image */
		fwrite(data, sizeof(u16), width * 4, stdout);
	}
}

float
clamp(float value)
{
	if (value >= 1.f) return 1.f;
	else if (value <= 0.f) return 0.f;
	else return value;
}
