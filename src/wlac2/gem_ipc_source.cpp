/*
 *	File: gem_ipc_source.cpp
 *
 *	Created on: Dec 23, 2015
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *  This file implements all functions connected to posix threading
 *		1) 
 *
 *
 */

#include "stdafx.h"
#include "common_include_for_wlac_sources.h"
#include <sys/time.h>
#include <io.h>
#include <malloc.h>
#include "redesigned/errno.h"
#include <ctype.h>
#include "sys/ioctl.h"
#include <net/if.h>
#include <pthread.h>
#include <redesigned/sys/types.h>
#include <sys/ipc.h>

__BEGIN_C_DECLS

#define		PLACE_FOR_MEM_SIZE	8

GEM_API segment_t shmget(key_t a_key, size_t a_size, int a_shmflg)
{
	HANDLE hShared;
	char* pcShMemName = (char*)alloca(IPC_STRING_DEF_LEN+1);

	PREPARE_STRING(pcShMemName,a_key, a_size, a_shmflg);
	
	hShared = OpenFileMappingA((DWORD)a_shmflg, TRUE, pcShMemName);
	if (hShared == NULL)return (segment_t)-1;
	return (segment_t)hShared;
}


GEM_API int shmctl(segment_t a_shmid, int a_cmd, struct shmid_ds *a_buf)
{
	switch (a_cmd)
	{
	case IPC_RMID:
		CloseHandle((HANDLE)a_shmid);
		break;
	default:
		break;
	}
	return 0;
}


GEM_API void *shmat(segment_t a_shmid, const void *shmaddr, int shmflg)
{
	size_t unSize;
	char* pcRet = (char*)MapViewOfFile((HANDLE)a_shmid, shmflg, 
		0,
		0,
		PLACE_FOR_MEM_SIZE);
	if (!pcRet) return (void*)-1;

	memcpy(&unSize, pcRet + 4, sizeof(size_t) < PLACE_FOR_MEM_SIZE ? sizeof(size_t) : PLACE_FOR_MEM_SIZE);
	UnmapViewOfFile((HANDLE)a_shmid);
	a_shmid = 0;
	if (!unSize)return (void*)-1;

	pcRet = (char*)MapViewOfFile((HANDLE)a_shmid, shmflg,
		0,
		0,
		unSize+PLACE_FOR_MEM_SIZE);
	if (!pcRet) return (void*)-1;

	return pcRet;
}


GEM_API int shmdt(const void *shmaddr)
{
	if (!UnmapViewOfFile(shmaddr)) return -1;
	return 0;
}


__END_C_DECLS
