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
 * WORD_FREQ header file
 * $Id: /mirror/yasa/trunk/yasa/word_freq.h 40 2008-04-16T06:40:38.016294Z clsung  $
 */
#ifndef _WORD_FREQ_H_
#define _WORD_FREQ_H_
struct WORD_FREQ {
    char *word;
    char *wend; // point to the end of word
    unsigned int freq;
};
typedef struct WORD_FREQ WORD_FREQ;
/* 20041109 for lookupWordIndex
 * this static cursor can remember the end of WORD_FREQ
 */
int lookupWordIndex (WORD_FREQ * /*word_array*/, const char* /*word*/, const int /*len*/);
/* 20041112 for lookupWordIndexForFreq */
unsigned int lookupWordIndexForFreq (WORD_FREQ * /*word_array*/, WORD_FREQ * /*c_cursor*/);

#define ASCII	0x00000001
#define UTF8	0x00000010
#endif // _WORD_FREQ_H_
