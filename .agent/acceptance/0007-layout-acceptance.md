# Acceptance criteria: hierarchical header layout

Additions to `.agent/acceptance/0001-acceptance.md`. They trace to
`.agent/testing/0006-layout-scenarios.md`.

## Must exhibit (exhibit)

- `include/unicode89.h`, `include/unicode89/grapheme.h`, `include/unicode89/width.h`,
  `include/unicode89/properties.h`, `include/unicode89/identifier.h`,
  `include/unicode89/casefold.h` and `include/unicode89/normalize.h` MUST be the only
  public headers.
- Each named header MUST include `<unicode89.h>` and compile standalone in strict
  C89.
- `just check`, `just test`, `just api-convention` and
  `just error-convention` MUST pass.
- `jml` MUST build and pass its suite against the migrated headers.

## Must reject / fail safely (reject)

- `include/unicode89_ext.h` or any exported `unicode89_ext_*` symbol MUST NOT remain.
- A named extension header MUST NOT include an unrelated sibling.
- `include/unicode89.h` MUST NOT include any `unicode89/*.h` header.
