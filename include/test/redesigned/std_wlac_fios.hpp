

// std_wlac_fios.hpp
// 2017 Dec 07

#ifndef  __std_wlac_fios_hpp__
#define  __std_wlac_fios_hpp__

#include <ios>

namespace std{

namespace ios_baseN{enum{text=0x100};}

template <typename TypeFios>
class wlac_fios : public TypeFios
{
public:
	virtual void open(const char* filename,ios_base::openmode mode = ios_base::in|ios_base::out);
};

}


#include "std_wlac_fios.impl.hpp"

#endif  // #ifndef  __std_wlac_fios_hpp__
