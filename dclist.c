/* linux kernel style doubly circularly linked list. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h> /* offsetof() */

/* macro function container_of(), copied from linux kernel */
#define container_of(ptr, type, member) ({				\
			const typeof( ((type *)0)->member ) *__mptr = (ptr); \
			(type *)( (char *)__mptr - offsetof(type,member) );})

/* linux kernel style list */
typedef struct list_head {
        struct list_head *_next;
        struct list_head *_prev;
} list_head;

typedef struct list_t {
        char      *_str;
        list_head _list;
} list_t;

static list_head* list_init(const char *str)
{
        list_t *new;
        if (!(new = (list_t*)malloc(sizeof(*new))))
                goto err;

        if (!(new->_str = strdup(str)))
                goto err;

        /* initial, points to self */
        new->_list._next = &new->_list;
        new->_list._prev = &new->_list;
        return &new->_list;
err:
        return NULL;
}

static list_head* list_add(list_head *p, const char *str)
{
        list_t *new;

        if (!p)
                goto err;
        if(!(new = (list_t*)malloc(sizeof(*new))))
                goto err;
        if (!(new->_str = strdup(str)))
                goto err;

        new->_list._prev      = p;
        new->_list._next      = p->_next;
        p->_next->_prev = &new->_list; /* head data */
        p->_next        = &new->_list;

        return &new->_list;
err:
        return NULL;
}

static list_head* list_del(list_head *p)
{
        list_t *q;
        list_head *head;

        if (!p)
                goto err;
        head = p->_prev;
        q = container_of(p, list_t, _list);
        if (!q->_str)
                goto err;
        p->_prev->_next = p->_next;
        p->_next->_prev = p->_prev;
        free(q->_str);
        free(q);
        return head;
err:
        return NULL;
}

static void list_show(list_head *p)
{
        list_head *pos;
        list_t *q;

        for (pos = p->_next; ; pos = pos->_next) {
                q = container_of(pos, list_t, _list);
                printf("this: %p next: %p prev: %p data: %s\n", &q->_list, q->_list._next, q->_list._prev, q->_str);
                if (pos == p)
                        break;
        }
}

static void list_clear(list_head *p)
{
        list_head *pos, *q;

        for (pos = p->_next; ; pos = pos->_next) {
                q = pos;
                q = list_del(q);
                if (pos == p)
                        break;
        }
}

int main(int argc, char **argv)
{
        int i;
        list_head *p = list_init(argv[0]);

        for (i=1; i<argc; i++) {
                p = list_add(p, argv[i]);
        }
        list_show(p);
        list_clear(p);
        return 0;
}
