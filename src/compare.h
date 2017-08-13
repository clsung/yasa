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
 * compare function for sorting
 * $Id: /mirror/yasa/trunk/yasa/compare.h 40 2008-04-16T06:40:38.016294Z clsung  $
 */
#ifndef _COMPARE_H_
#define _COMPARE_H_
int compareC (const void *, const void *);
int compareW (const void *, const void *);
int compareW2 (const void *, const void *);
int compareW3 (const void *, const void *); // sort by end of word
int compareW4 (const void *, const void *); // sort by freq > 0
#endif // _COMPARE_H_
