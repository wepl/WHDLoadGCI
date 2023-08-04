/*************************************************************************
;  :Module.	class.h
;  :Author.	Bert Jahn
;  :Version.	$Id: class.h 1.7 2020/04/27 22:53:40 wepl Exp wepl $
;  :History.	18.07.98 started
;		12.07.03 changes for reg to mem
;		30.12.18 MUIM_GCIHexEdit_NewFromCursor added
;		20.08.19 MUIA_GCIHexEdit_CursorAddressAdd added
;  :Copyright.	All Rights Reserved
;  :Language.	C
;  :Translator.	GCC
*************************************************************************/

/************************************************************************/
/*	defines								*/
/************************************************************************/

enum {
	MUIA_GCIHexEdit_CursorAddressAbs=TAG_USER,
	MUIA_GCIHexEdit_CursorAddressRel,
	MUIA_GCIHexEdit_CursorAddressAdd,
	MUIM_GCIHexEdit_NewFromCursor,
	MUIA_GCIStringCustom_Offset,
	MUIA_GCIStringCustom_Help,
	MUIA_GCIStringCustom_Long,
	MUIA_GCIStringCustom_ReadWrite,		/* in this case is _Offset=read and _ReadWrite=write */
	MUIM_GCIStringCustom_Changed,
	MUIA_GCIStringPart_Data,
	MUIA_GCIStringPart_Type,
	MUIM_GCIStringPart_Changed,
	MUIA_GCIStringRegister_Pointer,
	MUIA_GCIStringRegister_Help,
	MUIA_GCIStringRegister_Type,
	MUIA_GCIStringRegister_ReadOnly,
	MUIM_GCIStringRegister_Changed
};

enum {
	RegType_8, RegType_16, RegType_24, RegType_32, RegType_64
};

struct GCIStringPart_DataShared {
	ULONG offset;
	ULONG length;
	ULONG end;
	ULONG size;
	APTR gad_offset;
	APTR gad_length;
	APTR gad_end;
};

enum {
	GCIStringPartType_Offset,
	GCIStringPartType_Length,
	GCIStringPartType_End
};

/************************************************************************/
/* extern variables							*/
/************************************************************************/

extern struct MUI_CustomClass *GCIHexEdit_Class;
extern struct MUI_CustomClass *GCIStringCustom_Class;
extern struct MUI_CustomClass *GCIStringPart_Class;
extern struct MUI_CustomClass *GCIStringRegister_Class;
extern const char* StringHexAccept;
extern const char* StringHexMinAccept;

/************************************************************************/
/*	macros								*/
/************************************************************************/

/************************************************************************/
/*	mui macros for GCIStringCustom					*/
/************************************************************************/

/*
 *  create a label and a custom word register string gadget
 *  label	- label of the gadget (char*)
 *  offset	- register offset (UWORD)
 */
