/*****************************************************************************
;  :Module.	whddump.h
;  :Author.	Bert Jahn
;  :EMail.	wepl@whdload.de
;  :Version.	$Id: whddump.h 0.6 2008/02/25 21:08:23 wepl Exp wepl $
;  :History.	18.07.98 started
;		28.03.00 TERM chunk added
;		06.06.04 wdh_ShadowMem changed from APTR to ULONG
;		24.02.08 slave structure added
;  :Copyright.	All Rights Reserved
;  :Language.	C
;  :Tabsize.	4
;  :Translator.	GCC
*****************************************************************************/

#ifndef DOS_DATETIME_H
#include <dos/datetime.h>
#endif

#define ID_WHDD MAKE_ID('W','H','D','D')

#define ID_HEAD MAKE_ID('H','E','A','D')

struct whddump_header
{
	ULONG	wdh_BaseMemSize;
	ULONG	wdh_ShadowMem;
	ULONG	wdh_TermReason;
	ULONG	wdh_TermPrimary;
	ULONG	wdh_TermSecondary;
#define TERMSTRINGLEN 256
	char	wdh_TermString[TERMSTRINGLEN];
	ULONG	wdh_LastBlitPC;
	ULONG	wdh_ExpMemLog;
	ULONG	wdh_ExpMemPhy;
	ULONG	wdh_ExpMemLen;
	ULONG	wdh_ResLoadLog;
	ULONG	wdh_ResLoadPhy;
	ULONG	wdh_ResLoadLen;
	ULONG	wdh_SlaveLog;
	ULONG	wdh_SlavePhy;
	ULONG	wdh_SlaveLen;
#define SLAVENAMELEN 256
	char	wdh_SlaveName[SLAVENAMELEN];
	struct	DateStamp wdh_DateStamp;
	ULONG	wdh_kn;
	ULONG	wdh_rw;
	UWORD	wdh_cs;
	UWORD	wdh_CPU;
	WORD	wdh_WVer;
	WORD	wdh_WRev;
	WORD	wdh_WBuild;
	BYTE	wdh_fc;
	BYTE	wdh_zpt;
};

#define ID_TERM	MAKE_ID('T','E','R','M')	/* starting whdload v11 */

#define ID_CPU	MAKE_ID('C','P','U',' ')

struct whddump_cpu
{
	ULONG	wdc_regs[15];
	ULONG	wdc_pc;
	ULONG	wdc_usp;
	ULONG	wdc_ssp;		/* isp on 20-40 */
	ULONG	wdc_msp;		/* 20-40 */
	UWORD	wdc_sr;
	UBYTE	wdc_sfc;		/* 10-60 */
	UBYTE	wdc_dfc;		/* 10-60 */
	ULONG	wdc_vbr;		/* 10-60 */
	ULONG	wdc_caar;		/* 20-30 */
	ULONG	wdc_cacr;		/* 20-60 */
	ULONG	wdc_tt0;		/* 30 */
	ULONG	wdc_tt1;		/* 30 */
	ULONG	wdc_dtt0;		/* 40-60 */
	ULONG	wdc_dtt1;		/* 40-60 */
	ULONG	wdc_itt0;		/* 40-60 */
	ULONG	wdc_itt1;		/* 40-60 */
	ULONG	wdc_pcr;		/* 60 */
	ULONG	wdc_buscr;		/* 60 */
	ULONG	wdc_srp[2];		/* 30(64bit) 40-60(32bit) */
	ULONG	wdc_crp[2];		/* 30 */
	ULONG	wdc_drp[2];		/* 51 */
	ULONG	wdc_tc;			/* 30(32bit) 40-60(16bit) */
	ULONG	wdc_mmusr;		/* 30(16bit) 40(32bit) */
	ULONG	wdc_urp;		/* 40-60 */
	ULONG	wdc_fpregs[8*3];
	ULONG	wdc_fpcr;
	ULONG	wdc_fpsr;
	ULONG	wdc_fpiar;
};

#define ID_CUST MAKE_ID('C','U','S','T')

struct wdcu_flag
{
	unsigned int modi:1;
	unsigned int :5;
	unsigned int write:1;
	unsigned int read:1;
};

struct whddump_custom
{
	UWORD			wdcu_regs[256];
	struct wdcu_flag	wdcu_flags[512];
};

#define ID_CIAA MAKE_ID('C','I','A','A')
#define ID_CIAB MAKE_ID('C','I','A','B')

struct whddump_cia
{
	UBYTE	wdci_prai;		/* Port Register A Input */
	UBYTE	wdci_prbi;		/* Port Register B Input */
	UBYTE	wdci_prao;		/* Port Register A Output */
	UBYTE	wdci_prbo;		/* Port Register B Output */
	UBYTE	wdci_ddra;		/* Data Direction Register A */
	UBYTE	wdci_ddrb;		/* Data Direction Register B */
	UWORD	wdci_ta;		/* actual Timer A */
	UWORD	wdci_tb;		/* actual Timer B */
	UWORD	wdci_pa;		/* Latch Timer A */
	UWORD	wdci_pb;		/* Latch Timer B */
	ULONG	wdci_event;		/* event counter */
	ULONG	wdci_alarm;		/* alarm of event counter */
	UBYTE	wdci_sdr;		/* seriell port register */
	UBYTE	wdci_icr;		/* Interrupt Control Request */
	UBYTE	wdci_icm;		/* Interrupt Control Mask */
	UBYTE	wdci_cra;		/* Control Register A */
	UBYTE	wdci_crb;		/* Control Register B */
	UBYTE	wdci_pad[3];
};

#define ID_SLAV MAKE_ID('S','L','A','V')

struct whddump_slave
{
	char	ws_Security[4];		/* moveq #-1,d0 rts */
	char	ws_ID[8];		/* "WHDLOADS" */
	UWORD	ws_Version;		/* required WHDLoad version */
	UWORD	ws_Flags;		/* see below */
	ULONG	ws_BaseMemSize;		/* size of required memory (multiple of $1000) */
	ULONG	ws_ExecInstall;		/* must be 0 */
	RPTR	ws_GameLoader;		/* Slave code, called by WHDLoad */
	RPTR	ws_CurrentDir;		/* subdirectory for data files */
	RPTR	ws_DontCache;		/* pattern for files not to cache */
	/* version 4 */
	UBYTE	ws_keydebug;		/* raw key code to quit with debug, =0 means no key */
	UBYTE	ws_keyexit;		/* raw key code to exit, =0 means no key */
	/* version 8 */
	ULONG	ws_ExpMem;		/* size of required expansions memory */
	/* version 10 */
	RPTR	ws_name;		/* name of the installed program */
	RPTR	ws_copy;		/* year and owner of the copyright */
	RPTR	ws_info;		/* additional informations (author, version...) */
	/* version 16 */
	RPTR	ws_kickname;		/* name of kickstart image */
	ULONG	ws_kicksize;		/* size of kickstart image */
	UWORD	ws_kickcrc;		/* crc16 of kickstart image */
};

#define ID_MEM	MAKE_ID('M','E','M',' ')

#define ID_EMEM	MAKE_ID('E','M','E','M')

