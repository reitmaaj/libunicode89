/* unicode89_termwidth.c - terminal grapheme-cluster display width policy. */

#include "../include/unicode89.h"
#include <unicode89/identifier.h>
#include <unicode89/properties.h>
#include <unicode89/termwidth.h>

#define TW_ZWJ 1
#define TW_VS16 2
#define TW_VS15 3
#define TW_KC_MARK 4
#define TW_SKIP 5
#define TW_BASE 6

typedef struct tw_state
{
    int total;
    int zwj;
    int vs16;
    int emoji;
    int ep;
    int kc_base;
    int kc_mark;
    unicode89_width_ambig a;
} tw_state;

static void tw_init(tw_state *st, unicode89_width_ambig a)
{
    st->total = 0;
    st->zwj = 0;
    st->vs16 = 0;
    st->emoji = 0;
    st->ep = 0;
    st->kc_base = 0;
    st->kc_mark = 0;
    st->a = a;
}

/* ---- Scalar classification ------------------------------------------------
 */

static int tw_is_keycap_base(unicode89_cp cp)
{
    if (cp >= 0x30)
    {
        if (cp <= 0x39)
        {
            return 1;
        }
    }
    if (cp == 0x23)
    {
        return 1;
    }
    if (cp == 0x2A)
    {
        return 1;
    }
    return 0;
}

static int tw_class(unicode89_cp cp)
{
    int mark;
    int ign;

    if (cp == 0x200D)
    {
        return TW_ZWJ;
    }
    if (cp == 0xFE0F)
    {
        return TW_VS16;
    }
    if (cp == 0xFE0E)
    {
        return TW_VS15;
    }
    if (cp == 0x20E3)
    {
        return TW_KC_MARK;
    }
    mark = unicode89_properties_is_mark(cp);
    if (mark)
    {
        return TW_SKIP;
    }
    ign = unicode89_identifier_default_ignorable(cp);
    if (ign)
    {
        return TW_SKIP;
    }
    return TW_BASE;
}

/* ---- Base handling --------------------------------------------------------
 */

static int tw_add_width(int total, int w)
{
    if (w < 0)
    {
        return total;
    }
    return total + w;
}

static void tw_note_base(tw_state *st, unicode89_cp cp)
{
    int emoji;
    int ep;
    int kc;
    int w;

    emoji = unicode89_properties_is_emoji(cp);
    if (emoji)
    {
        st->emoji = 1;
    }
    ep = unicode89_properties_is_emoji_presentation(cp);
    if (ep)
    {
        st->ep = 1;
    }
    kc = tw_is_keycap_base(cp);
    if (kc)
    {
        st->kc_base = 1;
    }
    w = unicode89_width(cp, st->a);
    st->total = tw_add_width(st->total, w);
}

/* ---- Scanning -------------------------------------------------------------
 */

static void tw_apply(tw_state *st, unicode89_cp cp, int cls)
{
    if (cls == TW_ZWJ)
    {
        st->zwj = 1;
        return;
    }
    if (cls == TW_VS16)
    {
        st->vs16 = 1;
        return;
    }
    if (cls == TW_KC_MARK)
    {
        st->kc_mark = 1;
        return;
    }
    if (cls == TW_BASE)
    {
        tw_note_base(st, cp);
    }
}

static unicode89_status tw_step(tw_state *st, const unsigned char *s, size_t n,
                                size_t pos, size_t *next)
{
    unicode89_cp cp;
    unicode89_status r;
    size_t np;
    int cls;

    r = unicode89_utf8_decode(s, n, pos, &cp, &np);
    if (r != UNICODE89_OK)
    {
        return r;
    }
    cls = tw_class(cp);
    tw_apply(st, cp, cls);
    *next = np;
    return UNICODE89_OK;
}

/* ---- Policy decision ------------------------------------------------------
 */

static int tw_result(const tw_state *st)
{
    if (st->ep)
    {
        return 2;
    }
    if (st->vs16)
    {
        if (st->emoji)
        {
            return 2;
        }
    }
    if (st->zwj)
    {
        if (st->emoji)
        {
            return 2;
        }
    }
    if (st->kc_mark)
    {
        if (st->kc_base)
        {
            return 2;
        }
    }
    return st->total;
}

int unicode89_termwidth_cluster(const unsigned char *s, size_t n, size_t start,
                                size_t end, unicode89_width_ambig a)
{
    tw_state st;
    size_t pos;
    unicode89_status r;
    int out;

    if (start > end)
    {
        return -1;
    }
    if (end > n)
    {
        return -1;
    }
    tw_init(&st, a);
    pos = start;
    while (pos < end)
    {
        r = tw_step(&st, s, n, pos, &pos);
        if (r != UNICODE89_OK)
        {
            return -1;
        }
    }
    out = tw_result(&st);
    return out;
}
