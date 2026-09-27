cmake_minimum_required(VERSION 3.20)
project(libunicode89 C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Source files under the green profile. This list is grown module by module as
# each becomes C89-and-C23 clean under both GCC and Clang (see green.yaml).
set(UNICODE89_SRCS
    src/unicode89_version.c
    src/unicode89_xid.c
    src/unicode89_id.c
    src/unicode89_properties.c
    src/unicode89_width.c
    src/unicode89_utf8.c
    src/unicode89_grapheme.c
    src/unicode89_normalize.c
    src/unicode89_casefold.c
    src/unicode89_status.c
)

add_library(unicode89 STATIC ${UNICODE89_SRCS})
target_include_directories(unicode89 PUBLIC include PRIVATE src)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
