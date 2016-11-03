#ifndef _MBUF_H_
#define _MBUF_H_

#include "list.h"

#define MLEN		1024
#define mbuf_head	list_head

struct mbuf {
	struct list_head list;
	int len;	/* number of bytes in this mbuf */
	unsigned char databuf[MLEN];
};

void mbuf_init(struct mbuf_head *head);
void mbuf_add_after(struct mbuf_head *head, const unsigned char *data, int len);
void mbuf_add_ahead(struct mbuf_head *head, const unsigned char *data, int len);
int mbuf_get_len(const struct mbuf_head *head);
int mbuf_write(int fd, const struct mbuf_head *head);
void mbuf_free(struct mbuf_head *head);

#endif /* _MBUF_H_ */
