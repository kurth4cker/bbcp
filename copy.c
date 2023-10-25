#include <sys/stat.h>

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "util.h"

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
