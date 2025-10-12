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
    struct list_head *node = NULL;
    struct list_head *next = NULL;
    element_t *node_element = NULL;

    if (!head) {
        return;
    }

    node = head->next;
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
    element_t *new_element;

    if (!head) {
        return false;
    }
    new_element = (element_t *) malloc(sizeof(element_t));
    if (!new_element) {
        free(new_element);
        return false;
    }
    new_element->value = (char *) malloc(sizeof(char) * (strlen(s) + 1));
    if (!new_element->value) {
        free(new_element->value);
        free(new_element);
        return false;
    }

    memcpy(new_element->value, s, sizeof(char) * (strlen(s) + 1));
    list_add(&new_element->list, head);

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    element_t *new_element = NULL;

    if (!head)
        return false;
    new_element = (element_t *) malloc(sizeof(element_t));
    if (!new_element) {
        free(new_element);
        return false;
    }
    new_element->value = (char *) malloc(sizeof(char) * (strlen(s) + 1));
    if (!new_element->value) {
        free(new_element->value);
        free(new_element);
        return false;
    }
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
    if (!node || list_empty(head)) {
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
    if (!node || list_empty(head)) {
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
    struct list_head *node;
    int cnt = 0;
    if (!head) {
        return 0;
    }
    node = head->next;
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
    // aware of memory/pointer aliasing problem (different name pointer, but
    // points to same memory)
    if (!head || list_empty(head))
        return false;

    struct list_head **indirect = &head->next;
    element_t *element = NULL;
    for (struct list_head *fast = *indirect; fast != head && fast->next != head;
         fast = fast->next->next) {
        indirect = &(*indirect)->next;
    }
    struct list_head *del = *indirect;
    *indirect = (*indirect)->next;
    element = list_entry(del, element_t, list);

    list_del(del);
    free(element->value);
    free(element);

    return true;
}

/* Delete all nodes that have duplicate string */
// all delete!!!
// using one buf to record first appearance, if meeting same data, delete it!
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head || list_empty(head))
        return false;

    for (struct list_head *node = head->next;
         node != head && node->next != head;) {
        struct list_head *cur = node, *last = node->next;
        element_t *cur_element = list_entry(cur, element_t, list);
        element_t *last_element = list_entry(last, element_t, list);

        while (last != head &&
               strcmp(cur_element->value, last_element->value) == 0) {
            last = last->next;
            last_element = list_entry(last, element_t, list);
        }

        while (cur != last) {
            struct list_head *del = cur;
            element_t *del_element = list_entry(del, element_t, list);
            cur = cur->next;
            list_del(del);
            free(del_element->value);
            free(del_element);
        }
        node = last;
    }
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
    if (!head)
        return;

    for (struct list_head *cur = head->next;
         cur != head && cur->next != head;) {
        struct list_head *next = cur->next;
        struct list_head *prev = cur->prev, *next_next = next->next;
        /*swap next and prev */
        cur->next = next->next;
        next->prev = cur->prev;
        cur->prev = next;
        next->next = cur;
        /* swap node*/
        prev->next = next;
        next_next->prev = cur;

        cur = cur->next;
    }
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head)
        return;

    struct list_head *node = head;
    do {
        struct list_head *tmp = node->next;

        node->next = node->prev;
        node->prev = tmp;
        node = tmp;
    } while (node != head);
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
// NOTE: ans is correct but test checker will say error.
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head))
        return 0;

    struct list_head *node = head->next;
    struct list_head *stack = q_new();

    char min_str[] =
        "";  // use to simulate MIN for strcmp, empty is smallest in strcmp
    q_insert_tail(stack, min_str);

    while (node != head) {
        element_t *node_element = list_entry(node, element_t, list);
        element_t *top_element = list_entry(stack->prev, element_t, list);
        struct list_head *next = node->next;

        // accord to do_ascend in q_test.c, compare should use this expression
        while (strcmp(node_element->value, top_element->value) < 0) {
            list_del(stack->prev);
            free(top_element->value);
            free(top_element);

            // update new top
            top_element = list_entry(stack->prev, element_t, list);
        };
        list_del(node);              // remove from original queue
        list_add_tail(node, stack);  // add to ascend stack, beware of
                                     // LIST_POISONING enable!
        node = next;
    }

    // replace stack head to queue head
    head->next = (stack->next->next);  // ignore (top)int_max
    (stack->next->next)->prev = head;
    head->prev = stack->prev;
    stack->prev->next = head;

    // stack remove helper
    stack->next->next = stack;
    stack->prev = stack->next;
    // remove stack component
    q_free(stack);
    return q_size(head);
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head))
        return 0;

    struct list_head *node = head->next;
    struct list_head *stack = q_new();

    char max_str[64];  // use to simulate MAX for strcmp
    memset(max_str, 0xFF, sizeof(char) * 63);
    max_str[63] = '\0';
    q_insert_tail(stack, max_str);

    while (node != head) {
        element_t *node_element = list_entry(node, element_t, list);
        element_t *top_element = list_entry(stack->prev, element_t, list);
        struct list_head *next = node->next;

        // accord to do_ascend in q_test.c, compare should use this expression
        while (strcmp(node_element->value, top_element->value) > 0) {
            list_del(stack->prev);
            free(top_element->value);
            free(top_element);

            // update new top
            top_element = list_entry(stack->prev, element_t, list);
        };
        list_del(node);              // remove from original queue
        list_add_tail(node, stack);  // add to ascend stack, beware of
                                     // LIST_POISONING enable!
        node = next;
    }

    // replace stack head to queue head
    head->next = (stack->next->next);  // ignore (top)int_max
    (stack->next->next)->prev = head;
    head->prev = stack->prev;
    stack->prev->next = head;

    // stack remove helper
    stack->next->next = stack;
    stack->prev = stack->next;
    // remove stack component
    q_free(stack);
    return q_size(head);
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
static struct list_head *q_ctx_cmp(struct list_head *head, bool descend)
{
    if (!head || list_empty(head)) {
        return NULL;
    }

