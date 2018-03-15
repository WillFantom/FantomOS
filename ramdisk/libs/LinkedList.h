#ifndef __LINKEDLIST_H
#define __LINKEDLIST_H

typedef struct ListNode ListNode_t;

struct ListNode
{
    unsigned int *data;
    ListNode_t *nextNode;
    ListNode_t *prevNode;
};

typedef struct LinkedList_t
{
    unsigned int items;
    ListNode_t *front;
    ListNode_t *end;

} LinkedList;

#endif
