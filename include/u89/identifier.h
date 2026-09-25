#ifndef U89_IDENTIFIER_H
#define U89_IDENTIFIER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <u89.h>

/* UAX #31 identifier and pattern properties. */
int u89_identifier_xid_start(u89_cp cp);
int u89_identifier_xid_continue(u89_cp cp);

int u89_identifier_id_start(u89_cp cp);
int u89_identifier_id_continue(u89_cp cp);

int u89_identifier_default_ignorable(u89_cp cp);
int u89_identifier_pattern_syntax(u89_cp cp);
int u89_identifier_pattern_whitespace(u89_cp cp);
int u89_identifier_join_control(u89_cp cp);

#ifdef __cplusplus
}
#endif

#endif /* U89_IDENTIFIER_H */
