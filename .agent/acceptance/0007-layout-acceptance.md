# Acceptance criteria: hierarchical header layout

Additions to `.agent/acceptance/0001-acceptance.md`. They trace to
`.agent/testing/0006-layout-scenarios.md`.

## Must exhibit (exhibit)

- `include/u89.h`, `include/u89/grapheme.h`, `include/u89/width.h`,
  `include/u89/properties.h`, `include/u89/identifier.h`,
  `include/u89/casefold.h` and `include/u89/normalize.h` MUST be the only
  public headers.
- Each named header MUST include `<u89.h>` and compile standalone in strict
  C89.
- `just check`, `just test`, `just api-convention` and
  `just error-convention` MUST pass.
- `jml` MUST build and pass its suite against the migrated headers.

## Must reject / fail safely (reject)

- `include/u89_ext.h` or any exported `u89_ext_*` symbol MUST NOT remain.
- A named extension header MUST NOT include an unrelated sibling.
- `include/u89.h` MUST NOT include any `u89/*.h` header.
