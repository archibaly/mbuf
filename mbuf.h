#ifndef _MBUF_H_
#define _MBUF_H_

#include "list.h"

#define MLEN	256

struct mbuf {
	struct list_head list;
	int				 len;	/* amount of data in this mbuf */
	unsigned char	 databuf[MLEN];
};

void mbuf_add(const unsigned char *data, int len);
void mbuf_add_ahead(const unsigned char *data, int len);
int  mbuf_write(int fd);
void mbuf_free(void);

#endif /* _MBUF_H_ */
