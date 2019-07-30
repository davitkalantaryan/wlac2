// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <ldap.h> For WINDOWS MFC
 *
 *	Created on: Dec 22, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */
#ifndef __win_ldap_h__
#define __win_ldap_h__

#include <first_includes/new_winldap_common_include.h>
#include <first_includes/common_include_for_headers.h>
#include <sdef_gem_windows.h>

__BEGIN_C_DECLS

NWLDAP_FAR_API int ldap_initialize(LDAP **ldp, char *uri);
NWLDAP_FAR_API int ldap_unbind_ext(LDAP *ld, LDAPControl *sctrls[],LDAPControl *cctrls[]);

__END_C_DECLS


/*
* structure representing an ldap session which can
* encompass connections to multiple servers (in the
* face of referrals).
*/
//typedef struct ldap LDAP;

/* LBER lengths (32 bits or larger) */
//#define LBER_LEN_T long

/* lengths */
//typedef unsigned LBER_LEN_T ber_len_t;

#endif  /* #ifndef __win_ldap_h__ */
