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
 * suffix array for keyword extraction header file
 * $Id: /mirror/yasa/trunk/yasa/suffix.h 105 2008-07-01T03:36:56.977350Z clsung  $
 */
#ifndef _SUFFIX_H_
#define _SUFFIX_H_
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "char.h"
#include "compare.h"

/* TODO
 * UTF8 checking
 * http://en.wikipedia.org/wiki/UTF-8
 */
int comlen (const char *, const char *);
int do_suffix (char *, int, int, int, wchar_t **);
#endif // _SUFFIX_H_
