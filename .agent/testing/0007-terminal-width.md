# libunicode89 testing scenarios — terminal cluster width

`include/unicode89/termwidth.h` is the named-extension header for the
terminal grapheme-width policy: the display width, in terminal cells, of one
extended grapheme cluster under mainstream terminal behavior. It builds on
the scalar `unicode89_width` facts and the emoji property facts in
`<unicode89/properties.h>`; it owns no tables of its own.

## 0012 — Emoji presentation via a presentation selector
SCENARIO a text-default emoji base followed by the emoji presentation selector
    is two cells
GIVEN a grapheme cluster consisting of an Emoji-property scalar with
    Emoji_Presentation == 0 followed by U+FE0F
WHEN the cluster width is computed
THEN the width is 2, and the same base without U+FE0F keeps its scalar width.

## 0013 — Emoji ZWJ sequences
SCENARIO a joined emoji family is two cells
GIVEN a grapheme cluster containing a zero-width joiner and at least one
    Emoji-property scalar
WHEN the cluster width is computed
THEN the width is 2 regardless of how many emoji members the sequence has.

## 0014 — Regional-indicator scalars
SCENARIO regional indicators are two cells
GIVEN a grapheme cluster of one or two regional-indicator scalars
WHEN the cluster width is computed
THEN the width is 2: the pinned Unicode data marks regional indicators
    Emoji_Presentation, so a lone one and a two-member flag pair are both
    two cells.

## 0015 — Keycap sequences
SCENARIO a keycap sequence is two cells
GIVEN a grapheme cluster whose base is an ASCII digit, '#' or '*' followed by
    U+20E3 (with or without U+FE0F)
WHEN the cluster width is computed
THEN the width is 2, and U+20E3 over a non-keycap base contributes nothing.

## 0016 — Wide and fullwidth bases
SCENARIO East Asian W/F scalars are two cells at cluster level
GIVEN a grapheme cluster whose base carries EAW W or F
WHEN the cluster width is computed
THEN the width is 2, combining marks included.

## 0017 — Combining-only clusters
SCENARIO a cluster without a cell-bearing base is zero cells
GIVEN a grapheme cluster containing only marks or default-ignorable scalars
WHEN the cluster width is computed
THEN the width is 0.

## 0018 — Ordinary clusters and invalid input
SCENARIO plain clusters take their base width and bad spans are rejected
GIVEN an ordinary base-plus-marks cluster
WHEN the cluster width is computed
THEN the width equals the base's scalar width (0, 1, or 2); a span that is
    not well-formed UTF-8 or violates 0 <= start <= end <= n returns -1.
