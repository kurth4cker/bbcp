#include <sys/stat.h>

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef VERSION
#define VERSION "(null-version)"
#endif

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
dest_open(char *dest, char *src)
{
	struct stat sb;
	char *file;
	size_t len;

	file = dest;

	if (lstat(dest, &sb) == -1) {
		if (errno == ENOENT)
			goto just_open;
		return -1;
	}

	if (S_ISDIR(sb.st_mode)) {
		len = strlen(dest) + strlen(src) + 2;

		if ((file = malloc(len)) == NULL)
			return -1;

		snprintf(file, len, "%s/%s", dest, basename(src));
	}

just_open:
	return open(file, O_WRONLY|O_CREAT|O_TRUNC,
		S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);
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
	int ch;

	argv0 = argv[0];

	while ((ch = getopt(argc, argv, "hv")) != -1) {
		switch (ch) {
		case 'h':
			usage(EXIT_SUCCESS);
			/* FALLTHROUGH */
		case 'v':
			fprintf(stderr, "bbcp %s\n", VERSION);
			exit(EXIT_SUCCESS);
		default:
			usage(EXIT_FAILURE);
		}
	}

	if (argc < 3)
		usage(EXIT_FAILURE);

	src_fd = open(argv[1], O_RDONLY);
	if (src_fd == -1)
		err(EXIT_FAILURE, "open %s for reading", argv[1]);

	dest_fd = dest_open(argv[2], argv[1]);
	if (dest_fd == -1)
		err(EXIT_FAILURE, "cannot copy to %s", argv[2]);

	if (bbcopy(src_fd, dest_fd) == -1)
		err(EXIT_FAILURE, "cannot copy %s to %s", argv[1], argv[2]);

	close(dest_fd);
	close(src_fd);

	return 0;
}
