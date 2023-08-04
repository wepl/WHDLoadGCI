/*************************************************************************
;  :Module.	class.c
;  :Author.	Bert Jahn
;  :Version.	$Id: class.c 1.7 2020/04/27 22:53:08 wepl Exp wepl $
;  :History.	07.06.04 separated from winmem.c
;		18.04.06 goto_abs added
;		07.09.08 GCIStringPart added
;		30.12.18 scroll to first line on CursorAddressAbs
;                        MUI_CreateCustomClass fixed for GCIStringPart_Class
;		20.08.19 MUIA_GCIHexEdit_CursorAddressAdd added
;  :Copyright.	All Rights Reserved
;  :Language.	C
;  :Translator.	GCC
*************************************************************************/

#include <stdio.h>

#include <libraries/mui.h>
#include <mui/BetterString_mcc.h>
#include <mui/HexEdit_mcc.h>

#include <clib/alib_protos.h>
#include <clib/muimaster_protos.h>
#include <clib/utility_protos.h>

#include "WHDLoadGCI.h"
#include "class.h"
#include "whddump.h"

/************************************************************************/
/* Compiler Stuff                                                       */
/************************************************************************/

#define REG(x) register __ ## x
#define ASM    __asm
#define SAVEDS __saveds

/************************************************************************/
/* defines								*/
/************************************************************************/

/************************************************************************/
/* extern variables							*/
/************************************************************************/

extern ULONG winmem_cursornew;	/* address for new memory window via cursor */

/************************************************************************/
/* static variables							*/
/************************************************************************/

struct MUI_CustomClass *GCIHexEdit_Class = NULL;
struct MUI_CustomClass *GCIStringCustom_Class = NULL;
struct MUI_CustomClass *GCIStringPart_Class = NULL;
struct MUI_CustomClass *GCIStringRegister_Class = NULL;

const char * StringHexMinAccept = "-$0123456789abcdefABCDEF";
const char * StringHexAccept = "$0123456789abcdefABCDEF";

/************************************************************************/
/* function declarations						*/
/************************************************************************/

/************************************************************************/

struct GCIHexEdit_Data {
	int low_bound;		/* memory start */
	int high_bound;		/* memory end */
	int base_address;	/* virtual offset of memory */
	int len_base;		/* char length of base address */
	int len_off;		/* char length of offset address */
};

SAVEDS ULONG
GCIHexEdit_mNew(
	struct IClass *cl,
	Object *obj,
	Msg msg
) {
	struct TagItem *tags,*tag;
	int i;
	struct GCIHexEdit_Data tmp = {0,0,0,0,0};

	for (tags=((struct opSet *)msg)->ops_AttrList; (tag=NextTagItem(&tags)); )
	{
		switch (tag->ti_Tag)
		{
			case MUIA_HexEdit_LowBound: tmp.low_bound = tag->ti_Data; break;
			case MUIA_HexEdit_HighBound: tmp.high_bound = tag->ti_Data; break;
			case MUIA_HexEdit_BaseAddressOffset: tmp.base_address = tag->ti_Data; break;
			case MUIA_HexEdit_AddressChars:
				i = tmp.high_bound + tmp.base_address;
				tmp.len_base =	i < 0x10000 ? 4 :
						i < 0x100000 ? 5 :
						i < 0x1000000 ? 6 :
						i < 0x10000000 ? 7 : 8;
				if (tmp.low_bound + tmp.base_address) {
					i = tmp.high_bound - tmp.low_bound;
					tmp.len_off =	i < 0x1000 ? 3 :
							i < 0x10000 ? 4 :
							i < 0x100000 ? 5 : 6;
				} else {
					tmp.len_off = 0;
				}
				tag->ti_Data = tmp.len_base + (tmp.len_off ? 1 + tmp.len_off : 0);
				break;
		}
	}

	if (!(obj = (Object *)DoSuperMethodA(cl,obj,msg))) return(0);

	memcpy(INST_DATA(cl,obj), &tmp, sizeof(tmp));

	return ((ULONG)obj);
}

