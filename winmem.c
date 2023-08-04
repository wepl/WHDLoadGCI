/*************************************************************************
;  :Module.	winmem.c
;  :Author.	Bert Jahn
;  :Version.	$Id: winmem.c 1.13 2020/04/27 22:53:08 wepl Exp wepl $
;  :History.	21.03.00 separated from main
;		06.06.04 'Goto Address' fixed, MCC stuff seperated
;		18.04.06 goto_abs added
;		30.12.18 open window from cursor added
;		20.08.19 cursor add added
;  :Copyright.	All Rights Reserved
;  :Language.	C
;  :Translator.	GCC
*************************************************************************/

#include <stdio.h>

#include <exec/execbase.h>
#include <libraries/mui.h>
#include <mui/BetterString_mcc.h>
#include <mui/HexEdit_mcc.h>

#include <clib/alib_protos.h>
#include <clib/muimaster_protos.h>

#include "whddump.h"
#include "WHDLoadGCI.h"
#include "class.h"

/************************************************************************/
/* Compiler Stuff                                                       */
/************************************************************************/

/************************************************************************/
/* defines								*/
/************************************************************************/

#define MAXWINMEM 10	/* amount of possible memory hexdump windows */

/************************************************************************/
/* extern variables							*/
/************************************************************************/

/************************************************************************/
/* static variables							*/
/************************************************************************/

static APTR win_mem[MAXWINMEM];		/* memory hexdump windows */

/************************************************************************/
/* function declarations						*/
/************************************************************************/

/************************************************************************/

