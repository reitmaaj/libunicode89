#ifndef UNICODE89_WIDTH_H
#define UNICODE89_WIDTH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <unicode89.h>

typedef enum unicode89_width_ambig {
    UNICODE89_WIDTH_AMBIG_NARROW = 0, /* EAW "A" resolves to width 1 (classic wcwidth) */
    UNICODE89_WIDTH_AMBIG_WIDE = 1    /* EAW "A" resolves to width 2 (CJK terminals)   */
} unicode89_width_ambig;

/* Display width in terminal cells of scalar cp: 0 (combining/zero-width/
   control), 1 (narrow/neutral), or 2 (wide/fullwidth/ambiguous-wide).
   Returns -1 for a non-scalar or an unprintable control. */
int unicode89_width(unicode89_cp cp, unicode89_width_ambig a);

/* Return 1 if cp is a Unicode whitespace scalar (for word wrap). */
int unicode89_width_is_whitespace(unicode89_cp cp);

/* Word-wrap [s, s+n): break only at Unicode whitespace so that lines stay
   within max_cols (unbreakable words may overflow). Writes up to cap break
   positions (byte offsets where a line ends) into breaks; returns the number
   of breaks (may exceed cap). */
int unicode89_width_wrap(const unsigned char *s, size_t n, int max_cols,
                   unicode89_width_ambig a, size_t *breaks, size_t cap);

#ifdef __cplusplus
}
#endif

#endif /* UNICODE89_WIDTH_H */