SAVEDS ULONG
GCIHexEdit_mSet(
	struct IClass *cl,
	Object *obj,
	Msg msg
) {
	struct GCIHexEdit_Data *d = INST_DATA(cl,obj);
	struct TagItem *tags,*tag;
	char *s;
	int i,j;

	for (tags=((struct opSet *)msg)->ops_AttrList; (tag=NextTagItem(&tags)); )
	{
		switch (tag->ti_Tag)
		{
			case MUIA_GCIHexEdit_CursorAddressAbs:
				get( (APTR)tag->ti_Data, MUIA_String_Contents, &s );
				i = htoi( s ) - d->base_address - d->low_bound;
				/*printf("MUIA_GCIHexEdit_CursorAddressAbs num=%ld adr='%s'=%d=$%x low=$%lx high=$%lx base=$%lx\n",tag->ti_Data,s,i,i,d->low_bound,d->high_bound,d->base_address);*/
				set( obj, MUIA_HexEdit_First, i );
				set( obj, MUIA_HexEdit_CursorAddress, i );
				break;
			case MUIA_GCIHexEdit_CursorAddressRel:
				get( (APTR)tag->ti_Data, MUIA_String_Contents, &s );
				i = htoi( s );
				/*printf("MUIA_GCIHexEdit_CursorAddressRel num=%ld adr='%s'=%d=$%x\n",tag->ti_Data,s,i,i);*/
				set( obj, MUIA_HexEdit_CursorAddress, i );
				break;
			case MUIA_GCIHexEdit_CursorAddressAdd:
				get( (APTR)tag->ti_Data, MUIA_String_Contents, &s );
				get( obj, MUIA_HexEdit_CursorAddress, &j );
				i = htoi( s );
				/*printf("MUIA_GCIHexEdit_CursorAddressAdd num=%ld add='%s'=%d=$%x old=$%x\n",tag->ti_Data,s,i,i,j);*/
				set( obj, MUIA_HexEdit_CursorAddress, i+j );
				break;
		}
	}

	return DoSuperMethodA(cl,obj,msg);
}

SAVEDS ULONG
GCIHexEdit_mCreateDisplayAddress(
	struct IClass *cl,
	Object *obj,
	struct MUIP_HexEdit_CreateDisplayAddress *msg
) {
	struct GCIHexEdit_Data *d = INST_DATA(cl,obj);
	ULONG address;
	UBYTE i;
	UBYTE *hextable = "0123456789ABCDEF";

	address = (d->base_address + msg->address) << (32 - d->len_base * 4);
	for(i = 0; i < d->len_base; i++)
	{
		*(*msg->cp)++ = hextable[address >> (32 - 4)];
		address <<= 4;
	}

	address = (msg->address - d->low_bound) << (32 - d->len_off * 4);
	if (d->len_off) {
		*(*msg->cp)++ = (unsigned char) '·';
		for(i = 0; i < d->len_off; i++)
		{
			*(*msg->cp)++ = hextable[address >> (32 - 4)];
			address <<= 4;
		}
	}

	return(TRUE);
}

/* open a new memory window using the address where current
   cursor points to
   get the address and store in global variable, then use ReturnID
   to notify the app to open new window */

SAVEDS ULONG
GCIHexEdit_mNewFromCursor(
	struct IClass *cl,
	Object *obj,
	Msg msg
) {
	struct GCIHexEdit_Data *d = INST_DATA(cl,obj);
	ULONG address;

	get(obj, MUIA_HexEdit_CursorAddress, &address);
	address &= -2;	/* make it even */
	winmem_cursornew = *(ULONG*)(d->low_bound + address);
	/*printf("cursornew address=$%lx new=$%lx\n",address,winmem_cursornew);*/
	DoMethod(app,MUIM_Application_ReturnID,WMEM_MOREMEM);
	return(TRUE);
}

/*
SAVEDS ASM ULONG
GCIHexEdit_Dispatcher(
	REG(a0) struct IClass *cl,
	REG(a2) Object *obj,
	REG(a1) Msg msg
*/
SAVEDS ULONG
GCIHexEdit_Dispatcher(
	struct IClass *cl __asm("a0"),
	Object *obj __asm("a2"),
	Msg msg __asm("a1")
) {
	switch (msg->MethodID)
	{
		case OM_NEW				: return(GCIHexEdit_mNew			(cl,obj,(APTR)msg));
		case OM_SET				: return(GCIHexEdit_mSet			(cl,obj,(APTR)msg));
		case MUIM_HexEdit_CreateDisplayAddress	: return(GCIHexEdit_mCreateDisplayAddress	(cl,obj,(APTR)msg));
		case MUIM_GCIHexEdit_NewFromCursor	: return(GCIHexEdit_mNewFromCursor		(cl,obj,(APTR)msg));
	}
	return(DoSuperMethodA(cl,obj,msg));
}

/************************************************************************/

