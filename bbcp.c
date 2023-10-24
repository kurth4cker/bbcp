#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "util.h"

#define PERM (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)

static int
bbcopy(int fd1, int fd2)
{
	char buf[BUFSIZ];
	ssize_t n;

	while ((n = read(fd1, buf, sizeof(buf))) > 0)
		if (write(fd2, buf, n) == -1)
			return -1;

	return n;
}

static void
usage(int code)
{
	fprintf(stderr, "usage: %s file newfile\n", argv0);

	fputc('\n', stderr);
	fprintf(stderr, "see also bbcp(1)\n");

	exit(code);
}

int
main(int argc, char **argv)
{
	int ch;
	int fd1, fd2;

	argv0 = argv[0];

	while ((ch = getopt(argc, argv, "h")) != -1) {
		switch (ch) {
		case 'h':
			usage(EXIT_SUCCESS);
			/* FALLTHROUGH */
		default:
			usage(EXIT_FAILURE);
		}
	}

	if (argc != 3)
		usage(EXIT_FAILURE);

	if ((fd1 = open(argv[1], O_RDONLY)) == -1)
		die("can't read '%s':", argv[1]);
	if ((fd2 = creat(argv[2], PERM)) == -1)
		die("can't creat '%s':", argv[2]);

	if (bbcopy(fd1, fd2) == -1)
		die("can't copy '%s' to '%s':", argv[1], argv[2]);

	close(fd2);
	close(fd1);

	return 0;
}
