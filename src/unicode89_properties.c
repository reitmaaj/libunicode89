/* unicode89_properties.c - UAX #31 / UCD binary properties: Default_Ignorable,
   Pattern_Syntax, Pattern_White_Space, Join_Control. */

#include "../include/unicode89.h"
#include "unicode89_priv.h"
#include <unicode89/identifier.h>
#include <unicode89/properties.h>

/* Probe sorted, non-overlapping ranges. Returns 1 when cp lies in range i,
   2 when cp precedes range i (stop), otherwise 0 (continue). */
static int range_probe(const unicode89_priv_range *t, size_t i, unicode89_cp cp)
{
    unicode89_cp tlo;
    unicode89_cp thi;

    tlo = t[i].lo;
    if (cp < tlo)
    {
        return 2;
    }
    thi = t[i].hi;
    if (cp <= thi)
    {
        return 1;
    }
    return 0;
}

static int member(const unicode89_priv_range *t, size_t n, unicode89_cp cp)
{
    size_t i;
    int st;

    for (i = 0; i < n; ++i)
    {
        st = range_probe(t, i, cp);
        if (st == 1)
        {
            return 1;
        }
        if (st == 2)
        {
            return 0;
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

/* Value lookup over sorted, non-overlapping unicode89_priv_prop_range rows. Returns
   the stored value when cp lies in a row, else dflt. */
/* Probe one sorted unicode89_priv_prop_range row. Returns 1 when cp lies in row i
   (writing its value to *out), 2 when cp precedes row i (stop), else 0. */
static int gc_probe(const unicode89_priv_prop_range *t, size_t i, unicode89_cp cp,
                    unsigned short *out)
{
    if (cp < t[i].lo)
    {
        return 2;
    }
    if (cp > t[i].hi)
    {
        return 0;
    }
    *out = t[i].value;
    return 1;
}

static unsigned short prop_value(const unicode89_priv_prop_range *t, size_t n,
                                 unicode89_cp cp, unsigned short dflt)
{
    size_t i;
    int st;
    unsigned short found;

    found = dflt;
    for (i = 0; i < n; ++i)
    {
        st = gc_probe(t, i, cp, &found);
        if (st == 2)
        {
            return dflt;
        }
        if (st == 1)
        {
            return found;
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

    r = in_prop(unicode89_priv_emoji_pres_ranges, unicode89_priv_emoji_pres_count, cp);
    return r;
}
