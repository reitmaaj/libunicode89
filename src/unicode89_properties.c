/* unicode89_properties.c - UAX #31 / UCD binary properties: Default_Ignorable,
   Pattern_Syntax, Pattern_White_Space, Join_Control. */

#include "../include/unicode89.h"
#include "unicode89_priv.h"
#include <unicode89/identifier.h>
#include <unicode89/properties.h>

/* Probe sorted, non-overlapping ranges. Returns 1 when cp lies in range i,
   2 when cp precedes range i (stop), otherwise 0 (continue). */

/* Binary-search helpers over sorted, non-overlapping ranges. */
static size_t bs_mid(size_t lo, size_t hi)
{
    size_t span;

    span = hi - lo;
    span = span / 2;
    return lo + span;
}

static size_t bs_step(size_t mid)
{
    return mid + 1;
}

static int range_before(const unicode89_priv_range *t, size_t i,
                        unicode89_cp cp)
{
    if (cp < t[i].lo)
    {
        return 1;
    }
    return 0;
}

static int range_inside(const unicode89_priv_range *t, size_t i,
                        unicode89_cp cp)
{
    if (cp <= t[i].hi)
    {
        return 1;
    }
    return 0;
}

static int member(const unicode89_priv_range *t, size_t n, unicode89_cp cp)
{
    size_t lo;
    size_t hi;
    size_t mid;

    lo = 0;
    hi = n;
    while (lo < hi)
    {
        mid = bs_mid(lo, hi);
        if (range_before(t, mid, cp))
        {
            hi = mid;
        }
        else
        {
            if (range_inside(t, mid, cp))
            {
                return 1;
            }
            lo = bs_step(mid);
        }
    }
    return 0;
}

static int in_prop(const unicode89_priv_range *t, size_t n, unicode89_cp cp)
{
    int sc;
    int m;

    sc = unicode89_is_scalar(cp);
    if (!sc)
    {
        return 0;
    }
    m = member(t, n, cp);
    return m;
}

int unicode89_identifier_default_ignorable(unicode89_cp cp)
{
    int r;

    r = in_prop(unicode89_priv_defign_ranges, unicode89_priv_defign_count, cp);
    return r;
}

int unicode89_identifier_pattern_whitespace(unicode89_cp cp)
{
    int r;

    r = in_prop(unicode89_priv_patws_ranges, unicode89_priv_patws_count, cp);
    return r;
}

int unicode89_identifier_pattern_syntax(unicode89_cp cp)
{
    int r;

    r = in_prop(unicode89_priv_patsyn_ranges, unicode89_priv_patsyn_count, cp);
    return r;
}

int unicode89_identifier_join_control(unicode89_cp cp)
{
    int r;

    r = in_prop(unicode89_priv_join_ranges, unicode89_priv_join_count, cp);
    return r;
}

/* Value lookup over sorted, non-overlapping unicode89_priv_prop_range rows.
   Returns the stored value when cp lies in a row, else dflt. */
static int prange_before(const unicode89_priv_prop_range *t, size_t i,
                         unicode89_cp cp)
{
    if (cp < t[i].lo)
    {
        return 1;
    }
    return 0;
}

static int prange_after(const unicode89_priv_prop_range *t, size_t i,
                        unicode89_cp cp)
{
    if (cp > t[i].hi)
    {
        return 1;
    }
    return 0;
}

static unsigned short prop_at(const unicode89_priv_prop_range *t, size_t i)
{
    return t[i].value;
}

static unsigned short prop_value(const unicode89_priv_prop_range *t, size_t n,
                                 unicode89_cp cp, unsigned short dflt)
{
    size_t lo;
    size_t hi;
    size_t mid;

    lo = 0;
    hi = n;
    while (lo < hi)
    {
        mid = bs_mid(lo, hi);
        if (prange_before(t, mid, cp))
        {
            hi = mid;
        }
        else
        {
            if (prange_after(t, mid, cp))
            {
                lo = bs_step(mid);
            }
            else
            {
                return prop_at(t, mid);
            }
        }
    }
    return dflt;
}

int unicode89_properties_general_category(unicode89_cp cp)
{
    unsigned short v;

    v = prop_value(unicode89_priv_gc_ranges, unicode89_priv_gc_count, cp, 0);
    return (int)v;
}

unicode89_properties_eaw unicode89_properties_east_asian_width(unicode89_cp cp)
{
    unsigned short v;

    v = prop_value(unicode89_priv_eaw_ranges, unicode89_priv_eaw_count, cp, 0);
    return (unicode89_properties_eaw)v;
}

int unicode89_properties_is_mark(unicode89_cp cp)
{
    int gc;
    int r;

    gc = unicode89_properties_general_category(cp);
    r = 0;
    if (gc == UNICODE89_PROPERTIES_GC_Mn)
    {
        r = 1;
    }
    if (gc == UNICODE89_PROPERTIES_GC_Mc)
    {
        r = 1;
    }
    if (gc == UNICODE89_PROPERTIES_GC_Me)
    {
        r = 1;
    }
    return r;
}

int unicode89_properties_is_control(unicode89_cp cp)
{
    int gc;

    gc = unicode89_properties_general_category(cp);
    if (gc == UNICODE89_PROPERTIES_GC_Cc)
    {
        return 1;
    }
    return 0;
}

int unicode89_properties_is_emoji(unicode89_cp cp)
{
    int r;

    r = in_prop(unicode89_priv_emoji_ranges, unicode89_priv_emoji_count, cp);
    return r;
}

int unicode89_properties_is_emoji_presentation(unicode89_cp cp)
{
    int r;

    r = in_prop(unicode89_priv_emoji_pres_ranges,
                unicode89_priv_emoji_pres_count, cp);
    return r;
}
