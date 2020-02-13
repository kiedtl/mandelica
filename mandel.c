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
#include "argoat.h"
#include "mandel.h"
#include "util.h"
#include "types.h"

int
main(int argc, char **argv)
{
	opts = malloc(1 * sizeof(struct Options*));

	if (opts == NULL) {
		perror("mandel: error: ");
		return 1;
	}

	/* initialize default options */
	opts->height  = 1000;
	opts->width   = 2000;
	opts->iter    =  100;
	opts->verbose = FALSE;

	u64 total = opts->height * opts->width;

	/* column (x) counter */
	u64 pxctr = 0;

	/* parse arguments */
	/* TODO: add version, help */
	const struct argoat_sprig sprigs[9] = {
		{ NULL,      0, NULL, NULL },
		{ "height",  1, (void*) opts->height,  handle_uint },
		{ "h",       1, (void*) opts->height,  handle_uint },
		{ "width",   1, (void*) opts->width,   handle_uint },
		{ "w",       1, (void*) opts->width,   handle_uint },
		{ "iters",   1, (void*) opts->iter,    handle_uint },
		{ "i",       1, (void*) opts->iter,    handle_uint },
		{ "verbose", 0, (void*) opts->verbose, handle_bool },
		{ "v",       0, (void*) opts->verbose, handle_bool },
	};

	struct argoat args = { sprigs, sizeof(sprigs), NULL, 0, 0 };
	argoat_graze(&args, argc, argv);

	/*
	 * TODO: define the following and
	 * make it possible to tweak them
	 * via command-line arguments:
	 * 	- MinRe
	 * 	- MaxRe
	 * 	- MinIm
	 * 	- MaxIm
	 * 
	 * (and also re_fact, im_fact, etc)
	 */

	/* initialize farbfeld stuff */
	u32 buf;
	fputs("farbfeld", stdout);                   /* magic value */
	buf = htonl(opts->width);
	fwrite(&buf, sizeof(buf), 1, stdout);        /* width */
	buf = htonl(opts->height);
	fwrite(&buf, sizeof(buf), 1, stdout);        /* height */

	u16 *data = (u16*) malloc(opts->width * (4 * sizeof(u16)));
	if (data == NULL) {
		perror("mandel: error: ");
		return 1;
	}

	for (usize y = 0; y < opts->height; ++y) {
		double c_im = (y - opts->height / 1.8) * 3.5 / opts->width;

		for (usize x = 0; x < opts->width; ++x, ++pxctr) {
			double c_re = (x - opts->width / 1.8) * 3.5 / opts->width;
			double Z_re = c_re, Z_im = c_im;

        		/* number of iterations */
			usize ctr;

			for (ctr = 0; ctr < opts->iter; ++ctr) {
				double Z_re2 = Z_re * Z_re, Z_im2 = Z_im * Z_im;

				if (Z_re2 + Z_im2 > 4) break;

				Z_im = 2 * Z_re * Z_im + c_im;
				Z_re = Z_re2 - Z_im2 + c_re;
			}

			/* get color based on iterations */
			float quotient = ((float) ctr) / ((float) opts->iter);
			u16 color = (u16) (quotient * (float) 0xffff);
			
			/* set pixel */
			if (ctr != opts->iter) {
				if (ctr > (opts->iter / 2)) {
					data[4 * x + 0] = htons(color);     /* red */
					data[4 * x + 1] = htons(0xffff);    /* green */
					data[4 * x + 2] = htons(0xffff);    /* blue */
				} else {
					data[4 * x + 0] = htons(0x0000);    /* red */
					data[4 * x + 1] = htons(color);     /* green */
					data[4 * x + 2] = htons(color);     /* blue */
				}
			} else {
				data[4 * x + 0] = htons(0x0000);    /* red */
				data[4 * x + 1] = htons(0x0000);    /* green */
				data[4 * x + 2] = htons(0x0000);    /* blue */
			}

			data[4 * x + 3] = htons(0xffff);   /* alpha */
			
			if (pxctr % 2048 == 0 && opts->verbose) {
				fprintf(stderr,
					"%c[smandel: on pixel %i/%i, row %i/%i, %f%% done.%c[u",
					27, pxctr, total, y, opts->height,
					((double) pxctr) / ((double) total) * 100, 27);
			}
		}

		/* print row for image */
		fwrite(data, sizeof(u16), opts->width * 4, stdout);
	}
}
