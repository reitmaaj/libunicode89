#ifndef U89_NORMALIZE_H
#define U89_NORMALIZE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <u89.h>

/* Conservative number of u89_cp scalar slots a normalize_ex of [s, s+n) needs
   as workspace. Malformed UTF-8 returns U89_EUTF8. */
int u89_normalize_work_bound(const unsigned char *s, size_t n,
                             size_t *cp_bound);

/* Conservative UTF-8 byte capacity a normalized result can need. */
int u89_normalize_out_bound(const unsigned char *s, size_t n,
                            size_t *byte_bound);

/* Normalize [s, s+n) in mode (0=NFC, 1=NFD, 2=NFKC, 3=NFKD) into dst. work is
   caller-provided scalar scratch of work_cap u89_cp slots (at least
   u89_normalize_work_bound); dst MUST NOT overlap s or work. Returns the
   number of bytes written to dst, or U89_EUTF8 on malformed input, U89_EWORK
   when the workspace is too small, U89_ENOSPC when dst is too small. With
   dst == NULL and dst_cap 0 the exact output byte count is returned (work is
   still used). */
int u89_normalize_ex(int mode, const unsigned char *s, size_t n,
                     unsigned char *dst, size_t dst_cap, u89_cp *work,
                     size_t work_cap);

#ifdef __cplusplus
}
#endif

#endif /* U89_NORMALIZE_H */
