CC = cc

BIN = bbcp
OBJ = bbcp.o die.o
INC = util.h

all: ${BIN}
${BIN}: ${OBJ}
${OBJ}: ${INC}

clean:
	rm -f ${BIN} *.o

.SUFFIXES: .c .o
.c.o:
	${CC} ${CFLAGS} ${CPPFLAGS} -c $<

.o:
	${CC} ${LDFLAGS} -o $@ ${OBJ} ${LDLIBS}
