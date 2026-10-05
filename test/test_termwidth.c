#include "test.h"
#include "unicode89.h"
#include <string.h>
#include <unicode89/termwidth.h>

/* w(s) = width of the single grapheme cluster s under AMBIG_NARROW. */
static int w(const char *s)
{
    return unicode89_termwidth_cluster((const unsigned char *)s, strlen(s), 0,
                                       strlen(s), UNICODE89_WIDTH_AMBIG_NARROW);
}

/* ww(s) = the same under AMBIG_WIDE. */
static int ww(const char *s)
{
    return unicode89_termwidth_cluster((const unsigned char *)s, strlen(s), 0,
                                       strlen(s), UNICODE89_WIDTH_AMBIG_WIDE);
}

void test_termwidth(void)
{
    /* TW-01: text-default emoji base + presentation selector. */
    unicode89_check(w("\xE2\x98\x81") == 1, "cloud text-default is 1");
    unicode89_check(w("\xE2\x98\x81\xEF\xB8\x8F") == 2, "cloud VS16 is 2");
    unicode89_check(w("\xE2\x98\x81\xEF\xB8\x8E") == 1, "cloud VS15 stays 1");
    unicode89_check(w("\xE2\x9D\xA4") == 1, "heart text-default is 1");
    unicode89_check(w("\xE2\x9D\xA4\xEF\xB8\x8F") == 2, "heart VS16 is 2");

    /* TW-01/TW-07: an emoji-presentation scalar is 2 without a selector. */
    unicode89_check(w("\xF0\x9F\x98\x80") == 2, "grinning face is 2");
    unicode89_check(w("\xE2\x9C\x8A") == 2, "raised fist is 2");

    /* TW-02: emoji ZWJ sequences collapse to two cells. */
    unicode89_check(w("\xF0\x9F\x91\xA9\xE2\x80\x8D\xF0\x9F\x91\xA9"
                      "\xE2\x80\x8D\xF0\x9F\x91\xA7") == 2,
                    "family sequence is 2");
    unicode89_check(w("\xE2\x98\x81\xE2\x80\x8D\xE2\x98\x82") == 2,
                    "ZWJ text-default pair is 2");

    /* TW-03: regional-indicator scalars carry Emoji_Presentation in the
       pinned Unicode data, so a lone RI is two cells and a pair is two. */
    unicode89_check(w("\xF0\x9F\x87\xBA") == 2, "lone RI is 2");
    unicode89_check(w("\xF0\x9F\x87\xBA\xF0\x9F\x87\xB8") == 2, "RI pair is 2");

    /* TW-04: keycap sequences. */
    unicode89_check(w("1\xEF\xB8\x8F\xE2\x83\xA3") == 2, "keycap 1 VS16 is 2");
    unicode89_check(w("1\xE2\x83\xA3") == 2, "keycap 1 plain is 2");
    unicode89_check(w("#\xEF\xB8\x8F\xE2\x83\xA3") == 2, "keycap # is 2");
    unicode89_check(w("*\xE2\x83\xA3") == 2, "keycap * is 2");
    unicode89_check(w("a\xE2\x83\xA3") == 1, "20E3 over non-keycap adds 0");

    /* TW-05: wide and fullwidth bases at cluster level. */
    unicode89_check(w("\xE4\xB8\xAD") == 2, "CJK base is 2");
    unicode89_check(w("\xE4\xB8\xAD\xCC\x81") == 2, "CJK plus mark is 2");

    /* TW-06: combining-only clusters. */
    unicode89_check(w("\xCC\x81") == 0, "lone combining mark is 0");
    unicode89_check(w("\xE2\x80\x8D") == 0, "lone ZWJ is 0");
    unicode89_check(w("") == 0, "empty cluster is 0");

    /* TW-07: ordinary clusters and ambiguous policy. */
    unicode89_check(w("e\xCC\x81") == 1, "e + combining acute is 1");
    unicode89_check(w("\xCE\xA9") == 1, "Omega narrow is 1");
    unicode89_check(ww("\xCE\xA9") == 2, "Omega wide is 2");
    unicode89_check(w("\xCE\xA9\xCC\x81") == 1, "Omega+mark narrow is 1");
    unicode89_check(ww("\xCE\xA9\xCC\x81") == 2, "Omega+mark wide is 2");

    /* TW-08: invalid spans return -1 and read nothing outside the span. */
    {
        static const unsigned char s[] = {'a', 'b'};

        unicode89_check(unicode89_termwidth_cluster(
                            s, 2, 2, 1, UNICODE89_WIDTH_AMBIG_NARROW) == -1,
                        "start > end is -1");
        unicode89_check(unicode89_termwidth_cluster(
                            s, 2, 0, 3, UNICODE89_WIDTH_AMBIG_NARROW) == -1,
                        "end > n is -1");
        unicode89_check(unicode89_termwidth_cluster(
                            s, 2, 1, 2, UNICODE89_WIDTH_AMBIG_NARROW) == 1,
                        "suffix slice works");
    }
    {
        static const unsigned char bad[] = {'a', 0xFF};

        unicode89_check(unicode89_termwidth_cluster(
                            bad, 2, 0, 2, UNICODE89_WIDTH_AMBIG_NARROW) == -1,
                        "malformed UTF-8 is -1");
    }
}
