// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

// termios.h
// 2017 Nov 28
// created by D. Kalantaryan

#ifndef __wlac_termios_h__
#define __wlac_termios_h__

#include <first_includes/common_include_for_headers.h>

__BEGIN_C_DECLS

struct winsize
{
	unsigned short int ws_row;
	unsigned short int ws_col;
	unsigned short int ws_xpixel;	/*unused*/
	unsigned short int ws_ypixel;	/*unused*/
};

__END_C_DECLS


#endif // #ifndef __wlac_termios_h__
