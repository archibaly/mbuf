#include <stdio.h>

#include "mbuf.h"

int main()
{
	unsigned char buf[1024];

	int i;
	for (i = 0; i < sizeof(buf); i++)
		buf[i] = '*';

	struct mbuf_head packet_info;

	mbuf_init(&packet_info);

	mbuf_add_after(&packet_info, buf, sizeof(buf));
	mbuf_add_after(&packet_info, (unsigned char *)"\nWorld!\n", 9);

	mbuf_add_ahead(&packet_info, (unsigned char *)"World!\n", 8);
	mbuf_add_ahead(&packet_info, (unsigned char *)"Hello ", 6);

	mbuf_write(&packet_info, fileno(stdout));

	mbuf_free(&packet_info);

	printf("writed %d\n", mbuf_write(&packet_info, fileno(stdout)));

	return 0;
}
