#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <wctype.h>

int n_ns = 0, n_ns_max = 1;
long *ns;

void fskipspace (FILE *f) {
	wint_t x;
	for (;;) {
		x = fgetwc (f);
		if (x == WEOF) return;
		if (!iswspace (x)) break;
	}
	ungetwc (x, f);
}

void pushn (long n) {
	if (n_ns >= n_ns_max) {
		n_ns_max *= 2;
		ns = realloc (ns, sizeof (long)*n_ns_max);
		if (!ns) {
			fputs ("Failed to allocate memory\n", stderr);
			exit (-1);
		}
	}
	ns[n_ns++] = n;
}

long popn (void) {
	if (n_ns == 0) {
		fputs ("Empty Stack\n", stderr);
		exit (-1);
	}
	return ns[--n_ns];
}

void swapn (void) {
	if (n_ns <= 1) {
		fputs ("Empty Stack\n", stderr);
		exit (-1);
	}
	ns[n_ns-2] ^= ns[n_ns-1];
	ns[n_ns-1] ^= ns[n_ns-2];
	ns[n_ns-2] ^= ns[n_ns-1];
}

int iswnumeral (int base, wint_t x) {
	x = towupper (x);
	if (base <= 10) return (x >= '0' && x - base < '0');
	else return (iswnumeral (10, x) || x >= 'A' && x - (base - 10) < 'A');
}

int numeralValue (wint_t x) {
	x = towupper (x);
	return (x >= '0' && x <= '9' ? x - '0' : x - 'A' + 10);
}

long fparsen (FILE *f) {
	long n = 0;
	int base = 10;
	wint_t x;
	
	x = fgetwc (f);
	if (x == '0') { /* base ?= 1010 */
		x = fgetwc (f);
		switch (towlower (x)) {
		case 'b':
			base = 2;
			break;
		case 'o':
			base = 8;
			break;
		case 'd':
			base = 10;
			break;
		case 'x':
			base = 16;
			break;
		default:
			ungetwc (x, f);
		}
	}
	else ungetwc (x, f);
	
	for (;;) {
		x = fgetwc (f);
		if (x == WEOF) return n;
		if (!iswnumeral (base, x)) break;
		n *= base;
		n += numeralValue (x);
	}
	ungetwc (x, f);
	
	return n;
}

void fprintun (FILE *f, int base, long n) {
	if (n == 0) fputc ('0', f);
	else {
		if (n/base) fprintun (f, base, n/base);
		fputc ((n%base <= 9 ? '0' : 'A' - 10) + n%base, f);
	}
}

void fprintn (FILE *f, int base, long n) {
	if (n != 0) fputc (n > 0 ? '+' : '-', f);
	fprintun (f, base, abs (n));
}

void go (FILE *f, FILE *g) {
	wint_t x;
	
	/* otherwise P fails queerly */
	/* I have no damned clue why */
	setbuf (g, 0);
	fputwc (0, g);
	
	for (;;) {
		fskipspace (f);
		x = fgetwc (f);
		if (x >= '0' && x <= '9') {
			ungetwc (x, f);
			pushn (fparsen (f));
		}
		else switch (x) {
		case WEOF:
			return;
		case '&':
			pushn (popn () & popn ());
			break;
		case '|':
			pushn (popn () | popn ());
			break;
		case '^':
			pushn (popn () ^ popn ());
			break;
		case '-':
			pushn (-popn ());
			/* fall thru */
		case '+':
			pushn (popn () + popn ());
			break;
		case 'P':
			fputwc (popn (), g);
			break;
		case 'c':
			pushn (ns[n_ns-1]);
			break;
		case 'x':
			swapn ();
			break;
		case 'p':
			swapn ();
			fprintn (g, popn (), popn ());
			break;
		case 'q':
			exit (popn ());
			break;
		}
	}
}

int main (int argc, char *argu[]) {
	ns = malloc (sizeof (long)*n_ns_max);
	if (!ns) {
		fputs ("Failed to allocate memory\n", stderr);
		return (-1);
	}
	go (stdin, stdout);
	return 0;
}
