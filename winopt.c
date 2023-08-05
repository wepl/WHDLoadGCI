/*************************************************************************
;  :Module.	winopt.c
;  :Author.	Bert Jahn
;  :EMail.	wepl@whdload.de
;  :Version.	$Id: wininfo.c 1.9 2019/08/15 22:00:04 wepl Exp wepl $
;  :History.	17.07.23 created
;  :Copyright.	All Rights Reserved
;  :Language.	C
;  :Translator.	GCC
*************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libraries/mui.h>

#include <clib/alib_protos.h>
#include <clib/muimaster_protos.h>

#include "WHDLoadGCI.h"

/************************************************************************/
/* defines								*/
/************************************************************************/

/************************************************************************/
/* extern variables							*/
/************************************************************************/

/************************************************************************/
/* static variables							*/
/************************************************************************/

APTR win_opt = NULL;

/************************************************************************/
/* function declarations						*/
/************************************************************************/

/************************************************************************/

int compare(const void *a, const void *b) {
	return strcmp(*(const char **) a, *(const char **) b);
}

void make_win_opt(void) {
	ULONG open;
	int bcnt, ocnt, i, omax = 40;
	char c, *s, buf[400], *b, *optarray[omax+1], **o;
	char out[sizeof(buf)+1];
	BOOL quote = FALSE;

	if (! opt || ! *opt) return;

	if (win_opt) {
		get(win_opt,MUIA_Window_Open,&open);
		if (open)	set(win_opt,MUIA_Window_Open,FALSE);
		else		set(win_opt,MUIA_Window_Open,TRUE);
	} else {

		// split opt at spaces outside ""
		s = opt;
		b = buf; bcnt = 0;
		o = optarray; ocnt = 0;
		*o++ = b;
		while ((c = *s++) && bcnt < sizeof(buf) && ocnt < omax) {
			switch (c) {
				case ' ':
					if (! quote) {
						*b++ = 0;
						*o++ = b;	// optarray must be one element larger!
						ocnt++;
					} else {
						*b++ = c;
					}
					bcnt++;
					break;
				case '"':
					quote = ! quote;
				default:
					*b++ = c;
					bcnt++;
			}
		}
		// array too small?
		if (ocnt == omax) {
			o[-2] = MUIX_B "* array too small *" MUIX_N;
		// buffer too small?
		} else if (bcnt == sizeof(buf)) {
			*--o = MUIX_B "* buffer too small *" MUIX_N;
			ocnt++;
		// input end reached, terminate last option
		} else {
			*b++ = 0;
			ocnt++;
		}
		
		// sort array
		qsort(optarray, ocnt, sizeof(char*), compare);

		// create output
		for (*out=0, o=optarray, i=0; i<ocnt; i++) {
			strcat(out,*o++);
			strcat(out,"\n");
		}

		win_opt = WindowObject,
			MUIA_Window_Title, "WHDLoad Options",
			MUIA_Window_ID   , MAKE_ID('O','P','T',' '),
			WindowContents,
				TextObject, TextFrame, 
					MUIA_Background, MUII_TextBack,
					MUIA_Text_Contents, out,
					End,
			End;
		if (win_opt) {
			DoMethod(app,OM_ADDMEMBER,win_opt);
			DoMethod(win_opt,MUIM_Notify,MUIA_Window_CloseRequest,TRUE,app,2,MUIM_Application_ReturnID,MAIN_MOREOPT);
			set(win_opt,MUIA_Window_Open,TRUE);
		} else {
			MUI_Request(app,win,0,NULL,"Ok","Couldn't open Options window.");
		}
	}
}

/************************************************************************/

