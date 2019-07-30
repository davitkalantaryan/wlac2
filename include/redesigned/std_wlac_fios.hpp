// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

// std_wlac_fios.hpp
// 2017 Dec 07

#ifndef  __std_wlac_fios_hpp__
#define  __std_wlac_fios_hpp__

#include <ios>
#include <string>

namespace std{

namespace ios_baseN{enum{text=0x100};}

template <typename TypeFios>
class wlac_fios : public TypeFios
{
public:
	wlac_fios() {}
	//wlac_fios(const char* filename, ios_base::openmode mode = ios_base::in | ios_base::out);
	wlac_fios(const std::string& filename, ios_base::openmode mode = ios_base::in | ios_base::out);
	virtual ~wlac_fios() {}
	//virtual void open(const char* filename,ios_base::openmode mode = ios_base::in|ios_base::out);
	virtual void open(const std::string& filename, ios_base::openmode mode = ios_base::in | ios_base::out);
};

}


#include "impl.std_wlac_fios.hpp"

#endif  // #ifndef  __std_wlac_fios_hpp__