struct GCIStringCustom_Data {
	UWORD offset;
	UWORD readwrite;	/* write offset */
	char *(*help)(ULONG);
	char lw;
};

SAVEDS ULONG
GCIStringCustom_mNew(
	struct IClass *cl,
	Object *obj,
	Msg msg
) {
	struct TagItem *tags,*tag;
	struct GCIStringCustom_Data data = {0,0,0,0};
	ULONG v;

	for (tags=((struct opSet *)msg)->ops_AttrList; (tag=NextTagItem(&tags)); )
	{
		switch (tag->ti_Tag)
		{
			case MUIA_GCIStringCustom_Offset:
				data.offset = tag->ti_Data;
				break;
			case MUIA_GCIStringCustom_ReadWrite:
				data.readwrite = tag->ti_Data;
				break;
			case MUIA_GCIStringCustom_Help:
				data.help = (char *(*)(ULONG)) tag->ti_Data;
				break;
			case MUIA_GCIStringCustom_Long:
				data.lw = -1;
				break;
		}
	}

	if (!(obj = (Object *)DoSuperMethodA(cl,obj,msg))) return(0);

	memcpy(INST_DATA(cl,obj), &data, sizeof(data));
	
	set(obj,MUIA_String_MaxLen,data.lw ? 10 : 6);
	set(obj,MUIA_String_Format,MUIV_String_Format_Right);
	set(obj,MUIA_CycleChain,1);
	
	if (custom->wdcu_flags[data.offset].read || custom->wdcu_flags[data.offset].modi) {
		if (data.lw) {
			v = *(ULONG*)&(custom->wdcu_regs[data.offset/2]);
		} else {
			v = custom->wdcu_regs[data.offset/2];
		}
		set(obj,MUIA_String_Contents,val2hex(v));
		if (data.help)
			set(obj,MUIA_ShortHelp,data.help(v));
	}
	if (custom->wdcu_flags[data.offset].read && ! data.readwrite) {
		set(obj,MUIA_BetterString_NoInput,TRUE);
	/*	set(obj,MUIA_Background,"2:ffffffff,0,0");	doesn't work */
	} else {
		set(obj,MUIA_String_Accept,StringHexAccept);
		DoMethod(obj,MUIM_Notify,MUIA_String_Contents,MUIV_EveryTime,obj,1,MUIM_GCIStringCustom_Changed);
	}

	return ((ULONG)obj);
}

SAVEDS ULONG
GCIStringCustom_mChanged(
	struct IClass *cl,
	Object *obj,
	Msg msg
) {
	struct GCIStringCustom_Data *data = INST_DATA(cl,obj);
	char *s;
	ULONG v;
	
	get(obj, MUIA_String_Contents, &s );
	v = htoi( s );
	
	if (data->lw) {
		*(ULONG*)&(custom->wdcu_regs[data->offset/2]) = v;
	} else {
		v &= 0x0ffff;
		custom->wdcu_regs[data->offset/2] = v;
	}

	if (data->help) set(obj,MUIA_ShortHelp,data->help(v));

	return(TRUE);
}

SAVEDS ULONG
GCIStringCustom_Dispatcher(
	struct IClass *cl __asm("a0"),
	Object *obj __asm("a2"),
	Msg msg __asm("a1")
) {
	switch (msg->MethodID)
	{
		case OM_NEW				: return(GCIStringCustom_mNew			(cl,obj,(APTR)msg));
		case MUIM_GCIStringCustom_Changed	: return(GCIStringCustom_mChanged		(cl,obj,(APTR)msg));
	}
	return(DoSuperMethodA(cl,obj,msg));
}

/************************************************************************/

struct GCIStringPart_Data {
	struct GCIStringPart_DataShared *data;
	char type;
};

