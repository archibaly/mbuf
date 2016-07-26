#include <stdio.h>

#include "mbuf.h"

int main()
{
	unsigned char buf[1024];

	int i;
	for (i = 0; i < sizeof(buf); i++)
		buf[i] = '*';

	mbuf_add(buf, sizeof(buf));
	mbuf_add((unsigned char *)"\nWorld!\n", 9);

	mbuf_add_ahead((unsigned char *)"World!\n", 8);
	mbuf_add_ahead((unsigned char *)"Hello ", 6);

	mbuf_write(fileno(stdout));

	mbuf_free();

	return 0;
}
