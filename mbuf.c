#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mbuf.h"
#include "writen.h"

static LIST_HEAD(head);

static struct mbuf *mbuf_new(int len, const unsigned char *data)
{
	struct mbuf *mp;
	
	if (!(mp = malloc(sizeof(struct mbuf))))
		exit(-1);

	list_init(&mp->list);
	mp->len = len;
	memcpy(mp->databuf, data, len);

	return mp;
}

static int mbuf_append(struct mbuf *mp, int len, const unsigned char *data)
{
	int left = MLEN - mp->len;

	if (len < 0)
		return -1;

	if (len <= left) {
		memcpy(mp->databuf + mp->len, data, len);
		mp->len += len;
		return 0;
	} else {
		memcpy(mp->databuf + mp->len, data, left);
		mp->len += left;
		return len - left;
	}
}

void mbuf_add(int len, const unsigned char *data)
{
	int i = 0;
	struct mbuf *mp;
	struct list_head *hp = list_get_tail(&head);

	int left = len;

	if (!list_empty(&head)) {
		mp = list_entry(hp, struct mbuf, list);
		left = mbuf_append(mp, len, data);
	}

	for (i = 0; left > MLEN; i++) {
		mp = mbuf_new(MLEN, data + i * MLEN);
		list_add(&mp->list, hp);
		hp = &mp->list;
		left -= MLEN;
	}

	if (left > 0) {
		mp = mbuf_new(left, data + i * MLEN);
		list_add(&mp->list, hp);
	}
}

void mbuf_add_ahead(int len, const unsigned char *data)
{
	int i = 0;
	struct mbuf *mp;
	struct list_head *hp = &head;

	for (i = 0; len > MLEN; i++) {
		mp = mbuf_new(MLEN, data + i * MLEN);
		list_add(&mp->list, hp);
		hp = &mp->list;
		len -= MLEN;
	}

	if (len > 0) {
		mp = mbuf_new(len, data + i * MLEN);
		list_add(&mp->list, hp);
	}
}

int mbuf_write(int fd)
{
	int ret;
	int n = 0;

	struct mbuf *pos;
	list_for_each_entry(pos, &head, list) {
		if ((ret = writen(fd, pos->databuf, pos->len)) < 0)
			return -1;
		else
			n += ret;
	}

	return n;
}

void mbuf_free(void)
{
	struct mbuf *pos;

	while (!list_empty(&head)) {
		pos = list_entry(head.next, struct mbuf, list);
		list_del(head.next);
		free(pos);
	}
}
