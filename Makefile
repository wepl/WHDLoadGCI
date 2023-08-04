
SRCS = class.c dumpfile.c misc.c WHDLoadGCI.c wincia.c wincpu.c wincust.c wininfo.c winmem.c
OBJS = $(SRCS:.c=.o)

CC = gcc
CFLAGS = -mc68020 -Wall -IMUI:Developer/C/Include -noixemul -O2
LFLAGS = -noixemul -s

/C/WHDLoadGCI: $(OBJS)
	$(CC) $(LFLAGS) -o C:WHDLoadGCI $^

clean:
	rm $(OBJS) *.bak

depend:
	makedepend $(OBJS)

#DEPENDENCIES

class.o: class.c WHDLoadGCI.h class.h whddump.h

dumpfile.o: dumpfile.c whddump.h WHDLoadGCI.h

misc.o: misc.c

WHDLoadGCI.o: WHDLoadGCI.c class.h whddump.h WHDLoadGCI.h

wincia.o: wincia.c class.h whddump.h WHDLoadGCI.h

wincpu.o: wincpu.c class.h whddump.h WHDLoadGCI.h

wincust.o: wincust.c class.h whddump.h WHDLoadGCI.h

wininfo.o: wininfo.c WHDLoadGCI.h whddump.h

winmem.o: winmem.c whddump.h WHDLoadGCI.h class.h

