#ifndef UNICODE89_CASEFOLD_H
#define UNICODE89_CASEFOLD_H

#ifdef __cplusplus
extern "C" {
#endif

#include <unicode89.h>

/* unicode89_casefold mode. Full (default) folding uses CaseFolding.txt status C and
   F; Turkic additionally applies the T status overrides (I -> dotless i). */
#define UNICODE89_CASEFOLD_DEFAULT 0
#define UNICODE89_CASEFOLD_TURKIC 1

/* Full Unicode case fold of [s, s+n) into dst. One-to-many mappings are
   applied. Returns the number of bytes written, or UNICODE89_EUTF8 on malformed
   input, UNICODE89_ENOSPC when dst is too small, UNICODE89_EINVAL on a bad mode. With
   dst == NULL the exact output byte count is returned. */
int unicode89_casefold(int mode, const unsigned char *s, size_t n,
                 unsigned char *dst, size_t dst_cap);

#ifdef __cplusplus
}
#endif

#endif /* UNICODE89_CASEFOLD_H */
