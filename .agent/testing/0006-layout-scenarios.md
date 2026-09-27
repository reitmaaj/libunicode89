# libunicode89 testing scenarios (BDD) - hierarchical header layout

## 0006-layout-scenarios.md

The flat `include/unicode89_ext.h` is migrated to the hierarchical installed
layout required by `API.md`. Each orthogonal Unicode capability keeps its own
translation unit and gains one named header.

- SCENARIO installed headers: GIVEN the checked-in public headers WHEN the
  API convention gate inspects `include/` THEN the public headers are exactly
  `unicode89.h`, `unicode89/grapheme.h`, `unicode89/width.h`, `unicode89/properties.h`,
  `unicode89/identifier.h`, `unicode89/casefold.h` and `unicode89/normalize.h`, and no
  `unicode89_ext.h` remains.
- SCENARIO dependency direction: GIVEN each named header WHEN it is compiled
  standalone in strict C89 THEN it includes `<unicode89.h>` and includes no
  sibling extension header.
- SCENARIO symbol namespace: GIVEN the built archive WHEN the symbol audit
  enumerates its exported symbols THEN the capabilities carry
  `unicode89_grapheme_*`, `unicode89_width_*`, `unicode89_properties_*`, `unicode89_identifier_*`,
  `unicode89_casefold` and `unicode89_normalize_*` names, and no `unicode89_ext_*` symbol
  remains.
- SCENARIO consumer: GIVEN `jml` consumes the identifier and normalization
  APIs WHEN it is built and tested THEN it uses `<unicode89/identifier.h>` and
  `<unicode89/normalize.h>` and passes unchanged.
