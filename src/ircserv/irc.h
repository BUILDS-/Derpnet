#define __IRC_H
/*
	 This header defines a bunch of useful constants for grammer decoding
	  and avoiding "Magic Numbers". Add numeric reply codes as you see fit.
		Be careful when changing any alphabetic/numeric groups.
*/
//Reply codes
#define RPL_WELCOME 1
#define RPL_YOURHOST 2
#define RPL_CREATED 3
#define RPL_MYINFO 4
#define RPL_ISUPPORT 5
#define RPL_LUSERCLIENT 251
#define RPL_LUSEROP 252
#define RPL_LUSERCHANNELS 253
#define RPL_LUSERME 254
#define RPL_AWAY 301
#define RPL_UNAWAY 305
#define RPL_NOWAWAY 306
#define RPL_MOTD 372
#define RPL_MOTDSTART 375
#define RPL_ENDOFMOTD 376
//Error codes
#define ERR_NONICKNAMEGIVEN 431
#define ERR_ERRONEOUSNICKNAME 432
#define ERR_NICKNAMEINUSE 433
