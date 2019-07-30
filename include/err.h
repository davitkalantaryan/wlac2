// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <err.h> For WINDOWS MFC
 *
 *	Created on: Aug 17, 2016
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */

#ifndef __win_err_h___
#define __win_err_h___

#include <first_includes/common_include_for_headers.h>
#include <sdef_gem_windows.h>
# include <fcntl.h>

#include <features.h>

#define	__need___va_list
#include <stdarg.h>
#ifndef	__GNUC_VA_LIST
# define __gnuc_va_list	va_list 
#endif

__BEGIN_C_DECLS

/* Print "program: ", FORMAT, ": ", the standard error string for errno,
and a newline, on stderr.  */
GEM_API_FAR void warn(__const char *__format, ...);
extern void vwarn(__const char *__format, __gnuc_va_list);

/* Likewise, but without ": " and the standard error string.  */
extern void warnx(__const char *__format, ...);
extern void vwarnx(__const char *__format, __gnuc_va_list);

/* Likewise, and then exit with STATUS.  */
extern void err(int __status, __const char *__format, ...);
extern void verr(int __status, __const char *__format, __gnuc_va_list);
extern void errx(int __status, __const char *__format, ...);
extern void verrx(int __status, __const char *, __gnuc_va_list);

__END_C_DECLS

#endif  // #ifndef __win_err_h___
