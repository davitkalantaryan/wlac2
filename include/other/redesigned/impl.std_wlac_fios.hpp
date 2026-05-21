// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

// std_wlac_fios.impl.hpp
// 2017 Dec 07

#ifndef __impl_std_wlac_fios_hpp__
#define __impl_std_wlac_fios_hpp__

#ifndef __std_wlac_fios_hpp__
#error does not include this file directly!
#include "std_wlac_fios.hpp"
#endif


template <typename TypeFios>
//void std::wlac_fios<TypeFios>::open(const char* a_filename, ios_base::openmode a_mode)
void std::wlac_fios<TypeFios>::open(const std::string& a_filename, ios_base::openmode a_mode)
{
	unsigned unMode(a_mode), unMask(ios_baseN::text);
	if(!(unMode&unMask)){
		a_mode |= ios_base::binary;
	}
	else{
		a_mode &= ~ios_baseN::text;
	}

	TypeFios::open(a_filename.c_str(),a_mode);
}


template <typename TypeFios>
//std::wlac_fios<TypeFios>::wlac_fios(const char* a_filename, ios_base::openmode a_mode)
std::wlac_fios<TypeFios>::wlac_fios(const std::string& a_filename, ios_base::openmode a_mode)
	:
	TypeFios(a_filename.c_str(), a_mode)
{
}


#endif  // #ifndef __impl_std_wlac_fios_hpp__
