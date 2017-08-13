/*
 * CDDL HEADER START
 *
 * The contents of this file are subject to the terms of the
 * Common Development and Distribution License (the "License").
 * You may not use this file except in compliance with the License.
 *
 * You can obtain a copy of the license at usr/src/OPENSOLARIS.LICENSE
 * or http://www.opensolaris.org/os/licensing.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL HEADER in each
 * file and include the License file at usr/src/OPENSOLARIS.LICENSE.
 * If applicable, add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your own identifying
 * information: Portions Copyright [yyyy] [name of copyright owner]
 *
 * CDDL HEADER END
 */
/*
 * Portions Copyright [2007] [Cheng-Lung Sung]
 */
/*
 * mmap, allocate memory with file
 * $Id: /mirror/yasa/trunk/yasa/mmap.c 122 2008-07-10T08:24:05.753474Z clsung  $
 */
#include "mmap.h"
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#if defined (_WIN32) && !defined (__CYGWIN__)
MmapHandles * mmap_handle;
#endif
extern FILE * debugfile;
extern int mmap_len;
char * mmap_open_file (const char * file_name)
{
    char *mmap_ptr;
#if defined (_WIN32) && !defined (__CYGWIN__)
    MmapHandles tmp;
    int len;
    
	tmp.hFile = CreateFileA(file_name, GENERIC_READ, FILE_SHARE_READ,
	    NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (tmp.hFile == INVALID_HANDLE_VALUE) {
		fprintf (debugfile,"MMAP Create File Error: %d\n",GetLastError());
		return NULL;
	}
    len = GetFileSize(tmp.hFile, NULL);

    tmp.hMap = CreateFileMapping(tmp.hFile, NULL, PAGE_READONLY, 0, 0, NULL);
    if (tmp.hMap == NULL) {
        CloseHandle(tmp.hFile);
        return NULL;
    }

    mmap_ptr = (char*)MapViewOfFile(tmp.hMap, FILE_MAP_READ, 0, 0, 0);
    if (mmap_ptr == NULL) {    
	CloseHandle(tmp.hFile);
	CloseHandle(tmp.hMap);
    }

    if (!mmap_handle)
	mmap_handle = (MmapHandles*)malloc (sizeof(MmapHandles));
    mmap_handle->hFile = tmp.hFile;
    mmap_handle->hMap  = tmp.hMap;
#else
    int fd;
    struct stat stat;
    if ( ( (fd = open (file_name, O_RDONLY)) < 0 ) || (fstat(fd, &stat) < 0) )  {
	fprintf (debugfile,"MMAP Create File Error: %d\n",errno);
	fflush (NULL);
	return NULL;
    }
    mmap_ptr = (char *)mmap(0 /* select by system */,
	stat.st_size /* len */,
	PROT_READ /* Read */,
	MAP_SHARED /* Modifications are shared */,
	fd /* file pointer */,
	0 /* offset */
	);
    mmap_len = stat.st_size;
    close(fd);
    if (MAP_FAILED == mmap_ptr) {
	fprintf (debugfile,"MMAP Error: %d\n",errno);
	fflush (NULL);
	return NULL;
    }
#endif
    return mmap_ptr;
}

int mmap_close (char * mmap_ptr, int size)
{
#if defined (_WIN32) && !defined (__CYGWIN__)
    CloseHandle(mmap_handle->hFile);
    CloseHandle(mmap_handle->hMap);
    free (mmap_handle);
	mmap_handle = 0;
    return UnmapViewOfFile(mmap_ptr);
#else 
    return munmap(mmap_ptr, size);
#endif
}

