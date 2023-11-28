/* See LICENSE file for copyright and license details. */

#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

#include <sdie.h>

#include "copy.h"

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
