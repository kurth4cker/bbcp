/* See LICENSE file for copyright and license details. */

#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sdie.h>

#include "common.h"

static void
enmasse(const char *s1, const char *s2)
{
	struct stat st;

	if (stat(s2, &st) == 0 && S_ISDIR(st.st_mode)) {
		cp2dir(s1, s2);
		return;
	}

	cpck(s1, s2);
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

	argv += optind;
	argc -= optind;

	if (argc != 2)
		usage(EXIT_FAILURE);

	enmasse(argv[0], argv[1]);
}
