/* unicode89_xid.c - UAX #31 XID_Start / XID_Continue predicates. */

#include "../include/unicode89.h"
#include "unicode89_priv.h"
#include <unicode89/identifier.h>

/* Probe sorted, non-overlapping ranges. Returns 1 when cp lies in range i,
   2 when cp precedes range i (ranges are ascending, so scanning may stop),
   otherwise 0 (continue scanning). */
static int range_probe(const unicode89_priv_range *t, size_t i,
                       unicode89_cp cp);

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

static int in_ranges(const unicode89_priv_range *t, size_t n, unicode89_cp cp)
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

int unicode89_identifier_xid_start(unicode89_cp cp)
{
    int sc;
    int r;

    sc = unicode89_is_scalar(cp);
    if (!sc)
    {
        return 0;
    }
    r = in_ranges(unicode89_priv_xid_start_ranges,
                  unicode89_priv_xid_start_count, cp);
    return r;
}

int unicode89_identifier_xid_continue(unicode89_cp cp)
{
    int sc;
    int r;

    sc = unicode89_is_scalar(cp);
    if (!sc)
    {
        return 0;
    }
    r = in_ranges(unicode89_priv_xid_cont_ranges, unicode89_priv_xid_cont_count,
                  cp);
    return r;
}
