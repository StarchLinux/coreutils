#include <tommath.h>

#define BN_MASK2 (-1)
#define BN_ULONG unsigned long

#define BIGNUM mp_int
BIGNUM *BN_new (void);
void BN_free (BIGNUM *);
#define BN_init mp_init
#define BN_clear mp_clear
#define BN_clear_free BN_free

#define BN_CTX void
BN_CTX *BN_CTX_new (void);
void BN_CTX_init (BN_CTX *);
void BN_CTX_free (BN_CTX *);

BIGNUM *BN_copy (BIGNUM *, const BIGNUM *);
BIGNUM *BN_dup  (const BIGNUM *);
#define BN_swap mp_exch

int BN_num_bytes (const BIGNUM *);
int BN_num_bits  (const BIGNUM *);
int BN_num_bits_word (BN_ULONG);

int BN_add (BIGNUM *, BIGNUM *, BIGNUM *);
int BN_sub (BIGNUM *, BIGNUM *, BIGNUM *);
int BN_mul (BIGNUM *, BIGNUM *, BIGNUM *, BN_CTX *);
int BN_div (BIGNUM *, BIGNUM *, BIGNUM *, BIGNUM *, BN_CTX *);
int BN_mod (BIGNUM *, BIGNUM *, BIGNUM *, BN_CTX *);
int BN_exp (BIGNUM *, BIGNUM *, BIGNUM *, BN_CTX *);

int BN_add_word (BIGNUM *, BN_ULONG);
int BN_sub_word (BIGNUM *, BN_ULONG);
int BN_mul_word (BIGNUM *, BN_ULONG);
BN_ULONG BN_div_word (BIGNUM *, BN_ULONG);

#define BN_cmp mp_cmp
int BN_is_zero (BIGNUM *);
int BN_is_one  (BIGNUM *);

int BN_zero (BIGNUM *);
int BN_one  (BIGNUM *);
#define BN_set_word mp_set_int
#define BN_get_word mp_get_int

int BN_mask_bits (BIGNUM *, int);
int BN_rshift (BIGNUM *, BIGNUM *, int);
int BN_rshift1 (BIGNUM *, BIGNUM *);
int BN_is_bit_set (const BIGNUM *, int);