SAVEDS ULONG
GCIStringPart_mNew(
	struct IClass *cl,
	Object *obj,
	Msg msg
) {
	struct TagItem *tags,*tag;
	struct GCIStringPart_Data data = {0,0};
	ULONG v=0;
	char *s;

	for (tags=((struct opSet *)msg)->ops_AttrList; (tag=NextTagItem(&tags)); )
	{
		switch (tag->ti_Tag)
		{
			case MUIA_GCIStringPart_Data:
				data.data = (APTR)tag->ti_Data;
				break;
			case MUIA_GCIStringPart_Type:
				data.type = tag->ti_Data;
				break;
		}
	}
	if (!data.data) return (0);

	if (!(obj = (Object *)DoSuperMethodA(cl,obj,msg))) return(0);

	memcpy(INST_DATA(cl,obj), &data, sizeof(data));
	
	set(obj,MUIA_String_MaxLen,10);
	set(obj,MUIA_String_Format,MUIV_String_Format_Right);
	switch (data.type) {
		case GCIStringPartType_Offset:
			v = data.data->offset;
			break;
		case GCIStringPartType_Length:
			v = data.data->length;
			break;
		case GCIStringPartType_End:
			v = data.data->end;
			break;
	}
	s = val2hex(v);
	set(obj,MUIA_String_Contents,s);
	set(obj,MUIA_String_Accept,StringHexAccept);
	set(obj,MUIA_CycleChain,1);

	return ((ULONG)obj);
}

SAVEDS ULONG
GCIStringPart_mChanged(
	struct IClass *cl,
	Object *obj,
	Msg msg
) {
	struct GCIStringPart_Data *data = (INST_DATA(cl,obj));
	struct GCIStringPart_DataShared *d = data->data;
	char *s, *t;
	ULONG v;
	
	get(obj, MUIA_String_Contents, &s );
	v = htoi( s );
	printf("changed obj=%p s=%s v=%ld\n",obj,s,v);

	switch (data->type) {
		case GCIStringPartType_Offset:
			if (v != d->offset) {
				/* if (offset > end) { offset = end } */
				if (v > d->end) {
					v = d->end;
					d->offset = v;
					set(obj,MUIA_String_Contents,val2hexm(v,s));
				} else {
					d->offset = v;
				}
				/* length = end - offset */
				get(d->gad_length,MUIA_String_Contents,&t);
				set(d->gad_length,MUIA_String_Contents,val2hexm(d->end-v,t));
			}
			break;
		case GCIStringPartType_Length:
			if (v != d->length) {
				/* if (offset + length > size) { length = end - offset } */
				if (d->offset + v > d->size) {
					v = d->end - d->offset;
					d->length = v;
					set(obj,MUIA_String_Contents,val2hexm(v,s));
				} else {
					d->length = v;
				}
				/* end = offset + length */
				get(d->gad_end,MUIA_String_Contents,&t);
				set(d->gad_end,MUIA_String_Contents,val2hexm(d->offset+v,t));
			}
			break;
		case GCIStringPartType_End:
			if (v != d->end) {
				/* if (end < offset) { end = offset } */
				if (v < d->offset) {
					v = d->offset;
					d->end = v;
					set(obj,MUIA_String_Contents,val2hexm(v,s));
				} else {
					d->end = v;
				}
				/* length = end - offset */
				get(d->gad_length,MUIA_String_Contents,&t);
				set(d->gad_length,MUIA_String_Contents,val2hexm(v-d->offset,t));
			}
			break;
	}
	
	return(TRUE);
}

SAVEDS ULONG
GCIStringPart_Dispatcher(
	struct IClass *cl __asm("a0"),
	Object *obj __asm("a2"),
	Msg msg __asm("a1")
) {
	switch (msg->MethodID)
	{
		case OM_NEW			: return(GCIStringPart_mNew		    (cl,obj,(APTR)msg));
		case MUIM_GCIStringPart_Changed	: return(GCIStringPart_mChanged		    (cl,obj,(APTR)msg));
	}
	return(DoSuperMethodA(cl,obj,msg));
}

/************************************************************************/

struct GCIStringRegister_Data {
	APTR ptr;		/* where data is stored */
	char *(*help)(ULONG);	/* bubble help */
	char type;		/* 8..64 bit */
	char readonly;
};