void
wmem_make(
	int adr		/* start address (logical) of memory to display */
) {
	int n;
	APTR gadabs, rel, add, cursornew, hex, prop;
	ULONG open;
	APTR low,high;
	ULONG off,cur;
	char *title, infotext[40];
	
	/*
	 *  search a free window
	 *  if a window is closed dispose it and reuse
	 */
	for (n=0; (n<MAXWINMEM) && win_mem[n]; n++) {
		get(win_mem[n],MUIA_Window_Open,&open);
		if (!open) {
			DoMethod(app,OM_REMMEMBER,win_mem[n]);
			MUI_DisposeObject(win_mem[n]);
			win_mem[n] = NULL;
			break;
		}
	}
	if (n==MAXWINMEM) {
		MUI_Request(app,win,0,NULL,"Ok","Sorry, too many memory windows already open.");
		return;
	}
	
	/*
	 *  check which memory is requested
	 */
	if (adr < header->wdh_BaseMemSize) {
		title = "Base Memory (Chip)";
		low = mem;
		high = ((UBYTE*)mem) + header->wdh_BaseMemSize - 1;
		off = -(ULONG)mem;
		cur = adr;
	} else if ((adr >= header->wdh_ExpMemLog) && (adr < header->wdh_ExpMemLog + header->wdh_ExpMemLen)) {
		title = "Expansion Memory (Fast)";
		low = emem;
		high = ((UBYTE*)emem) + header->wdh_ExpMemLen - 1;
		off = header->wdh_ExpMemLog - (ULONG)emem;
		cur = adr - header->wdh_ExpMemLog;
	} else if ((adr >= header->wdh_SlaveLog) && (adr < header->wdh_SlaveLog + header->wdh_SlaveLen)) {
		title = "Slave Memory";
		low = slave;
		high = ((UBYTE*)slave) + header->wdh_SlaveLen - 1;
		off = header->wdh_SlaveLog - (ULONG)slave;
		cur = adr - header->wdh_SlaveLog;
	} else {
		MUI_Request(app,win,0,NULL,"Ok","Sorry, address is outside the defined memory.");
		return;
	}
	sprintf(infotext,"Start=$%lx  End=$%lx ",(ULONG)low+off,(ULONG)high+off);
	/*printf("infotext %ld\n",strlen(infotext));*/

	/*
	 *  create the window
	 */
	win_mem[n] = WindowObject,
		MUIA_Window_Title, title,
		MUIA_Window_ID   , n + MAKE_ID('M','E','M','0'),
		WindowContents, VGroup,
			Child, HGroup,
				Child, Label2(infotext),
				Child, cursornew = SimpleButton("New from Cursor"),
				End,
			Child, HGroup,
				Child, Label2("Goto Adr Abs:"),
				Child, gadabs = BetterStringObject,
					StringFrame,
					MUIA_String_Accept , StringHexAccept,
					MUIA_String_MaxLen , 10,
					MUIA_String_Format , MUIV_String_Format_Right,
					MUIA_ShortHelp, "Goto Address Absolut",
					MUIA_CycleChain, 1,
					End,
				Child, Label2("Goto Adr Rel:"),
				Child, rel = BetterStringObject,
					StringFrame,
					MUIA_String_Accept , StringHexAccept,
					MUIA_String_MaxLen , 10,
					MUIA_String_Format , MUIV_String_Format_Right,
					MUIA_ShortHelp, "Goto Offset Relative",
					MUIA_CycleChain, 1,
					End,
				Child, Label2("Add to Cursor:"),
				Child, add = BetterStringObject,
					StringFrame,
					MUIA_String_Accept , StringHexMinAccept,
					MUIA_String_MaxLen , 10,
					MUIA_String_Format , MUIV_String_Format_Right,
					MUIA_ShortHelp, "Advance cursor position (+/-) with given offset",
					MUIA_CycleChain, 1,
					End,
				End,
			Child, HGroup,
				Child, hex = NewObject(GCIHexEdit_Class->mcc_Class,0, VirtualFrame,
					MUIA_HexEdit_LowBound, low,
					MUIA_HexEdit_HighBound, high,
					MUIA_HexEdit_BaseAddressOffset, off,
					MUIA_HexEdit_AddressChars, 0,	/* must be after Low, High and BaseAddressOffset! */
					MUIA_HexEdit_EditMode, TRUE,
					MUIA_HexEdit_SelectMode, MUIV_HexEdit_SelectMode_Byte,
					MUIA_HexEdit_CursorAddress, cur,
					MUIA_CycleChain, 1,
					End,
				Child, prop = ScrollbarObject,
					MUIA_Prop_UseWinBorder, MUIV_Prop_UseWinBorder_Right,
					End,
				End,
			End,
		End;
	if (!win_mem[n]) {
		MUI_Request(app,win,0,NULL,"Ok","Couldn't open window.");
		return;
	}

	/*
	 *  add window to application and open it
	 */
	DoMethod(app,OM_ADDMEMBER,win_mem[n]);
	/* connect prop to hex */
	set(hex,MUIA_HexEdit_PropObject,prop);
	/* window close */
	DoMethod(win_mem[n],MUIM_Notify,MUIA_Window_CloseRequest,TRUE,
		win_mem[n],3,MUIM_Set,MUIA_Window_Open,FALSE);
	/* goto absolut address */
	DoMethod(gadabs,MUIM_Notify,MUIA_String_Contents,MUIV_EveryTime,
		hex,3,MUIM_Set,MUIA_GCIHexEdit_CursorAddressAbs,gadabs);
	/* goto relative address */
	DoMethod(rel,MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,
		hex,3,MUIM_Set,MUIA_GCIHexEdit_CursorAddressRel,rel);
	/* add to address */
	DoMethod(add,MUIM_Notify,MUIA_String_Acknowledge,MUIV_EveryTime,
		hex,3,MUIM_Set,MUIA_GCIHexEdit_CursorAddressAdd,add);
	/* open new mem window from cursor contents */
	DoMethod(cursornew,MUIM_Notify,MUIA_Pressed,FALSE,
		hex,2,MUIM_GCIHexEdit_NewFromCursor,TRUE);
	/* update goto rel on cursor changes */
	DoMethod(hex,MUIM_Notify,MUIA_HexEdit_CursorAddress,MUIV_EveryTime,
		rel,4,MUIM_SetAsString,MUIA_String_Contents,"$%lx",MUIV_TriggerValue);
	/* open window */
	set(win_mem[n],MUIA_Window_Open,TRUE);
	/* set initial cursor position */
	DoMethod(gadabs,MUIM_SetAsString,MUIA_String_Contents,"$%lx",adr);

}

/****************************************************************************/

