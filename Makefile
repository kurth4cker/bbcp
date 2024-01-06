.POSIX:
# recommended for end user and packager customization below
# and LDFLAGS and LDLIBS
CC = cc
CFLAGS = -g -Wall -Wextra -Werror

# internal settings
BIN = bbcp
OBJ = main.o cpck.o cp2dir.o
INC = common.h

BIN_LDFLAGS = -lsdie ${LDFLAGS}

all: ${BIN}
${OBJ}: ${INC}

${BIN}: ${OBJ}
	${CC} -o $@ ${OBJ} ${BIN_LDFLAGS}

clean:
	rm -f ${BIN} *.o

check t test: ${BIN}
	sh testcp.sh -p ${BIN}

.SUFFIXES: .c .o
.c.o:
	${CC} ${CFLAGS} -c $<
