# 0006 — status acceptance (CONVENTIONS.md section 14)

Run `just test` and `just check` in `libu89`.

## Must exhibit

- A1. `U89_OK == 0` and every `u89_status` failure is negative.
- A2. Every `u89_status` enumerator carries an explicit numeric value.
- A3. `u89_status_name()` returns `"OK"`, a stable failure token, and
      `"UNKNOWN"` for an unknown value; it is total and allocation-free.
- A4. `u89_status_message()` returns non-NULL static text for every input.
- A5. `just check-error-convention --lib libu89` reports
      `error-convention: ok`.

## Must reject (unacceptable behavior)

- B1. A `u89_status` failure with a positive value.
- B2. A `u89_status` enumerator without an explicit value.
- B3. `u89_status_name()` returning NULL for an unknown value.
- B4. Any `errno` read or write in the library.
