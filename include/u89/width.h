#ifndef U89_WIDTH_H
#define U89_WIDTH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <u89.h>

typedef enum u89_width_ambig {
    U89_WIDTH_AMBIG_NARROW = 0, /* EAW "A" resolves to width 1 (classic wcwidth) */
    U89_WIDTH_AMBIG_WIDE = 1    /* EAW "A" resolves to width 2 (CJK terminals)   */
} u89_width_ambig;

/* Display width in terminal cells of scalar cp: 0 (combining/zero-width/
   control), 1 (narrow/neutral), or 2 (wide/fullwidth/ambiguous-wide).
   Returns -1 for a non-scalar or an unprintable control. */
int u89_width(u89_cp cp, u89_width_ambig a);

/* Return 1 if cp is a Unicode whitespace scalar (for word wrap). */
int u89_width_is_whitespace(u89_cp cp);

/* Word-wrap [s, s+n): break only at Unicode whitespace so that lines stay
   within max_cols (unbreakable words may overflow). Writes up to cap break
   positions (byte offsets where a line ends) into breaks; returns the number
   of breaks (may exceed cap). */
int u89_width_wrap(const unsigned char *s, size_t n, int max_cols,
                   u89_width_ambig a, size_t *breaks, size_t cap);

#ifdef __cplusplus
}
#endif

#endif /* U89_WIDTH_H */
