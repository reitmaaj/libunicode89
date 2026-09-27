#ifndef UNICODE89_H
#define UNICODE89_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/* Unicode data version pinned by this build (Unicode 17.0.0). */
#define UNICODE89_UNICODE_MAJOR 17
#define UNICODE89_UNICODE_MINOR 0
#define UNICODE89_UNICODE_PATCH 0

/* Unicode scalar value. Any code point except the surrogate range
   U+D800..U+DFFF. Range U+0000..U+D7FF and U+E000..U+10FFFF. */
typedef unsigned int unicode89_cp;   /* 32-bit Unicode scalar value */

/* ---- Version ------------------------------------------------------------- */

/* Return "MAJOR.MINOR.PATCH" of the pinned Unicode data (immutable static
   ASCII). Caller must not free or modify the returned pointer. */
const char *unicode89_unicode_version(void);

/* ---- Scalar semantics ---------------------------------------------------- */

int unicode89_is_scalar(unicode89_cp cp);

/* ---- Status codes --------------------------------------------------------- */

/* Status of a positional Unicode operation. UNICODE89_OK is 0; every error code is
   negative. UNICODE89_EINVAL, UNICODE89_EUTF8, UNICODE89_ENOSPC, UNICODE89_EWORK and UNICODE89_EOVERFLOW
   keep the values they had as object-like macros; UNICODE89_ERANGE is new. */
typedef enum unicode89_status {
    UNICODE89_OK = 0,
    UNICODE89_EINVAL = -1,
    UNICODE89_EUTF8 = -2,
    UNICODE89_ENOSPC = -3,
    UNICODE89_EWORK = -4,
    UNICODE89_EOVERFLOW = -5,
    UNICODE89_ERANGE = -6
} unicode89_status;

/* Stable ASCII token, e.g. "OK" or "EINVAL"; "UNKNOWN" for any other value.
   Immutable static storage; allocation free; total. */
const char *unicode89_status_name(unicode89_status status);

/* Human-readable static text; wording is not part of the contract.
   Immutable static storage; allocation free; total. */
const char *unicode89_status_message(unicode89_status status);

/* ---- UTF-8 validation ---------------------------------------------------- */

/* Validate that bytes [s, s+n) are well-formed UTF-8: rejects overlong
   encodings, UTF-8-encoded surrogates, code points above U+10FFFF, and
   truncated sequences. Returns 1 if valid, 0 otherwise. NUL is valid. */
int unicode89_utf8_valid(const unsigned char *s, size_t n);

/* Length in bytes of the UTF-8 sequence encoding cp, or 0 if cp is not a
   scalar. */
int unicode89_utf8_len(unicode89_cp cp);

/* Expected byte length (1..4) of the UTF-8 sequence introduced by lead, or 0
   when lead cannot begin a sequence (continuation byte, overlong lead C0/C1,
   or above F4). Continuation validity and overlong/surrogate values are
   still decided by unicode89_utf8_decode/unicode89_utf8_valid. */
int unicode89_utf8_seq_len(unsigned char lead);

/* Encode cp into out (must hold unicode89_utf8_len(cp) bytes). Returns bytes
   written, or 0 if cp is not a scalar. */
int unicode89_utf8_encode(unicode89_cp cp, unsigned char *out);

/* ---- UTF-8 iteration ----------------------------------------------------- */

typedef struct unicode89_iter {
    const unsigned char *cur;
    const unsigned char *end;
    unicode89_cp cp;            /* last decoded scalar (valid when err==0) */
    int err;              /* 0 while ok; 1 on malformed input; 2 at end */
    int leading;          /* bytes consumed by the last scalar */
} unicode89_iter;

/* Initialize an iterator over [s, s+n). */
void unicode89_iter_init(unicode89_iter *it, const unsigned char *s, size_t n);

/* Advance to the next scalar. Sets err=2 at end of input, err=1 on
   malformed input (and stops; recovery to U+FFFD is deliberately not
   provided). Returns 0 if a scalar was decoded, nonzero otherwise. */
int unicode89_iter_next(unicode89_iter *it);

/* ---- Byte/codepoint/UTF-16 mapping (editor offsets) ---------------------- */

/* Decode the scalar beginning at byte position pos. On success writes the
   scalar to *cp and the first byte after it to *next and returns UNICODE89_OK.
   Returns UNICODE89_ERANGE when pos >= n, UNICODE89_EUTF8 on malformed or truncated
   input. cp and next may be null. */
unicode89_status unicode89_utf8_decode(const unsigned char *s, size_t n, size_t pos,
                           unicode89_cp *cp, size_t *next);

/* Find the scalar ending at byte position pos. On success writes the scalar
   to *cp and its starting offset to *prev and returns UNICODE89_OK. Returns
   UNICODE89_ERANGE when pos == 0 or pos > n, UNICODE89_EUTF8 when the bytes before pos
   are malformed or pos does not lie on a scalar boundary. */
unicode89_status unicode89_utf8_prev(const unsigned char *s, size_t n, size_t pos,
                         unicode89_cp *cp, size_t *prev);

/* Number of UTF-16 code units needed to encode cp (1 for BMP, 2 for
   supplementary). Returns 0 if cp is not a scalar. */
int unicode89_utf16_units(unicode89_cp cp);

/* Return 1 iff unit is a UTF-16 high-surrogate code unit (0xD800..0xDBFF),
   otherwise 0. Values above 0xFFFF return 0. */
int unicode89_utf16_is_high_surrogate(unsigned int unit);

/* Return 1 iff unit is a UTF-16 low-surrogate code unit (0xDC00..0xDFFF),
   otherwise 0. Values above 0xFFFF return 0. */
int unicode89_utf16_is_low_surrogate(unsigned int unit);

/* Decode one UTF-16 surrogate pair. high must lie in 0xD800..0xDBFF and low
   in 0xDC00..0xDFFF. On success writes the corresponding supplementary
   scalar to *cp when cp is non-null and returns 1. On invalid input leaves
   *cp unchanged and returns 0. Passing cp == NULL performs only the validity
   test. Decodes exactly one pair; it does not parse or traverse UTF-16
   strings and does not accept an unpaired BMP unit. */
int unicode89_utf16_decode_pair(unsigned int high, unsigned int low, unicode89_cp *cp);

#ifdef __cplusplus
}
#endif

#endif
