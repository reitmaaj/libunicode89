#include "test.h"
#include "unicode89.h"
#include <unicode89/identifier.h>

/* Every ID_Start scalar is also an ID_Continue scalar (UAX #31), and XID_Start
   is a subset of ID_Start. Exhaustive over all scalars. */
static void check_subsets(void)
{
    unicode89_cp cp;

    for (cp = 0; cp <= 0x10FFFF; cp++) {
        if (cp >= 0xD800 && cp <= 0xDFFF) {
            continue;
        }
        if (unicode89_identifier_id_start(cp)) {
            unicode89_check_ctx(unicode89_identifier_id_continue(cp), "ID_Start subset ID_Continue",
                          "cp");
        }
        if (unicode89_identifier_xid_start(cp)) {
            unicode89_check_ctx(unicode89_identifier_id_start(cp), "XID_Start subset ID_Start", "cp");
        }
    }
}

void test_id(void)
{
    /* ASCII letters/digits/underscore */
    unicode89_check(unicode89_identifier_id_start(0x0041UL) == 1, "ID_Start 'A'");
    unicode89_check(unicode89_identifier_id_continue(0x005FUL) == 1, "ID_Continue '_'");
    unicode89_check(unicode89_identifier_id_continue(0x0030UL) == 1, "ID_Continue '0'");
    unicode89_check(unicode89_identifier_id_start(0x0030UL) == 0, "ID_Start '0' rejected");
    unicode89_check(unicode89_identifier_id_start(0x0020UL) == 0, "ID_Start space rejected");

    /* Greek, CJK */
    unicode89_check(unicode89_identifier_id_start(0x0391UL) == 1, "ID_Start Greek Alpha");
    unicode89_check(unicode89_identifier_id_start(0x4E00UL) == 1, "ID_Start CJK ideograph");

    /* non-scalar rejected */
    unicode89_check(unicode89_identifier_id_start(0xD800UL) == 0, "ID_Start surrogate rejected");
    unicode89_check(unicode89_identifier_id_start(0x110000UL) == 0, "ID_Start out-of-range rejected");

    check_subsets();
}
