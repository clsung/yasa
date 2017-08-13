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
 * character handling functions
 * $Id: /mirror/yasa/trunk/yasa/char.h 76 2008-05-12T04:09:11.566471Z clsung  $
 */
#ifndef _CHAR_H_
#define _CHAR_H_
#include <stdio.h>
#include <ctype.h>

// self-defined definition of alphabet
int ismyalpha (const char);
// move forward to next english word
char * moveforward_by_english_word (const char *, const char *);
// move forward
char * moveforward (const char *, const char *);
/* TODO
 * UTF8 checking
 * http://en.wikipedia.org/wiki/UTF-8
 */
int validUTF8ChineseWord (const char*);

int validUTF8ChineseSymbol (const char*);

// to make sure string pointed by const char* with length int are all UTF8 Chinese
int validAllUTF8ChineseWord (const char*, const int);

// to make sure ?
int validUTF8Word (const char*, const int);

// to make sure ?
int validUTF8 (const char*, const int);
int trim2validUTF8ChineseWord (const char*, int, int);
int trim2validUTF8 (const char*, int);

// trim char* to valid English word
int trim2validEnglishPhrase (const char*, int);

int trim2validPhrase (const char*, int );

// retrieve common prefix length
int comlen (const char *, const char *);

/*  UTF-8 range as following:
 *
 *  Char. number range  |        UTF-8 octet sequence
 *  (hexadecimal)    |              (binary)
 *  --------------------+---------------------------------------------
 *  0000 0000-0000 007F | 0xxxxxxx
 *  0000 0080-0000 07FF | 110xxxxx 10xxxxxx
 *  0000 0800-0000 FFFF | 1110xxxx 10xxxxxx 10xxxxxx
 *  0001 0000-0010 FFFF | 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
 */
/* A14  　 ， 、 。 ． • ； ： ？ ！ ︰ … ‥ ﹐ ､ ﹒
 * A15  · ﹔ ﹕ ﹖ ﹗ ｜ – ︱ — ︳ ▁ ︴ ﹏ （ ） ︵
 * A16  ︶ ｛ ｝ ︷ ︸ 〔 〕 ︹ ︺ 【 】 ︻ ︼ 《 》 ︽
 * ...
 * A44  一 乙 丁 七 乃 九 了 二 人 儿 入 八 几 刀 刁 力
 */

// return the UTF8 length of current char pointer
int UTF8Length(const char*);
#endif // _CHAR_H_
