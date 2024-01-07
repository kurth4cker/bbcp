.POSIX:
VERSION = 0.0.0_git

PREFIX = /usr/local
BINDIR = ${PREFIX}/bin
MANDIR = ${PREFIX}/share/man

CC = cc
CFLAGS = -g -Wall -Wextra -Werror

BIN_LDFLAGS = -lsdie ${LDFLAGS}

BIN = bbcp
OBJ = main.o cpck.o cp2dir.o
SRC = ${OBJ:.o=.c}
INC = common.h
MAN1 = bbcp.1

DISTNAME = ${BIN}-${VERSION}
DISTFILES = LICENSE Makefile testcp.sh ${SRC} ${INC} ${MAN1}

all: ${BIN}
${OBJ}: ${INC}

${BIN}: ${OBJ}
	${CC} -o $@ ${OBJ} ${BIN_LDFLAGS}

clean:
	rm -f ${BIN} *.o ${DISTNAME}.tar.gz

check t test: ${BIN}
	sh testcp.sh -p ${BIN}

dist: ${DISTFILES}
	mkdir ${DISTNAME}
	cp -f ${DISTFILES} ${DISTNAME}
	tar -c ${DISTNAME} | gzip -c > ${DISTNAME}.tar.gz
	rm -r ${DISTNAME}

install: ${BIN} ${MAN1}
	mkdir -p ${DESTDIR}${BINDIR} ${DESTDIR}${MANDIR}/man1
	cp -f ${BIN} ${DESTDIR}${BINDIR}
	cp -f ${MAN1} ${DESTDIR}${MANDIR}/man1

uninstall:
	rm -f ${DESTDIR}${BINDIR}/${BIN}
	rm -f ${DESTDIR}${MANDIR}/man1/${MAN1}

.SUFFIXES: .c .o
.c.o:
	${CC} ${CFLAGS} -c $<
