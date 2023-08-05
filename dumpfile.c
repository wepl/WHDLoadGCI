/*****************************************************************************
;  :Module.	dumpfile.c
;  :Author.	Bert Jahn
;  :EMail.	wepl@whdload.de
;  :History.	18.07.98 started
;		13.12.98 dumpfilename from whdload.prefs
;		02.03.00 expmem stuff added
;			 freedump() now resets all pointer
;		24.02.08 extracting functions added
;			 minor changes on loaddump
;		04.09.08 dumpfile name handling fixed when no s:whdload.prefs present
;			 filerequester if dumpfile loading fails
;			 partial saving BaseMem/ExpMem added
;		17.07.23 reading cols and options added
;  :Copyright.	All Rights Reserved
;  :Language.	C
;  :Translator.	GCC
*****************************************************************************/

#include <string.h>

#include <dos/doshunks.h>
#include <dos/exall.h>
#include <exec/memory.h>
#include <libraries/asl.h>
#include <libraries/iffparse.h>
#include <libraries/mui.h>

#include <clib/dos_protos.h>
#include <clib/exec_protos.h>
#include <clib/muimaster_protos.h>

#include "whddump.h"
#include "WHDLoadGCI.h"

/****************************************************************************/

APTR	* dumpfile = NULL;
char	defpath[256] = "";

/****************************************************************************/

int getfilename(char *buffer, int buflen, const char *file, const char *title) {
	struct FileRequester *req;
	int rc = FALSE;

	if ((req = MUI_AllocAslRequestTags(ASL_FileRequest,TAG_END))) {
		if (MUI_AslRequestTags(req,
			ASLFR_TitleText,title,
			ASLFR_RejectIcons,TRUE,
			ASLFR_InitialFile,file,
			ASLFR_InitialDrawer,defpath,
			ASLFR_DoSaveMode,TRUE,
			TAG_END)
		) {
			strncpyz(buffer,req->fr_Drawer,buflen-1);
			AddPart(buffer,req->fr_File,buflen);
			strncpyz(defpath,req->fr_Drawer,sizeof(defpath)-1);
			rc = TRUE;
		}
		MUI_FreeAslRequest(req);
	}
	return rc;
}

void freedump(void) {
	if (dumpfile) {
		FreeVec(dumpfile);
		dumpfile = NULL;
	}
	header = NULL;
	term = NULL;
	opt = NULL;
	cpu = NULL;
	custom = NULL;
	ciaa = NULL;
	ciab = NULL;
	slave = NULL;
	mem = NULL;
	emem = NULL;
	cols = NULL;
}

