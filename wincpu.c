/*****************************************************************************
;  :Module.	wincpu.c
;  :Author.	Bert Jahn
;  :Address.	Franz-Liszt-Straﬂe 16, Rudolstadt, 07404, Germany
;  :Version.	$Id: wincpu.c 0.7 2004/06/14 19:11:52 wepl Exp wepl $
;  :History.	18.07.98 started
;		03.03.00 bubble help added to all fields
;			 some cleanup
;		28.03.00 more cleanup
;  :Copyright.	All Rights Reserved
;  :Language.	C
;  :Translator.	GCC
****************************************************************************/

#include <exec/execbase.h>
#include <libraries/mui.h>

#include <clib/alib_protos.h>
#include <clib/muimaster_protos.h>

#include "class.h"
#include "whddump.h"
#include "WHDLoadGCI.h"

/****************************************************************************/

APTR win_cpu = NULL;

/****************************************************************************/

#define RegSFC Reg8LRH(sfc,&cpu->wdc_sfc,sfc_h)
#define RegDFC Reg8LRH(dfc,&cpu->wdc_dfc,dfc_h)
#define RegVBR Reg32LRH(vbr,&cpu->wdc_vbr,vbr_h)
#define RegMSP Reg32LEH(msp,&cpu->wdc_msp,msp_h)
#define RegCAAR Reg32LRH(caar,&cpu->wdc_caar,caar_h)

/****************************************************************************/

