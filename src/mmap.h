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
 * mmap header file
 * $Id: /mirror/yasa/trunk/yasa/mmap.h 40 2008-04-16T06:40:38.016294Z clsung  $
 */
#ifndef _MMAP_H_
#define _MMAP_H_
#include <fcntl.h>
#include <sys/stat.h> // for stat
#if defined (_WIN32) && !defined (__CYGWIN__)
#  include <windows.h>
typedef struct {
    HANDLE hFile;
    HANDLE hMap;
} MmapHandles;
#else
#  include <sys/mman.h> // for mmap
#  include <unistd.h>   // for open, close
#endif
char * mmap_open_file (const char * /*file_name*/);
int mmap_close (char * /*mmap_ptr*/, int /*size*/);
#endif // _MMAP_H_
