#ifndef _MBUF_H_
#define _MBUF_H_

#include "list.h"

#define MLEN	256

struct mbuf {
	struct list_head list;
	int				 len;	/* amount of data in this mbuf */
	unsigned char	 databuf[MLEN];
};

struct mbuf_head {
	struct list_head head;
};

void mbuf_init(struct mbuf_head *head);
void mbuf_add_after(struct mbuf_head *head, const unsigned char *data, int len);
void mbuf_add_ahead(struct mbuf_head *head, const unsigned char *data, int len);
int mbuf_write(const struct mbuf_head *head, int fd);
void mbuf_free(struct mbuf_head *head);

#endif /* _MBUF_H_ */
