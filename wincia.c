/*****************************************************************************
;  :Module.	wincia.c
;  :Author.	Bert Jahn
;  :Address.	Clara-Zetkin-Straﬂe 52, Zwickau, 08058, Germany
;  :Version.	$Id: wincia.c 1.2 2004/06/14 19:11:15 wepl Exp wepl $
;  :History.	13.06.04 started
;  :Copyright.	All Rights Reserved
;  :Language.	C
;  :Translator.	GCC
*****************************************************************************/

#define _CHKSTRLEN__

#include <stdio.h>

#include <libraries/mui.h>
#include <clib/alib_protos.h>
#include <clib/muimaster_protos.h>

#include "class.h"
#include "whddump.h"
#include "WHDLoadGCI.h"

/****************************************************************************/

APTR win_cia = NULL;

const char * prai_l = "pra in";
const char * prao_l = "pra out";
const char * ddra_l = "ddra";
const char * prbi_l = "prb in";
const char * prbo_l = "prb out";
const char * ddrb_l = "ddrb";
const char * ta_l = "ta";
const char * tb_l = "tb";
const char * pa_l = "ta max";
const char * pb_l = "tb max";
const char * event_l = "event";
const char * alarm_l = "alarm";
const char * sdr_l = "sd";
const char * icr_l = "icr";
const char * icm_l = "icm";
const char * cra_l = "cra";
const char * crb_l = "crb";

const char * prai_h = "Port A Input";
const char * prao_h = "Port A Output";
const char * ddra_h = "Data Direction Port A";
const char * aprbi_h = "Port B Input (parallel)";
const char * aprbo_h = "Port B Output (parallel)";
const char * addrb_h = "Data Direction Port B (parallel)";
const char * ta_h = "Timer A actual";
const char * tb_h = "Timer B actual";
const char * pa_h = "Latch Timer A";
const char * pb_h = "Latch Timer B";
const char * event_h = "Event Counter";
const char * alarm_h = "Alarm of Event Counter";
const char * sdr_h = "Seriell Data";

/****************************************************************************/

void apra_h (char *s,const char *ss,UBYTE v) {
	sprintf(s,MUIX_C MUIX_B "%s\n" MUIX_N MUIX_R
		"/ Fire 1 = %d\n"
		"/ Fire 0 = %d\n"
		"/ Disk Ready = %d\n"
		"/ Cylinder 0 = %d\n"
		"/ Write Protected = %d\n"
		"/ Changed = %d\n"
		"/ Filter/LED = %d\n"
		"Overlay = %d\n",
		ss,
		btst(v,7),
		btst(v,6),
		btst(v,5),
		btst(v,4),
		btst(v,3),
		btst(v,2),
		btst(v,1),
		btst(v,0)
	);
}
char * aprai_h (UBYTE v) {
	static char s[147];
	apra_h(s,prai_h,v);
#ifdef __CHKSTRLEN__
	printf("aprai_h v=$%x len=%ld<%ld\n",v,strlen(s),sizeof(s));
#endif
	return s;
}
char * aprao_h (UBYTE v) {
	static char s[148];
	apra_h(s,prao_h,v);
#ifdef __CHKSTRLEN__
	printf("aprao_h v=$%x len=%ld<%ld\n",v,strlen(s),sizeof(s));
#endif
	return s;
}
char * addra_h (UBYTE v) {
	static char s[156];
	apra_h(s,ddra_h,v);
#ifdef __CHKSTRLEN__
	printf("addra_h v=$%x len=%ld<%ld\n",v,strlen(s),sizeof(s));
#endif
	return s;
}

void bpra_h (char *s,const char *ss,UBYTE v) {
	sprintf(s,MUIX_C MUIX_B "%s\n" MUIX_N MUIX_R
		"/ DTR = %d\n"
		"/ RTS = %d\n"
		"/ CD = %d\n"
		"/ CTS = %d\n"
		"/ DSR = %d\n"
		"SEL = %d\n"
		"POUT = %d\n"
		"BUSY = %d\n",
		ss,
		btst(v,7),
		btst(v,6),
		btst(v,5),
		btst(v,4),
		btst(v,3),
		btst(v,2),
		btst(v,1),
		btst(v,0)
	);
}
char * bprai_h (UBYTE v) {
	static char s[147];
	bpra_h(s,prai_h,v);
#ifdef __CHKSTRLEN__
	printf("bprai_h v=$%x len=%ld<%ld\n",v,strlen(s),sizeof(s));
#endif
	return s;
}
char * bprao_h (UBYTE v) {
	static char s[148];
	bpra_h(s,prao_h,v);
#ifdef __CHKSTRLEN__
	printf("bprao_h v=$%x len=%ld<%ld\n",v,strlen(s),sizeof(s));
#endif
	return s;
}
char * bddra_h (UBYTE v) {
	static char s[156];
	bpra_h(s,ddra_h,v);
#ifdef __CHKSTRLEN__
	printf("bddra_h v=$%x len=%ld<%ld\n",v,strlen(s),sizeof(s));
#endif
	return s;
}

