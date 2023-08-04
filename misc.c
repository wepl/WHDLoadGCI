/****************************************************************************
;  :Module.	misc.c
;  :Author.	Bert Jahn
;  :EMail.	wepl@whdload.de
;  :Version.	$Id: misc.c 1.10 2020/04/27 22:53:40 wepl Exp wepl $
;  :History.	28.03.00 extracted from whdloadgci.c
;		06.06.04 htoi added
;		24.02.08 strncpyz added
;		07.09.08 val2hexm added
;  :Copyright.	All Rights Reserved
;  :Language.	C
;  :Translator.	GCC
****************************************************************************/

#include <stdio.h>
#include <string.h>

#include <exec/types.h>

/***************************************************************************/

/*
 *	return string containing hexadecimal representation of value
 */
STRPTR val2hex(ULONG value) {
	static char s[10];
	sprintf(s,value < 16 ? "%d" : "$%x",value);
	return s;
}

/*
 *	return string with remembering decimal/hexadeciaml notation
 */
STRPTR val2hexm(ULONG value, char *t) {
	static char s[10];
	sprintf(s,*t != '$' ? "%d" : "$%x",value);
	return s;
}

/*
 *	return string containing 64-bit hexadecimal representation of value
 */
STRPTR val2hex64(ULONG value1, ULONG value2) {
	static char s[20];
	if (value1) {
		sprintf(s,"$%lx%08lx",value1,value2);
		return s;
	} else {
		return val2hex(value2);
	}
}

/*
 *	convert ascii into number, supports '$' as hex
 */
int htoi(const char *s) {
	int i=0, n=0;
	char c;
	while (*s == ' ' || *s == '\t') s++;
	if (*s == '-') {
		n++; s++;
	}
	if (*s == '$') {
		/* hex */
		s++;
		while ((c = *s++)) {
			if (c >= '0' && c <= '9') {
				c -= '0';
			} else if (c >= 'a' && c <= 'f') {
				c -= 'a' - 10;
			} else if (c >= 'A' && c <= 'F') {
				c -= 'A' - 10;
			} else {
				break;
			}
			i = i*16 + c;
		}
	} else {
		/* decimal */
		while (*s >= '0' && *s <= '9') {
			i = i*10 + *s++ - '0';
		}
	}
	if (n) {
		return -i;
	} else {
		return i;
	}
}

/*
 *	copy string to buffer and terminate
 */
char * strncpyz(char *dest, const char *src, int n) {
	strncpy(dest, src, n);
	dest[n] = 0;
	return dest;
}

