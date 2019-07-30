// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

#ifndef __win_bits_types_h__
#define __win_bits_types_h__

#include <first_includes/common_include_for_headers.h>

#if !defined(bits_h_force_include) & (_MSC_VER < 1400)
#include <bits.h>  //erevi
#else
#endif

#ifndef ____caddr_t_defined
typedef char *__caddr_t;
#define ____caddr_t_defined
#endif

__BEGIN_C_DECLS

__END_C_DECLS

#endif  /* #ifndef __win_bits_types_h__ */
