/*************************************************************************
;  :Module.	wininfo.c
;  :Author.	Bert Jahn
;  :EMail.	wepl@whdload.de
;  :Version.	$Id: wininfo.c 1.9 2019/08/15 22:00:04 wepl Exp wepl $
;  :History.	22.03.00 created
;		24.02.08 slave structure infos added
;		15.08.19 added output for end of memory regions
;  :Copyright.	All Rights Reserved
;  :Language.	C
;  :Translator.	GCC
*************************************************************************/

#include <stdio.h>
#include <string.h>

#include <libraries/mui.h>

#include <clib/alib_protos.h>
#include <clib/muimaster_protos.h>

#include "WHDLoadGCI.h"
#include "whddump.h"

/************************************************************************/
/* defines								*/
/************************************************************************/

/************************************************************************/
/* extern variables							*/
/************************************************************************/

/************************************************************************/
/* static variables							*/
/************************************************************************/

APTR win_info = NULL;

/************************************************************************/
/* function declarations						*/
/************************************************************************/

/************************************************************************/

void make_win_info(void) {
	ULONG open;
	char buf[2048], flags[16*60], currentdir[256]="", dontcache[256]="", temp[256], *s;
	int t;
	UWORD *p;

	if (win_info) {
		get(win_info,MUIA_Window_Open,&open);
		if (open)	set(win_info,MUIA_Window_Open,FALSE);
		else		set(win_info,MUIA_Window_Open,TRUE);
	} else {

	flags[0] = 0;
	t = slave->ws_Flags;
	if (t & 1) { strcat(flags," Disk - diskimages are used\n"); }
	if (t & 2) { strcat(flags," NoError - quit on every error\n"); }
	if (t & 4) { strcat(flags," EmulTrap - emulate trap #n instructions\n"); }
	if (t & 8) { strcat(flags," NoDivZero - emulate division by zero exceptions\n"); }
	if (t & 16) { strcat(flags," Req68020 - MC68020+ required\n"); }
	if (t & 32) { strcat(flags," ReqAGA - AGA chipset required\n"); }
	if (t & 64) { strcat(flags," NoKbd - WHDLoad permitted to touch keyboard hardware\n"); }
	if (t & 128) { strcat(flags," EmulLineA - emulate line-a execptions\n"); }
	if (t & 256) { strcat(flags," EmulTrapV - emulate trapv exceptions\n"); }
	if (t & 512) { strcat(flags," EmulChk - emulate chk/chk2 exceptions\n"); }
	if (t & 1024) { strcat(flags," EmulPriv - emulate privilege violation exceptions\n"); }
	if (t & 2048) { strcat(flags," EmulLineF - emulate line-f exceptions\n"); }
	if (t & 4096) { strcat(flags," ClearMem - clear all memory\n"); }
	if (t & 8192) { strcat(flags," Examine - uses resload_Examine/ExNext\n"); }
	if (t & 16384) { strcat(flags," EmulDivZero - emulate division by zero exceptions\n"); }
	if (t & 32768) { strcat(flags," EmulIllegal - emulate illegal instruction exceptions\n"); }

	if (slave->ws_CurrentDir) {
		sprintf(currentdir," = \"%s\"",(STRPTR)((APTR)slave)+slave->ws_CurrentDir);
	}
	if (slave->ws_DontCache) {
		sprintf(dontcache," = \"%s\"",(STRPTR)((APTR)slave)+slave->ws_DontCache);
	}

	sprintf(buf,
		MUIX_B "WHDLoad variables:" MUIX_N "\n"
		"BaseMemSize = $%lx = %ld\n"
		"ShadowMem = $%lx\n"
		"TermReason = $%lx = %ld\n"
		"TermPrimary = $%lx = %ld\n"
		"TermSecondary = $%lx = %ld\n"
		"TermString = \"%s\"\n"
		"LastBlitPC = $%lx\n"
		"ExpMemLog = $%lx - $%lx\n"
		"ExpMemPhy = $%lx - $%lx\n"
		"ExpMemLen = $%lx = %ld\n"
		"ResLoadLog = $%lx - $%lx\n"
		"ResLoadPhy = $%lx - $%lx\n"
		"ResLoadLen = $%lx = %ld\n"
		"SlaveLog = $%lx - $%lx\n"
		"SlavePhy = $%lx - $%lx\n"
		"SlaveLen = $%lx = %ld\n"
		"SlaveName = \"%s\"\n"
		"kn = %ld\n"
		"rw = $%lx\n"
		"cs = $%x\n"
		"CPU = $%x\n"
		"WVer = %d\n"
		"WRev = %d\n"
		"WBuild = %d\n"
		"fc = %d\n"
		"zpt = %d\n"
		MUIX_B "Slave structure:" MUIX_N "\n"
		"Version = %d\n"
		"Flags = $%x\n%s"
		"ExecInstall = $%lx\n"
		"GameLoader = $%x (slave offset) = $%lx (absolut)\n"
		"CurrentDir = $%x%s\n"
		"DontCache = $%x%s"
		,header->wdh_BaseMemSize,header->wdh_BaseMemSize
		,header->wdh_ShadowMem
		,header->wdh_TermReason,header->wdh_TermReason
		,header->wdh_TermPrimary,header->wdh_TermPrimary
		,header->wdh_TermSecondary,header->wdh_TermSecondary
		,header->wdh_TermString
		,header->wdh_LastBlitPC
		,header->wdh_ExpMemLog,header->wdh_ExpMemLog+header->wdh_ExpMemLen
		,header->wdh_ExpMemPhy,header->wdh_ExpMemPhy+header->wdh_ExpMemLen
		,header->wdh_ExpMemLen,header->wdh_ExpMemLen
		,header->wdh_ResLoadLog,header->wdh_ResLoadLog+header->wdh_ResLoadLen
		,header->wdh_ResLoadPhy,header->wdh_ResLoadPhy+header->wdh_ResLoadLen
		,header->wdh_ResLoadLen,header->wdh_ResLoadLen
		,header->wdh_SlaveLog,header->wdh_SlaveLog+header->wdh_SlaveLen
		,header->wdh_SlavePhy,header->wdh_SlavePhy+header->wdh_SlaveLen
		,header->wdh_SlaveLen,header->wdh_SlaveLen
		,header->wdh_SlaveName
		,header->wdh_kn
		,header->wdh_rw
		,header->wdh_cs
		,header->wdh_CPU
		,header->wdh_WVer
		,header->wdh_WRev
		,header->wdh_WBuild
		,header->wdh_fc
		,header->wdh_zpt
		,slave->ws_Version
		,slave->ws_Flags,flags
		,slave->ws_ExecInstall
		,slave->ws_GameLoader,header->wdh_SlaveLog+slave->ws_GameLoader
		,slave->ws_CurrentDir,currentdir
		,slave->ws_DontCache,dontcache
	);
	if (slave->ws_Version >= 4) {
		sprintf(temp,"\nkeydebug = $%x\nkeyexit =$%x",
			slave->ws_keydebug, slave->ws_keyexit);
		strcat(buf,temp);
	}
	if (slave->ws_Version >= 8) {
		sprintf(temp,"\nExpMem = $%lx",	slave->ws_ExpMem);
		strcat(buf,temp);
	}
	if (slave->ws_Version >= 10) {
		sprintf(temp,"\nname = $%x", slave->ws_name);
		strcat(buf,temp);
		sprintf(temp," = \"%s\"",(STRPTR)((APTR)slave)+slave->ws_name);
		strcat(buf,temp);
		sprintf(temp,"\ncopy = $%x", slave->ws_copy);
		strcat(buf,temp);
		sprintf(temp," = \"%s\"",(STRPTR)((APTR)slave)+slave->ws_copy);
		strcat(buf,temp);
		sprintf(temp,"\ninfo = $%x", slave->ws_info);
		strcat(buf,temp);
		sprintf(temp," = \"%s\"",(STRPTR)((APTR)slave)+slave->ws_info);
		while ((s = strpbrk(temp,"\x0ff"))) { *s='\n'; }
		strcat(buf,temp);
	}
	if (slave->ws_Version >= 16) {
		sprintf(temp,"\nkickname = $%x", slave->ws_kickname);
		strcat(buf,temp);
		if (slave->ws_kickname) {
			p = ((APTR)slave)+slave->ws_kickname;
			if (
				slave->ws_kickcrc == *p ||
				slave->ws_kickcrc == *(p+2) ||
				slave->ws_kickcrc == *(p+4)
			) {
				while (*p) {
					sprintf(temp,"\n $%x,\"%s\"",*p,(STRPTR)((APTR)slave) + *(p+1));
					strcat(buf,temp);
					p += 2;
				}

			} else {
				sprintf(temp," = \"%s\"",(STRPTR)((APTR)slave)+slave->ws_kickname);
				strcat(buf,temp);
			}
		}
		sprintf(temp,"\nkicksize = $%lx\nkickcrc = $%x", slave->ws_kicksize,slave->ws_kickcrc);
		strcat(buf,temp);
	}

		win_info = WindowObject,
			MUIA_Window_Title, "WHDLoad Info",
			MUIA_Window_ID   , MAKE_ID('I','N','F','O'),
			WindowContents,
				TextObject, TextFrame, 
					MUIA_Background, MUII_TextBack,
					MUIA_Text_Contents, buf,
					End,
			End;
		if (win_info) {
			DoMethod(app,OM_ADDMEMBER,win_info);
			DoMethod(win_info,MUIM_Notify,MUIA_Window_CloseRequest,TRUE,app,2,MUIM_Application_ReturnID,MAIN_MOREINFO);
			set(win_info,MUIA_Window_Open,TRUE);
		} else {
			MUI_Request(app,win,0,NULL,"Ok","Couldn't open Info window.");
		}
	}
}

/************************************************************************/

