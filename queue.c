#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *new_head =
        (struct list_head *) malloc(sizeof(struct list_head));
    if (!new_head) {
        return NULL;
    }
    INIT_LIST_HEAD(new_head);
    return new_head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    struct list_head *node = head->next;
    struct list_head *next = NULL;
    element_t *node_element = NULL;
    if (!head) {
        return;
    }
    while (node != head) {
        next = node->next;
        node_element = list_entry(node, element_t, list);
        list_del(node);
        free(node_element->value);
        free(node_element);
        node = next;
    }

    list_del_init(head);
    free(head);

    return;
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    element_t *new_element = (element_t *) malloc(sizeof(element_t));
    new_element->value = (char *) malloc(sizeof(char) * (strlen(s) + 1));
    memcpy(new_element->value, s, sizeof(char) * (strlen(s) + 1));
    list_add(&new_element->list, head);


    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    element_t *new_element = (element_t *) malloc(sizeof(element_t));
    new_element->value = (char *) malloc(sizeof(char) * (strlen(s) + 1));
    memcpy(new_element->value, s, sizeof(char) * (strlen(s) + 1));
    list_add_tail(&new_element->list, head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    // return the element that has been removed.
    element_t *element = NULL;
    struct list_head *node = head;
    if (!node) {
        return NULL;
    }
    node = node->next;
    element = list_entry(node, element_t, list);
    strncpy(sp, element->value, bufsize - 1);
    sp[bufsize - 1] = '\0';

    list_del(node);
    return list_entry(node, element_t, list);
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    // return the element that has been removed.
    element_t *element = NULL;
    struct list_head *node = head;
    if (!node) {
        return NULL;
    }
    node = node->prev;
    element = list_entry(node, element_t, list);
    strncpy(sp, element->value, bufsize - 1);
    sp[bufsize - 1] = '\0';

    list_del(node);
    return list_entry(node, element_t, list);
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    struct list_head *node = head->next;
    int cnt = 0;
    if (!node) {
        return -1;
    }
    while (node != head) {
        node = node->next;
        cnt++;
    }

    return cnt;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
