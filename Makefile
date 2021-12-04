# PREFIX is the install location. Defaults to cwd.
# PREFIX = /usr/local/bin
PREFIX ?= .

# These two definitions are used if you 'make install'
# the value of LIBDIR should be the same as OMEGALIB in defs.h
BINDIR = ${PREFIX}/bin
LIBDIR = ${PREFIX}/share/omega
# the value of SAVEDIR should be the same as SAVEDIR in defs.h
SAVEDIR = ${LIBDIR}/saves

# Name of the generated binary
BINFILE = omega21

# One of these should be uncommented, as appropriate, unless your compiler
# does it for you.  You can test this by simply trying to 'make' omega -
# it will fail if none of them are defined.  If you do uncomment
# one, make sure you comment out the other definition of CFLAGS lower down

CFLAGS += \
  -O2 \
  -pipe \
  -Wno-return-type \
  -fstack-protector-strong \
  -fno-strict-aliasing  \
  -DBSD \
  -DOMEGALIB=\"${LIBDIR}/\"  \
  -DSAVEDIR=\"${SAVEDIR}/\" \
  -Wl,-rpath=/usr/local/lib

#CFLAGS = -DSYSV -O
# I also had to define -cckr (K&R style C) for system V
#CFLAGS = -DMSDOS -O
#CFLAGS = -DAMIGA -O

# CPP should contain the command to run the C preprocessor.
#CPP = cc -E
#CPP = /lib/cpp
CPP = ${CC} -E

# If you have gcc and don't intend to hack around with the game,
# I recommend setting CC to gcc and using -O (as the CFLAGS).

#CFLAGS = -O
#LDFLAGS = -s
#CC = gcc

# comment out one of the following two, after establishing whether your
# machine uses termcap (most BSD machines) or terminfo (System-V)
#LIBS = -lcurses -ltermcap
#LIBS = -lcurses -ltermlib
# Linux links in term?? automatically.
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
      spell.o time.o trap.o util.o village.o compress.o

$(BINFILE): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) $(LIBS) -o $(BINFILE)

install: $(BINFILE)
	mkdir -p $(BINDIR)
	mkdir -p $(LIBDIR)
	mkdir -p $(SAVEDIR)
	cp $(BINFILE) $(BINDIR)
	chmod 4711 $(BINDIR)/$(BINFILE)
	- cp lib/* $(LIBDIR)
	chmod 0644 $(LIBDIR)/help*.txt $(LIBDIR)/license.txt $(LIBDIR)/motd.txt $(LIBDIR)/thanks.txt $(LIBDIR)/update.txt
	chmod 0600 $(LIBDIR)/abyss.txt $(LIBDIR)/scroll[1234].txt $(LIBDIR)/*.dat
	chmod 0600 $(LIBDIR)/omega.hi $(LIBDIR)/omega.log $(LIBDIR)/omegahi.bak

install_not_suid: $(BINFILE) $(BINDIR) $(LIBDIR)
	cp $(BINFILE) $(BINDIR)
	chmod 0711 $(BINDIR)/$(BINFILE)
	- cp lib/* $(LIBDIR)
	chmod 0644 $(LIBDIR)/help*.txt $(LIBDIR)/license.txt $(LIBDIR)/motd.txt $(LIBDIR)/thanks.txt $(LIBDIR)/update.txt
	chmod 0644 $(LIBDIR)/abyss.txt $(LIBDIR)/scroll[1234].txt $(LIBDIR)/*.dat
	chmod 0666 $(LIBDIR)/omega.hi $(LIBDIR)/omega.log
	chmod 0600 $(LIBDIR)/omegahi.bak

clean:
	rm -f $(OBJ) genclr genclr.o
	rm -i $(BINFILE)

$(OBJ): clrgen.h defs.h extern.h glob.h

clrgen.c: genclr
	$(CPP) $(CFLAGS) -DOMEGA_CLRGEN *.[ch] | ./genclr clrgen.c clrgen.h
clrgen.h: | clrgen.c

genclr: genclr.o
	$(CC) $(CFLAGS) $(LDFLAGS) genclr.o -o genclr
