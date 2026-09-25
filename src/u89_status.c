/* u89_status.c - status inspection (CONVENTIONS.md section 14).
 *
 * Both functions are total: an unknown integer value yields a defined
 * fallback. Neither allocates; both return immutable static storage.
 */

#include "u89.h"

const char *u89_status_name(u89_status status)
{
    if (status == U89_OK)
    {
        return "OK";
    }
    if (status == U89_EINVAL)
    {
        return "EINVAL";
    }
    if (status == U89_EUTF8)
    {
        return "EUTF8";
    }
    if (status == U89_ENOSPC)
    {
        return "ENOSPC";
    }
    if (status == U89_EWORK)
    {
        return "EWORK";
    }
    if (status == U89_EOVERFLOW)
    {
        return "EOVERFLOW";
    }
    if (status == U89_ERANGE)
    {
        return "ERANGE";
    }
    return "UNKNOWN";
}

const char *u89_status_message(u89_status status)
{
    if (status == U89_OK)
    {
        return "success";
    }
    if (status == U89_EINVAL)
    {
        return "invalid argument";
    }
    if (status == U89_EUTF8)
    {
        return "malformed UTF-8";
    }
    if (status == U89_ENOSPC)
    {
        return "destination is too small";
    }
    if (status == U89_EWORK)
    {
        return "working buffer is too small";
    }
    if (status == U89_EOVERFLOW)
    {
        return "size computation overflowed";
    }
    if (status == U89_ERANGE)
    {
        return "position out of range";
    }
    return "unknown status";
}
