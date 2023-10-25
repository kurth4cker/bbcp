#include <sys/stat.h>

#include <fcntl.h>
#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

static int
cp(const char *s1, const char *s2)
{
	int fd1, fd2;
	int status;

	if ((fd1 = open(s1, O_RDONLY)) == -1)
		die("read %s:", s1);
	if ((fd2 = creat(s2, PERM)) == -1)
		die("create %s:", s2);

	status = bbcopy(fd1, fd2);

	close(fd2);
	close(fd1);
	return status;
}

static void
cpck(const char *s1, const char *s2)
{
	struct stat st1, st2;

	if (stat(s1, &st1) == 0
	    && stat(s2, &st2) == 0
	    && st1.st_dev == st2.st_dev
	    && st1.st_ino == st2.st_ino) {
		die("%s -> %s: same file\n", s1, s2);
	}

	if (cp(s1, s2) == -1)
		die("%s -> %s:", s1, s2);
}

static void
usage(int code)
{
	fprintf(stderr, "usage: %s file newfile\n", argv0);
	fprintf(stderr, "usage: %s file dir\n", argv0);

	fputc('\n', stderr);
	fprintf(stderr, "see also bbcp(1)\n");

	exit(code);
}

int
main(int argc, char **argv)
{
	struct stat st;
	char buf[PATH_MAX];
	const char *src;
	const char *dir;
	const char *bname;
	size_t dlen;
	int ch;
	int len;

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

	if (!(stat(argv[2], &st) == 0 && S_ISDIR(st.st_mode))) {
		cpck(argv[1], argv[2]);
		return 0;
	}

	dir = argv[2];
	if ((src = strdup(argv[1])) == NULL)
		die("strdup:");

	dlen = strlen(dir);
	bname = basename(argv[1]);
	if (dlen > 0 && dir[dlen - 1] == '/')
		len = snprintf(buf, sizeof(buf), "%s%s", dir, bname);
	else
		len = snprintf(buf, sizeof(buf), "%s/%s", dir, bname);
	if (len < 0 || (size_t)len >= sizeof(buf))
		die("%s/%s: filename too long\n", dir, bname);

	cpck(src, buf);
	return 0;
}
