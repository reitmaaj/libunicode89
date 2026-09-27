/* test_status.c - CONVENTIONS.md section 14 status surface. */

#include <string.h>

#include "test.h"
#include "unicode89.h"

void test_status(void)
{
    unicode89_check(UNICODE89_OK == 0, "status: OK is zero");
    unicode89_check(UNICODE89_EINVAL < 0, "status: EINVAL negative");
    unicode89_check(UNICODE89_ERANGE < 0, "status: ERANGE negative");

    unicode89_check(strcmp(unicode89_status_name(UNICODE89_OK), "OK") == 0, "status: name OK");
    unicode89_check(strcmp(unicode89_status_name(UNICODE89_EUTF8), "EUTF8") == 0,
              "status: name EUTF8");
    unicode89_check(strcmp(unicode89_status_name((unicode89_status)9999), "UNKNOWN") == 0,
              "status: name unknown");

    unicode89_check(unicode89_status_message(UNICODE89_OK) != NULL, "status: message OK");
    unicode89_check(unicode89_status_message((unicode89_status)9999) != NULL,
              "status: message unknown");
}
