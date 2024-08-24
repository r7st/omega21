# PREFIX is the base install location. Defaults to cwd.
PREFIX ?= .

# binary install dir
BINDIR = ${PREFIX}/bin

# lib dir for maps, help, scores, etc.
# the value of LIBDIR should be the same as OMEGALIB in defs.h
LIBDIR = ${PREFIX}/share/omega

# where to keep save files
# the value of SAVEDIR should be the same as SAVEDIR in defs.h
SAVEDIR = ${LIBDIR}/saves

# Name of the generated binary
BINFILE = omega21

# Note - DO NOT disable optimization with gcc. This is a known bug causing a segfault
CFLAGS += \
  -O2 \
  -pipe \
  -Wno-return-type \
  -fstack-protector-strong \
  -fno-strict-aliasing  \
  -DBSD \
  -DOMEGALIB=\"${LIBDIR}/\"  \
  -DSAVEDIR=\"${SAVEDIR}/\" \
  -Wno-dangling-else \
  -Wno-incompatible-pointer-types \
  -Wno-knr-promoted-parameter \
  -Wno-parentheses \
  -Wno-shift-count-overflow

CC = gcc
CPP = ${CC} -E

# Included depends, may differ between Linux/MacOS/BSD
#LIBS = -lcurses -ltermcap
#LIBS = -lcurses -ltermlib
LIBS = -lncurses

#################### that's it for changing the Makefile ####################

OBJ = omega.o abyss.o aux1.o aux2.o aux3.o char.o city.o\
      clrgen.o command1.o command2.o command3.o\
      country.o effect1.o effect2.o effect3.o\
      etc.o env.o file.o gen1.o gen2.o guild1.o guild2.o house.o\
      init.o inv.o item.o itemf1.o itemf2.o itemf3.o lev.o\
      mmelee.o mmove.o mon.o move.o movef.o mspec.o\
      mstrike.o mtalk.o priest.o\
      save.o scr.o site1.o site2.o\
      spell.o time.o trap.o util.o village.o compress.o\
      opts.o class.o

$(BINFILE): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) $(LIBS) -o $(BINFILE)

define install_common
	mkdir -p $(BINDIR)
	mkdir -p $(LIBDIR)
	mkdir -p $(SAVEDIR)
	cp $(BINFILE) $(BINDIR)
	- cp lib/* $(LIBDIR)
	chmod 0644 $(LIBDIR)/help*.txt $(LIBDIR)/license.txt $(LIBDIR)/motd.txt $(LIBDIR)/thanks.txt $(LIBDIR)/update.txt
	chmod 0644 $(LIBDIR)/abyss.txt $(LIBDIR)/scroll[1234].txt $(LIBDIR)/*.dat
	chmod 0666 $(LIBDIR)/omega.hi $(LIBDIR)/omega.log
endef

install: $(BINFILE)
	$(install_common)
	chmod 0711 $(BINDIR)/$(BINFILE)

install_suid: $(BINFILE) $(BINDIR) $(LIBDIR)
	$(install_common)
	chmod 4711 $(BINDIR)/$(BINFILE)

clean:
	rm -f $(OBJ) genclr genclr.o
	rm $(BINFILE)

$(OBJ): clrgen.h defs.h extern.h glob.h

clrgen.c: genclr
	$(CPP) $(CFLAGS) -DOMEGA_CLRGEN *.[ch] | ./genclr clrgen.c clrgen.h
clrgen.h: | clrgen.c

genclr: genclr.o
	$(CC) $(CFLAGS) $(LDFLAGS) genclr.o -o genclr
