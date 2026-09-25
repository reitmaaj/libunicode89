/* test_status.c - CONVENTIONS.md section 14 status surface. */

#include <string.h>

#include "test.h"
#include "u89.h"

void test_status(void)
{
    u89_check(U89_OK == 0, "status: OK is zero");
    u89_check(U89_EINVAL < 0, "status: EINVAL negative");
    u89_check(U89_ERANGE < 0, "status: ERANGE negative");

    u89_check(strcmp(u89_status_name(U89_OK), "OK") == 0, "status: name OK");
    u89_check(strcmp(u89_status_name(U89_EUTF8), "EUTF8") == 0,
              "status: name EUTF8");
    u89_check(strcmp(u89_status_name((u89_status)9999), "UNKNOWN") == 0,
              "status: name unknown");

    u89_check(u89_status_message(U89_OK) != NULL, "status: message OK");
    u89_check(u89_status_message((u89_status)9999) != NULL,
              "status: message unknown");
}
