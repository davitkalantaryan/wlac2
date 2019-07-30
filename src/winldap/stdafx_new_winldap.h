/*
 *	File: stdafx_new_winldap.h 
 *
 *	Created on: Dec 24, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */
#ifndef __new_winldap_stdafx_new_winldap_h__
#define __new_winldap_stdafx_new_winldap_h__


//#include "first_includes/common_definations_wul.h"


//#ifdef __USING_NWLDAP_SOURCES__
#if(_MSC_VER >= 1400)
#define NWLDAP_FAR_API
#define NWLDAP_FAR_VAR		extern 
#define NWLDAP_API			
#define NWLDAP_VAR			
#else
#define NWLDAP_FAR_API		EXPORT_TO_LIB_API
#define NWLDAP_FAR_VAR		extern EXPORT_TO_LIB_VAR
#define NWLDAP_API			EXPORT_TO_LIB_API
#define NWLDAP_VAR			EXPORT_TO_LIB_VAR
#endif  // #ifdef __USING_NWLDAP_SOURCES__


#endif  /* #ifndef __new_winldap_stdafx_new_winldap_h__ */
