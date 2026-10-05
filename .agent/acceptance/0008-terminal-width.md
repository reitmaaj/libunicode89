# libunicode89 acceptance tests — terminal cluster width

Additions to the renderer-facing Unicode surface. They trace to
`.agent/testing/0007-terminal-width.md`.

## Must exhibit (exhibit)

- `include/unicode89/termwidth.h` MUST declare `unicode89_termwidth_cluster`
  and include `<unicode89.h>` (and only its declared sibling dependencies).
- TW-01. Text-default emoji base + U+FE0F MUST be width 2; the same base
  without the selector MUST keep its scalar width.
- TW-02. An emoji sequence containing a zero-width joiner MUST be width 2
  whatever its member count.
- TW-03. Regional indicators MUST be two cells: the pinned Unicode data
  marks them Emoji_Presentation, so both a lone regional indicator and a
  two-member flag pair MUST be width 2.
- TW-04. A keycap sequence (digit/#/* + U+20E3, with or without U+FE0F) MUST
  be width 2; U+20E3 over any other base MUST add nothing.
- TW-05. EAW W/F bases MUST be width 2 at cluster level.
- TW-06. A combining-only cluster MUST be width 0.
- TW-07. Ordinary base-plus-marks clusters MUST take the base's scalar width;
  both ambiguous-width policies MUST be honored through the parameter.
- TW-08. A span that is not well-formed UTF-8, or with start > end or
  end > n, MUST return -1 and MUST NOT read outside [0, n).
- `just test`, `just check`, `just api-convention` and `just error-convention`
  MUST pass with the new header and translation unit.

## Must reject / fail safely (reject)

- `unicode89_termwidth_cluster` MUST NOT allocate and MUST NOT write to the
  input buffer.
- The header MUST NOT declare identifiers outside the `unicode89_termwidth_*`
  namespace (other than its declared family dependencies).
- The API MUST NOT introduce any new failure class: it returns the existing
  width values 0/1/2 and -1 for invalid spans.
