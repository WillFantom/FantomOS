
//ME
#include <modules/LinkedList.h>

//REQUIREMENTS
#include <modules/HeapMem_SBRK.h>

//CORE
#include <core/Common.h>
#include <core/Debug.h>

LinkedList *LinkedList_create()
{
    LinkedList *list = (uint32_t)calloc(1, sizeof(LinkedList));
    list->front = NULL;
    list->end = NULL;
    list->items = 0;
    return list;
}

void LinkedList_destroy(LinkedList *list)
{
    ListNode_t *curr = list->front;
    for(uint32_t i = 0 ; i < list->items ; i++)
    {
        ListNode_t *tmp = curr->nextNode;
        free(curr);
        curr = tmp;
    }

    free(list);
}

void LinkedList_addNode(LinkedList *list, void *data)
{
    ListNode_t *item = calloc(1, sizeof(ListNode_t));

    item->data = data;
    item->nextNode = NULL;
    if(list->items == 0)
    {
        item->prevNode = NULL;
        list->front = item;

    }
    else
    {
        item->prevNode = list->end;
        list->end->nextNode = item;
    }
    list->end = item;
    list->items++;
}

void *LinkedList_removeNode(LinkedList *list, ListNode_t *item)
{
    if(item == list->front && item == list->end)
    {
        list->front = NULL;
        list->end = NULL;
    }
    else if(item == list->front)
    {
        list->front = item->nextNode;
        list->front->prevNode = NULL;
    }
    else if(item == list->end)
    {
        list->end = item->prevNode;
        list->end->nextNode = NULL;
    }
    else
    {
        ListNode_t *tmpN = item->nextNode;
        ListNode_t *tmpP = item->prevNode;
        tmpN->prevNode = tmpP;
        tmpP->nextNode = tmpN;
    }
    void *data = item->data;
    free(item);
    list->items--;
    return data;
}

ListNode_t *LinkedList_getNodeByIndex(LinkedList *list, uint32_t index)
{
    if(index >= list->items)
        return 0;

    ListNode_t *item = list->front;
    for(uint32_t i = 0 ; i < list->items ; i++)
    {
        if(i == index)
            return item;
        item = item->nextNode;
    }

    return 0;
}

ListNode_t *LinkedList_getNodeByData(LinkedList *list, void *data)
{
    ListNode_t *item = list->front;

    while(item != NULL)
    {
        if(item->data == data)
            return item;

        item = item->nextNode;
    }

    return 0;
}
