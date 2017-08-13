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
 * $Id: /mirror/yasa/trunk/yasa/compare.c 124 2009-02-17T06:20:47.461621Z clsung  $
 */
#include "compare.h"
#include "word_freq.h"
#include <string.h>
#include <assert.h>

#define MAXLENGTH 4096
static char buf1[MAXLENGTH];
static char buf2[MAXLENGTH];
int
compareC (const void * va, const void * vb) {
    char ** a = (char **)va;
    char ** b = (char **)vb;
    return strcmp(*a,*b);
}
int
compareW (const void * va, const void * vb) {
    WORD_FREQ * a = (WORD_FREQ *)va;
    WORD_FREQ * b = (WORD_FREQ *)vb;
    assert (a->word && a->wend && b->word && b->wend);
    strncpy (buf1,a->word,a->wend-a->word);
    assert ( (a->wend-a->word) < MAXLENGTH);
    buf1[a->wend-a->word] = '\0';
    assert ( (b->wend-b->word) < MAXLENGTH);
    strncpy (buf2,b->word,b->wend-b->word);
    buf2[b->wend-b->word] = '\0';
    return strcmp(buf1,buf2);
}

int
compareW2 (const void * va, const void * vb) {
    WORD_FREQ * a = (WORD_FREQ *)va;
    WORD_FREQ * b = (WORD_FREQ *)vb;
    if (a->freq > b->freq)
        return -1;
    else if (a->freq < b->freq)
        return 1;
    return compareW(va,vb);
}

int
compareW3 (const void * va, const void * vb) {
    WORD_FREQ * a = (WORD_FREQ *)va;
    WORD_FREQ * b = (WORD_FREQ *)vb;
    assert (a->wend && b->wend);
    if (a->wend > b->wend)
        return -1;
    else if (a->wend < b->wend)
        return 1;

    // else
    assert (a->wend && b->wend);
    if ( (a->wend - a->word) > (b->wend - b->word))
        return -1;
    else if ( (a->wend - a->word) < (b->wend - b->word))
        return 1;
    return 0;
}

int
compareW4 (const void * va, const void * vb) {
    WORD_FREQ * a = (WORD_FREQ *)va;
    WORD_FREQ * b = (WORD_FREQ *)vb;
    strncpy (buf1,a->wend-3,3);
    buf1[3] = '\0';
    strncpy (buf2,b->wend-3,3);
    buf2[3] = '\0';
    if (0 != strcmp(buf1,buf2)) return strcmp(buf1,buf2);
    assert (a->wend && b->wend);
    if ( (a->wend - a->word) > (b->wend - b->word))
        return 1;
    else if ( (a->wend - a->word) < (b->wend - b->word))
        return -1;
    return 0;
}
