/*
 * cheapbot - The simply bot library for C
 *
 * cheapbot.c
 *
 * Copyright (c) 2017 sasairc
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar.HocevarHocevar See the COPYING file or http://www.wtfpl.net/
 * for more details.
 */

#include "./cheapbot.h"
#include <stdlib.h>
#include <string.h>
#include <oniguruma.h>

static int regist_bot(BOT** bot, BOT_PATTERN patterns[]);
static int regist_default_bot(BOT** bot, int (*default_func)(BOT_MATCH* match));
static void unregist_bot(BOT** bot);
static int talk_bot(BOT* bot, const char* str);
static void release_bot(BOT* bot);

static int compile_regex_pattern(ONIG_STAT** onigstat, regex_t** regex, const char* pattern);
static int regist_dict(BOT** bot, BOT_PATTERN pattern);
static void unregist_dict(BOT** bot, size_t pos);
static int allocate_dict(BOT** bot);
static int reallocate_dict(BOT** bot);
static int init_onigstat(ONIG_STAT** onigstat);
static void release_onigstat(ONIG_STAT* onigstat);

int init_bot(BOT** bot)
{
    int     status  = 0;

    BOT*    bt      = NULL;

    if ((bt = (BOT*)
                malloc(sizeof(BOT))) == NULL) {
        status = -1; goto ERR;
    } else {
        bt->onigstat        = NULL;
        bt->dict            = NULL;
        bt->dictc           = 0;
        bt->default_func    = NULL;
        bt->regist          = regist_bot;
        bt->regist_default  = regist_default_bot;
        bt->unregist        = unregist_bot;
        bt->talk            = talk_bot;
        bt->release         = release_bot;
        bt->dict_size       = DEFAULT_DICT_SIZE;
    }
    if (allocate_dict(&bt) < 0) {
        status = -2; goto ERR;
    }
    if (init_onigstat(&bt->onigstat) < 0) {
        status = -3; goto ERR;
    }
    *bot = bt;

    return 0;

ERR:
    switch (status) {
        case    -1:
        case    -2:
            break;
        case    -3:
            if (bt != NULL) {
                free(bt);
                bt = NULL;
            }
            break;
    }

    return status;
}

static
int regist_bot(BOT** bot, BOT_PATTERN patterns[])
{
    int     status  = 0;

    while (patterns[(*bot)->dictc].pattern != NULL) {
        if ((*bot)->dict_size <= (*bot)->dictc) {
            if (reallocate_dict(bot) < 0) {
                status = -1; goto ERR;
            }
        }
        if (regist_dict(bot, patterns[(*bot)->dictc]) < 0) {
            status = -2; goto ERR;
        }
        (*bot)->dictc++;
    }

    return 0;

ERR:
    (*bot)->unregist(bot);

    return status;
}

static
void unregist_bot(BOT** bot)
{
    size_t      i       = 0,
                j       = 0;

    j = (*bot)->dictc - 1;
    while (i < (*bot)->dictc && i <= j) {
        unregist_dict(bot, i);
        unregist_dict(bot, j);
        i++;
        j--;
    }
    (*bot)->dictc = 0;

    return;
}

static
int regist_default_bot(BOT** bot, int (*default_func)(BOT_MATCH* match))
{
    (*bot)->default_func = default_func;

    return 0;
}

static
int talk_bot(BOT* bot, const char* str)
{
    int         status  = 0;

    size_t      i       = 0,
                len     = 0;

    OnigUChar*  str2    = NULL,
        *       start   = NULL,
        *       end     = NULL,
        *       range   = NULL;

    BOT_MATCH   match   = {
        NULL, onig_region_new(),
    };

    len = strlen(str);
    if ((str2 = (OnigUChar*)
                malloc(sizeof(char) * (len + 1))) == NULL) {
        status = -1; goto ERR;
    } else {
        memcpy(str2, str, len);
        *(str2 + len) = '\0';
        start = str2;
        end = str2 + len;
        range = end;
    }

    while (i < bot->dictc) {
        if (onig_search(bot->dict[i]->regex,
                str2, end, start, range, match.region, ONIG_OPTION_NONE) != ONIG_MISMATCH) {
            match.str = (char*)str2;
            if (bot->dict[i]->func != NULL)
                bot->dict[i]->func(&match);
            else if (bot->default_func != NULL)
                bot->default_func(&match);
            break;
        }
        i++;
    }
    if (i == bot->dictc && bot->default_func != NULL)
        bot->default_func(&match);

    onig_region_free(match.region, 1);
    if (str2 != NULL) {
        free(str2);
        str2 = NULL;
    }

    return 0;

ERR:
    return status;
}

