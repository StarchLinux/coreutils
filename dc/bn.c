#include "bn.h"

BIGNUM *BN_new () {
	BIGNUM *n;
	n = malloc (sizeof (BIGNUM));
	if (!n) return n;
	BN_init (n);
	return n;
}

void BN_free (BIGNUM *n) {
	mp_clear (n);
	free (n);
}

BN_CTX *BN_CTX_new () {
	return malloc (1);
}

void BN_CTX_init (BN_CTX *_) {
}

void BN_CTX_free (BN_CTX *c) {
	free (c);
}

BIGNUM *BN_copy (BIGNUM *t, const BIGNUM *s) {
	mp_copy (s, t);
	return t;
}

BIGNUM *BN_dup (const BIGNUM *s) {
	BIGNUM *t;
	t = BN_new ();
	if (!t) return t;
	BN_copy (t, s);
	return t;
}

int BN_num_bytes (const BIGNUM *n) {
	return n -> used;
}

int BN_num_bits (const BIGNUM *n) {
	int nb = 8*(n -> used - 1);
	unsigned short x = (n -> dp)[n -> used - 1];
	while (x) {
		x >>= 1;
		nb++;
	}
	return nb;
}

int BN_num_bits_word (BN_ULONG w) {
	int nb;
	while (w) {
		w >>= 1;
		nb++;
	}
	return nb;
}

int BN_add (BIGNUM *t, BIGNUM *r, BIGNUM *s) {
	return !mp_add (r, s, t);
}

int BN_sub (BIGNUM *t, BIGNUM *r, BIGNUM *s) {
	return !mp_sub (r, s, t);
}

int BN_mul (BIGNUM *t, BIGNUM *r, BIGNUM *s, BN_CTX *_) {
	return !mp_mul (r, s, t);
}

int BN_div (BIGNUM *q, BIGNUM *p, BIGNUM *r, BIGNUM *s, BN_CTX *_) {
	return !mp_div (r, s, q, p);
}

int BN_mod (BIGNUM *t, BIGNUM *r, BIGNUM *s, BN_CTX *_) {
	return !mp_mod (r, s, t);
}

int BN_exp (BIGNUM *t, BIGNUM *b, BIGNUM *m, BN_CTX *_) {
	BIGNUM n, one;
	BN_one (&one);
	BN_one (t);
	for (BN_copy (&n, m); !BN_is_zero (&n); BN_sub (&n, &n, &one)) if (!BN_mul (t, t, b, 0)) return 1;
	return 0;
}

int BN_add_word (BIGNUM *x, BN_ULONG w) {
	return !mp_add_d (x, w, x);
}

int BN_sub_word (BIGNUM *x, BN_ULONG w) {
	return !mp_sub_d (x, w, x);
}

int BN_mul_word (BIGNUM *x, BN_ULONG w) {
	return !mp_mul_d (x, w, x);
}

BN_ULONG BN_div_word (BIGNUM *x, BN_ULONG w) {
	BN_ULONG p;
	mp_div_d (x, w, x, &p);
	return p;
}

int BN_is_zero (BIGNUM *x) {
	return mp_iszero(x);
}

int BN_is_one (BIGNUM *x) {
	return (BN_num_bits (x) == 1);
}

int BN_zero (BIGNUM *x) {
	mp_zero (x);
	return 1;
}

int BN_one (BIGNUM *x) {
	mp_set (x, 1);
	return 1;
}

int BN_mask_bits (BIGNUM *x, int n) {
	int nB = n/8, nb = n%8;
	if (nB > x -> used) return 1;
	(x -> dp)[nB] &= ~((-1) << nb);
	return 1;
}

int BN_rshift (BIGNUM *t, BIGNUM *s, int n) {
	return !mp_mul_2d (s, n, t);
}

int BN_rshift1 (BIGNUM *t, BIGNUM *s) {
	return !mp_mul_2 (s, t);
}

int BN_is_bit_set (const BIGNUM *x, int n) {
	int nB = n/8, nb = n%8;
	if (nB > x -> used) return 0;
	return (((x -> dp)[nB] & (1 << nb)) ? 1 : 0);
}
