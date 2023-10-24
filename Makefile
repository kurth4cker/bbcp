CC = cc

BIN = bbcp
OBJ = bbcp.o die.o
INC = util.h

all: ${BIN}
${BIN}: ${OBJ}
${OBJ}: ${INC}

clean:
	rm -f ${BIN} *.o

check t test: $(BIN)
	sh testcp.sh -p $(BIN)

.SUFFIXES: .c .o
.c.o:
	${CC} ${CFLAGS} ${CPPFLAGS} -c $<

.o:
	${CC} ${LDFLAGS} -o $@ ${OBJ} ${LDLIBS}
