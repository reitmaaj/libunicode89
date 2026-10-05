/* unicode89_casefold.c - Unicode full case folding (default and Turkic).

   Written in green worker/controller shape. Folding is a per-scalar mapping
   with no inter-scalar state, so both passes (length, then write) iterate the
   same decision via thin workers. */

#include "../include/unicode89.h"
#include "unicode89_priv.h"
#include <unicode89/casefold.h>

/* Probe the sorted mapping rows. Returns 1 when row i matches cp, 2 when cp
   precedes row i (stop), else 0. */
static int map_probe(const unicode89_priv_mapping *t, size_t i, unicode89_cp cp)
{
    if (cp < t[i].cp)
    {
        return 2;
    }
    if (cp > t[i].cp)
    {
        return 0;
    }
    return 1;
}

/* Return the index of cp in the mapping rows, or n when cp has no mapping. */
static size_t fold_index(const unicode89_priv_mapping *t, size_t n,
                         unicode89_cp cp)
{
    size_t i;
    int st;

    for (i = 0; i < n; ++i)
    {
        st = map_probe(t, i, cp);
        if (st == 1)
        {
            return i;
        }
        if (st == 2)
        {
            return n;
        }
    }
    return n;
}

/* Advance one scalar at *i. Returns 0 on success with *cp set, 1 on malformed
   input. */
static int next_cp(const unsigned char *s, size_t n, size_t *i,
                   unicode89_cp *cp)
{
    size_t next;
    unicode89_status st;

    st = unicode89_utf8_decode(s, n, *i, cp, &next);
    if (st != UNICODE89_OK)
    {
        return 1;
    }
    *i = next;
    return 0;
}

/* Add the UTF-8 length of cp to *total. */
static void id_len_add(unicode89_cp cp, size_t *total)
{
    int cl;

    cl = unicode89_utf8_len(cp);
    *total = *total + (size_t)cl;
}

/* Add the UTF-8 length of the next stored scalar to *total; returns 0 when all
   scalars of the sequence are consumed. */
static int seq_len_step(const unicode89_cp *pool, unsigned long off, size_t len,
                        size_t *k, size_t *total)
{
    int cl;
    unicode89_cp cp;

    if (*k >= len)
    {
        return 0;
    }
    cp = pool[off + *k];
    cl = unicode89_utf8_len(cp);
    *total = *total + (size_t)cl;
    *k = *k + 1;
    return 1;
}

/* Add the UTF-8 length of the stored sequence for row idx to *total. */
static void seq_len_add(const unicode89_priv_mapping *t, size_t idx,
                        size_t *total)
{
    size_t k;
    size_t len;
    unsigned long off;
    int go;

    len = t[idx].length;
    off = t[idx].offset;
    k = 0;
    go = 1;
    while (go != 0)
    {
        go = seq_len_step(unicode89_priv_mapping_pool, off, len, &k, total);
    }
}

/* Add the folded byte length of cp (identity, or stored at idx) to *total. */
static void fold_add(const unicode89_priv_mapping *t, size_t tn,
                     unicode89_cp cp, size_t idx, size_t *total)
{
    if (idx == tn)
    {
        id_len_add(cp, total);
    }
    else
    {
        seq_len_add(t, idx, total);
    }
}

/* Fold one scalar at *i, adding to *total. Returns 0 on success, 1 on
   malformed input. */
static int fold_len_step(const unicode89_priv_mapping *t, size_t tn,
                         const unsigned char *s, size_t n, size_t *i,
                         size_t *total)
{
    unicode89_cp cp;
    int bad;
    size_t idx;

    bad = next_cp(s, n, i, &cp);
    if (bad)
    {
        return 1;
    }
    idx = fold_index(t, tn, cp);
    fold_add(t, tn, cp, idx, total);
    return 0;
}

/* Compute the total folded byte length of [s, n) into *out. Returns 0 on
   success, 1 on malformed input. */
