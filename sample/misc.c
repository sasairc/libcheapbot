/*
 * misc.c
 */
#define _GNU_SOURCE

#include "./misc.h"
#include <cheapbot.h>
#include <benly/proc.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int oudon(BOT_MATCH* match)
{
    fprintf(stdout, "おうどん (🍜)\n");

    return 0;
}

int osakana(BOT_MATCH* match)
{
    fprintf(stdout, "おさかな (🐟)\n");

    return 0;
}

int uptime(BOT_MATCH* match)
{
    int     fd[2]       = {0};

    char    buf[128]    = {'\0'};

    FILE*   fp          = NULL;

    PROC*   proc        = NULL;

    init_proc(&proc);
    proc->set(&proc, "hostname");
    pipe(fd);
    if (proc->fork(&proc) == 0) {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        proc->exec(proc);
        exit(1);
    } else {
        close(fd[1]);
        fp = fdopen(fd[0], "r");
        fread(buf, sizeof(char), sizeof(buf), fp);
        proc->wait(&proc, 0);
        fclose(fp);
    }
    buf[strlen(buf) - 1] = ' ';
    proc->set(&proc, "uptime");
    pipe(fd);
    if (proc->fork(&proc) == 0) {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        proc->exec(proc);
        exit(1);
    } else {
        close(fd[1]);
        fp = fdopen(fd[0], "r");
        fread(buf + strlen(buf), sizeof(char), sizeof(buf) - strlen(buf), fp);
        proc->wait(&proc, 0);
        fclose(fp);
    }
    fprintf(stdout, "%.*s\n",
            strlen(buf) - 1, buf);
    proc->release(proc);

    return 0;
}
