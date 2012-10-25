/* This work is free.
 * I, Strake <strake888@gmail.com>, the authour,
 * 	waive all copyright to this work, but
 * 	waive NO moral rights to this work.
 *
 * This work cometh AS IS, WITH ALL FAULTS, WITH NO WARRANTY OF ANY KIND WHATSOEVER, not even the implied warranties of MERCHANTABILITY, FITNESS, and TITLE.
 * In NO CASE SHALL THE AUTHOUR BE LIABLE for ANY DAMAGES OR HARMS CAUSED BY OR LINKED TO THIS WORK.
 */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <tomcrypt.h>

typedef u_int8_t byte;
typedef struct ltc_hash_descriptor Hash;

int fShowNybble (FILE *f, byte x) {
	x &= 0x0F;
	if ((x < 10 ? fputc ('0' + x, f) : fputc ('A' + x - 10, f)) == EOF) return EOF;
}

int fShowByte (FILE *f, byte x) {
	if (fShowNybble (f, x >> 4) == EOF ||
	    fShowNybble (f, x)      == EOF) return EOF;
}

int fShowBlock (FILE *f, byte *xs, unsigned int n) {
	unsigned int ii;
	for (ii = 0; ii < n; ii++) if (fShowByte (f, xs[ii]) == EOF) return EOF;
}

void main (int argc, char *argu[]) {
	Hash hs[] = {
		sha512_desc,
		sha384_desc,
		sha256_desc,
		sha224_desc,
		sha1_desc,
		md5_desc,
		md4_desc,
		md2_desc,
		rmd320_desc,
		rmd256_desc,
		rmd160_desc,
		rmd128_desc,
		tiger_desc,
		whirlpool_desc,
		{ 0 }
	};
	Hash h;
	hash_state hst;
	char *name;
	byte x[MAXBLOCKSIZE], b[4096];
	ssize_t nb;
	
	name = strrchr (argu[0], '.');
	if (name) name++;
	else name = "";

	{
		int ii;
		for (ii = 0; hs[ii].name; ii++) if (strcasecmp (hs[ii].name, name) == 0) break;
		h = hs[ii];
		if (!h.name) {
			fprintf (stderr, "hash: unknown algorithm: %s\n", name);
			exit (1);
		}
	}
	
	if (h.init (&hst) != 0) {
		fprintf (stderr, "hash: failed to start algorithm: %s\n", name);
		exit (1);
	}
	do {
		nb = read (0, b, 4096);
		if (nb < 0) {
			fprintf (stderr, "hash: failed to read: %s\n", strerror (errno));
			exit (1);
		}
		if (h.process (&hst, b, (unsigned long)nb) != 0) {
			fprintf (stderr, "hash: failed to do algorithm: %s\n", name);
			exit (1);
		}
	} while (nb > 0);
	if (h.done (&hst, x) != 0) {
		fprintf (stderr, "hash: failed to finish algorithm: %s\n", name);
		exit (1);
	}
	
	fShowBlock (stdout, x, h.hashsize);
	fputc ('\n', stdout);
}