static int fold_length(const unicode89_priv_mapping *t, size_t tn,
                       const unsigned char *s, size_t n, size_t *out)
{
    size_t i;
    size_t total;
    int st;

    i = 0;
    total = 0;
    while (i < n)
    {
        st = fold_len_step(t, tn, s, n, &i, &total);
        if (st != 0)
        {
            return 1;
        }
    }
    *out = total;
    return 0;
}

/* Write cp into dst at *off. */
static void id_enc_add(unicode89_cp cp, unsigned char *dst, size_t *off)
{
    int d;

    d = unicode89_utf8_encode(cp, dst + *off);
    *off = *off + (size_t)d;
}

/* Write the next stored scalar of the sequence into dst at *off; returns 0
   when all scalars are written. */
static int seq_enc_step(const unicode89_cp *pool, unsigned long off, size_t len,
                        size_t *k, unsigned char *dst, size_t *outoff)
{
    int d;
    unicode89_cp cp;

    if (*k >= len)
    {
        return 0;
    }
    cp = pool[off + *k];
    d = unicode89_utf8_encode(cp, dst + *outoff);
    *outoff = *outoff + (size_t)d;
    *k = *k + 1;
    return 1;
}

/* Write the stored sequence for row idx into dst at *off. */
static void seq_enc_add(const unicode89_priv_mapping *t, size_t idx,
                        unsigned char *dst, size_t *off)
{
    size_t k;
    size_t len;
    unsigned long base;
    int go;

    len = t[idx].length;
    base = t[idx].offset;
    k = 0;
    go = 1;
    while (go != 0)
    {
        go = seq_enc_step(unicode89_priv_mapping_pool, base, len, &k, dst, off);
    }
}

/* Write the folded bytes of cp (identity, or stored at idx) into dst at
 *off. */
static void fold_write_cp(const unicode89_priv_mapping *t, size_t tn,
                          unicode89_cp cp, size_t idx, unsigned char *dst,
                          size_t *off)
{
    if (idx == tn)
    {
        id_enc_add(cp, dst, off);
    }
    else
    {
        seq_enc_add(t, idx, dst, off);
    }
}

/* Fold one scalar at *i, writing its bytes into dst at *off. Returns 0 on
   success, 1 on malformed input. */
static int fold_wr_step(const unicode89_priv_mapping *t, size_t tn,
                        const unsigned char *s, size_t n, size_t *i,
                        unsigned char *dst, size_t *off)
{
    unicode89_cp cp;
    int bad;
    size_t idx;

    bad = next_cp(s, n, i, &cp);
    if (bad)
    {
        return 1;
    }
    idx = fold_index(t, tn, cp);
    fold_write_cp(t, tn, cp, idx, dst, off);
    return 0;
}

int unicode89_casefold(int mode, const unsigned char *s, size_t n,
                       unsigned char *dst, size_t dst_cap)
{
    const unicode89_priv_mapping *t;
    size_t tn;
    size_t total;
    size_t i;
    size_t off;
    int st;
    int bad;
    int vm;

    if (mode == 0)
    {
        vm = 1;
    }
    else if (mode == 1)
    {
        vm = 1;
    }
    else
    {
        vm = 0;
    }
    if (vm == 0)
    {
        return UNICODE89_EINVAL;
    }
    if (mode == 0)
    {
        t = unicode89_priv_casefold_map;
    }
    else
    {
        t = unicode89_priv_casefold_turkic_map;
    }
    if (mode == 0)
    {
        tn = unicode89_priv_casefold_map_count;
    }
    else
    {
        tn = unicode89_priv_casefold_turkic_map_count;
    }
    bad = fold_length(t, tn, s, n, &total);
    if (bad)
    {
        return UNICODE89_EUTF8;
    }
    if (dst == NULL)
    {
        return (int)total;
    }
    if (dst_cap < total)
    {
        return UNICODE89_ENOSPC;
    }
    off = 0;
    i = 0;
    while (i < n)
    {
        st = fold_wr_step(t, tn, s, n, &i, dst, &off);
        if (st != 0)
        {
            return UNICODE89_EUTF8;
        }
    }
    return (int)off;
}
