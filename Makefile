BIN = bbcp
OBJ = bbcp.o die.o
INC = util.h

include config.mk

all: ${BIN}
${BIN}: ${OBJ}
	${CC} ${BBCP_LDFLAGS} -o $@ ${OBJ} ${BBCP_LDLIBS}

${OBJ}: ${INC}

clean:
	rm -f ${BIN} *.o

.SUFFIXES: .c .o
.c.o:
	${CC} ${BBCP_CFLAGS} -c $<
