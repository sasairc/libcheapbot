libcheapbot
===

cheap bot library for C.

## Requirements

* [kkos/oniguruma](https://github.com/kkos/oniguruma)

## Install

```shellsession
% make
# make install
```

## Usage

```c
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

int init_bot(BOT** bot);
```

## Example

```c
/*
 * cheap.c
 */
#include <cheapbot.h>
#include <stdio.h>

int bot_default(BOT_MATCH* match)
{
    fprintf(stdout, "なるほど\n");

    return 0;
}
int bot_oudon(BOT_MATCH* match)
{
    fprintf(stdout, "おうどん\n");

    return 0;
}

int bot_echo(BOT_MATCH* match)
{
    fprintf(stdout, "%s\n",
            match->str + match->region->end[0]);

    return 0;
}

int main(void)
{
    int     status  = 0;

    BOT*    bot  = NULL;

    BOT_PATTERN patterns[]  = {
        {"^(お?うどん|o?udon)$",    bot_oudon},
        {"^echo\\s",                bot_echo},
        {NULL, NULL},
    };

    if (init_bot(&bot) < 0) {
        status = 1; goto ERR;
    }
    if (bot->regist(&bot, patterns) < 0) {
        status = 2; goto ERR;
    }
    bot->regist_default(&bot, bot_default);
    bot->talk(bot, "おうどん");
    bot->talk(bot, "echo 地球に優しく");
    bot->talk(bot, "これは、既定という意味です。");
    bot->release(bot);

    return 0;

ERR:
    switch (status) {
        case    2:
            bot->release(bot);
        case    1:
            break;
    }

    return status;
}
```

```shellsession
% gcc cheap.c -lcheapbot cheap
% ./cheap
おうどん
地球に優しく
なるほど
```

## License

[WTFPL version 2](http://www.wtfpl.net/txt/copying/)

## Author

sasairc (https://github.com/sasairc)
