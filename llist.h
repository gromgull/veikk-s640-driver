// simple linked list for veikks
#ifndef VEIKK_S640_DRIVER_LLIST_H
#define VEIKK_S640_DRIVER_LLIST_H

#include <linux/slab.h>

// llist structs
struct llnode {
    void *data;
    struct llnode *next;
};
struct llist {
    struct llnode *start;
    int length;
};

// func definitions
struct llist *llist_create(void);
void llist_destroy(struct llist *);
void llist_append(struct llist *, void *);
void llist_remove(struct llist *, void *);

#endif //VEIKK_S640_DRIVER_LLIST_H
