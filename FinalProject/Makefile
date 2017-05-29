#	$Id: Makefile,v 1.0 2009/02/26 15:41:04 cberardi Exp $ 

CC=gcc
CFLAGS=-O

checkers: checkers.o ai.o draw.o move.o populate.o validate.o
	${CC} ${CFLAGS} checkers.o ai.o draw.o move.o populate.o validate.o \
	-o checkers -lcurses

checkers.o: checkers.c
	${CC} -c checkers.c

ai.o: ai.c
	${CC} -c ai.c

draw.o: draw.c
	${CC} -c draw.c

move.o: move.c
	${CC} -c move.c

populate.o: populate.c
	${CC} -c populate.c

validate.o: validate.c
	${CC} -c validate.c

clean:
	rm *.o

uninstall:
	rm *.o
	rm checkers
