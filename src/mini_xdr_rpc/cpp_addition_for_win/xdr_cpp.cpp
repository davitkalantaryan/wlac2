

// xdr_cpp.cpp
// 2017 Dec 23

#if defined(__cplusplus) & defined(_WIN64)

#include <intrin.h>
#include "win_socket_common.h"
#include "rpc/xdr.h"
#include <stdint.h>

__BEGIN_C_DECLS

__END_C_DECLS


__declspec(dllexport) bool_t xdr_long(XDR *a_xdrs, ptrdiff_t *a_lp)
{
	return xdr_long_as_ptrdiff_t(a_xdrs, a_lp);
}



__declspec(dllexport) bool_t xdr_u_long(XDR* a_xdrs, size_t * a_lp)
{
	return xdr_u_long_as_size_t(a_xdrs, a_lp);
}

#endif  // #ifdef __cplusplus
