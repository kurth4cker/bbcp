.POSIX:
# recommended for end user and packager customization below
# and LDFLAGS and LDLIBS
CC = cc
DEBUG = yes
CFLAGS = -Wall -Wextra -Werror

# internal settings
BIN = bbcp
OBJ = bbcp.o cpck.o cp2dir.o
INC = common.h

CFLAGS_DEBUG_yes = -g
CFLAGS_DEBUG_no = -DNDEBUG

OBJ_CFLAGS = $(CFLAGS_DEBUG_$(DEBUG)) $(CFLAGS)
LIBS = -lsdie ${LDLIBS}

all: ${BIN}
${BIN}: ${OBJ}
${OBJ}: ${INC}

clean:
	rm -f ${BIN} *.o

check t test: $(BIN)
	sh testcp.sh -p $(BIN)

.SUFFIXES: .c .o
.c.o:
	${CC} ${OBJ_CFLAGS} -c $<

.o:
	${CC} ${LDFLAGS} -o $@ ${OBJ} ${LIBS}
