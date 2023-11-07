#include <sys/stat.h>

#include <fcntl.h>
#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sdie.h>

#include "copy.h"

#define PERM (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)

/* copy fd1's content into fd2 */
int
bbcopy(int fd1, int fd2)
{
	char buf[BUFSIZ];
	ssize_t n;

	while ((n = read(fd1, buf, sizeof(buf))) > 0)
		if (write(fd2, buf, n) == -1)
			return -1;

	return n;
}

/* create s2 and copy s1's content to s2 */
int
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

void
cp2dir(const char *file, const char *dir)
{
	char buf[PATH_MAX];
	char *src;
	char *bname;
	size_t dlen;
	int len;

	if ((src = strdup(file)) == NULL)
		die("strdup: '%s':", file);

	bname = basename(src);
	dlen = strlen(dir);

	if (dlen > 0 && dir[dlen - 1] == '/')
		len = snprintf(buf, sizeof(buf), "%s%s", dir, bname);
	else
		len = snprintf(buf, sizeof(buf), "%s/%s", dir, bname);

	if (len < 0 || (size_t)len >= sizeof(buf))
		die("'%s/%s': name too long\n", dir, bname);

	cpck(file, buf);
}

void
cpck(const char *a, const char *b)
{
	struct stat sta, stb;

	if (stat(a, &sta) == 0
	    && stat(b, &stb) == 0
	    && sta.st_dev == stb.st_dev
	    && sta.st_ino == stb.st_ino) {
		die("%s -> %s: same file\n", a, b);
	}

	if (cp(a, b) == -1)
		die("%s -> %s:", a, b);
}
