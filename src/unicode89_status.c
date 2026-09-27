/* unicode89_status.c - status inspection (CONVENTIONS.md section 14).
 *
 * Both functions are total: an unknown integer value yields a defined
 * fallback. Neither allocates; both return immutable static storage.
 */

#include "unicode89.h"

const char *unicode89_status_name(unicode89_status status)
{
    if (status == UNICODE89_OK)
    {
        return "OK";
    }
    if (status == UNICODE89_EINVAL)
    {
        return "EINVAL";
    }
    if (status == UNICODE89_EUTF8)
    {
        return "EUTF8";
    }
    if (status == UNICODE89_ENOSPC)
    {
        return "ENOSPC";
    }
    if (status == UNICODE89_EWORK)
    {
        return "EWORK";
    }
    if (status == UNICODE89_EOVERFLOW)
    {
        return "EOVERFLOW";
    }
    if (status == UNICODE89_ERANGE)
    {
        return "ERANGE";
    }
    return "UNKNOWN";
}

const char *unicode89_status_message(unicode89_status status)
{
    if (status == UNICODE89_OK)
    {
        return "success";
    }
    if (status == UNICODE89_EINVAL)
    {
        return "invalid argument";
    }
    if (status == UNICODE89_EUTF8)
    {
        return "malformed UTF-8";
    }
    if (status == UNICODE89_ENOSPC)
    {
        return "destination is too small";
    }
    if (status == UNICODE89_EWORK)
    {
        return "working buffer is too small";
    }
    if (status == UNICODE89_EOVERFLOW)
    {
        return "size computation overflowed";
    }
    if (status == UNICODE89_ERANGE)
    {
        return "position out of range";
    }
    return "unknown status";
}
