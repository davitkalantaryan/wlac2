/*
 *	File: <company_def_file.h> For WINDOWS MFC
 *
 *	Created on: Dec 29, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 * //
 *
 *
 */
#ifndef __company_def_file_h__
#define __company_def_file_h__

#ifndef COMP_LIBRARY_PATH
#ifdef _WIN64
#define	COMP_LIBRARY_PATH "//afs/ifh.de/group/pitz/doocs/lib/"
#else
#define	COMP_LIBRARY_PATH "//afs/ifh.de/group/pitz/doocs/lib/s32bit/"
#endif
#endif  // #ifndef COMP_LIBRARY_PATH

#endif  /* #ifndef __company_def_file_h__ */
