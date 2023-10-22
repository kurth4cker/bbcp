#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "util.h"

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

static int
cp(const char *s1, const char *s2)
{
	int fd1, fd2;
	int status;
	mode_t perm;

	perm = S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH;

	if ((fd1 = open(s1, O_RDONLY)) == -1)
		die("can't read %s:", s1);
	if ((fd2 = creat(s2, perm)) == -1)
		die("can't create %s:", s2);

	status = bbcopy(fd1, fd2);

	close(fd2);
	close(fd1);
	return status;
}

/*
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
*/

static void
usage(int code)
{
	fprintf(stderr, "usage: %s file newfile\n", argv0);
//	fprintf(stderr, "usage: %s file dir\n", argv0);
	fprintf(stderr, "\n");
	fprintf(stderr, "see also bbcp(1)\n");

	exit(code);
}

int
main(int argc, char **argv)
{
	int ch;

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

	if (argc < 3)
		usage(EXIT_FAILURE);

	if (cp(argv[1], argv[2]) == -1)
		die("can't copy %s to %s:", argv[1], argv[2]);

	return 0;
}