    queue_contex_t *q_ctx_cur, *q_ctx_safe;
    element_t *extrem_1st_element, *cur_1st_element;
    char max_str[64];
    element_t max_element = {.value = max_str};

    memset(max_str, 0xFF, sizeof(char) * 63);
    max_str[63] = '\0';
    extrem_1st_element = &max_element;

    list_for_each_entry_safe(q_ctx_cur, q_ctx_safe, head, chain) {
        if (!q_ctx_cur->q || list_empty(q_ctx_cur->q)) {
            // no element, do nothing
        } else {
            cur_1st_element = list_first_entry(q_ctx_cur->q, element_t, list);
            if (strcmp(extrem_1st_element->value, cur_1st_element->value) >=
                0) {
                extrem_1st_element = cur_1st_element;
            }
        }
    }
    list_del(&extrem_1st_element->list);
    return &extrem_1st_element->list;
}

static bool q_is_all_empty(struct list_head *head)
{
    queue_contex_t *q_ctx_cur, *q_ctx_safe;

    list_for_each_entry_safe(q_ctx_cur, q_ctx_safe, head, chain) {
        if (!list_empty(q_ctx_cur->q))
            return false;
    }
    return true;
}
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    struct list_head merged_head;
    queue_contex_t *q_ctx_1st_entry =
        list_first_entry(head, queue_contex_t, chain);

    INIT_LIST_HEAD(&merged_head);
    while (!q_is_all_empty(head)) {
        struct list_head *extrem_node = q_ctx_cmp(head, descend);
        if (descend)
            list_add(extrem_node, &merged_head);
        else
            list_add_tail(extrem_node, &merged_head);

        // printf("show merged list:");
        // list_for_each_entry_safe(node, safe, &merged_head, list) {
        //     printf("%s, ", node->value);
        // }
        // printf("\r\n");
    }

    list_splice(&merged_head, q_ctx_1st_entry->q);

    // printf("show result list:");
    // list_for_each_entry_safe(node, safe, q_ctx_1st_entry->q, list) {
    //     printf("%s, ", node->value);
    // }
    // printf("\r\n");

    return q_size(q_ctx_1st_entry->q);
}
