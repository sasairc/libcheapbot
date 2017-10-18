/*
 * ncipher.c
 */
#include "./ncipher.h"
#include <cheapbot.h>
#include <n_cipher.h>
#include <stdio.h>
#include <stdlib.h>

int ncipher_encode(BOT_MATCH* match)
{
    char*       enc         = NULL;
    
    N_CIPHER*   n_cipher    = NULL;

    init_n_cipher(&n_cipher);
    n_cipher->config(&n_cipher, Y_SEED, Y_DELIMITER);
    enc = n_cipher->encode(&n_cipher,
            match->str + match->region->beg[1]);
    fprintf(stdout, "%s\n",
            enc);
    free(enc);
    n_cipher->release(n_cipher);

    return 0;

}

int ncipher_decode(BOT_MATCH* match)
{
    char*       dec         = NULL;

    N_CIPHER*   n_cipher    = NULL;

    init_n_cipher(&n_cipher);
    n_cipher->config(&n_cipher, Y_SEED, Y_DELIMITER);
    if ((dec = n_cipher->decode(&n_cipher,
            match->str + match->region->beg[1])) == NULL) {
        fprintf(stdout, "暗号になってない！\n");
    } else {
        fprintf(stdout, "%s\n",
                dec);
        free(dec);
    }
    n_cipher->release(n_cipher);

    return 0;
}
