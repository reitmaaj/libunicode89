#include "test.h"
#include "unicode89.h"
#include <unicode89/identifier.h>

void test_xid(void)
{
    /* ASCII identifier chars */
    unicode89_check(unicode89_identifier_xid_start(0x0041UL) == 1, "XID_Start 'A'");
    unicode89_check(unicode89_identifier_xid_continue(0x005FUL) == 1, "XID_Continue '_'");
    unicode89_check(unicode89_identifier_xid_continue(0x0030UL) == 1, "XID_Continue '0'");

    /* I. reject non-start scalars */
    unicode89_check(unicode89_identifier_xid_start(0x005FUL) == 0, "XID_Start '_' rejected");
    unicode89_check(unicode89_identifier_xid_start(0x0030UL) == 0, "XID_Start '0' rejected");
    unicode89_check(unicode89_identifier_xid_start(0x0020UL) == 0, "XID_Start space rejected");
    unicode89_check(unicode89_identifier_xid_start(0x0301UL) == 0, "XID_Start combining mark rejected");

    /* C. Greek, CJK start */
    unicode89_check(unicode89_identifier_xid_start(0x0391UL) == 1, "XID_Start Greek Alpha");
    unicode89_check(unicode89_identifier_xid_start(0x4E00UL) == 1, "XID_Start CJK ideograph");
    unicode89_check(unicode89_identifier_xid_continue(0x0391UL) == 1, "XID_Continue Greek");

    /* non-scalar rejected */
    unicode89_check(unicode89_identifier_xid_start(0xD800UL) == 0, "XID_Start surrogate rejected");
    unicode89_check(unicode89_identifier_xid_start(0x110000UL) == 0, "XID_Start out-of-range rejected");
}
