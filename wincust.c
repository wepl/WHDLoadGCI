/*****************************************************************************
;  :Module.	wincust.c
;  :Author.	Bert Jahn
;  :Address.	Clara-Zetkin-Straﬂe 52, Zwickau, 08058, Germany
;  :Version.	$Id: wincust.c 0.8 2005/02/21 21:56:17 wepl Exp wepl $
;  :History.	18.07.98 started
;		09.06.04 complete rework for new custom class
;		12.06.04 more bubble help added
;		21.02.05 help for bplcon* added
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

APTR win_cust = NULL;

/****************************************************************************/

char * adkcon_h (UWORD v) {
	static char s[406];
	UWORD precmp = (v>>13)&3;
	const char *precmp_strs[] = {"none","140 ns","280 ns","560 ns"};
	const char *precmp_str = precmp_strs[precmp];
	UWORD mfmprec = btst(v,12);
	const char *mfmprec_strs[] = {"MFM","GCR"};
	const char *mfmprec_str = mfmprec_strs[mfmprec];
	UWORD fast = btst(v,8);
	const char *fast_strs[] = {"2 µs","4 µs"};
	const char *fast_str = fast_strs[fast];
	sprintf(s,MUIX_C MUIX_B "Audio Disk Uart Control\n" MUIX_N MUIX_R
		"PreCompensation = %d = %s\n"
		"PreCompensation Type = %d = %s\n"
		"Uart Break = %d\n"
		"Word Sync = %d\n"
		"MSB (GCR) Sync = %d\n"
		"Fast Disk Clock = %d = %s\n"
		"Audio 3 Modulate Nothing = %d\n"
		"Audio 2 Modulate Period 3 = %d\n"
		"Audio 1 Modulate Period 2 = %d\n"
		"Audio 0 Modulate Period 1 = %d\n"
		"Audio 3 Modulate Nothing = %d\n"
		"Audio 2 Modulate Volume 3 = %d\n"
		"Audio 1 Modulate Volume 2 = %d\n"
		"Audio 0 Modulate Volume 1 = %d\n",
		precmp,precmp_str,
		mfmprec,mfmprec_str,
		btst(v,11),
		btst(v,10),
		btst(v,9),
		fast,fast_str,
		btst(v,7),btst(v,6),btst(v,5),btst(v,4),btst(v,3),btst(v,2),btst(v,1),btst(v,0)
	);
#ifdef __CHKSTRLEN__
	printf("adkcon_h v=$%x len=%ld<%ld\n",v,strlen(s),sizeof(s));
#endif
	return s;
}
char * bltcon0_h (UWORD v) {
	static char s[182];
	sprintf(s,MUIX_C MUIX_B "Blitter Control 0\n" MUIX_N MUIX_R
		"Shift Source A = %d\n"
		"DMA Channel A = %d\nDMA Channel B = %d\nDMA Channel C = %d\nDMA Channel D = %d\n"
		"ABC = %d aBC = %d\nABc = %d aBc = %d\nAbC = %d abC = %d\nAbc = %d abc = %d",
		v>>12,
		btst(v,11),btst(v,10),btst(v,9),btst(v,8),
		btst(v,7),btst(v,3),btst(v,6),btst(v,2),btst(v,5),btst(v,1),btst(v,4),btst(v,0)
	);
#ifdef __CHKSTRLEN__
	printf("bltcon0_h v=$%x len=%ld<%ld\n",v,strlen(s),sizeof(s));
#endif
	return s;
}
char * bltcon1_h (UWORD v) {
	static char s[161];
	sprintf(s,MUIX_C MUIX_B "Blitter Control 1\n" MUIX_N MUIX_R
		"Shift Source B = %d\n"
		"Disable D-Output (hires) = %d\n"
		"Exclusive Fill = %d\n"
		"Inclusive Fill = %d\n"
		"Fill Carry In = %d\n"
		"Descending = %d\n"
		"Line Mode = %d\n",
		v>>12,
		btst(v,7),
		btst(v,4),
		btst(v,3),
		btst(v,2),
		btst(v,1),
		btst(v,0)
	);
#ifdef __CHKSTRLEN__
	printf("bltcon1_h v=$%x len=%ld<%ld\n",v,strlen(s),sizeof(s));
#endif
	return s;
}
char * bltsize_h (UWORD v) {
	static char s[124];
	UWORD h=v>>5, w=v&63;
	if (!h) h = 1024;
	if (!w) w = 64;
	sprintf(s,MUIX_C MUIX_B "Blitter Size & Start\n" MUIX_N MUIX_R
		"Height (lines) = %d\n"
		"Width (words) = %d\n"
		"Width (pixel) = %d\n"
		"Window (bytes) = %d = $%x",
		h,w,w*16,h*w*2,h*w*2
	);
#ifdef __CHKSTRLEN__
	printf("bltsize_h v=$%x len=%ld<%ld\n",v,strlen(s),sizeof(s));
#endif
	return s;
}
char * vhpos_h (ULONG v) {
	static char s[200], *chipname;
	ULONG chipid = (v>>24)&127;
	switch (chipid) {
		case 0x000: chipname = "8367(pal) or 8371(fat-pal)"; break;
		case 0x010: chipname = "8361(regular) or 8370(fat-ntsc)"; break;
		case 0x020: chipname = "8372(fat-hr-pal) rev.4"; break;
		case 0x021: chipname = "8372(fat-hr-pal) rev.5"; break;
		case 0x022: chipname = "8374(alice-pal)"; break;
		case 0x030: chipname = "8372(fat-hr-ntsc) rev.4"; break;
		case 0x031: chipname = "8372(fat-hr-ntsc) rev.5"; break;
		case 0x032: chipname = "8374(alice-ntsc)"; break;
		default:    chipname = "unknown";
	}
	sprintf(s,MUIX_C MUIX_B "Raster Position & Agnus ID\n" MUIX_N MUIX_R
		"Long Frame = %ld\n"
		"Agnus ID = $%lx\n"
		"Agnus Name = %s\n"
		"Long Raster Line = %ld\n"
		"Vertical Pos = %ld\n"
		"Horizontal Pos = %ld\n",
		btst(v,31),
		chipid, chipname,
		btst(v,23),
		(v>>8)&2047,
		v&255
	);
#ifdef __CHKSTRLEN__
	printf("vhpos_h v=$%lx len=%ld<%ld\n",v,strlen(s),sizeof(s));
#endif
	return s;
}
char * bplcon0_h (UWORD v) {
	static char s[231];
	sprintf(s,MUIX_C MUIX_B "Bitplane Control 0\n" MUIX_N MUIX_R
		"HiRes = %d\n"
		"Bitplane Use = %d\n"
		"HAM = %d\n"
		"Dual Playfield = %d\n"
		"Color = %d\n"
		"Genlock Audio Enable = %d\n"
		"Ultra HiRes = %d\n"
		"Super HiRes = %d\n"
		"Bypass = %d\n"
		"Light Pen Enable = %d\n"
		"Interlace = %d\n"
		"External Resync = %d\n"
		"ECS Enable = %d\n",
		btst(v,15),
		((v>>12)&7)+((v>>1)&8),
		btst(v,11),
		btst(v,10),
		btst(v,9),
		btst(v,8),
		btst(v,7),
		btst(v,6),
		btst(v,5),
		btst(v,3),
		btst(v,2),
		btst(v,1),
		btst(v,0)
	);
#ifdef __CHKSTRLEN__
	printf("bplcon0_h v=$%x len=%ld<%ld\n",v,strlen(s),sizeof(s));
#endif
	return s;
}
char * bplcon1_h (UWORD v) {
	static char s[106];
	sprintf(s,MUIX_C MUIX_B "Bitplane Control 1\n" MUIX_N MUIX_R
		"Playfield 1 Scroll:\n"
		"OCS = %d\n"
		"AGA = %d\n"
		"Playfield 2 Scroll:\n"
		"OCS = %d\n"
		"AGA = %d\n",
		v&15,
		((v>>4)&192) + ((v<<2)&60) + ((v>>8)&3),
		(v>>4)&15,
		((v>>8)&192) + ((v>>2)&60) + ((v>>12)&3)
	);
#ifdef __CHKSTRLEN__
	printf("bplcon1_h v=$%x len=%ld<%ld\n",v,strlen(s),sizeof(s));
#endif
	return s;
}
char * bplcon2_h (UWORD v) {
	static char s[211];
	sprintf(s,MUIX_C MUIX_B "Bitplane Control 2\n" MUIX_N MUIX_R
		"ZD Bitplane = %d\n"
		"ZD Bitplane Enable = %d\n"
		"ZD Color Enable = %d\n"
		"Kill EHB = %d\n"
		"Read Color Table = %d\n"
		"SOG Enable = %d\n"
		"Playfield 2 before 1 = %d\n"
		"Playfield 2 Priority = %d\n"
		"Playfield 1 Priority = %d\n",
		((v>>12)&7)+1,
		btst(v,11),
		btst(v,10),
		btst(v,9),
		btst(v,8),
		btst(v,7),
		btst(v,6),
		(v>>3)&7,
		v&7
	);
#ifdef __CHKSTRLEN__
	printf("bplcon2_h v=$%x len=%ld<%ld\n",v,strlen(s),sizeof(s));
#endif
	return s;
}
char * bplcon3_h (UWORD v) {
	static char s[226];
	UWORD spres = (v>>6)&3;
	const char *spres_t[] = {"default","LORES 140 ns","HIRES 70 ns","SHRES 35 ns"};
	sprintf(s,MUIX_C MUIX_B "Bitplane Control 3\n" MUIX_N MUIX_R
		"Color Bank = %d\n"
		"Playfield 2 Color Offset = %d\n"
		"Low Color Table = %d\n"
		"Sprite Resolution = %d = %s\n"
		"Border Blank = %d\n"
		"Border Transparent = %d\n"
		"ZD Clock Enable = %d\n"
		"Border Sprites = %d\n"
		"Extra Blank = %d\n",
		(v>>13)&7,
		(1<<((v>>10)&7))&254,
		btst(v,9),
		spres, spres_t[spres],
		btst(v,5),
		btst(v,4),
		btst(v,2),
		btst(v,1),
		btst(v,0)
	);
#ifdef __CHKSTRLEN__
	printf("bplcon3_h v=$%x len=%ld<%ld\n",v,strlen(s),sizeof(s));
#endif
	return s;
}
char * bplcon4_h (UWORD v) {
	static char s[256];
	sprintf(s,MUIX_C MUIX_B "Bitplane Control 4\n" MUIX_N MUIX_R
		"Color XOR = $%x\n"
		"Even Sprite Color Offset = %d\n"
		"Odd Sprite Color Offset = %d\n",
		v>>8,
		v&0x0f0,
		(v<<4)&0x0f0
	);
#ifdef __CHKSTRLEN__
	printf("bplcon4_h v=$%x len=%ld<%ld\n",v,strlen(s),sizeof(s));
#endif
	return s;
}

