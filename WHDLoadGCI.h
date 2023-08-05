/*************************************************************************
;  :Module.	WHDLoadGCI.h
;  :Author.	Bert Jahn
;  :History.	18.07.98 started
;		12.07.03 changes for reg to mem
;		24.02.08 strncpyz added
;		17.07.23 opt, cols added
;  :Copyright.	All Rights Reserved
;  :Language.	C
;  :Translator.	GCC
*************************************************************************/

#include <exec/types.h>

/************************************************************************/
/*	defines								*/
/************************************************************************/

#define AFF_68060 (1L<<7)

enum {
	MEN_PROJECT=1,MEN_EXSLAVE,MEN_EXBASEMEM,MEN_EXBASEPART,MEN_EXEXPMEM,MEN_EXEXPPART,MEN_ABOUT,MEN_ABOUTMUI,MEN_QUIT,
	MAIN_MORECIA,MAIN_MORECPU,MAIN_MORECUST,MAIN_MOREMEM,MAIN_MOREEMEM,MAIN_MORESLAVE,MAIN_MOREINFO,MAIN_MOREOPT,
	MAIN_MORED0,MAIN_MORED1,MAIN_MORED2,MAIN_MORED3,MAIN_MORED4,MAIN_MORED5,MAIN_MORED6,MAIN_MORED7,
	MAIN_MOREA0,MAIN_MOREA1,MAIN_MOREA2,MAIN_MOREA3,MAIN_MOREA4,MAIN_MOREA5,MAIN_MOREA6,MAIN_MOREUSP,MAIN_MORESSP,MAIN_MOREPC,
	MAIN_MORECOP1LC,MAIN_MORECOP2LC,
	WMEM_MOREMEM,
	PART_CLOSE,PART_OK
};

/************************************************************************/
/* extern variables							*/
/************************************************************************/

extern struct Library * MUIMasterBase;
extern APTR app,win;
extern struct whddump_cia * ciaa;
extern struct whddump_cia * ciab;
extern struct whddump_cpu * cpu;
extern struct whddump_custom * custom;
extern struct whddump_header * header;
extern char * term;
extern char * opt;
extern struct whddump_slave * slave;
extern APTR * mem;
extern APTR * emem;
extern APTR * cols;

/************************************************************************/
/*	macros								*/
/************************************************************************/

#ifndef MAKE_ID
#define MAKE_ID(a,b,c,d) ((ULONG) (a)<<24 | (ULONG) (b)<<16 | (ULONG) (c)<<8 | (ULONG) (d))
#endif

#define btst(i,n) ((i >> n) & 1)

/************************************************************************/
/*	mui macros							*/
/************************************************************************/

/*
 *  Child if true
 */
#define ChildIf(rule) rule ? Child : TAG_IGNORE

/*
 *  SimpleButton if true
 */
#define SimpleButtonIf(rule,text) rule ? SimpleButton(text) : NULL

/************************************************************************/
/*	function prototypes						*/
/************************************************************************/

/* misc.c */
STRPTR	val2hex		(ULONG value);
STRPTR	val2hexm	(ULONG value, char *s);
STRPTR	val2hex64	(ULONG value1, ULONG value2);
int	htoi		(const char *s);
char *	strncpyz	(char *dest, const char *src, int n);


/************************************************************************/

