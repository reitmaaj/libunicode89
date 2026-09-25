#ifndef U89_GRAPHEME_H
#define U89_GRAPHEME_H

#ifdef __cplusplus
extern "C" {
#endif

#include <u89.h>

/* The input must be valid UTF-8 and pos <= n. Boundaries follow the pinned
   Unicode release (GCB, InCB, Extended_Pictographic). next() returns the
   smallest boundary strictly greater than pos (n at end of input); prev()
   returns the largest boundary strictly less than pos (0 at start);
   boundary() reports whether pos is a boundary. Allocation free. */
size_t u89_grapheme_next(const unsigned char *s, size_t n, size_t pos);
size_t u89_grapheme_prev(const unsigned char *s, size_t n, size_t pos);
int u89_grapheme_boundary(const unsigned char *s, size_t n, size_t pos);

#ifdef __cplusplus
}
#endif

#endif /* U89_GRAPHEME_H */
