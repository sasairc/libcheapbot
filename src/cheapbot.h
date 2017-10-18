/*
 * cheapbot - The simply bot library for C
 *
 * cheapbot.h
 *
 * Copyright (c) 2017 sasairc
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.HocevarHocevar See the COPYING file or http://www.wtfpl.net/
 * for more details.
 */

#ifndef CHEAPBOTT_H
#define CHEAPBOTT_H
#ifdef  __cplusplus
extern "C" {
/* __cplusplus */
#endif

#include <stddef.h>
#include <oniguruma.h>

#ifndef DEFAULT_DICT_SIZE
#define DEFAULT_DICT_SIZE   32
/* DEFAULT_DICT_SIZE */
#endif

#ifndef BOT_ENCODING
#define BOT_ENCODING    ONIG_ENCODING_UTF8
/* BOT_ENCODING */
#endif

typedef struct BOT_MATCH {
    char*           str;
    OnigRegion*     region;
} BOT_MATCH;

typedef struct BOT_PATTERN {
    char*           pattern;
    int             (*func)(BOT_MATCH* match);
} BOT_PATTERN;

typedef struct BOT_DICT {
    regex_t*        regex;
    int             (*func)(BOT_MATCH* match);
} BOT_DICT;

typedef struct ONIG_STAT {
    int             status;
    OnigEncoding    encode;
    OnigErrorInfo   errinf;
    void            (*release)(struct ONIG_STAT* onigstat);
} ONIG_STAT;

typedef struct BOT {
    ONIG_STAT*      onigstat;
    BOT_DICT**      dict;
    size_t          dictc;
    int             (*regist)(struct BOT** bot, BOT_PATTERN patterns[]);
    int             (*regist_default)(struct BOT** bot, int (*default_func)(BOT_MATCH* match));
    int             (*talk)(struct BOT* bot, const char* str);
    void            (*unregist)(struct BOT** bot);
    void            (*release)(struct BOT* bot);
    int             (*default_func)(BOT_MATCH* match);
    size_t          dict_size;
} BOT;

extern int init_bot(BOT** bot);

#ifdef  __cplusplus
}
/* __cplusplus */
#endif
/* CHEAPBOT */
#endif