#define RegCustomWord(label,offset)\
	Child, Label2(label),\
	Child, NewObject(GCIStringCustom_Class->mcc_Class,0,\
		StringFrame,\
		MUIA_GCIStringCustom_Offset,offset,\
End

/*
 *  create a label and a custom long register string gadget
 *  label	- label of the gadget (char*)
 *  offset	- register offset (UWORD)
 */
#define RegCustomLong(label,offset)\
	Child, Label2(label),\
	Child, NewObject(GCIStringCustom_Class->mcc_Class,0,\
		StringFrame,\
		MUIA_GCIStringCustom_Offset,offset,\
		MUIA_GCIStringCustom_Long,TRUE,\
End

/*
 *  create a button and custom long register, editable
 *  object	- variable of button object to create (APTR)
 *  label	- label for button (char*)
 *  offset	- register offset (UWORD)
 */
#define RegCust32BE(object,label,offset)\
	Child, object = SimpleButton(label),\
	Child, NewObject(GCIStringCustom_Class->mcc_Class,0,\
		StringFrame,\
		MUIA_GCIStringCustom_Offset,offset,\
		MUIA_GCIStringCustom_Long,TRUE,\
End

/*
 *  create a label and a custom read/write word register string gadget with help
 *  label	- label of the gadget (char*)
 *  offset	- register offset for read (UWORD)
 *  write	- register offset for write (UWORD)
 *  help	- function for bubble help (char (*) (UWORD))
 */
#define RegCustomWordReadWrite(label,offset,write,help)\
	Child, Label2(label),\
	Child, NewObject(GCIStringCustom_Class->mcc_Class,0,\
		StringFrame,\
		MUIA_GCIStringCustom_Offset,offset,\
		MUIA_GCIStringCustom_ReadWrite,write,\
		MUIA_GCIStringCustom_Help,help,\
End

/*
 *  create a label and a custom word register string gadget with help
 *  label	- label of the gadget (char*)
 *  offset	- register offset (UWORD)
 *  help	- function for bubble help (char (*) (UWORD))
 */
#define RegCustomWordHelp(label,offset,help)\
	Child, Label2(label),\
	Child, NewObject(GCIStringCustom_Class->mcc_Class,0,\
		StringFrame,\
		MUIA_GCIStringCustom_Offset,offset,\
		MUIA_GCIStringCustom_Help,help,\
End

/*
 *  create a label and a custom word register string gadget with help
 *  label	- label of the gadget (char*)
 *  offset	- register offset (UWORD)
 *  help	- bubble help (char*)
 */
#define RegCustomWordHelpS(label,offset,help)\
	Child, Label2(label),\
	Child, NewObject(GCIStringCustom_Class->mcc_Class,0,\
		StringFrame,\
		MUIA_GCIStringCustom_Offset,offset,\
		MUIA_ShortHelp,help,\
End

/*
 *  create a label and a custom long register string gadget with help
 *  label	- label of the gadget (char*)
 *  offset	- register offset (UWORD)
 *  help	- function for bubble help (char (*) (LONG))
 */
#define RegCustomLongHelp(label,offset,help)\
	Child, Label2(label),\
	Child, NewObject(GCIStringCustom_Class->mcc_Class,0,\
		StringFrame,\
		MUIA_GCIStringCustom_Offset,offset,\
		MUIA_GCIStringCustom_Long,TRUE,\
		MUIA_GCIStringCustom_Help,help,\
End

/*
 *  create a label and a custom long register string gadget with help
 *  label	- label of the gadget (char*)
 *  offset	- register offset (UWORD)
 *  help	- bubble help (char*)
 */
#define RegCustomLongHelpS(label,offset,help)\
	Child, Label2(label),\
	Child, NewObject(GCIStringCustom_Class->mcc_Class,0,\
		StringFrame,\
		MUIA_GCIStringCustom_Offset,offset,\
		MUIA_GCIStringCustom_Long,TRUE,\
		MUIA_ShortHelp,help,\
End

/************************************************************************/
/*	mui macros for GCIStringRegister				*/
/************************************************************************/

/*
 *  create a label and a byte register string gadget
 *  label	- label of the gadget (char*)
 *  pointer	- data address (UBYTE)
 *  help	- bubble help (char*)
 */
#define Reg8LEH(label,pointer,help)\
	Child, Label2(label),\
	Child, NewObject(GCIStringRegister_Class->mcc_Class,0,\
		StringFrame,\
		MUIA_GCIStringRegister_Pointer,pointer,\
		MUIA_ShortHelp,help,\
End

/*
 *  create a label and a byte register string gadget
 *  label	- label of the gadget (char*)
 *  pointer	- data address (UBYTE*)
 *  help	- bubble help (char*)
 */
#define Reg8LRH(label,pointer,help)\
	Child, Label2(label),\
	Child, NewObject(GCIStringRegister_Class->mcc_Class,0,\
		StringFrame,\
		MUIA_GCIStringRegister_Pointer,pointer,\
		MUIA_GCIStringRegister_ReadOnly,TRUE,\
		MUIA_ShortHelp,help,\
End

/*
 *  create a label and a byte register string gadget
 *  label	- label of the gadget (char*)
 *  pointer	- data address (UBYTE)
 *  help	- function for bubble help (char (*) (UWORD))
 */
#define Reg8LEI(label,pointer,help)\
	Child, Label2(label),\
	Child, NewObject(GCIStringRegister_Class->mcc_Class,0,\
		StringFrame,\
		MUIA_GCIStringRegister_Pointer,pointer,\
		MUIA_GCIStringRegister_Help,help,\
End

/*
 *  create a label and a byte register string gadget
 *  label	- label of the gadget (char*)
 *  pointer	- data address (UBYTE*)
 *  help	- function for bubble help (char (*) (UWORD))
 */
#define Reg8LRI(label,pointer,help)\
	Child, Label2(label),\
	Child, NewObject(GCIStringRegister_Class->mcc_Class,0,\
		StringFrame,\
		MUIA_GCIStringRegister_Pointer,pointer,\
		MUIA_GCIStringRegister_ReadOnly,TRUE,\
		MUIA_GCIStringRegister_Help,help,\
End

/*
 *  create a label and a word register string gadget
 *  label	- label of the gadget (char*)
 *  pointer	- data address (UWORD)
 *  help	- bubble help (char*)
 */
#define Reg16LEH(label,pointer,help)\
	Child, Label2(label),\
	Child, NewObject(GCIStringRegister_Class->mcc_Class,0,\
		StringFrame,\
		MUIA_GCIStringRegister_Pointer,pointer,\
		MUIA_GCIStringRegister_Type,RegType_16,\
		MUIA_ShortHelp,help,\
End

/*
 *  create a label and 16bit register, editable, bubble help
 *  label	- label of the gadget (char*)
 *  pointer	- data address (UWORD)
 *  help	- function for bubble help (char (*) (UWORD))
 */
#define Reg16LEI(label,pointer,help)\
	Child, Label2(label),\
	Child, NewObject(GCIStringRegister_Class->mcc_Class,0,\
		StringFrame,\
		MUIA_GCIStringRegister_Pointer,pointer,\
		MUIA_GCIStringRegister_Type,RegType_16,\
		MUIA_GCIStringRegister_Help,help,\
End

/*
 *  create a label and a word register string gadget
 *  label	- label of the gadget (char*)
 *  pointer	- data address (UWORD)
 *  help	- bubble help (char*)
 */
#define Reg16LRH(label,pointer,help)\
	Child, Label2(label),\
	Child, NewObject(GCIStringRegister_Class->mcc_Class,0,\
		StringFrame,\
		MUIA_GCIStringRegister_Pointer,pointer,\
		MUIA_GCIStringRegister_Type,RegType_16,\
		MUIA_GCIStringRegister_ReadOnly,TRUE,\
		MUIA_ShortHelp,help,\
End

/*
 *  create a label and a 24-bit register string gadget
 *  label	- label of the gadget (char*)
 *  pointer	- data address (ULONG)
 *  help	- bubble help (char*)
 */
#define Reg24LEH(label,pointer,help)\
	Child, Label2(label),\
	Child, NewObject(GCIStringRegister_Class->mcc_Class,0,\
		StringFrame,\
		MUIA_GCIStringRegister_Pointer,pointer,\
		MUIA_GCIStringRegister_Type,RegType_24,\
		MUIA_ShortHelp,help,\
End

/*
 *  create a button and 32bit register, editable
 *  object	- variable of button object to create (APTR)
 *  label	- label for button (char*)
 *  pointer	- data address (ULONG*)
 */
#define Reg32BE(object,label,pointer)\
	Child, object = SimpleButton(label),\
	Child, NewObject(GCIStringRegister_Class->mcc_Class,0,\
		StringFrame,\
		MUIA_GCIStringRegister_Pointer,pointer,\
		MUIA_GCIStringRegister_Type,RegType_32,\
End

/*
 *  create a button and 32bit register, editable, buble help
 *  object	- variable of button object to create (APTR)
 *  label	- label for button (char*)
 *  pointer	- data address (ULONG*)
 *  help	- text for bubble help (char*)
 */
#define Reg32BEH(object,label,pointer,help)\
	Child, object = SimpleButton(label),\
	Child, NewObject(GCIStringRegister_Class->mcc_Class,0,\
		StringFrame,\
		MUIA_GCIStringRegister_Pointer,pointer,\
		MUIA_GCIStringRegister_Type,RegType_32,\
		MUIA_ShortHelp, help,\
End

/*
 *  create a label and a long register string gadget
 *  label	- label of the gadget (char*)
 *  pointer	- data address (ULONG*)
 *  help	- bubble help (char*)
 */
#define Reg32LEH(label,pointer,help)\
	Child, Label2(label),\
	Child, NewObject(GCIStringRegister_Class->mcc_Class,0,\
		StringFrame,\
		MUIA_GCIStringRegister_Pointer,pointer,\
		MUIA_GCIStringRegister_Type,RegType_32,\
		MUIA_ShortHelp,help,\
End

/*
 *  create a label and a long register string gadget
 *  label	- label of the gadget (char*)
 *  pointer	- data address (ULONG*)
 *  help	- bubble help (char*)
 */
#define Reg32LRH(label,pointer,help)\
	Child, Label2(label),\
	Child, NewObject(GCIStringRegister_Class->mcc_Class,0,\
		StringFrame,\
		MUIA_GCIStringRegister_Pointer,pointer,\
		MUIA_GCIStringRegister_ReadOnly,TRUE,\
		MUIA_GCIStringRegister_Type,RegType_32,\
		MUIA_ShortHelp,help,\
End

/*
 *  create a label and a long long register string gadget
 *  label	- label of the gadget (char*)
 *  pointer	- data address (ULONG[2])
 *  help	- bubble help (char*)
 */
#define Reg64LRH(label,pointer,help)\
	Child, Label2(label),\
	Child, NewObject(GCIStringRegister_Class->mcc_Class,0,\
		StringFrame,\
		MUIA_GCIStringRegister_Pointer,pointer,\
		MUIA_GCIStringRegister_ReadOnly,TRUE,\
		MUIA_GCIStringRegister_Type,RegType_64,\
		MUIA_ShortHelp,help,\
End

/************************************************************************/
/*	function prototypes						*/
/************************************************************************/

int	class_init	(void);
void	class_finit	(void);

/************************************************************************/

