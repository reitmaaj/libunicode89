# 0005 — status scenarios (CONVENTIONS.md section 14)

## SCENARIO U-STATUS-1 — signed partition

GIVEN a fallible libunicode89 operation
WHEN it succeeds THEN it returns `UNICODE89_OK`, which is zero
AND WHEN it fails THEN it returns a negative `unicode89_status`.

## SCENARIO U-STATUS-2 — explicit stable values

GIVEN the public `unicode89_status` enum
WHEN it is inspected
THEN every enumerator carries an explicit numeric value
AND `UNICODE89_OK` is zero and every failure is negative.

## SCENARIO U-STATUS-3 — inspection functions are total

GIVEN `unicode89_status_name` and `unicode89_status_message`
WHEN they receive a known or unknown value
THEN each returns a non-NULL pointer to immutable static storage
AND `unicode89_status_name` returns the stable token for known values and
    `"UNKNOWN"` otherwise
AND no allocation occurs.

## SCENARIO U-STATUS-4 — portable profile

GIVEN libunicode89 declares the portable status profile
WHEN any operation runs
THEN the library neither reads nor writes `errno`.
