// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <libgen.h> For WINDOWS MFC
 *
 *	Created on: May 6, 2016
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *
 */
/* Copyright (C) 1996, 1997, 1999, 2000 Free Software Foundation, Inc.
This file is part of the GNU C Library.

The GNU C Library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

The GNU C Library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with the GNU C Library; if not, write to the Free
Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
02111-1307 USA.  */

#ifndef _LIBGEN_H
#define _LIBGEN_H	1

#include <first_includes/common_include_for_headers.h>
#include <features.h>

__BEGIN_C_DECLS

/* Return directory part of PATH or "." if none is available.  */
GEM_API_FAR char *dirname(char *__path) __THROW;

/* Return final component of PATH.

This is the weird XPG version of this function.  It sometimes will
modify its argument.  Therefore we normally use the GNU version (in
<string.h>) and only if this header is included make the XPG
version available under the real name.  */
GEM_API_FAR char *__xpg_basename(char *__path) __THROW;
#define basename	__xpg_basename

__END_C_DECLS

#endif /* libgen.h */
