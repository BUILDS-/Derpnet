/*
 * This file is part of Derpnet.
 * 
 * Derpnet is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Derpnet is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Derpnet.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) 2011 The Derpnet Team.
 */

#ifndef __IRC_H
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
#define ERR_NOMOTD 422
#define ERR_NONICKNAMEGIVEN 431
#define ERR_ERRONEOUSNICKNAME 432
#define ERR_NICKNAMEINUSE 433

//End of numeric codes
#define ALPHA_L "abcdefghijklmnopqrstuvwxyz"
#define ALPHA_U "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define NUM "0123456789"
#define SPECIAL "`_+=[]{}|<>"
#define WHITESPACE " \r\t\n"
#define IRCD_VERSION "Derpnet0.0.1"

//Modes

#define MODES_A "b"
#define MODES_B "k"
#define MODES_C "l"
#define MODES_D "im"
#define MODES_USR "ir"

#endif
