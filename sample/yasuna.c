/*
 * yasuna.c
 */
#define _GNU_SOURCE

#include "./yasuna.h"
#include <cheapbot.h>
#include <benly/proc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int yasuna_talk(BOT_MATCH* match)
{
    PROC*   yasuna  = NULL;

    init_proc(&yasuna);
    yasuna->set(&yasuna, "yasuna");
    switch (yasuna->fork(&yasuna)) {
        case    0:
            yasuna->exec(yasuna);
            exit(1);
        default:
            yasuna->wait(&yasuna, 0);
            break;
    }
    yasuna->release(yasuna);

    return 0;
}

int yasuna_number(BOT_MATCH* match)
{
    char*   p       = NULL,
            cmd[64] = {'\0'};

    PROC*   yasuna  = NULL;

    p = match->str + match->region->end[1] + 1;
    memcpy(cmd, "yasuna --number ", 16);
    memcpy(cmd + 16, p, strlen(p));
    cmd[strlen(cmd)] = '\0';
    init_proc(&yasuna);
    yasuna->set(&yasuna, cmd);
    switch (yasuna->fork(&yasuna)) {
        case    0:
            yasuna->exec(yasuna);
            exit(1);
        default:
            yasuna->wait(&yasuna, 0);
            break;
    }
    yasuna->release(yasuna);

    return 0;
}

int yasuna_version(BOT_MATCH* match)
{
    PROC*   yasuna  = NULL;

    init_proc(&yasuna);
    yasuna->set(&yasuna, "yasuna --version");
    switch (yasuna->fork(&yasuna)) {
        case    0:
            yasuna->exec(yasuna);
            exit(1);
        default:
            yasuna->wait(&yasuna, 0);
            break;
    }
    yasuna->release(yasuna);

    return 0;
}
