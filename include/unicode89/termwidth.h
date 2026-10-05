#ifndef UNICODE89_TERMWIDTH_H
#define UNICODE89_TERMWIDTH_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <unicode89.h>
#include <unicode89/width.h>

    /* Display width in terminal cells of the grapheme cluster [start, end) in
       [s, s+n) under the mainstream terminal policy:
       - a wide (EAW W) or fullwidth (EAW F) base is two cells;
       - an emoji-presentation scalar is two cells;
       - an Emoji-property base followed by the emoji presentation selector
         (U+FE0F) is two cells;
       - an emoji sequence joined by a zero-width joiner is two cells whatever
         its member count;
       - regional indicators carry Emoji_Presentation in the pinned Unicode
       data, so a lone regional indicator is two cells and a two-member pair (a
       flag) is two cells;
       - a keycap sequence (ASCII digit, '#' or '*' followed by U+20E3, with or
         without U+FE0F) is two cells;
       - marks and default-ignorables contribute nothing, so a combining-only
         cluster is zero cells;
       - otherwise the cluster is the sum of its per-scalar widths under the
         ambiguous-width policy a (0, 1 or 2 for a valid grapheme cluster).
       Returns 0, 1 or 2, or -1 when the span is not well-formed UTF-8 or
       violates 0 <= start <= end <= n. Allocation free; does not write to s.
       The caller is expected to pass one extended grapheme cluster. */
    int unicode89_termwidth_cluster(const unsigned char *s, size_t n,
                                    size_t start, size_t end,
                                    unicode89_width_ambig a);

#ifdef __cplusplus
}
#endif

#endif /* UNICODE89_TERMWIDTH_H */
