# $Id: /mirror/yasa/trunk/yasa/Makefile 132 2009-02-20T13:59:56.470860Z clsung  $
CC = cc
DFLAGS = -Wall
INCLUDEDIRS= -I./
CFLAGS = -fPIC -c -O3 $(INCLUDEDIRS) 
CFLAGS+= ${DFLAGS}
OFLAGS = -o
DFLAGS = -pg -g
PROG = yasa
test = testyasa
test_src = Test/testyasa.c
LIBRARY= libyasa.so.0.2.4
LFLAGS = -shared -W1,-soname,libyasa.so.0

all:	${PROG} ${LIBRARY}

TOBJS		= $(LSRCS:%.c=%.o)
TSRCS		= Test/testyasa.c

LOBJS		= $(LSRCS:%.c=%.o)
LSRCS		= suffix.c \
		  char.c \
		  compare.c \
		  word_freq.c \
		  mmap.c
COBJS		= $(CSRCS:%.c=%.o)
CSRCS		= $(LSRCS) \
		  yasa.c

${LIBRARY}: $(LOBJS) $(LPPOBJS)
	$(CC) $(DFLAGS) $(LOBJS) $(LFLAGS) $(OFLAGS) $(LIBRARY)
${PROG}: $(COBJS) $(CPPOBJS)
	$(CC) $(DFLAGS) $(COBJS) $(CPPOBJS) $(OFLAGS) $(PROG)

clean: 
	rm *.o *.so.*
	rm -f ${PROG}

test: $(LOBJS) testyasa.o
	@echo Linking $(test)...
	$(CC) $(DFLAGS) -L/usr/local/lib -lcunit -o $(test) $(LOBJS) testyasa.o
	@echo Running unit tests...
	@./$(test)

testyasa.o : $(test_src)
	@echo $<
	$(CC) -ITest/UnitTest++ $(CFLAGS) -I/usr/local/include -c $< -o testyasa.o

