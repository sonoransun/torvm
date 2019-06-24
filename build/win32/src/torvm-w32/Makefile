# Makefile for Tor VM win32 process and network control utility

.PHONY: all clean distclean install 

BINDIR=C:\Tor_VM
CPPFLAGS += -I. -I/usr/include -I/usr/local/include -MMD -MP
CPPFLAGS += -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE
LIBS += -lws2_32 -liphlpapi

all: torvm$(EXESUF)

OBJS+=torvm.o torvmrc.o apicommon.o creds.o thr.o

torvm$(EXESUF): $(OBJS) vmconfig.h
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

torvm.o: torvm.c vmconfig.h
	$(CC) $(CPPFLAGS) -c -o $@ $<

torvmrc.o: torvm.rc
	windres torvm.rc torvmrc.o

apicommon.o: apicommon.c
	$(CC) $(CPPFLAGS) -c -o $@ $<

creds.o: creds.c
	$(CC) $(CPPFLAGS) -c -o $@ $<

thr.o: thr.c
	$(CC) $(CPPFLAGS) -c -o $@ $<

vmconfig.h: vmconfig.h.in
	@if [ ! -f $@ ]; then cp $< $@; else touch $@; fi

clean:
	rm -f *.o *.d torvm$(EXESUF)


install: all
	ls -l torvm$(EXESUF)
	cp -f torvm$(EXESUF) $(BINDIR)