void bprb_h (char *s,char *ss,UBYTE v) {
	sprintf(s,MUIX_C MUIX_B "%s\n" MUIX_N MUIX_R
		"/ Motor = %d\n"
		"/ Select 3 = %d\n"
		"/ Select 2 = %d\n"
		"/ Select 1 = %d\n"
		"/ Select 0 = %d\n"
		"/ Diskside = %d\n"
		"Direction = %d\n"
		"Step = %d\n",
		ss,
		btst(v,7),
		btst(v,6),
		btst(v,5),
		btst(v,4),
		btst(v,3),
		btst(v,2),
		btst(v,1),
		btst(v,0)
	);
}
char * bprbi_h (UBYTE v) {
	static char s[147];
	bprb_h(s,"Port B Input",v);
#ifdef __CHKSTRLEN__
	printf("bprbi_h v=$%x len=%ld<%ld\n",v,strlen(s),sizeof(s));
#endif
	return s;
}
char * bprbo_h (UBYTE v) {
	static char s[148];
	bprb_h(s,"Port B Output",v);
#ifdef __CHKSTRLEN__
	printf("bprbo_h v=$%x len=%ld<%ld\n",v,strlen(s),sizeof(s));
#endif
	return s;
}
char * bddrb_h (UBYTE v) {
	static char s[156];
	bprb_h(s,"Data Direction Port B",v);
#ifdef __CHKSTRLEN__
	printf("bddrb_h v=$%x len=%ld<%ld\n",v,strlen(s),sizeof(s));
#endif
	return s;
}

void ic_h (char *s,char *ss,UBYTE v) {
	sprintf(s,MUIX_C MUIX_B "Interrupt Control %s\n" MUIX_N MUIX_R
		"Request = %d\n"
		"FLAG = %d\n"
		"SP = %d\n"
		"Alarm = %d\n"
		"Timer B = %d\n"
		"Timer A = %d\n",
		ss,
		btst(v,7),
		btst(v,4),
		btst(v,3),
		btst(v,2),
		btst(v,1),
		btst(v,0)
	);
}
char * aicr_h (UBYTE v) {
	static char s[97];
	ic_h(s,"Request",v);
#ifdef __CHKSTRLEN__
	printf("aicr_h v=$%x len=%ld<%ld\n",v,strlen(s),sizeof(s));
#endif
	return s;
}
char * aicm_h (UBYTE v) {
	static char s[94];
	ic_h(s,"Mask",v);
#ifdef __CHKSTRLEN__
	printf("aicm_h v=$%x len=%ld<%ld\n",v,strlen(s),sizeof(s));
#endif
	return s;
}
char * bicr_h (UBYTE v) {
	static char s[97];
	ic_h(s,"Request",v);
#ifdef __CHKSTRLEN__
	printf("bicr_h v=$%x len=%ld<%ld\n",v,strlen(s),sizeof(s));
#endif
	return s;
}
char * bicm_h (UBYTE v) {
	static char s[94];
	ic_h(s,"Mask",v);
#ifdef __CHKSTRLEN__
	printf("bicm_h v=$%x len=%ld<%ld\n",v,strlen(s),sizeof(s));
#endif
	return s;
}

void cra_h (char *s,UBYTE v) {
	sprintf(s,MUIX_C MUIX_B "Control A\n" MUIX_N MUIX_R
		"SP Mode = %s = %d\n"
		"In Mode = %s = %d\n"
		"Load = %d\n"
		"Run Mode = %s = %d\n"
		"Out Mode = %s = %d\n"
		"PB on = %d\n"
		"Start = %d\n",
		btst(v,6) ? "out" : "in",btst(v,6),
		btst(v,5) ? "eclock" : "cnt",btst(v,5),
		btst(v,4),
		btst(v,3) ? "oneshot" : "continue",btst(v,3),
		btst(v,2) ? "toggle" : "pulse",btst(v,2),
		btst(v,1),
		btst(v,0)
	);
}
char * acra_h (UBYTE v) {
	static char s[133];
	cra_h(s,v);
#ifdef __CHKSTRLEN__
	printf("acra_h v=$%x len=%ld<%ld\n",v,strlen(s),sizeof(s));
#endif
	return s;
}
char * bcra_h (UBYTE v) {
	static char s[133];
	cra_h(s,v);
#ifdef __CHKSTRLEN__
	printf("bcra_h v=$%x len=%ld<%ld\n",v,strlen(s),sizeof(s));
#endif
	return s;
}
void crb_h (char *s,UBYTE v) {
	UBYTE inmode = (v>>5)&3;
	const char * inmode_strs[] = {"eclock","cnt","timer a","timer a + cnt"};
	const char *inmode_str = inmode_strs[inmode];
	sprintf(s,MUIX_C MUIX_B "Control B\n" MUIX_N MUIX_R
		"Alarm Mode = %s = %d\n"
		"In Mode = %s = %d\n"
		"Load = %d\n"
		"Run Mode = %s = %d\n"
		"Out Mode = %s = %d\n"
		"PB on = %d\n"
		"Start = %d\n",
		btst(v,7) ? "alarm" : "tod",btst(v,7),
		inmode_str,inmode,
		btst(v,4),
		btst(v,3) ? "oneshot" : "continue",btst(v,3),
		btst(v,2) ? "toggle" : "pulse",btst(v,2),
		btst(v,1),
		btst(v,0)
	);
}
char * acrb_h (UBYTE v) {
	static char s[145];
	crb_h(s,v);
#ifdef __CHKSTRLEN__
	printf("acrb_h v=$%x len=%ld<%ld\n",v,strlen(s),sizeof(s));
#endif
	return s;
}
char * bcrb_h (UBYTE v) {
	static char s[145];
	crb_h(s,v);
#ifdef __CHKSTRLEN__
	printf("bcrb_h v=$%x len=%ld<%ld\n",v,strlen(s),sizeof(s));
#endif
	return s;
}

