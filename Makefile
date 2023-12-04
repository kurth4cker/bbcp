.POSIX:
# recommended for end user and packager customization below
# and LDFLAGS and LDLIBS
CC = cc
CFLAGS = -Wall -Wextra -Werror

DEBUG = yes

# internal settings
BIN = bbcp
OBJ = bbcp.o cpck.o cp2dir.o
INC = common.h

CFLAGS_DEBUG_yes = -g
CFLAGS_DEBUG_no = -DNDEBUG

OBJ_CFLAGS = ${CFLAGS_DEBUG_${DEBUG}} ${CFLAGS}
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
	${CC} ${OBJ_CFLAGS} -c $<
