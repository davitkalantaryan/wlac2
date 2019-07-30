
#ifndef __common_macroses_h__
#define __common_macroses_h__

//#pragma warning(disable : 4996)
//__pragma(warning(disable:6246))
//#pragma include_alias( <stdio.h>, <stdio.h> )

#ifdef _MSC_VER
#define SET_WLAC_ALIAS(_file_)	__pragma (include_alias(#_file_, "redesigned/##_file_"))
#define RESET_WLAC_ALIAS(_file_)	__pragma (include_alias(#_file_, #_file_))
#endif // #ifdef _MSC_VER


#endif  // #ifndef __common_macroses_h__