void make_win_cia(void) {
	ULONG open;

	if (win_cia) {
		get(win_cia,MUIA_Window_Open,&open);
		if (open)	set(win_cia,MUIA_Window_Open,FALSE);
		else		set(win_cia,MUIA_Window_Open,TRUE);
	} else {
		win_cia = WindowObject,
			MUIA_Window_Title, "Cia's",
			MUIA_Window_ID   , MAKE_ID('C','I','A',' '),
			WindowContents, HGroup,
				Child, HGroup, GroupFrameT("Cia-A ($bfe001)"), MUIA_Group_Columns, 2,
					Reg8LEI(prai_l,&ciaa->wdci_prai,aprai_h),
					Reg8LEI(prao_l,&ciaa->wdci_prao,aprao_h),
					Reg8LEI(ddra_l,&ciaa->wdci_ddra,addra_h),
					Reg8LEH(prbi_l,&ciaa->wdci_prbi,aprbi_h),
					Reg8LEH(prbo_l,&ciaa->wdci_prbo,aprbo_h),
					Reg8LEH(ddrb_l,&ciaa->wdci_ddrb,addrb_h),
					Reg16LEH(ta_l,&ciaa->wdci_ta,ta_h),
					Reg16LEH(pa_l,&ciaa->wdci_pa,pa_h),
					Reg16LEH(tb_l,&ciaa->wdci_tb,tb_h),
					Reg16LEH(pb_l,&ciaa->wdci_pb,pb_h),
					Reg24LEH(event_l,&ciaa->wdci_event,event_h),
					Reg24LEH(alarm_l,&ciaa->wdci_alarm,alarm_h),
					Reg8LEH(sdr_l,&ciaa->wdci_sdr,sdr_h),
					Reg8LRI(icr_l,&ciaa->wdci_icr,aicr_h),
					Reg8LEI(icm_l,&ciaa->wdci_icm,aicm_h),
					Reg8LEI(cra_l,&ciaa->wdci_cra,acra_h),
					Reg8LEI(crb_l,&ciaa->wdci_crb,acrb_h),
   					End,
				Child, HGroup, GroupFrameT("Cia-B ($bfd000)"), MUIA_Group_Columns, 2,
					Reg8LEI(prai_l,&ciab->wdci_prai,bprai_h),
					Reg8LEI(prao_l,&ciab->wdci_prao,bprao_h),
					Reg8LEI(ddra_l,&ciab->wdci_ddra,bddra_h),
					Reg8LEI(prbi_l,&ciab->wdci_prbi,bprbi_h),
					Reg8LEI(prbo_l,&ciab->wdci_prbo,bprbo_h),
					Reg8LEI(ddrb_l,&ciab->wdci_ddrb,bddrb_h),
					Reg16LEH(ta_l,&ciab->wdci_ta,ta_h),
					Reg16LEH(pa_l,&ciab->wdci_pa,pa_h),
					Reg16LEH(tb_l,&ciab->wdci_tb,tb_h),
					Reg16LEH(pb_l,&ciab->wdci_pb,pb_h),
					Reg24LEH(event_l,&ciab->wdci_event,event_h),
					Reg24LEH(alarm_l,&ciab->wdci_alarm,alarm_h),
					Reg8LEH(sdr_l,&ciab->wdci_sdr,sdr_h),
					Reg8LRI(icr_l,&ciab->wdci_icr,bicr_h),
					Reg8LEI(icm_l,&ciab->wdci_icm,bicm_h),
					Reg8LEI(cra_l,&ciab->wdci_cra,bcra_h),
					Reg8LEI(crb_l,&ciab->wdci_crb,bcrb_h),
					End,
				End,
			End;
		if (win_cia) {
			DoMethod(app,OM_ADDMEMBER,win_cia);
			DoMethod(win_cia,MUIM_Notify,MUIA_Window_CloseRequest,TRUE,app,2,MUIM_Application_ReturnID,MAIN_MORECIA);
			set(win_cia,MUIA_Window_Open,TRUE);
		} else {
			MUI_Request(app,win,0,NULL,"Ok","Couldn't open Cia's window.");
		}
	}
}

/****************************************************************************/

