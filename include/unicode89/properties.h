#ifndef UNICODE89_PROPERTIES_H
#define UNICODE89_PROPERTIES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <unicode89.h>

/* East Asian Width class. Unassigned code points default to
   UNICODE89_PROPERTIES_EAW_N. */
typedef enum unicode89_properties_eaw {
    UNICODE89_PROPERTIES_EAW_N = 0,
    UNICODE89_PROPERTIES_EAW_A,
    UNICODE89_PROPERTIES_EAW_H,
    UNICODE89_PROPERTIES_EAW_W,
    UNICODE89_PROPERTIES_EAW_F,
    UNICODE89_PROPERTIES_EAW_NA
} unicode89_properties_eaw;

unicode89_properties_eaw unicode89_properties_east_asian_width(unicode89_cp cp);

/* 1 when cp is General_Category Mn, Mc, or Me. */
int unicode89_properties_is_mark(unicode89_cp cp);

/* 1 when cp is General_Category Cc (LF, HT, DEL, C1, ESC). */
int unicode89_properties_is_control(unicode89_cp cp);

/* 1 when cp carries the Emoji property. */
int unicode89_properties_is_emoji(unicode89_cp cp);

/* 1 when cp carries the Emoji_Presentation property. */
int unicode89_properties_is_emoji_presentation(unicode89_cp cp);

/* General_Category value codes returned by unicode89_properties_general_category. 0
   means the General_Category Cn (unassigned) or a non-scalar argument. */
#define UNICODE89_PROPERTIES_GC_Cc 1
#define UNICODE89_PROPERTIES_GC_Cf 2
#define UNICODE89_PROPERTIES_GC_Co 3
#define UNICODE89_PROPERTIES_GC_Cs 4
#define UNICODE89_PROPERTIES_GC_Ll 5
#define UNICODE89_PROPERTIES_GC_Lm 6
#define UNICODE89_PROPERTIES_GC_Lo 7
#define UNICODE89_PROPERTIES_GC_Lt 8
#define UNICODE89_PROPERTIES_GC_Lu 9
#define UNICODE89_PROPERTIES_GC_Mc 10
#define UNICODE89_PROPERTIES_GC_Me 11
#define UNICODE89_PROPERTIES_GC_Mn 12
#define UNICODE89_PROPERTIES_GC_Nd 13
#define UNICODE89_PROPERTIES_GC_Nl 14
#define UNICODE89_PROPERTIES_GC_No 15
#define UNICODE89_PROPERTIES_GC_Pc 16
#define UNICODE89_PROPERTIES_GC_Pd 17
#define UNICODE89_PROPERTIES_GC_Pe 18
#define UNICODE89_PROPERTIES_GC_Pf 19
#define UNICODE89_PROPERTIES_GC_Pi 20
#define UNICODE89_PROPERTIES_GC_Po 21
#define UNICODE89_PROPERTIES_GC_Ps 22
#define UNICODE89_PROPERTIES_GC_Sc 23
#define UNICODE89_PROPERTIES_GC_Sk 24
#define UNICODE89_PROPERTIES_GC_Sm 25
#define UNICODE89_PROPERTIES_GC_So 26
#define UNICODE89_PROPERTIES_GC_Zl 27
#define UNICODE89_PROPERTIES_GC_Zp 28
#define UNICODE89_PROPERTIES_GC_Zs 29

int unicode89_properties_general_category(unicode89_cp cp);

#ifdef __cplusplus
}
#endif

#endif /* UNICODE89_PROPERTIES_H */
