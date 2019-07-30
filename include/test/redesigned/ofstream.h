

// wlac/redesigned/ofstream
// 2017 Dec 07

#if 0
#ifndef __wlac_redesigen_ofstream__
#define __wlac_redesigen_ofstream__

#pragma include_alias( <ofstream>, <ofstream> )
#pragma include_alias( "ofstream", "ofstream" )
#include <ofstream>
#include "std_wlac_fios.hpp"

namespace std {
	typedef wlac_fios<::std::ofstream> wlac_ofstream;
}


#define ofstream wlac_ofstream


#endif  // #ifndef __wlac_redesigen_ofstream__
#endif