BOOL loaddump(STRPTR name) {
	BOOL ret = FALSE;
	BPTR fh;
	ULONG size;
	ULONG *tmp;
	char filename[256] = "";
	char s[256];
	char *t;
	ULONG chk_id,chk_len;
	int retry = 0;
	char *reqgad = "Open other Dump|Quit";

	/*
	 * free any loaded dump
	 */
	freedump();

	/*
	 * if there is no filename for the dump overgiven try to load
	 * whdload config and get the path from there
	 */
	if (name) {
		strncpyz(filename,name,sizeof(filename)-1);
	} else {
		fh = Open("S:whdload.prefs",MODE_OLDFILE);
		if (fh) {
			while (FGets(fh,s,sizeof(s))) {
				if (strnicmp("coredumppath=",s,13) == 0) {
					t = strpbrk(&s[13]," \t\n\r");
					if (t) strncpyz(filename,&s[13],t-s-13);
					else strcpy(filename,&s[13]);
					break;
				}
			}
			Close(fh);
		}
		AddPart(filename,".whdl_dump",sizeof(filename));
	}

	/*
	 * remember path for later save operations
	 */
	t = PathPart(filename);
	strncpyz(defpath,filename,t-filename);
		
	/*
	 * load dump
	 */
	do {
		if (retry) {
			getfilename(filename,sizeof(filename),NULL,"Select dumpfile");
			retry = 0;
		}
	if (! (fh = Open(filename,MODE_OLDFILE))) {
		retry = MUI_Request(app,win,0,NULL,reqgad,"Could not open dumpfile \"%s\".",filename);
	} else {
		Seek(fh,0,OFFSET_END);
		size = Seek(fh,0,OFFSET_BEGINNING);
		if ((dumpfile = AllocVec(size,0))) {
			if (size == Read(fh,dumpfile,size)) {
				tmp = (ULONG*)dumpfile;
				if (*tmp++ == ID_FORM && *tmp++ == size-8 && *tmp++ == ID_WHDD) {
					size -= 12;
					do {
						chk_id = *tmp++;
						chk_len = *tmp++;
						// printf("off=$%5lx id=%.4s len=%ld\n",4*(((APTR*)tmp)-dumpfile),&chk_id,chk_len);
						switch (chk_id) {
						case ID_HEAD:
							header = (struct whddump_header*)tmp;
							break;
						case ID_TERM:
							term = (char*)tmp;
							break;
						case ID_OPT:
							opt = (char*)tmp;
							break;
						case ID_CPU:
							cpu = (struct whddump_cpu*)tmp;
							break;
						case ID_CUST:
							custom = (struct whddump_custom*)tmp;
							break;
						case ID_CIAA:
							ciaa = (struct whddump_cia*)tmp;
							break;
						case ID_CIAB:
							ciab = (struct whddump_cia*)tmp;
							break;
						case ID_SLAV:
							slave = (APTR)tmp;
							break;
						case ID_MEM:
							mem = (APTR)tmp;
							break;
						case ID_EMEM:
							emem = (APTR)tmp;
							break;
						case ID_COLS:
							cols = (APTR)tmp;
							break;
						}
						size -= 8 + chk_len;
						tmp += chk_len>>2;
						if (!size) ret = TRUE;
					} while (size>8 && !ret);
				}
			}
			if (!ret) {
				FreeVec(dumpfile);
				dumpfile = NULL;
				retry = MUI_Request(app,win,0,NULL,reqgad,"Dumpfile \"%s\" is corrupt.",filename);
			}
		}
		Close(fh);
	}
	} while (retry && !ret);
	return ret;
}

void extractslave(void) {
	char filename[256];
	BPTR fh;
	ULONG filehead[]={HUNK_HEADER,0,1,0,0,0,HUNK_CODE,0};
	ULONG filetail[]={HUNK_END};

	if (slave && getfilename(filename,sizeof(filename),header->wdh_SlaveName,"Save Slave as...")) {
		filehead[5] = filehead[7] = header->wdh_SlaveLen/4;
		if (! (fh = Open(filename,MODE_NEWFILE))) {
			MUI_Request(app,win,0,NULL,"Ok","Could not open file \"%s\" for writing.",filename);
		} else {
			if (
				sizeof(filehead) != Write(fh,filehead,sizeof(filehead)) ||
				header->wdh_SlaveLen != Write(fh,slave,header->wdh_SlaveLen) ||
				sizeof(filetail) != Write(fh,filetail,sizeof(filetail))
			) {
				MUI_Request(app,win,0,NULL,"Ok","Could not write to file \"%s\".",filename);
				Close(fh);
				DeleteFile(filename);
			} else {
				Close(fh);
			}
		}
	}
}

void extractmem(APTR p, ULONG offset, ULONG length, char *filename, char *winname) {
	char name[256];
	BPTR fh;

	/* printf("p=%p po=%p o=%lx l=%lx f=%s w=%s\n",p,p+offset,offset,length,filename,winname); */

	if (p && length && getfilename(name,sizeof(name),filename,winname)) {
		if (! (fh = Open(name,MODE_NEWFILE))) {
			MUI_Request(app,win,0,NULL,"Ok","Couldn't open file \"%s\" for writing.",name);
		} else {
			if (length != Write(fh,p+offset,length)) {
				MUI_Request(app,win,0,NULL,"Ok","Couldn't write to file \"%s\".",name);
				Close(fh);
				DeleteFile(name);
			} else {
				Close(fh);
			}
		}
	}
}

void extractbasemem(ULONG offset, ULONG length) {
	extractmem(mem,offset,length,".whdl_memory","Save BaseMem as...");
}

void extractexpmem(ULONG offset, ULONG length) {
	extractmem(emem,offset,length,".whdl_expmem","Save ExpMem as...");
}

/****************************************************************************/

