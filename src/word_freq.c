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
 * WORD_FREQ function
 * $Id: /mirror/yasa/trunk/yasa/word_freq.c 40 2008-04-16T06:40:38.016294Z clsung  $
 */
#include "char.h"
#include "word_freq.h"
#include <string.h>

/* 20041112
 * I'd like to add another lookupWordIndexForFreq
 * This is based on the discussion with cana/aska 20041112 morning :)
 * Why don't we skip store information from 1....N ?
 * just store N :)
 */
WORD_FREQ *e_cursor;
extern FILE* debugfile;

int wt_flag; // Word type flag, 0001 -> ascii, 0010 -> utf8
int
lookupWordIndex (WORD_FREQ * word_array, const char* word, const int len) {
    WORD_FREQ *cursor = e_cursor;
    int cursor_length;
    // replace int i = 0 by cursor - word_array
    /* 20041109	Why we always search from begin?
     * Theoretically, words with common prefix should clustered near end 
     * So I'd like to rewrite to search from end.
     * i.e. replace cursor = word_array; with e_cursor = word_array.end //pseudo code
     */
    if (e_cursor == 0)	return 0;
    cursor_length = cursor->wend - cursor->word;
    if (len > cursor_length) return (e_cursor-word_array+1);
    while (cursor >= word_array) { // at least we won't search over :-)
        cursor_length = cursor->wend - cursor->word;
        if (len < cursor_length) {
            cursor--;
            continue;
        }
        /* 20041112
         * 很好笑, 這兩行 strncmp (cursor->length 和 len) 的執行跑起來會相反 :p
         * 一個是 insert 會很快, Find Freq 很慢 (cursor->length) , 而這行 code 是不正確的
        // Here we have to compare cursor->word and word by length of len, not cursor->length -_-
         * 而 (len) 是 insert 慢, 因為要找 wi, 但 Find Freq 就快了...
         * 加起來的時間差不多，看來我得要在這裡用更快的 search 方式
         * 也許可以用 bsearch :)
         * 20041112
         * 最後我採取先檢查長度，如果最後一個 (cursor->word) 比要檢查的 word 的 len 還小
         * 那我們就可以確定 (因為這是 sorted array) 不會有了
         * 因該是這樣 :)
         */
        if (len > cursor_length || strncmp(cursor->word,word,len)) { // not equal
            return (e_cursor-word_array+1); // move forward
        }
        /* 20041112
         * if we can pass the above "if" statement,
         * that means we are in that word clusters
         * then we only need to check cursor->length first :)
         */
        /* The following code is bad, since we only want to compare the
         * real string, not just the pointer.
         * For Example, 玩去玩 
         *               0 1 2
         * here 0 != 2, but 玩 == 玩
         * if (cursor->word == word && cursor->length == len) {
         */
        if (cursor_length == len && !strncmp(cursor->word,word,len)) {
            return (cursor-word_array); // do we need to devide sizeof(WORD_FREQ)?
            // the anwser is no :p their distance are 1 2 3 4... :)
        }
        cursor--;
    }
    fprintf(debugfile, "We have go thourgh the whole word_array!!\n");
    fflush(NULL);
    return (e_cursor-word_array+1);
}

unsigned int
lookupWordIndexForFreq (WORD_FREQ * word_array, WORD_FREQ * c_cursor) {
    WORD_FREQ *cursor = c_cursor;
    int c_cursor_length = c_cursor->wend - c_cursor->word;
    unsigned int toAddFreq = 0;
    if (cursor == e_cursor) return 0;
    while (cursor <= e_cursor ) { // at least we won't search over :-)
        int cursor_length = cursor->wend - cursor->word;
        // test if corsor->word != word (in the length not equal)
        if (!cursor_length || strncmp(c_cursor->word,cursor->word,c_cursor_length)) {
            break;
        }
        if ((wt_flag & ASCII) && ismyalpha(cursor->word[c_cursor_length]))  {
            break;
        }
        toAddFreq += (cursor->freq-1); // -1, since every freq start from 2
        cursor++;
    }
    toAddFreq -= (c_cursor->freq - 1); // don't add myself
    return toAddFreq;
}

