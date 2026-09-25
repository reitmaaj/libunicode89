#ifndef U89_PROPERTIES_H
#define U89_PROPERTIES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <u89.h>

/* East Asian Width class. Unassigned code points default to
   U89_PROPERTIES_EAW_N. */
typedef enum u89_properties_eaw {
    U89_PROPERTIES_EAW_N = 0,
    U89_PROPERTIES_EAW_A,
    U89_PROPERTIES_EAW_H,
    U89_PROPERTIES_EAW_W,
    U89_PROPERTIES_EAW_F,
    U89_PROPERTIES_EAW_NA
} u89_properties_eaw;

u89_properties_eaw u89_properties_east_asian_width(u89_cp cp);

/* 1 when cp is General_Category Mn, Mc, or Me. */
int u89_properties_is_mark(u89_cp cp);

/* 1 when cp is General_Category Cc (LF, HT, DEL, C1, ESC). */
int u89_properties_is_control(u89_cp cp);

/* 1 when cp carries the Emoji property. */
int u89_properties_is_emoji(u89_cp cp);

/* 1 when cp carries the Emoji_Presentation property. */
int u89_properties_is_emoji_presentation(u89_cp cp);

/* General_Category value codes returned by u89_properties_general_category. 0
   means the General_Category Cn (unassigned) or a non-scalar argument. */
#define U89_PROPERTIES_GC_Cc 1
#define U89_PROPERTIES_GC_Cf 2
#define U89_PROPERTIES_GC_Co 3
#define U89_PROPERTIES_GC_Cs 4
#define U89_PROPERTIES_GC_Ll 5
#define U89_PROPERTIES_GC_Lm 6
#define U89_PROPERTIES_GC_Lo 7
#define U89_PROPERTIES_GC_Lt 8
#define U89_PROPERTIES_GC_Lu 9
#define U89_PROPERTIES_GC_Mc 10
#define U89_PROPERTIES_GC_Me 11
#define U89_PROPERTIES_GC_Mn 12
#define U89_PROPERTIES_GC_Nd 13
#define U89_PROPERTIES_GC_Nl 14
#define U89_PROPERTIES_GC_No 15
#define U89_PROPERTIES_GC_Pc 16
#define U89_PROPERTIES_GC_Pd 17
#define U89_PROPERTIES_GC_Pe 18
#define U89_PROPERTIES_GC_Pf 19
#define U89_PROPERTIES_GC_Pi 20
#define U89_PROPERTIES_GC_Po 21
#define U89_PROPERTIES_GC_Ps 22
#define U89_PROPERTIES_GC_Sc 23
#define U89_PROPERTIES_GC_Sk 24
#define U89_PROPERTIES_GC_Sm 25
#define U89_PROPERTIES_GC_So 26
#define U89_PROPERTIES_GC_Zl 27
#define U89_PROPERTIES_GC_Zp 28
#define U89_PROPERTIES_GC_Zs 29

int u89_properties_general_category(u89_cp cp);

#ifdef __cplusplus
}
#endif

#endif /* U89_PROPERTIES_H */