void make_win_cust(void) {
	ULONG open;
	APTR cop1lc_b,cop2lc_b;

	if (win_cust) {
		get(win_cust,MUIA_Window_Open,&open);
		if (open)	set(win_cust,MUIA_Window_Open,FALSE);
		else		set(win_cust,MUIA_Window_Open,TRUE);
	} else {
		win_cust = WindowObject,
			MUIA_Window_Title, "More Custom",
			MUIA_Window_ID   , MAKE_ID('C','U','S','T'),
			WindowContents, HGroup,
				Child, VGroup,
				Child, HGroup, GroupFrameT("In/Output"), MUIA_Group_Columns, 2,
					RegCustomWord("joy0dat:",0x00a),
					RegCustomWord("joy1dat:",0x00c),
					RegCustomWord("pot0dat:",0x012),
					RegCustomWord("pot1dat:",0x014),
					RegCustomWord("potgo:",0x034),
					RegCustomWord("joytest:",0x036),
   					End,
				Child, HGroup, GroupFrameT("Disk"), MUIA_Group_Columns, 2,
					RegCustomWordReadWrite("adkcon:",0x010,0x09e,adkcon_h),
					RegCustomWord("dskbyt:",0x01a),
					RegCustomLong("dskpt:",0x022),
					RegCustomWord("dsklen:",0x024),
					RegCustomWord("dsksync:",0x07e),
   					End,
				Child, HGroup, GroupFrameT("Blitter"), MUIA_Group_Columns, 2,
					RegCustomWordHelp("bltcon0:",0x040,bltcon0_h),
					RegCustomWordHelp("bltcon1:",0x042,bltcon1_h),
					RegCustomWordHelpS("bltafwm:",0x044,MUIX_C MUIX_B "Blitter First Word Mask A"),
					RegCustomWordHelpS("bltalwm:",0x046,MUIX_C MUIX_B "Blitter Last Word Mask A"),
					RegCustomLongHelpS("bltapt:",0x050,MUIX_C MUIX_B "Blitter Pointer A"),
					RegCustomLongHelpS("bltbpt:",0x04c,MUIX_C MUIX_B "Blitter Pointer B"),
					RegCustomLongHelpS("bltcpt:",0x048,MUIX_C MUIX_B "Blitter Pointer C"),
					RegCustomLongHelpS("bltdpt:",0x054,MUIX_C MUIX_B "Blitter Pointer D"),
					RegCustomWordHelpS("bltamod:",0x064,MUIX_C MUIX_B "Blitter Modulo A"),
					RegCustomWordHelpS("bltbmod:",0x062,MUIX_C MUIX_B "Blitter Modulo B"),
					RegCustomWordHelpS("bltcmod:",0x060,MUIX_C MUIX_B "Blitter Modulo C"),
					RegCustomWordHelpS("bltdmod:",0x066,MUIX_C MUIX_B "Blitter Modulo D"),
					RegCustomWordHelpS("bltadat:",0x074,MUIX_C MUIX_B "Blitter Data A"),
					RegCustomWordHelpS("bltbdat:",0x072,MUIX_C MUIX_B "Blitter Data B"),
					RegCustomWordHelpS("bltcdat:",0x070,MUIX_C MUIX_B "Blitter Data C"),
					RegCustomWordHelp("bltsize:",0x058,bltsize_h),
					RegCustomWordHelpS("bltsizv:",0x05c,MUIX_C MUIX_B "Blitter Size Width"),
					RegCustomWordHelpS("bltsizh:",0x05e,MUIX_C MUIX_B "Blitter Size Height & Start"),
					End,
				End,
				Child, VGroup,
				Child, HGroup, GroupFrameT("Copper"), MUIA_Group_Columns, 2,
					RegCustomWord("copcon:",0x02e),
					RegCust32BE(cop1lc_b,"cop1lc:",0x080),
					RegCust32BE(cop2lc_b,"cop2lc:",0x084),
   					End,
				Child, HGroup, GroupFrameT("Raster"), MUIA_Group_Columns, 2,
					RegCustomLongHelp("vhpos:",0x004,vhpos_h),
					RegCustomWord("diwstrt:",0x08e),
					RegCustomWord("diwstop:",0x090),
					RegCustomWord("ddfstrt:",0x092),
					RegCustomWord("ddfstop:",0x094),
					RegCustomWordHelp("bplcon0:",0x0100,bplcon0_h),
					RegCustomWordHelp("bplcon1:",0x0102,bplcon1_h),
					RegCustomWordHelp("bplcon2:",0x0104,bplcon2_h),
					RegCustomWordHelp("bplcon3:",0x0106,bplcon3_h),
					RegCustomWordHelp("bplcon4:",0x010c,bplcon4_h),
					RegCustomWord("clxcon:",0x098),
					RegCustomWord("clxcon2:",0x010e),
					RegCustomWord("clxdat:",0x0e),
					RegCustomWord("bpl1mod:",0x0108),
					RegCustomWord("bpl2mod:",0x010a),
					RegCustomLong("bpl1pt:",0x0e0),
					RegCustomLong("bpl2pt:",0x0e4),
					RegCustomLong("bpl3pt:",0x0e8),
					RegCustomLong("bpl4pt:",0x0ec),
					RegCustomLong("bpl5pt:",0x0f0),
					RegCustomLong("bpl6pt:",0x0f4),
					RegCustomLong("bpl7pt:",0x0f8),
					RegCustomLong("bpl8pt:",0x0fc),
					End,
				Child, HGroup, GroupFrameT("Serial"), MUIA_Group_Columns, 2,
					RegCustomWord("serdat:",0x018),
					RegCustomWord("serper:",0x032),
   					End,
   				End,
				Child, HGroup, GroupFrameT("Color"), MUIA_Group_Columns, 2,
					RegCustomWord("color00:",0x0180),
					RegCustomWord("color01:",0x0182),
					RegCustomWord("color02:",0x0184),
					RegCustomWord("color03:",0x0186),
					RegCustomWord("color04:",0x0188),
					RegCustomWord("color05:",0x018a),
					RegCustomWord("color06:",0x018c),
					RegCustomWord("color07:",0x018e),
					RegCustomWord("color08:",0x0190),
					RegCustomWord("color09:",0x0192),
					RegCustomWord("color10:",0x0194),
					RegCustomWord("color11:",0x0196),
					RegCustomWord("color12:",0x0198),
					RegCustomWord("color13:",0x019a),
					RegCustomWord("color14:",0x019c),
					RegCustomWord("color15:",0x019e),
					RegCustomWord("color16:",0x01a0),
					RegCustomWord("color17:",0x01a2),
					RegCustomWord("color18:",0x01a4),
					RegCustomWord("color19:",0x01a6),
					RegCustomWord("color20:",0x01a8),
					RegCustomWord("color21:",0x01aa),
					RegCustomWord("color22:",0x01ac),
					RegCustomWord("color23:",0x01ae),
					RegCustomWord("color24:",0x01b0),
					RegCustomWord("color25:",0x01b2),
					RegCustomWord("color26:",0x01b4),
					RegCustomWord("color27:",0x01b6),
					RegCustomWord("color28:",0x01b8),
					RegCustomWord("color29:",0x01ba),
					RegCustomWord("color30:",0x01bc),
					RegCustomWord("color31:",0x01be),
					End,
				Child, VGroup,
				Child, HGroup, GroupFrameT("Sprite"), MUIA_Group_Columns, 2,
					RegCustomLong("spr0pt:",0x0120),
					RegCustomLong("spr1pt:",0x0124),
					RegCustomLong("spr2pt:",0x0128),
					RegCustomLong("spr3pt:",0x012c),
					RegCustomLong("spr4pt:",0x0130),
					RegCustomLong("spr5pt:",0x0134),
					RegCustomLong("spr6pt:",0x0138),
					RegCustomLong("spr7pt:",0x013c),
					End,
				Child, HGroup, GroupFrameT("Audio"), MUIA_Group_Columns, 2,
					RegCustomLong("aud0lc:",0x0a0),
					RegCustomWord("aud0len:",0x0a4),
					RegCustomWord("aud0per:",0x0a6),
					RegCustomWord("aud0vol:",0x0a8),
					RegCustomWord("aud0dat:",0x0aa),
					RegCustomLong("aud1lc:",0x0b0),
					RegCustomWord("aud1len:",0x0b4),
					RegCustomWord("aud1per:",0x0b6),
					RegCustomWord("aud1vol:",0x0b8),
					RegCustomWord("aud1dat:",0x0ba),
					RegCustomLong("aud2lc:",0x0c0),
					RegCustomWord("aud2len:",0x0c4),
					RegCustomWord("aud2per:",0x0c6),
					RegCustomWord("aud2vol:",0x0c8),
					RegCustomWord("aud2dat:",0x0ca),
					RegCustomLong("aud3lc:",0x0d0),
					RegCustomWord("aud3len:",0x0d4),
					RegCustomWord("aud3per:",0x0d6),
					RegCustomWord("aud3vol:",0x0d8),
					RegCustomWord("aud3dat:",0x0da),
					End,
				End,
				Child, HGroup, GroupFrameT("ECS/AGA"), MUIA_Group_Columns, 2,
					RegCustomWord("lisaid:",0x07c),
					RegCustomWord("htotal:",0x01c0),
					RegCustomWord("hsstop:",0x01c2),
					RegCustomWord("hbstrt:",0x01c4),
					RegCustomWord("hbstop:",0x01c6),
					RegCustomWord("vtotal:",0x01c8),
					RegCustomWord("vsstop:",0x01ca),
					RegCustomWord("vbstrt:",0x01cc),
					RegCustomWord("vbstop:",0x01ce),
					RegCustomWord("sprhstrt:",0x01d0),
					RegCustomWord("sprhstop:",0x01d2),
					RegCustomWord("bplhstrt:",0x01d4),
					RegCustomWord("bplhstop:",0x01d6),
					RegCustomWord("hhpos:",0x01da),
					RegCustomWord("beamcon0:",0x01dc),
					RegCustomWord("hsstrt:",0x01de),
					RegCustomWord("vsstrt:",0x01e0),
					RegCustomWord("hcenter:",0x01e2),
					RegCustomWord("diwhigh:",0x01e4),
					RegCustomWord("bplhmod:",0x01e6),
					RegCustomWord("sprhpt:",0x01e8),
					RegCustomWord("bplhpt:",0x01ec),
					RegCustomWord("fmode:",0x01fc),
					End,
				End,
			End;
		if (win_cust) {
			DoMethod(app,OM_ADDMEMBER,win_cust);
			DoMethod(win_cust,MUIM_Notify,MUIA_Window_CloseRequest,TRUE,app,2,MUIM_Application_ReturnID,MAIN_MORECUST);
			DoMethod(cop1lc_b,MUIM_Notify,MUIA_Pressed,FALSE,app,2,MUIM_Application_ReturnID,MAIN_MORECOP1LC);
			DoMethod(cop2lc_b,MUIM_Notify,MUIA_Pressed,FALSE,app,2,MUIM_Application_ReturnID,MAIN_MORECOP2LC);
			set(win_cust,MUIA_Window_Open,TRUE);
		} else {
			MUI_Request(app,win,0,NULL,"Ok","Couldn't open Custom window.");
		}
	}
}

/****************************************************************************/

