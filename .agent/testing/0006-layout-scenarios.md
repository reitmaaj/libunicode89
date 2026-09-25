# libu89 testing scenarios (BDD) - hierarchical header layout

## 0006-layout-scenarios.md

The flat `include/u89_ext.h` is migrated to the hierarchical installed
layout required by `API.md`. Each orthogonal Unicode capability keeps its own
translation unit and gains one named header.

- SCENARIO installed headers: GIVEN the checked-in public headers WHEN the
  API convention gate inspects `include/` THEN the public headers are exactly
  `u89.h`, `u89/grapheme.h`, `u89/width.h`, `u89/properties.h`,
  `u89/identifier.h`, `u89/casefold.h` and `u89/normalize.h`, and no
  `u89_ext.h` remains.
- SCENARIO dependency direction: GIVEN each named header WHEN it is compiled
  standalone in strict C89 THEN it includes `<u89.h>` and includes no
  sibling extension header.
- SCENARIO symbol namespace: GIVEN the built archive WHEN the symbol audit
  enumerates its exported symbols THEN the capabilities carry
  `u89_grapheme_*`, `u89_width_*`, `u89_properties_*`, `u89_identifier_*`,
  `u89_casefold` and `u89_normalize_*` names, and no `u89_ext_*` symbol
  remains.
- SCENARIO consumer: GIVEN `jml` consumes the identifier and normalization
  APIs WHEN it is built and tested THEN it uses `<u89/identifier.h>` and
  `<u89/normalize.h>` and passes unchanged.
