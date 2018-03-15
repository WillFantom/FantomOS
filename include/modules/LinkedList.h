#ifndef __LINKEDLIST_H
#define __LINKEDLIST_H

#include <core/Common.h>

typedef struct ListNode ListNode_t;

struct ListNode
{
    void *data;
    ListNode_t *nextNode;
    ListNode_t *prevNode;
};

typedef struct LinkedList_t
{
    uint32_t items;
    ListNode_t *front;
    ListNode_t *end;

} LinkedList;

LinkedList *LinkedList_create();
void LinkedList_destroy(LinkedList *list);
void LinkedList_addNode(LinkedList *list, void *data);
void *LinkedList_removeNode(LinkedList *list, ListNode_t *item);
ListNode_t *LinkedList_getNodeByIndex(LinkedList *list, uint32_t index);
ListNode_t *LinkedList_getNodeByData(LinkedList *list, void *data);

#endif
