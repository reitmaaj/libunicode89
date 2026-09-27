GREEN := env_var_or_default("GREEN", "../green/.agent/tmp/build/green")
GREEN_FORMAT := env_var_or_default("GREEN_FORMAT", "../green/share/green/clang-format.yaml")
UCD := env_var_or_default("UCD", "unicode-testdata/17.0.0")
CC := env_var_or_default("CC", "cc")

build:
	make libunicode89

test: build
	make test

# Best-effort 32-bit library build; reports a skip when multilib is absent.
build32:
	@mkdir -p build/32; \
	if ! printf '#include <stdio.h>\nint main(void){return 0;}\n' | {{CC}} -m32 -x c - -o build/32/probe 2>/dev/null; then \
	    echo "build32: SKIPPED: no 32-bit toolchain (install gcc multilib)"; \
	    exit 0; \
	fi; \
	make CC="{{CC}} -m32" BUILD=build/32 libunicode89

# Run the unit suite on ILP32 when the toolchain supports it.
test32: build32
	@if [ ! -f build/32/libunicode89.a ]; then exit 0; fi; \
	make CC="{{CC}} -m32" BUILD=build/32 test

# Generate the GCC and Clang compilation databases the green gate reads.
setup-green:
	cmake -S . -B build/gcc -DCMAKE_C_COMPILER=gcc -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
	cmake -S . -B build/clang -DCMAKE_C_COMPILER=clang -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# Full green gate: strict C89 AND C23, GCC and Clang, green-* semantic checks,
# canonical Allman formatting. Runs over every unit in the compilation databases.
check: setup-green
	{{GREEN}} check

gmatrix:
	{{GREEN}} matrix

glint:
	{{GREEN}} lint

# Regenerate Unicode 17.0.0 tables from the vendored UCD data.
tables:
	UCD={{UCD}} python3 tools/gen_unicode_tables.py

# Regenerate the pinned UCD provenance manifest
# (unicode-testdata/17.0.0/SOURCES.txt).
manifest:
	sh tools/gen_manifest.sh

# Rewrite the handwritten sources to canonical green formatting. Generated
# tables and the public header are excluded (the header keeps its compact
# declaration style; regenerate tables with `just tables`).
format:
	clang-format -i --style=file:{{GREEN_FORMAT}} src/unicode89_version.c src/unicode89_utf8.c src/unicode89_grapheme.c src/unicode89_xid.c src/unicode89_id.c src/unicode89_properties.c src/unicode89_width.c src/unicode89_normalize.c src/unicode89_casefold.c src/unicode89_status.c

# Run the full Unicode NormalizationTest.txt conformance suite (UAX #15).
conform-norm:
	make libunicode89
	cc -O1 -Iinclude -o build/conformance_norm tools/conformance_norm.c build/libunicode89.a
	./build/conformance_norm unicode-testdata/17.0.0/NormalizationTest.txt

# Run the full Unicode GraphemeBreakTest.txt conformance suite (UAX #29).
conform-grapheme:
	make libunicode89
	cc -O1 -Iinclude -o build/conformance_grapheme tools/conformance_grapheme.c build/libunicode89.a
	./build/conformance_grapheme unicode-testdata/17.0.0/auxiliary/GraphemeBreakTest.txt

# Independent generator oracle: cross-check committed property tables against a
# fresh parse of the vendored Unicode data.
gendiff:
	python3 tools/gen_check.py unicode-testdata/17.0.0

# Differential test: libunicode89 unicode89_normalize vs Python unicodedata (needs a unicode89cli
# build; runs N random BMP strings, default 20000).
diftest COUNT="20000":
	python3 tools/gen_unicode_tables.py
	make build/libunicode89.a
	@cc -std=c89 -pedantic-errors -Wall -Wextra -Werror -Wno-long-long -Wconversion \
		-Wsign-conversion -Wmissing-prototypes -Wold-style-definition \
		-Iinclude -Isrc -O0 -g -o build/unicode89cli tools/unicode89cli.c build/libunicode89.a
	python3 tools/difftest.py {{COUNT}}

clean:
	make clean
	rm -rf build/gcc build/clang

# Family API convention check: source rules plus this archive's
# exported-symbol classification.
api-convention: build
	sh scripts/check-api-convention.sh --symbols --lib .

# CONVENTIONS.md section 14 error-surface check.
error-convention:
	sh scripts/check-error-convention.sh --lib .
