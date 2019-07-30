// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <lber.h> For WINDOWS MFC
 *
 *	Created on: Dec 22, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */
#ifndef __win_lber_h__
#define __win_lber_h__

#include <first_includes/new_winldap_common_include.h>
#include <first_includes/common_include_for_headers.h>
#include <sdef_gem_windows.h>

/*
* ber_tag_t represents the identifier octets at the beginning of BER
* elements.  OpenLDAP treats them as mere big-endian unsigned integers.
*
* Actually the BER identifier octets look like this:
*
*	Bits of 1st octet:
*	______
*	8 7 | CLASS
*	0 0 = UNIVERSAL
*	0 1 = APPLICATION
*	1 0 = CONTEXT-SPECIFIC
*	1 1 = PRIVATE
*		_____
*		| 6 | DATA-TYPE
*		  0 = PRIMITIVE
*		  1 = CONSTRUCTED
*			___________
*			| 5 ... 1 | TAG-NUMBER
*
*  For ASN.1 tag numbers >= 0x1F, TAG-NUMBER above is 0x1F and the next
*  BER octets contain the actual ASN.1 tag number:  Big-endian, base
*  128, 8.bit = 1 in all but the last octet, minimum number of octets.
*/

#endif  /* #ifndef __win_lber_h__ */
