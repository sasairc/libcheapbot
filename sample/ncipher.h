/*
 * ncipher.h
 */
#ifndef	BOT_NCIPHER_H
#define	BOT_NCIPHER_H

#include <cheapbot.h>

#define	Y_SEED		"くそぅ"
#define	Y_DELIMITER	"！"

extern int ncipher_encode(BOT_MATCH* match);
extern int ncipher_decode(BOT_MATCH* match);

/* BOT_NCIPHER_H */
#endif
