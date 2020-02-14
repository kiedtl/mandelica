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
	opts->height  = 400;
	opts->width   = 800;
	opts->iter    = 100;
	opts->verbose = FALSE;

	/* pixel counter */
	u64 pxctr = 0;

	/* data counter */
	u64 bctr = 0;

	/* parse arguments */
	/* TODO: add version, help */
	const struct argoat_sprig sprigs[9] = {
		{ NULL,      0, NULL, NULL },
		{ "height",  1, (void*) &opts->height,  handle_uint },
		{ "h",       1, (void*) &opts->height,  handle_uint },
		{ "width",   1, (void*) &opts->width,   handle_uint },
		{ "w",       1, (void*) &opts->width,   handle_uint },
		{ "iters",   1, (void*) &opts->iter,    handle_uint },
		{ "i",       1, (void*) &opts->iter,    handle_uint },
		{ "verbose", 0, (void*) &opts->verbose, handle_bool },
		{ "v",       0, (void*) &opts->verbose, handle_bool },
	};

	struct argoat args = { sprigs, sizeof(sprigs), NULL, 0, 0 };
	argoat_graze(&args, argc, argv);

	u64 total = opts->height * opts->width;

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
	u32 tmp;
	fputs("farbfeld", stdout);                   /* magic value */
	tmp = htonl(opts->width);
	fwrite(&tmp, sizeof(tmp), 1, stdout);        /* width */
	tmp = htonl(opts->height);
	fwrite(&tmp, sizeof(tmp), 1, stdout);        /* height */

	u16 *buf = malloc(opts->width * (4 * sizeof(u16)));
	if (buf == NULL) {
		perror("mandel: error: ");
		return 1;
	}

	usize y;
	for (y = 0; y < opts->height; ++y) {
		float c_im = (y - opts->height / 1.8) * 3.5 / opts->width;

		for (usize x = 0; x < opts->width; ++x, ++pxctr) {
			float c_re = (x - opts->width / 1.8) * 3.5 / opts->width;
			float Z_re = c_re, Z_im = c_im;

        		/* number of iterations */
			usize ctr;

			for (ctr = 0; ctr < opts->iter; ++ctr) {
				float Z_re2 = Z_re * Z_re, Z_im2 = Z_im * Z_im;

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
					buf[4 * x + 0] = htons(color);     /* red */
					buf[4 * x + 1] = htons(0xffff);    /* green */
					buf[4 * x + 2] = htons(0xffff);    /* blue */
				} else {
					buf[4 * x + 0] = htons(0x0000);    /* red */
					buf[4 * x + 1] = htons(color);     /* green */
					buf[4 * x + 2] = htons(color);     /* blue */
				}
			} else {
				buf[4 * x + 0] = htons(0x0000);    /* red */
				buf[4 * x + 1] = htons(0x0000);    /* green */
				buf[4 * x + 2] = htons(0x0000);    /* blue */
			}

			buf[4 * x + 3] = htons(0xffff);   /* alpha */
			
			if ((pxctr & 2047) == 0 && opts->verbose) {
				fprintf(stderr, "\rmandel: pixel %lli/%lli, row %i/%i, wrote %lliMiB, %f%% completed.",
						pxctr, total, y, opts->height,
						bctr / 1024 / 1024,
						(float) (((float) pxctr) / ((float) total) * 100));
			}
		}

		/* print row for image */
		bctr += (fwrite(buf, sizeof(u16), opts->width * 4, stdout) * sizeof(u16));
	}

	fprintf(stderr, "\rmandel: pixel %lli/%lli, row %i/%i, wrote %lliMiB, %f%% completed.\n",
			pxctr, total, y, opts->height,
			bctr / 1024 / 1024,
			(float) (((float) pxctr) / ((float) total) * 100));
	if (buf) free(buf);
}
