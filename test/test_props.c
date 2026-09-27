#include "test.h"
#include "unicode89.h"
#include <unicode89/identifier.h>
#include <unicode89/properties.h>

void test_props(void)
{
    /* Default_Ignorable_Code_Point */
    unicode89_check(unicode89_identifier_default_ignorable(0x00ADUL) == 1, "DIC soft hyphen");
    unicode89_check(unicode89_identifier_default_ignorable(0x200BUL) == 1, "DIC ZWSP");
    unicode89_check(unicode89_identifier_default_ignorable(0x2060UL) == 1, "DIC WJ");
    unicode89_check(unicode89_identifier_default_ignorable(0x034FUL) == 1, "DIC CGJ");
    unicode89_check(unicode89_identifier_default_ignorable(0x0041UL) == 0, "DIC 'A' excluded");
    unicode89_check(unicode89_identifier_default_ignorable(0x0020UL) == 0, "DIC space excluded");

    /* Pattern_White_Space */
    unicode89_check(unicode89_identifier_pattern_whitespace(0x0009UL) == 1, "PWS tab");
    unicode89_check(unicode89_identifier_pattern_whitespace(0x0020UL) == 1, "PWS space");
    unicode89_check(unicode89_identifier_pattern_whitespace(0x200EUL) == 1, "PWS LRM");
    unicode89_check(unicode89_identifier_pattern_whitespace(0x3000UL) == 0, "PWS ideographic space excluded");

    /* Pattern_Syntax */
    unicode89_check(unicode89_identifier_pattern_syntax(0x002BUL) == 1, "PATS +");
    unicode89_check(unicode89_identifier_pattern_syntax(0x0024UL) == 1, "PATS $");
    unicode89_check(unicode89_identifier_pattern_syntax(0x002DUL) == 1, "PATS -");
    unicode89_check(unicode89_identifier_pattern_syntax(0x005FUL) == 0, "PATS _ excluded");
    unicode89_check(unicode89_identifier_pattern_syntax(0x0041UL) == 0, "PATS 'A' excluded");

    /* Join_Control */
    unicode89_check(unicode89_identifier_join_control(0x200CUL) == 1, "JC ZWNJ");
    unicode89_check(unicode89_identifier_join_control(0x200DUL) == 1, "JC ZWJ");
    unicode89_check(unicode89_identifier_join_control(0x200EUL) == 0, "JC LRM excluded");

    /* non-scalar rejected for every predicate */
    unicode89_check(unicode89_identifier_default_ignorable(0xD800UL) == 0, "DIC surrogate rejected");
    unicode89_check(unicode89_identifier_pattern_syntax(0xD800UL) == 0, "PATS surrogate rejected");
    unicode89_check(unicode89_identifier_pattern_whitespace(0xD800UL) == 0, "PWS surrogate rejected");
    unicode89_check(unicode89_identifier_join_control(0x110000UL) == 0, "JC out-of-range rejected");

    /* General_Category */
    unicode89_check(unicode89_properties_general_category(0x0041UL) == UNICODE89_PROPERTIES_GC_Lu, "GC 'A' Lu");
    unicode89_check(unicode89_properties_general_category(0x0061UL) == UNICODE89_PROPERTIES_GC_Ll, "GC 'a' Ll");
    unicode89_check(unicode89_properties_general_category(0x0030UL) == UNICODE89_PROPERTIES_GC_Nd, "GC '0' Nd");
    unicode89_check(unicode89_properties_general_category(0x0020UL) == UNICODE89_PROPERTIES_GC_Zs, "GC space Zs");
    unicode89_check(unicode89_properties_general_category(0x200DUL) == UNICODE89_PROPERTIES_GC_Cf, "GC ZWJ Cf");
    unicode89_check(unicode89_properties_general_category(0xE000UL) == UNICODE89_PROPERTIES_GC_Co, "GC PUA Co");
    unicode89_check(unicode89_properties_general_category(0xAC00UL) == UNICODE89_PROPERTIES_GC_Lo, "GC Hangul Lo");
    unicode89_check(unicode89_properties_general_category(0x0300UL) == UNICODE89_PROPERTIES_GC_Mn, "GC combining Mn");
    unicode89_check(unicode89_properties_general_category(0xD800UL) == UNICODE89_PROPERTIES_GC_Cs, "GC surrogate Cs");
    unicode89_check(unicode89_properties_general_category(0x0378UL) == 0, "GC unassigned Cn");
    unicode89_check(unicode89_properties_general_category(0x110000UL) == 0, "GC out-of-range 0");

    /* unicode89_properties_is_control: General_Category Cc only */
    unicode89_check(unicode89_properties_is_control(0x0009UL) == 1, "control tab");
    unicode89_check(unicode89_properties_is_control(0x000AUL) == 1, "control LF");
    unicode89_check(unicode89_properties_is_control(0x001BUL) == 1, "control ESC");
    unicode89_check(unicode89_properties_is_control(0x007FUL) == 1, "control DEL");
    unicode89_check(unicode89_properties_is_control(0x0085UL) == 1, "control NEL (C1)");
    unicode89_check(unicode89_properties_is_control(0x009BUL) == 1, "control CSI (C1)");
    unicode89_check(unicode89_properties_is_control(0x0041UL) == 0, "control 'A' excluded");
    unicode89_check(unicode89_properties_is_control(0x200DUL) == 0, "control ZWJ excluded (Cf)");
    unicode89_check(unicode89_properties_is_control(0xD800UL) == 0, "control surrogate excluded");

    /* unicode89_properties_is_mark: Mn, Mc, Me only */
    unicode89_check(unicode89_properties_is_mark(0x0301UL) == 1, "mark combining acute Mn");
    unicode89_check(unicode89_properties_is_mark(0x0903UL) == 1, "mark devanagari visarga Mc");
    unicode89_check(unicode89_properties_is_mark(0x20DDUL) == 1, "mark enclosing circle Me");
    unicode89_check(unicode89_properties_is_mark(0x0041UL) == 0, "mark 'A' excluded");
    unicode89_check(unicode89_properties_is_mark(0x1F3FBUL) == 0, "mark skin tone is Sk");
    unicode89_check(unicode89_properties_is_mark(0xD800UL) == 0, "mark surrogate excluded");

    /* East Asian Width classes */
    unicode89_check(unicode89_properties_east_asian_width(0x0041UL) == UNICODE89_PROPERTIES_EAW_NA, "EAW 'A' Na");
    unicode89_check(unicode89_properties_east_asian_width(0x4E00UL) == UNICODE89_PROPERTIES_EAW_W, "EAW CJK W");
    unicode89_check(unicode89_properties_east_asian_width(0xFF21UL) == UNICODE89_PROPERTIES_EAW_F, "EAW fullwidth F");
    unicode89_check(unicode89_properties_east_asian_width(0x00A1UL) == UNICODE89_PROPERTIES_EAW_A, "EAW inverted ! A");
    unicode89_check(unicode89_properties_east_asian_width(0xFF61UL) == UNICODE89_PROPERTIES_EAW_H, "EAW halfwidth H");
    unicode89_check(unicode89_properties_east_asian_width(0x0378UL) == UNICODE89_PROPERTIES_EAW_N, "EAW unassigned N");
    unicode89_check(unicode89_properties_east_asian_width(0x1F600UL) == UNICODE89_PROPERTIES_EAW_W, "EAW emoji W");
    unicode89_check(unicode89_properties_east_asian_width(0xD800UL) == UNICODE89_PROPERTIES_EAW_N, "EAW surrogate N");

    /* Emoji and Emoji_Presentation */
    unicode89_check(unicode89_properties_is_emoji(0x1F600UL) == 1, "emoji grinning");
    unicode89_check(unicode89_properties_is_emoji(0x00A9UL) == 1, "emoji copyright");
    unicode89_check(unicode89_properties_is_emoji(0x0041UL) == 0, "emoji 'A' excluded");
    unicode89_check(unicode89_properties_is_emoji(0xD800UL) == 0, "emoji surrogate excluded");
    unicode89_check(unicode89_properties_is_emoji_presentation(0x1F600UL) == 1, "emoji pres grinning");
    unicode89_check(unicode89_properties_is_emoji_presentation(0x00A9UL) == 0, "emoji pres copyright no");
    unicode89_check(unicode89_properties_is_emoji_presentation(0x2708UL) == 0, "emoji pres airplane no");
    unicode89_check(unicode89_properties_is_emoji_presentation(0xD800UL) == 0, "emoji pres surrogate no");
}
