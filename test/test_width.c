#include <string.h>
#include "test.h"
#include "unicode89.h"
#include <unicode89/width.h>

void test_width(void)
{
    /* D. width classes */
    unicode89_check(unicode89_width(0x0041UL, UNICODE89_WIDTH_AMBIG_NARROW) == 1, "A narrow=1");
    unicode89_check(unicode89_width(0x4E00UL, UNICODE89_WIDTH_AMBIG_NARROW) == 2, "CJK wide=2");
    unicode89_check(unicode89_width(0xFF21UL, UNICODE89_WIDTH_AMBIG_NARROW) == 2, "fullwidth=2");
    unicode89_check(unicode89_width(0x0301UL, UNICODE89_WIDTH_AMBIG_NARROW) == 0, "combining=0");
    unicode89_check(unicode89_width(0x200BUL, UNICODE89_WIDTH_AMBIG_NARROW) == 0, "ZWSP=0");
    unicode89_check(unicode89_width(0x200DUL, UNICODE89_WIDTH_AMBIG_NARROW) == 0, "ZWJ=0");

    /* ambiguous per policy */
    unicode89_check(unicode89_width(0x03A9UL, UNICODE89_WIDTH_AMBIG_NARROW) == 1, "ambig narrow=1");
    unicode89_check(unicode89_width(0x03A9UL, UNICODE89_WIDTH_AMBIG_WIDE) == 2, "ambig wide=2");

    /* J. control flagged unprintable */
    unicode89_check(unicode89_width(0x0007UL, UNICODE89_WIDTH_AMBIG_NARROW) == -1, "control -1");
    unicode89_check(unicode89_width(0x001BL, UNICODE89_WIDTH_AMBIG_NARROW) == -1, "ESC -1");
    unicode89_check(unicode89_width(0xD800UL, UNICODE89_WIDTH_AMBIG_NARROW) == -1, "non-scalar -1");

    /* E. wrap at Unicode whitespace */
    {
        /* "aaa bbb" at width 3 -> break at 4 */
        static const unsigned char s[] = "aaa bbb";
        size_t breaks[8];
        int k;

        k = unicode89_width_wrap(s, sizeof s - 1, 3, UNICODE89_WIDTH_AMBIG_NARROW, breaks, 8);
        unicode89_check(k == 1, "wrap one break");
        unicode89_check(breaks[0] == 4, "wrap break at 4");
    }
    {
        /* NBSP (U+00A0) as break separator, wide CJK chars */
        unsigned char s[] = { 0xE4, 0xB8, 0x80, 0xC2, 0xA0,
                              0xE4, 0xBA, 0x8C };   /* 一 NBSP 二 */
        size_t breaks[8];
        int k;

        k = unicode89_width_wrap(s, sizeof s, 2, UNICODE89_WIDTH_AMBIG_NARROW, breaks, 8);
        unicode89_check(k == 1, "wrap breaks at NBSP");
        unicode89_check(breaks[0] == 5, "wrap break at word start (5)");
    }
    {
        /* no break inside a single word longer than width */
        static const unsigned char s[] = "abcdef";
        size_t breaks[8];
        int k;

        k = unicode89_width_wrap(s, sizeof s - 1, 3, UNICODE89_WIDTH_AMBIG_NARROW, breaks, 8);
        unicode89_check(k == 0, "no break in unbreakable word");
    }
    {
        /* M. no break except at whitespace: "ab cd" width 3 */
        static const unsigned char s[] = "ab cd";
        size_t breaks[8];
        int k;

        k = unicode89_width_wrap(s, sizeof s - 1, 3, UNICODE89_WIDTH_AMBIG_NARROW, breaks, 8);
        unicode89_check(k == 1, "wrap breaks at space");
        unicode89_check(breaks[0] == 3, "break at 3");
    }
}
