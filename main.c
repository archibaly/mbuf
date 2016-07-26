#include <stdio.h>

#include "mbuf.h"

int main()
{
	unsigned char buf[1024];

	int i;
	for (i = 0; i < sizeof(buf); i++)
		buf[i] = 'b';

	mbuf_add(sizeof(buf), buf);
	mbuf_add(8, (unsigned char *)" world!");
	mbuf_add(9, (unsigned char *)" world!\n");

	mbuf_add_ahead(8, (unsigned char *)" world!");
	mbuf_add_ahead(8, (unsigned char *)" world!");

	mbuf_write(fileno(stdout));

	mbuf_free();

	return 0;
}
