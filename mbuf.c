#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <err.h>

#include "mbuf.h"
#include "writen.h"

static struct mbuf *mbuf_new(const unsigned char *data, int len)
{
	struct mbuf *mp;

	if (!(mp = malloc(sizeof(struct mbuf))))
		err(1, "malloc()");

	INIT_LIST_HEAD(&mp->list);
	mp->len = len;
	memcpy(mp->databuf, data, len);

	return mp;
}

static int mbuf_append(struct mbuf *mp, const unsigned char *data, int len)
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

void mbuf_init(struct mbuf_head *head)
{
	INIT_LIST_HEAD(head);
}

void mbuf_add_after(struct mbuf_head *head, const unsigned char *data, int len)
{
	int i = 0;
	struct mbuf *mp;
	struct list_head *hp = list_get_tail(head);

	int left = len;

	if (!list_empty(head)) {
		mp = list_entry(hp, struct mbuf, list);
		left = mbuf_append(mp, data, len);
	}

	for (i = 0; left > MLEN; i++) {
		mp = mbuf_new(data + i * MLEN, MLEN);
		list_add(&mp->list, hp);
		hp = &mp->list;
		left -= MLEN;
	}

	if (left > 0) {
		mp = mbuf_new(data + i * MLEN, left);
		list_add(&mp->list, hp);
	}
}

void mbuf_add_ahead(struct mbuf_head *head, const unsigned char *data, int len)
{
	int i = 0;
	struct mbuf *mp;
	struct list_head *hp = head;

	for (i = 0; len > MLEN; i++) {
		mp = mbuf_new(data + i * MLEN, MLEN);
		list_add(&mp->list, hp);
		hp = &mp->list;
		len -= MLEN;
	}

	if (len > 0) {
		mp = mbuf_new(data + i * MLEN, len);
		list_add(&mp->list, hp);
	}
}

int mbuf_get_len(const struct mbuf_head *head)
{
	int len = 0;
	struct mbuf *pos;

	if (list_empty(head))
		return 0;

	list_for_each_entry(pos, head, list)
		len += pos->len;

	return len;
}

int mbuf_write(int fd, const struct mbuf_head *head)
{
	int ret;
	int n = 0;

	struct mbuf *pos;
	list_for_each_entry(pos, head, list) {
		if ((ret = writen(fd, pos->databuf, pos->len)) < 0)
			break;
		else
			n += ret;
	}

	return n;
}

void mbuf_free(struct mbuf_head *head)
{
	struct mbuf *pos;

	while (!list_empty(head)) {
		pos = list_entry(head->next, struct mbuf, list);
		list_del(head->next);
		free(pos);
	}
}
