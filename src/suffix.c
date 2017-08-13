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
 * suffix array for keyword extraction
 * $Id: /mirror/yasa/trunk/yasa/suffix.c 148 2009-09-21T12:40:20.395698Z clsung  $
 */
#include "suffix.h"
#include "mmap.h"
#include "word_freq.h"
#ifndef WINVER
#include <unistd.h>
#include <err.h>
#endif
#include <errno.h>
#include <time.h>
#include <string.h>

static char buf[1024];
static char *bof;
FILE * debugfile;
extern WORD_FREQ *e_cursor;
extern int wt_flag;
extern int prearr_len;
extern int postarr_len;
extern int mmap_len;
int do_suffix (char *str_ptr, int utf8_enable, int max_length, int min_length, wchar_t **out_ptr) {
    time_t start,end,p_s,p_e;
    clock_t s_c, e_c; // unsigned long
    double i_c[10]; // i_c => interval of each process

    unsigned int word_freq_max_size; // size of word_freq allocated
    char *word_ptr;
    void *v_ptr;
    char *eof;
    char *mmap_ptr = str_ptr;
    unsigned int per = 0; // %
    unsigned int len;
    unsigned int j = 0;
    unsigned int i = 0;
    unsigned int wi = 0;
    unsigned int maxlen = 0;
    unsigned int last_length = 0;
    char **chararray;

    int dflag, uflag = 1; // default utf8

    WORD_FREQ *word_freq;

    dflag = 0;
    uflag = utf8_enable;

    p_s = time(NULL);
    s_c = clock();
    mmap_ptr = str_ptr;
    if (!mmap_ptr) return errno;
    e_c = clock();
    i_c[0] = (double)(e_c - s_c) / CLOCKS_PER_SEC;
    word_ptr = mmap_ptr;
    len = mmap_len;
    eof = mmap_ptr + len;
    bof = mmap_ptr;

    fprintf (debugfile,"sizeof char is %ld, sizeof int is %ld\n",sizeof(char),sizeof(int));
    fprintf (debugfile,"bof is %d, eof is %d, size is %d\n",bof,eof,len);
    fflush(NULL);
    s_c = clock();

    if (uflag != 0) {
        wt_flag = 0x00000010;
        fprintf (debugfile,"UTF8 mode\n");
    } else  {
        wt_flag = 0x00000001;
        fprintf (debugfile,"ASCII mode\n");
    }

    if (wt_flag & UTF8) {
        min_length *=3;
        max_length *=3;
    }
    fprintf (debugfile, "min char length is %d, max char length is %d\n",min_length, max_length);
    while (word_ptr < eof) {
        if (wt_flag & UTF8 && (word_ptr+2 < eof)) {
            if (validUTF8Word(word_ptr,eof) &&
#ifdef _WIN32
            (!iswspace(word_ptr[0]))) { // words do not begin with ' '
#else
            (!isspace(word_ptr[0]))) { // words do not begin with ' '
#endif
                i++;
            }
            word_ptr = moveforward(word_ptr,eof);
        } else {
            if (ismyalpha(word_ptr[0])) { // words do not begin with ' '
                i++;
            }
            word_ptr++;
        }
    }

    fprintf (debugfile, "initial i is %d\n",i);
    chararray = (char**)malloc(i * sizeof(char*));
    if (!chararray) {
        printf ("Allocate memory for chararray (size is %d) Error: %d\n",len/3,errno);
        fflush (NULL);
        return errno;
    }
    i = 0;
    word_ptr = mmap_ptr;
    while (word_ptr < eof) {
        if (wt_flag & UTF8  && (word_ptr+2 < eof)) {
            if (validUTF8Word(word_ptr,eof) &&
#ifdef _WIN32
            (!iswspace(word_ptr[0]))) { // words do not begin with ' '
#else
            (!isspace(word_ptr[0]))) { // words do not begin with ' '
#endif
                // prefix word specify in here
                chararray[i] = word_ptr;
                i++;
            }
            word_ptr = moveforward(word_ptr,eof);
        } else {
#ifdef _WIN32
            if (!iswspace(word_ptr[0])) { // words do not begin with ' '
#else
            if (!isspace(word_ptr[0])) { // words do not begin with ' '
#endif
                if (
                    word_ptr == bof // we skip check bof
                    ||
#ifdef _WIN32
                    iswspace(*(word_ptr-1))
#else
                    isspace(*(word_ptr-1))
#endif
                )  {
                    chararray[i] = word_ptr;
                    i++;
                }
            }
            word_ptr++;
        }
    }

    e_c = clock();
    i_c[1] = (double)(e_c - s_c) / CLOCKS_PER_SEC;
    fprintf (debugfile,"size of char * is %ld, size of chararray is %ld, size of word_freq is %ld\n", sizeof (char*),i, sizeof(WORD_FREQ));
    fflush(NULL);
    word_freq_max_size = i/4;// initial
    v_ptr = calloc(word_freq_max_size , sizeof(WORD_FREQ));
    if (!v_ptr) {
        printf ("Allocate memory for word_freq Error: %d\n",errno);
        fflush (NULL);
        free (chararray);
        return errno;
    }
    word_freq = (WORD_FREQ *)v_ptr;
    e_cursor = 0;

    assert(i>0);
    fprintf (debugfile,"Begin to qsort\n");
    start = time(NULL);
    fflush(NULL);
    s_c = clock();
    qsort (chararray,i,sizeof(char*),compareC);
    e_c = clock();
    i_c[2] = (double)(e_c - s_c) / CLOCKS_PER_SEC;
    end = time(NULL);
    fprintf (debugfile,"Qsort done: %d sec\n",(int)(end-start));
    start = time(NULL);
    fflush(NULL);

    s_c = clock();
    for (j = 0; j < i-1; j++) {
        int cl = comlen(chararray[j],chararray[j+1]);
        int length;
        if (cl < min_length) continue;
        word_ptr = chararray[j];
        if (((j*100)%i) == 0) {
            if ((j*100)/i > per) {
                per = (j*100)/i;
                fprintf(debugfile,"P: %d %%\n",per);
                fflush (NULL);
            }
        }
        if (wt_flag & UTF8) {
            if (cl > max_length) cl = max_length;
            cl = trim2validPhrase(word_ptr,cl);
        } else {
            cl = trim2validEnglishPhrase(word_ptr,cl);
        }
        if (cl == 0) continue;
        if (wi > i) {
            fprintf (debugfile, "wi Full!! wi is %d\n",wi);
            fflush(NULL);
            break;
        }

        wi = lookupWordIndex (word_freq,word_ptr,cl);
        if (0 && (wi % 10000) == 0) {
            fprintf (debugfile, "Wi is %d ( j is %d, i is %d )\n",wi,j,i);
            fflush(NULL);
        }
        word_freq[wi].word = word_ptr;
        length = cl;
        word_freq[wi].wend = word_ptr + cl;
        if (0) {
            strncpy (buf,word_freq[wi].word,length);
            buf[length] = '\0';
            printf("%d,%d, gagabuf is %s,%d,%d\n",&word_freq[wi],wi,buf,word_freq[wi].freq,cl);
            fflush(NULL);
        }
        maxlen = length > maxlen ? length : maxlen;
        if (word_freq[wi].freq == 0)
            word_freq[wi].freq = 2;
        else  {
            word_freq[wi].freq++;
        }
        if (&word_freq[wi] > e_cursor) {
            e_cursor = &word_freq[wi];
        }
        if ((wi+1) == word_freq_max_size) { // here we need to double our word_freq size
            word_freq_max_size*=2;
            word_freq = (WORD_FREQ*)realloc(word_freq,word_freq_max_size * sizeof (WORD_FREQ));
            if (!word_freq) {
                fprintf(debugfile, "realloc for word_freq failed\n");
                fflush(NULL);
                return errno;
            }
            memset(word_freq+word_freq_max_size/2, 0, (word_freq_max_size/2) * sizeof(WORD_FREQ));
            fprintf(debugfile, "word_freq_max_size become %d\n",word_freq_max_size);
            fflush(NULL);
            e_cursor = &word_freq[wi]; // reset e_cursor;
        }
    }
    fprintf (debugfile,"Free chararray\n");
    fflush(NULL);
    free (chararray);
    fprintf (debugfile,"Freeed chararray\n");
    fflush(NULL);
    e_c = clock();
    i_c[3] = (double)(e_c - s_c) / CLOCKS_PER_SEC;
    fprintf (debugfile,"maxlen is %d\n",maxlen);
    fflush(NULL);

    s_c = clock();
    word_freq = (WORD_FREQ*) realloc(word_freq,(wi+1) * sizeof(WORD_FREQ)); // shrink to fit actuall size
    if (!word_freq) {
        fprintf(debugfile, "realloc for word_freq failed\n");
        fflush(NULL);
        return errno;
    }
    fprintf(debugfile, "word_freq size from %d become %d\n",word_freq_max_size,wi);
    fflush(debugfile);
    wi++; // ya, 0..wi means wi+1
    if (0) {
        for (j = 0; j < wi; j++) {
            int length = word_freq[j].wend-word_freq[j].word;
            assert (word_freq[j].freq >= 0);
            strncpy(buf, word_freq[j].word,length);
            buf[length] = '\0';
            printf ("%d/%d,beforeword is %s,%d,%d\n",j,wi,buf,length,word_freq[j].freq);
            fflush(NULL);
        }
    }

    fprintf (debugfile,"qsorting by compareW\n");
    fflush(NULL);
    qsort (word_freq,wi,sizeof(WORD_FREQ),compareW);
    fprintf (debugfile,"set e_cursor\n");
    fflush(NULL);
    e_cursor = &word_freq[wi-1]; // since we need e_cursor for FindFreq
    e_c = clock();
    i_c[4] = (double)(e_c - s_c) / CLOCKS_PER_SEC;
    i = 0;
    fprintf(debugfile,"Now looking for frequency\n");
    fflush(NULL);
    s_c = clock();
    per = 0;

    for (j = 0; j < wi; j++) {
        word_freq[j].freq += lookupWordIndexForFreq (word_freq,&word_freq[j]);
        if (0 && (j*1000)/wi > per) {
            per = (j*1000)/wi;
            fprintf(debugfile, "FindFreq: %2f %% j is %d, wi is %d\n",(double)per/10,j,wi);
            fflush (NULL);
        }
    }
    e_c = clock();
    i_c[5] = (double)(e_c - s_c) / CLOCKS_PER_SEC;

    s_c = clock();
    qsort (word_freq,wi,sizeof(WORD_FREQ),compareW3);
    e_c = clock();
    i_c[7] = (double)(e_c - s_c) / CLOCKS_PER_SEC;
    s_c = clock();
    for (j = 1, i = j - 1; j < wi; j++) {
        // This function check "abcd,4,2" and "bcd,3,2"
        // then set "bcd,3,2" to "bcd,3,0"
        if (word_freq[j].wend == word_freq[i].wend && word_freq[j].freq == word_freq[i].freq) {
            word_freq[j].freq = 0;
        } else
            i = j; // j will become j+1 later (for loop)
    }
    e_c = clock();
    i_c[8] = (double)(e_c - s_c) / CLOCKS_PER_SEC;

    s_c = clock();
    qsort (word_freq,wi,sizeof(WORD_FREQ),compareW2);
    e_c = clock();
    i_c[9] = (double)(e_c - s_c) / CLOCKS_PER_SEC;
    i = 0;
    s_c = clock();
    if (out_ptr != NULL) {
        last_length = wi * 12;
        *out_ptr = (wchar_t *)calloc(last_length + 1, sizeof(wchar_t)); // average size
    }
    for (j = 0; j < wi; j++) {
        int length = word_freq[j].wend-word_freq[j].word;
        if (word_freq[j].word == NULL) break;
        if (word_freq[j].freq == 0) break;
        if (length < min_length) continue;
        strncpy(buf, word_freq[j].word,length);
        buf[length] = '\0';
        i++;
        assert (word_freq[j].freq >= 0);
        sprintf (buf, "%s\t%d\n",buf,word_freq[j].freq);
        if (out_ptr != NULL) {
            if ((strlen(*out_ptr) + strlen(buf)) >= last_length)  {
                *out_ptr = (wchar_t*) realloc(*out_ptr, (last_length + strlen(buf) + 1) * sizeof(wchar_t));
                last_length += strlen(buf);
            }
            strncat(*out_ptr, buf, strlen(buf));
        } else 
            printf ("%s",buf);
    }
    e_c = clock();
    i_c[6] = (double)(e_c - s_c) / CLOCKS_PER_SEC;
    fflush(NULL);
    free (word_freq);
    end = time(NULL);
    fprintf (debugfile,"Total subwords: %d\n",i);
    fprintf (debugfile,"Substr time: %d\n",(int)(end-start));
    p_e = time(NULL);
    fprintf (debugfile,"Total time: %d\n",(int)(p_e-p_s));
    fprintf (debugfile,"CPU time:\n Read file:%fs\n",i_c[0]);
    fprintf (debugfile,"PointChar Time:%fs\n",i_c[1]);
    fprintf (debugfile,"QSort Time:%fs\n",i_c[2]);
    fprintf (debugfile,"Insert Wi Time:%fs\n",i_c[3]);
    fprintf (debugfile,"QSortWi Time:%fs\n",i_c[4]);
    fprintf (debugfile,"FindFreq Time:%fs\n",i_c[5]);
    fprintf (debugfile,"QSortWi3 Time:%fs\n",i_c[7]);
    fprintf (debugfile,"TrimEndWords Time:%fs\n",i_c[8]);
    fprintf (debugfile,"QSortWi2 Time:%fs\n",i_c[9]);
    fprintf (debugfile,"Print Time:%fs\n",i_c[6]);

    return 1;
}