void make_win_cpu(void) {
	ULONG open;
	const char
		*sfc="SFC:",
		*dfc="DFC:",
		*vbr="VBR:",
		*msp="MSP:",
		*caar="CAAR:",
		*cacr="CACR:",
		*tc="TC:",
		*crp="CRP:",
		*srp="SRP:",
		*urp="URP:",
		*mmusr="MMUSR:",
		*tt0="TT0:",
		*tt1="TT1:",
		*dtt0="DTT0:",
		*dtt1="DTT1:",
		*itt0="ITT0:",
		*itt1="ITT1:",
		*sfc_h="Source Function Code",
		*dfc_h="Destination Function Code",
		*vbr_h="Vector Base Register",
		*msp_h="Master Stack Pointer",
		*caar_h="Cache Address Register",
		*cacr_h="Cache Control Register",
		*tc_h="Translation Control (MMU)",
		*crp_h="CPU Root Pointer (MMU)",
		*srp_h="Supervisor Root Pointer (MMU)",
		*urp_h="User Root Pointer (MMU)",
		*mmusr_h="MMU Status Register",
		*tt0_h="Transparent Translation 0",
		*tt1_h="Transparent Translation 1",
		*dtt0_h="Data Transparent Translation 0",
		*dtt1_h="Data Transparent Translation 1",
		*itt0_h="Instruction Transparent Translation 0",
		*itt1_h="Instruction Transparent Translation 1",
		*t_morecpu="More CPU";

	if (win_cpu) {
		get(win_cpu,MUIA_Window_Open,&open);
		if (open)	set(win_cpu,MUIA_Window_Open,FALSE);
		else		set(win_cpu,MUIA_Window_Open,TRUE);
	} else {
		switch ((header->wdh_CPU)&(AFF_68010|AFF_68020|AFF_68030|AFF_68040|AFF_68060)) {
		case 0:
			win_cpu = WindowObject,
				MUIA_Window_Title, t_morecpu,
				MUIA_Window_ID   , MAKE_ID('C','P','U',' '),
				WindowContents,
					TextObject, GroupFrame, MUIA_Text_Contents, "\33cMC68000", End,
				End;
			break;
		case AFF_68010:
			win_cpu = WindowObject,
				MUIA_Window_Title, t_morecpu,
				MUIA_Window_ID   , MAKE_ID('C','P','U',' '),
				WindowContents, VGroup,
					Child, TextObject, GroupFrame, MUIA_Text_Contents, "\33cMC68010", End,
					Child, HGroup, MUIA_Group_Columns, 2,
						RegSFC,
						RegDFC,
						RegVBR,
						End,
					End,
				End;
			break;
		case AFF_68010|AFF_68020:
			win_cpu = WindowObject,
				MUIA_Window_Title, t_morecpu,
				MUIA_Window_ID   , MAKE_ID('C','P','U',' '),
				WindowContents, VGroup,
					Child, TextObject, GroupFrame, MUIA_Text_Contents, "\33cMC68020", End,
					Child, HGroup, MUIA_Group_Columns, 2,
						RegSFC,
						RegDFC,
						RegVBR,
						RegMSP,
						RegCAAR,
						Reg32LEH(cacr,&cpu->wdc_cacr,cacr_h),
						End,
					End,
				End;
			break;
		case AFF_68010|AFF_68020|AFF_68030:
			win_cpu = WindowObject,
				MUIA_Window_Title, t_morecpu,
				MUIA_Window_ID   , MAKE_ID('C','P','U',' '),
				WindowContents, VGroup,
					Child, TextObject, GroupFrame, MUIA_Text_Contents, "\33cMC68030", End,
					Child, HGroup, MUIA_Group_Columns, 2,
						RegSFC,
						RegDFC,
						RegVBR,
						RegMSP,
						RegCAAR,
						Reg32LEH(cacr,&cpu->wdc_cacr,cacr_h),
						Reg32LRH(tc,&cpu->wdc_tc,tc_h),
						Reg64LRH(crp,&cpu->wdc_crp,crp_h),
						Reg64LRH(srp,&cpu->wdc_srp,srp_h),
						Reg32LRH(mmusr,&cpu->wdc_mmusr,mmusr_h),
						Reg32LRH(tt0,&cpu->wdc_tt0,tt0_h),
						Reg32LRH(tt1,&cpu->wdc_tt1,tt1_h),
						End,
					End,
				End;
			break;
		case AFF_68010|AFF_68020|AFF_68030|AFF_68040:
			win_cpu = WindowObject,
				MUIA_Window_Title, t_morecpu,
				MUIA_Window_ID   , MAKE_ID('C','P','U',' '),
				WindowContents, VGroup,
					Child, TextObject, GroupFrame, MUIA_Text_Contents, "\33cMC68040", End,
					Child, HGroup, MUIA_Group_Columns, 2,
						RegSFC,
						RegDFC,
						RegVBR,
						RegMSP,
						Reg32LEH(cacr,&cpu->wdc_cacr,cacr_h),
						Reg32LRH(tc,&cpu->wdc_tc,tc_h),
						Reg32LRH(srp,&cpu->wdc_srp,srp_h),
						Reg32LRH(urp,&cpu->wdc_urp,urp_h),
						Reg32LRH(mmusr,&cpu->wdc_mmusr,mmusr_h),
						Reg32LRH(dtt0,&cpu->wdc_dtt0,dtt0_h),
						Reg32LRH(dtt1,&cpu->wdc_dtt1,dtt1_h),
						Reg32LRH(itt0,&cpu->wdc_itt0,itt0_h),
						Reg32LRH(itt1,&cpu->wdc_itt1,itt1_h),
						End,
					End,
				End;
			break;
		case AFF_68010|AFF_68020|AFF_68030|AFF_68040|AFF_68060:
			win_cpu = WindowObject,
				MUIA_Window_Title, t_morecpu,
				MUIA_Window_ID   , MAKE_ID('C','P','U',' '),
				WindowContents, VGroup,
					Child, TextObject, GroupFrame, MUIA_Text_Contents, "\33cMC68060", End,
					Child, HGroup, MUIA_Group_Columns, 2,
						RegSFC,
						RegDFC,
						RegVBR,
						Reg32LEH(cacr,&cpu->wdc_cacr,cacr_h),
						Reg32LEH("PCR:",&cpu->wdc_pcr,"Processor Configuration Register"),
						Reg32LEH("BUSCR:",&cpu->wdc_buscr,"Bus Control Register"),
						Reg32LRH(tc,&cpu->wdc_tc,tc_h),
						Reg32LRH(srp,&cpu->wdc_srp,srp_h),
						Reg32LRH(urp,&cpu->wdc_urp,urp_h),
						Reg32LRH(mmusr,&cpu->wdc_mmusr,mmusr_h),
						Reg32LRH(dtt0,&cpu->wdc_dtt0,dtt0_h),
						Reg32LRH(dtt1,&cpu->wdc_dtt1,dtt1_h),
						Reg32LRH(itt0,&cpu->wdc_itt0,itt0_h),
						Reg32LRH(itt1,&cpu->wdc_itt1,itt1_h),
						End,
					End,
				End;
			break;
		}
		if (win_cpu) {
			DoMethod(app,OM_ADDMEMBER,win_cpu);
			DoMethod(win_cpu,MUIM_Notify,MUIA_Window_CloseRequest,TRUE,app,2,MUIM_Application_ReturnID,MAIN_MORECPU);
			set(win_cpu,MUIA_Window_Open,TRUE);
		} else {
			MUI_Request(app,win,0,NULL,"Ok","Couldn't open CPU window.");
		}
	}
}

/****************************************************************************/

