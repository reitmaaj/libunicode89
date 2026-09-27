CC := gcc
CFLAGS := -std=c89 -pedantic-errors -Wall -Wextra -Werror \
    -Wconversion -Wsign-conversion -Wstrict-prototypes \
    -Wmissing-prototypes -Wold-style-definition -Wundef -Wshadow -Wformat=2 -Wno-long-long \
    -Iinclude -Isrc -O0 -g
BUILD := build

SRC := src/unicode89_version.c src/unicode89_utf8.c src/unicode89_grapheme.c src/unicode89_xid.c src/unicode89_id.c src/unicode89_properties.c src/unicode89_width.c src/unicode89_normalize.c src/unicode89_casefold.c src/unicode89_priv_tables.c src/unicode89_status.c
OBJ := $(patsubst src/%.c,$(BUILD)/%.o,$(SRC))
TEST_SRC := test/test_main.c test/test_utf8.c test/test_grapheme.c test/test_xid.c test/test_id.c test/test_props.c test/test_casefold.c test/test_width.c test/test_nfc.c test/test_consistency.c test/test_status.c

.PHONY: all libunicode89 test clean

all: libunicode89

libunicode89: $(BUILD)/libunicode89.a

$(BUILD)/libunicode89.a: $(OBJ)
	@mkdir -p $(BUILD)
	ar rcs $@ $^

$(BUILD)/%.o: src/%.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/test: $(TEST_SRC) $(BUILD)/libunicode89.a
	$(CC) $(CFLAGS) -o $@ $(TEST_SRC) $(BUILD)/libunicode89.a

test: $(BUILD)/test
	$(BUILD)/test

clean:
	rm -rf $(BUILD)
