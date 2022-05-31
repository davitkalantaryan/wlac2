// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <new_winldap_common_include.h> 
 *
 *	Created on: Dec 24, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */
#ifndef __new_winldap_new_winldap_common_include_h__
#define __new_winldap_new_winldap_common_include_h__

//#include <first_includes/common_definations_wul.h>
//#include <windows.h>
//#include <winldap.h>
//
//
//
//#ifndef NWLDAP_FAR_API
//#define NWLDAP_FAR_API
//#define NWLDAP_FAR_VAR
//#endif  // #ifndef NWLDAP_FAR_API
//
//#ifndef LDAP_SCOPE_SUBTREE
//#define LDAP_SCOPE_SUBTREE		((int) 0x0002)
//#endif
//
//#ifndef LDAP_SCOPE_SUB
//#define LDAP_SCOPE_SUB			LDAP_SCOPE_SUBTREE
//#endif
//
//#ifndef ldap_unbind_ext_s
//#define ldap_unbind_ext_s(_a1,_a2,_a3)	ldap_unbind((_a1))
//#endif
//
//#if(_MSC_VER < 1400) || !defined(_In_)
//
//#ifndef _In_
//#define _In_
//#endif
//
//#ifndef _In_opt_
//#define _In_opt_
//#endif
//
//#ifndef _Outptr_result_maybenull_
//#define _Outptr_result_maybenull_
//#endif
//
//#ifndef _Null_terminated_
//#define _Null_terminated_
//#endif
//
//#ifndef PZPSTR
//#define PZPSTR _Null_terminated_ PSTR*
//#endif 
//
//#endif // #if(_MSC_VER < 1400)
//
//__BEGIN_C_DECLS
//
//WINLDAPAPI VOID LDAPAPI ber_free(BerElement *ber, int freebuf);
//
//__END_C_DECLS
//
//#ifdef ldap_first_attribute
//#undef ldap_first_attribute
//#endif
//#define ldap_first_attribute ldap_first_attributeA
//
//#ifdef ldap_next_attribute
//#undef ldap_next_attribute
//#endif
//#define ldap_next_attribute ldap_next_attributeA
//
//#ifdef ldap_get_values_len
//#undef ldap_get_values_len
//#endif
//#define ldap_get_values_len ldap_get_values_lenA
//
//#ifdef ldap_search_ext_s
//#undef ldap_search_ext_s
//#endif
//#define ldap_search_ext_s ldap_search_ext_sA
//
////#define ldap_msgfree ldap_msgfree_new
//
//
////#define ldap_first_entry ldap_first_entry_new
//
//// ldap_count_entries_new
////#define ldap_count_entries ldap_count_entries_new
//
//// ldap_value_free_len_new
////#define ldap_value_free_len ldap_value_free_len_new
//
//// ldap_next_entry_new
////#define ldap_next_entry ldap_next_entry_new
//
////ldap_memfreeA_new
//#ifdef ldap_memfree
//#undef ldap_memfree
//#endif
//#define ldap_memfree ldap_memfreeA
//
////ber_free
////#define ber_free ber_free_new


#endif  /* #ifndef __new_winldap_new_winldap_common_include_h__ */
