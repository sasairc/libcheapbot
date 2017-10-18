/*
 * main.c
 */
#include "./yasuna.h"
#include "./ncipher.h"
#include "./misc.h"
#include <cheapbot.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    int     i           = 0,
            c           = 0;

    char    buf[1024]   = {'\0'};

    BOT*    bot         = NULL;

    BOT_PATTERN patterns[]  = {
        {"^(お?うどん|o?udon)$",    oudon},
        {"^(お?さかな|o?sakana)$",  osakana},
        {"encode\\s(.+)",           ncipher_encode},
        {"decode\\s(.+)",           ncipher_decode},
        {"talk(?:.*)\\z",           yasuna_talk},
        {"(number|n)\\s[0-9]+$",    yasuna_number},
        {"version$",                yasuna_version},
        {"uptime$",                 uptime},
        {NULL, NULL},
    };

    init_bot(&bot);
    bot->regist(&bot, patterns);
    bot->regist_default(&bot, yasuna_talk);

    while (1) {
        fprintf(stdout, "local-yasuna> ");
        memset(buf, '\0', sizeof(buf));
        i = 0;
        while((c = fgetc(stdin)) != EOF && i < sizeof(buf)) {
            if (c == '\n') {
                buf[i] = '\0';
                break;
            }
            buf[i] = c;
            i++;
        }
        if (memcmp(buf, "exit\0", 5) == 0)
            break;
        else
            bot->talk(bot, buf);
    }
    bot->release(bot);

    return 0;
}