static
void release_bot(BOT* bot)
{
    if (bot != NULL) {
        if (bot->dict != NULL) {
            bot->unregist(&bot);
            bot->dict = NULL;
        }
        if (bot->onigstat != NULL) {
            bot->onigstat->release(bot->onigstat);
            bot->onigstat = NULL;
        }
        free(bot);
        bot = NULL;
    }

    return;
}

static
int compile_regex_pattern(ONIG_STAT** onigstat, regex_t** regex, const char* pattern)
{
    if (((*onigstat)->status = onig_new(
                    regex, (OnigUChar*)pattern, (OnigUChar*)pattern + strlen(pattern),
                    ONIG_OPTION_DEFAULT, (*onigstat)->encode, ONIG_SYNTAX_DEFAULT,
                    &(*onigstat)->errinf)) != ONIG_NORMAL)
        return -1;

    return 0;
}

static
int regist_dict(BOT** bot, BOT_PATTERN pattern)
{
    int     status  = 0;

    if ((*((*bot)->dict + (*bot)->dictc) = (BOT_DICT*)
                malloc(sizeof(BOT_DICT))) == NULL) {
        status = -1; goto ERR;
    }
    if (((*bot)->onigstat->status = compile_regex_pattern(
                &(*bot)->onigstat, &(*bot)->dict[(*bot)->dictc]->regex, pattern.pattern)) < 0) {
        status = -2; goto ERR;
    }
    if (pattern.func != NULL)
        (*bot)->dict[(*bot)->dictc]->func = pattern.func;
    else
        (*bot)->dict[(*bot)->dictc]->func = NULL;

    return 0;

ERR:
    (*bot)->unregist(bot);

    return status;
}

static
void unregist_dict(BOT** bot, size_t pos)
{
    if (*((*bot)->dict + pos) != NULL) {
        free(*((*bot)->dict + pos));
        *((*bot)->dict + pos) = NULL;
    }

    return;
}

static
int allocate_dict(BOT** bot)
{
    int     status  = 0;

    size_t  i       = 0,
            j       = 0;

    if (((*bot)->dict = (BOT_DICT**)
                malloc(sizeof(BOT_DICT*) * (*bot)->dict_size)) == NULL) {
        status = -1; goto ERR;
    }
    j = (*bot)->dict_size - 1;
    while (i < (*bot)->dict_size && i <= j) {
        *((*bot)->dict + i) = *((*bot)->dict + j) = NULL;
        i++;
        j--;
    }

    return 0;

ERR:
    return status;
}

static
int reallocate_dict(BOT** bot)
{
    int     status  = 0;

    size_t  i       = 0,
            j       = 0;

    (*bot)->dict_size += DEFAULT_DICT_SIZE;
    if (((*bot)->dict = (BOT_DICT**)
                realloc((*bot)->dict, sizeof(BOT_DICT))) == NULL) {
        status = -1; goto ERR;
    }
    i = (*bot)->dictc;
    j = (*bot)->dict_size - 1;
    while (i < (*bot)->dict_size && i <= j) {
        *((*bot)->dict + i) = *((*bot)->dict + j) = NULL;
        i++;
        j--;
    }

    return 0;

ERR:
    return status;
}

static
int init_onigstat(ONIG_STAT** onigstat)
{
    ONIG_STAT*  os  = NULL;

    if ((os = (ONIG_STAT*)
                malloc(sizeof(ONIG_STAT))) == NULL) {
        return -1;
    } else {
        os->status      = 0;
        os->encode      = BOT_ENCODING;
        os->release     = release_onigstat;
    }
    *onigstat = os;

    return 0;
}

static
void release_onigstat(ONIG_STAT* onigstat)
{
    if (onigstat != NULL) {
        free(onigstat);
        onigstat = NULL;
    }

    return;
}