SAVEDS ULONG
GCIStringRegister_mNew(
	struct IClass *cl,
	Object *obj,
	Msg msg
) {
	struct TagItem *tags,*tag;
	struct GCIStringRegister_Data data = {0,0,0,0};
	ULONG v=0;
	UWORD maxlen=1;

	for (tags=((struct opSet *)msg)->ops_AttrList; (tag=NextTagItem(&tags)); )
	{
		switch (tag->ti_Tag)
		{
			case MUIA_GCIStringRegister_Pointer:
				data.ptr = (APTR)tag->ti_Data;
				break;
			case MUIA_GCIStringRegister_Help:
				data.help = (char *(*)(ULONG)) tag->ti_Data;
				break;
			case MUIA_GCIStringRegister_Type:
				data.type = tag->ti_Data;
				break;
			case MUIA_GCIStringRegister_ReadOnly:
				data.readonly = TRUE;
				break;
		}
	}

	if (!(obj = (Object *)DoSuperMethodA(cl,obj,msg))) return(0);

	memcpy(INST_DATA(cl,obj), &data, sizeof(data));

	switch (data.type)
	{
		case RegType_8:
			v = *(UBYTE*)data.ptr;
			maxlen = 4;
			break;
		case RegType_16:
			v = *(UWORD*)data.ptr;
			maxlen = 6;
			break;
		case RegType_24:
			v = *(ULONG*)data.ptr;
			maxlen = 8;
			break;
		case RegType_32:
			v = *(ULONG*)data.ptr;
			maxlen = 10;
			break;
		case RegType_64:
			set(obj,MUIA_String_Contents,val2hex64(*(ULONG*)data.ptr,*(1+(ULONG*)data.ptr)));
			maxlen = 18;
			break;
	}

	set(obj,MUIA_String_MaxLen,maxlen);
	set(obj,MUIA_String_Format,MUIV_String_Format_Right);
	if (data.type != RegType_64) set(obj,MUIA_String_Contents,val2hex(v));
	if (data.help) set(obj,MUIA_ShortHelp,data.help(v));


	if (data.readonly) {
		set(obj,MUIA_BetterString_NoInput,TRUE);
	} else {
		set(obj,MUIA_String_Accept,StringHexAccept);
		set(obj,MUIA_CycleChain,1);
		DoMethod(obj,MUIM_Notify,MUIA_String_Contents,MUIV_EveryTime,obj,1,MUIM_GCIStringRegister_Changed);
	}

	return ((ULONG)obj);
}

SAVEDS ULONG
GCIStringRegister_mChanged(
	struct IClass *cl,
	Object *obj,
	Msg msg
) {
	struct GCIStringRegister_Data *data = INST_DATA(cl,obj);
	char *s;
	ULONG v;

	get(obj, MUIA_String_Contents, &s );
	v = htoi( s );

	switch (data->type)
	{
		case RegType_8:
			*(UBYTE*)data->ptr = v;
			break;
		case RegType_16:
			*(UWORD*)data->ptr = v;
			break;
		case RegType_24:
			*(ULONG*)data->ptr = v;
			break;
		case RegType_32:
			*(ULONG*)data->ptr = v;
			break;
	}

	if (data->help) set(obj,MUIA_ShortHelp,data->help(v));

	return(TRUE);
}

SAVEDS ULONG
GCIStringRegister_Dispatcher(
	struct IClass *cl __asm("a0"),
	Object *obj __asm("a2"),
	Msg msg __asm("a1")
) {
	switch (msg->MethodID)
	{
		case OM_NEW				: return(GCIStringRegister_mNew			(cl,obj,(APTR)msg));
		case MUIM_GCIStringRegister_Changed	: return(GCIStringRegister_mChanged		(cl,obj,(APTR)msg));
	}
	return(DoSuperMethodA(cl,obj,msg));
}

/****************************************************************************/

int class_init() {
	GCIHexEdit_Class	= MUI_CreateCustomClass(NULL,MUIC_HexEdit,NULL,sizeof(struct GCIHexEdit_Data),GCIHexEdit_Dispatcher);
	GCIStringCustom_Class	= MUI_CreateCustomClass(NULL,MUIC_BetterString,NULL,sizeof(struct GCIStringCustom_Data),GCIStringCustom_Dispatcher);
	GCIStringPart_Class	= MUI_CreateCustomClass(NULL,MUIC_BetterString,NULL,sizeof(struct GCIStringPart_Data),GCIStringPart_Dispatcher);
	GCIStringRegister_Class = MUI_CreateCustomClass(NULL,MUIC_BetterString,NULL,sizeof(struct GCIStringRegister_Data),GCIStringRegister_Dispatcher);
	if (GCIHexEdit_Class || GCIStringCustom_Class || GCIStringPart_Class || GCIStringRegister_Class) {
		return 1;
	} else {
		class_finit();
		return 0;
	}
}

void class_finit() {
	if (GCIHexEdit_Class)		MUI_DeleteCustomClass(GCIHexEdit_Class);
	if (GCIStringCustom_Class)	MUI_DeleteCustomClass(GCIStringCustom_Class);
	if (GCIStringPart_Class)	MUI_DeleteCustomClass(GCIStringPart_Class);
	if (GCIStringRegister_Class)	MUI_DeleteCustomClass(GCIStringRegister_Class);
}

/****************************************************************************/

