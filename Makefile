PROG = bbcp
OBJ = bbcp.o

include config.mk

all: ${PROG}
${PROG}: ${OBJ}
	${CC} -o $@ ${OBJ} ${BBCP_LDFLAGS}

clean:
	rm -f ${PROG} *.o

.SUFFIXES: .c .o
.c.o:
	${CC} ${BBCP_CFLAGS} -c $<
