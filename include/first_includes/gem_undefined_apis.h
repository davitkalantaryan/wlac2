// 
// (c) 2015-2018 WLAC. For details refers to LICENSE.md
//

/*
 *	File: <first_includes\gem_undefined_apis.h> For WINDOWS 
 *
 *	Created on: Oct 20, 2016
 *	Author    : Davit Kalantaryan (Email: davit.kalantaryan@desy.de)
 *
 *  This is the include file that is included by all gem related headers
 *
 */


#ifndef __gem_undefined_apis_h__
#define __gem_undefined_apis_h__

#include <wlac2_internal_header.h>
#include <first_includes/common_definations_wul.h>
#include <first_includes/gem_first_common_include.h>
#include <malloc.h>
#include <stdio.h>
#include <stdarg.h>

#ifndef UNDEFINED_APIS
#define UNDEFINED_APIS
#define START_UNDEFINED_APIS
#define END_UNDEFINED_APIS
#endif

#ifndef __const
#define __const const
#endif

__BEGIN_C_DECLS

GEM_VAR_FAR int kb_main_total;
GEM_VAR_FAR int kb_main_free;
GEM_VAR_FAR int kb_main_cached;
GEM_VAR_FAR int kb_swap_total;
GEM_VAR_FAR int kb_swap_free;
GEM_VAR_FAR int g_numProcessors;

GEM_API_FAR void uptime(double* uptime_secs, double* idle_secs);
GEM_API_FAR void meminfo(void);


START_UNDEFINED_APIS

#ifndef HOST_NAME_MAX
#define	HOST_NAME_MAX	128
#endif

struct disk_stat
{
	unsigned long reads;
	unsigned long merged_reads;
	unsigned long reads_sectors;
	unsigned long milli_reading;
	unsigned long writes;
	unsigned long merged_writes;
	unsigned long written_sectors;
	unsigned long milli_writing;
	unsigned long inprogress_IO;
	unsigned long milli_spent_IO;
};

struct partition_stat
{
	int m_s1;
};


static _inline int statfs(__const char *__file, struct statfs *__buf)
{
	WLAC2_STATIC_CAST(void,__file);
	WLAC2_STATIC_CAST(void,__buf);
	return 0;
}

static _inline int getdiskstat(struct disk_stat** disks, struct partition_stat** partitions)
{
	*partitions = (struct partition_stat*)malloc(sizeof(struct partition_stat));
	//*disks = &s_disk_stat;
	*disks = (struct disk_stat*)malloc(sizeof(struct disk_stat));
	return 1;
}

static _inline int loadavg(double *a_arg1, double *a_arg2, double *a_arg3)
{
	*a_arg1 = 3;
	*a_arg2 = 4;
	*a_arg3 = 5;
	return 0;
}

END_UNDEFINED_APIS

__END_C_DECLS

#endif  // #ifndef __gem_undefined_apis_h__
