// simple linked list for veikks
#include "llist.h"

struct llist *llist_create() {
    struct llist *list = (struct llist *)
            kmalloc(sizeof(struct llist), GFP_KERNEL);
    struct llnode *start = (struct llnode *)
            kmalloc(sizeof(struct llnode), GFP_KERNEL);
    start->data = NULL;
    start->next = NULL;
    list->start = start;
    list->length = 0;
    return list;
}
void llist_destroy(struct llist *llist) {
    struct llnode *iter,
            *iter_next = llist->start;
    while((iter = iter_next) != NULL) {
        iter_next = iter->next;
        kfree(iter);
    }
    kfree(llist);
}
void llist_append(struct llist *l, void *elem) {
    struct llnode *node = l->start->next,
            *new_node = (struct llnode *)
            kmalloc(sizeof(struct llnode), GFP_KERNEL);
    new_node->data = elem;
    new_node->next = node;
    l->start->next = new_node;
    l->length++;
}
void llist_remove(struct llist *l, void *elem) {
    struct llnode *node = l->start,
            *next_node;
    while(node->next != NULL && node->next->data != elem)
        node = node->next;
    if(node->next != NULL) {
        next_node = node->next->next;
        kfree(node->next);
        node->next = next_node;
        l->length--;
    }
}

