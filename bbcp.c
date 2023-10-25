#include <sys/stat.h>

#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "util.h"

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
