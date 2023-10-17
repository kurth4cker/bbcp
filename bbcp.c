#include <sys/stat.h>

#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static char *argv0;

static int
bbcopy(int src_fd, int dest_fd)
{
	char buf[BUFSIZ];
	ssize_t nbyte;

	for (;;) {
		nbyte = read(src_fd, buf, sizeof(buf));
		if (nbyte <= 0)
			break;

		if (write(dest_fd, buf, nbyte) == -1)
			return -1;
	}

	return nbyte;
}

static int
dest_open(char *name)
{
	return open(name, O_WRONLY|O_TRUNC|O_CREAT, S_IRWXU|S_IRWXG|S_IRWXO);
}

static void
usage(int code)
{
	fprintf(stderr, "usage: %s file newfile\n", argv0);
	fprintf(stderr, "usage: %s file dir\n", argv0);
	fprintf(stderr, "\n");
	fprintf(stderr, "see also bbcp(1)\n");

	exit(code);
}

int
main(int argc, char **argv)
{
	int src_fd, dest_fd;

	argv0 = argv[0];

	if (argc < 3)
		usage(EXIT_FAILURE);

	src_fd = open(argv[1], O_RDONLY);
	if (src_fd == -1)
		err(EXIT_FAILURE, "open %s for reading", argv[1]);

	dest_fd = dest_open(argv[2]);
	if (dest_fd == -1)
		err(EXIT_FAILURE, "cannot create %s", argv[2]);

	if (bbcopy(src_fd, dest_fd) == -1)
		err(EXIT_FAILURE, "cannot copy %s to %s", argv[1], argv[2]);

	close(dest_fd);
	close(src_fd);

	return 0;
}
