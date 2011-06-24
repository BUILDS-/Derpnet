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
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "errors.h"

void colorPrefix(const char* prefix, bool light, int color, const char* fmt, va_list args, bool partial) {
	char newFmt[strlen(fmt)+strlen(prefix) + 16];
	sprintf(newFmt,"\033[%d;%dm%s\033[0;%dm",light,color,prefix,FG_WHITE);
	strcat(newFmt,fmt);
	vprintf(newFmt,args);
	if(!partial) {
		printf("\n");
	} else {
		printf("...\t\t");
	}
}

void colorPrefix(const char* prefix, bool light, int color, const char* fmt, va_list args) {
	colorPrefix(prefix,light, color,fmt,args,false);
}

void statusMsg(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	if(!hasColors) {
		colorPrefix(" * ", false, FG_WHITE, fmt, args);
	} else {
		colorPrefix(" * ", false, FG_GREEN, fmt, args);
	}
}

void warningMsg(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	if(!hasColors) {
		colorPrefix(" * ", false, FG_WHITE, fmt, args);
	} else {
		colorPrefix(" * ", false, FG_YELLOW, fmt, args);
	}
}

void errorMsg(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	if(!hasColors) {
		colorPrefix(" ! ", false, FG_WHITE, fmt, args);
	} else {
		colorPrefix(" ! " , false, FG_RED, fmt, args);
	}
}

void majorError(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	if(!hasColors) {
		colorPrefix("!!! ", true, FG_WHITE, fmt, args);
	} else {
		colorPrefix("!!! ", true, FG_RED, fmt, args);
	}
}

void partialLine(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	if(!hasColors) {
		colorPrefix(" * ", false, FG_WHITE, fmt, args,true);
	} else {
		colorPrefix(" * ", false, FG_GREEN, fmt, args,true);
	}
}


