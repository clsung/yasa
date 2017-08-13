#include "char.h"

int
validUTF8ChineseWord (const char* word) {
    unsigned int uByte = word[0];
//   unsigned int mByte = word[1];
    unsigned int lByte = word[2];
    // 一 ==> e4 b8 80
    // ?? ==> e9 be a5
    if (
        ( (uByte >= '\xe4') && (uByte <= '\xe9')) &&
        ( (lByte >= '\x80') && (lByte <= '\xbf'))
    ) // &&
        return 1;
    return 0;
}

// http://www.utf8-chartable.de/unicode-utf8-table.pl
// This is kind of ugly, we should take care of all CJKV words...
// but for effeciency, just get Chinese words
int
validUTF8ChineseSymbol (const char* word) {
    unsigned int uByte = word[0];
    unsigned int lByte = word[1];
    unsigned int lByte2 = word[2];
    if (
        (  (uByte == '\xe3') &&
           ( (lByte >= '\x80') && (lByte2 >= '\x81') && (lByte2 <= '\x9f'))
        )
        ||
        (  (uByte == '\xef') &&
           ( (lByte >= '\xbc') && (lByte2 >= '\x81') && (lByte2 <= '\xa3'))
        )
    )
        return 1;
    return 0;
}

int
validAllUTF8ChineseWord (const char* word, const int len) {
    char *eof = (char *)word+len;
    char *cursor = (char*)word;// = moveforward(word, eof);
    while (cursor != NULL && (validUTF8ChineseWord (cursor) || validUTF8ChineseSymbol (cursor))&& cursor < eof) {
        cursor = moveforward(cursor, eof);
    }
    return (cursor == eof);
}

int
trim2validPhrase (const char* word, int len) {
    char * cursor = (char *) word;
    int i;
    for (i = 0; i < len; i++) {
        if (cursor[i] == '\r' || cursor[i] == '\n')
            len = i;
    }
    while (len > 0 && (cursor[len-1] == ' ' || !validUTF8(word,len))) {
        len--;
    }
    return len;
}

int
trim2validUTF8ChineseWord (const char* word, int len, int max_length) {
    if (len > max_length) len = max_length;
    while (len > 0 && !validAllUTF8ChineseWord(word,len)) {
        len--;
    }
    return len;
};

int
validUTF8Word (const char* word, const int len) {
    char *eof = (char *)word+len;
	char *cursor = moveforward(word, eof);
    if (word[0] == '\r' || word[0] == '\n')
        return 0;
    return (cursor != NULL);
}
int
validUTF8 (const char* word, const int len) {
    char *eof = (char *)word+len;
    char *cursor = moveforward(word, eof);
    while (cursor != NULL && cursor < eof) {
        cursor = moveforward(cursor, eof);
    }
    return (cursor == eof);
}
int
trim2validUTF8 (const char* word, int len) {
    while (len > 0 && !validUTF8(word,len)) {
        len--;
    }
    return len;
}

/*
 * Input:   given a character 'c'
 * Algo:    check if it is alphabet, or (, ), -.
 * Output:  1 if satisfied, otherwise 0.
 */
int
ismyalpha (const char c) {
    return (c != ' ');
    /*
    return (isalnum(c) 
     || 
     c == ')' || c == '(' || c == '-' || c == '_' || c == '/'
     ||
     c == '+' || c == '.' || c == '\'' || c == '%'
     );
    */
}

int
trim2validEnglishPhrase (const char* word, int len) {
    int newlen = len;
    //char buf[250];

//    while (newlen > 0 && !(iscntrl(word[newlen]) || isspace(word[newlen])) ) {
    while (newlen > 0 && ismyalpha(word[newlen-1]) ) {
        newlen--;
    }
    if (newlen == 0 && !ismyalpha(word[len])) {// only one word
        newlen = len;
    }
    while (newlen > 0 && !ismyalpha(word[newlen-1])) { //(iscntrl(word[newlen-1]) || isspace(word[newlen-1])) ) {
        newlen--;
    }
    return newlen;
};

/*
 * Input:   text point to current text position
 * Output:  UTF-8 length of the text point 
 */
int UTF8Length(const char * p) {
    if ((unsigned int)p[0] < '\x80') {
        return 1;
    } else if ((p[0] & 0xe0) == 0xc0) {
        return 2;
    } else if ((p[0] & 0xf0) == 0xe0) {
        return 3;
    } else if ((p[0] & 0xf8) == 0xf0) {
        return 4;
    } else if ((p[0] & 0xfc) == 0xf8) {
        return 5;
    } else if ((p[0] & 0xfe) == 0xfc) {
        return 6;
    } else {
        /* invalid character */
        return 0;
    }
}

/*
 * Input:   text point to current text position, text's End-of-File
 * Algo:    first go through current word (1st while),
 *	    then go through every space/control char (2nd while),
 *	    finally go through every non-alphabet (3rd while)
 * Output:  pointer to next english word, or NULL if
 *	    we reach End-of-File
 */
char *
moveforward_by_english_word (const char * text, const char *eof) {
    char *cursor = (char *) text;
    int len = 0;
    while ( isalpha(cursor[len]) && cursor + len < eof)
        len++;
    while ( (iscntrl(cursor[len]) || isspace(cursor[len])) && cursor + len < eof)
        len++;
    while ( !isalpha(cursor[len]) && cursor + len < eof)
        len++;
    cursor += len;
    if (cursor >= eof)
        return NULL;
    else
        return cursor;
}

char *
moveforward (const char * text, const char *eof) {
    char *cursor = (char *) text;
    int len = UTF8Length(text);
    if (0 == len || (cursor + len) > eof)
        return NULL; // invalid character

    cursor += len;
    return cursor;
}

/*
 * Input:   string a, string b
 * Output:  common length of a and b
 */
int comlen(const char * a, const char *b) {
    int i = 0;
    while (
        (i == 0 || !(a[i-1] == '\r' || a[i-1] == '\n'))
        &&
        (a[i] == b[i]
         ||
#ifdef _WIN32
         (iswspace(a[i]) && iswspace(b[i]))
#else
         (isspace(a[i]) && isspace(b[i]))
#endif
        )
    ) i++;
    return i;
}
