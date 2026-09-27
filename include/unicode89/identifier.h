#ifndef UNICODE89_IDENTIFIER_H
#define UNICODE89_IDENTIFIER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <unicode89.h>

/* UAX #31 identifier and pattern properties. */
int unicode89_identifier_xid_start(unicode89_cp cp);
int unicode89_identifier_xid_continue(unicode89_cp cp);

int unicode89_identifier_id_start(unicode89_cp cp);
int unicode89_identifier_id_continue(unicode89_cp cp);

int unicode89_identifier_default_ignorable(unicode89_cp cp);
int unicode89_identifier_pattern_syntax(unicode89_cp cp);
int unicode89_identifier_pattern_whitespace(unicode89_cp cp);
int unicode89_identifier_join_control(unicode89_cp cp);

#ifdef __cplusplus
}
#endif

#endif /* UNICODE89_IDENTIFIER_H */
