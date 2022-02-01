/**
 * @file LinkedList.c
 * @author Jeff Hatton
 * @brief Implementation for LinkedList
 * @date 2022-01-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "LinkedList.h"
#include "Status.h"

#include "stdbool.h"
#include "string.h"

// Private Types ****************************************

// Constants ********************************************

// Macros ***********************************************

// Functions Prototypes *********************************

// Private Variables ************************************

// Private Functions *************************************

// Public Functions *************************************

Status_t LinkedList_Init(LinkedList_t* list)
{
    if (list == NULL) return Status_NullPtr;
    if (!STATUS_IS_CODE(list->Status, Status_NotInitialized)) return Status_AlreadyInitialized;
    
    list->Head = NULL;
    list->Status = Status_Ok;
    list->Size = 0;

    return list->Status;
}

Status_t LinkedList_Reset(LinkedList_t* list)
{
    if (list == NULL) return Status_NullPtr;
    if (STATUS_CHECK(list->Status)) return STATUS_PASS_UP(list->Status);

    LinkedListNode_t* ptr = list->Head;
    LinkedListNode_t* next;

    // Clear all links to ensure no accidental 
    // iterations
    while (ptr)
    {
        next = ptr->Next;
        ptr->Next = NULL;
        ptr = next;
    }

    list->Head = NULL;
    list->Size = 0;

    return Status_Ok;
}

Status_t LinkedList_Append(LinkedList_t* list, LinkedListNode_t* node, void* data, uint16_t size)
{
    if (list == NULL || node == NULL) return Status_NullPtr;
    if (STATUS_CHECK(list->Status)) return STATUS_PASS_UP(list->Status);

    if (list->Head == NULL)
    {
        list->Head = node;
    }
    else
    {
        LinkedListNode_t* ptr = list->Head;
        while (ptr->Next)
        {            
            if (ptr == node) return Status_AlreadyInBuffer;
            ptr = ptr->Next;
        }

        if (ptr == node) return Status_AlreadyInBuffer;
        ptr->Next = node;
    }

    node->Data = data;
    node->Size = size;
    node->Next = NULL;

    list->Size++;
    list->Rev++;

    return Status_Ok;
}

Status_t LinkedList_RemoveNode(LinkedList_t* list, LinkedListNode_t* node)
{
    if (list == NULL || node == NULL) return Status_NullPtr;
    if (STATUS_CHECK(list->Status)) return STATUS_PASS_UP(list->Status);
    if (list->Head == NULL) return Status_BufferEmpty;

    LinkedListNode_t* ptr = list->Head;

    // special case for head
    if (ptr == node) {
        LinkedListNode_t* next = ptr->Next;
        ptr->Next = NULL;
        list->Head = next;
        list->Size--;
        list->Rev++;
        return Status_Ok;
    }

    while (ptr)
    {         
        if (ptr->Next == node) {
            LinkedListNode_t* next = ptr->Next->Next;
            ptr->Next->Next = NULL;
            ptr->Next = next;
            list->Size--;
            list->Rev++;
            break;
        }
        ptr = ptr->Next;
    }

    if (ptr == NULL) return Status_NotFound;

    return Status_Ok;
}

Status_t LinkedList_RemoveData(LinkedList_t* list, void* data, uint16_t size)
{
    if (list == NULL || data == NULL) return Status_NullPtr;
    if (STATUS_CHECK(list->Status)) return STATUS_PASS_UP(list->Status);

    if (list->Head == NULL) return Status_BufferEmpty;

    LinkedListNode_t* ptr = list->Head;

    // Special case for head
    if (ptr->Size == size)
    {
        if (0 == memcmp(data, ptr->Data, size))
        {
            LinkedListNode_t* next = ptr->Next;
            ptr->Next = NULL;            
            list->Head = next;
            list->Size--;
            list->Rev++;
            return Status_Ok;
        }
    } 

    bool found = false;
    while (ptr->Next)
    {         
        if (ptr->Next->Size == size)
        {
            if (0 == memcmp(data, ptr->Next->Data, size))
            {
                found = true;
                LinkedListNode_t* next = ptr->Next->Next;
                ptr->Next->Next = NULL;
                ptr->Next = next;
                list->Size--;
                list->Rev++;
                break;
            }
        }

        ptr = ptr->Next;
    }

    if (!found) return Status_NotFound;

    return Status_Ok;
}

Status_t LinkedList_RemoveAt(LinkedList_t* list, uint16_t iter)
{
    if (list == NULL) return Status_NullPtr;
    if (STATUS_CHECK(list->Status)) return STATUS_PASS_UP(list->Status);

    if (list->Head == NULL) return Status_BufferEmpty;
    if (list->Size <= iter) return Status_IndexOOR;

    LinkedListNode_t* ptr = list->Head;

    // Special case for head
    if (iter == 0)
    {
        LinkedListNode_t* next = ptr->Next;
        ptr->Next = NULL;            
        list->Head = next;
        list->Size--;
        list->Rev++;
        return Status_Ok;
    } 

    while (iter--)
    {        
        ptr = ptr->Next;
    }

    LinkedListNode_t* next = ptr->Next->Next;
    ptr->Next->Next = NULL;
    ptr->Next = next;
    list->Size--;
    list->Rev++;

    return Status_Ok;
}

Status_t LinkedList_FindByData(LinkedList_t* list, void* data, uint16_t size, LinkedListNode_t** node)
{
    if (list == NULL || data == NULL || node == NULL) return Status_NullPtr;
    if (STATUS_CHECK(list->Status)) return STATUS_PASS_UP(list->Status);

    *node = NULL;
    LinkedListNode_t* ptr = list->Head;
    while (ptr)
    {         
        if (ptr->Size == size)
        {
            if (0 == memcmp(data, ptr->Data, size))
            {
                *node = ptr->Next;
                break;
            }
        }

        ptr = ptr->Next;
    }

    if (ptr == NULL) return Status_NotFound;

    return Status_Ok;
}

Status_t LinkedList_FindByMember(LinkedList_t* list, uint16_t nodeSize, uint32_t offset, void* data, uint16_t size, LinkedListNode_t** node)
{
    if (list == NULL || node == NULL || data == NULL) return Status_NullPtr;
    if (STATUS_CHECK(list->Status)) return STATUS_PASS_UP(list->Status);

    if (list->Head == NULL) return Status_BufferEmpty;

    LinkedListNode_t* ptr = list->Head;

    bool found = false;
    while (ptr)
    {         
        if (ptr->Size == nodeSize)
        {
            if (0 == memcmp(data, ptr->Data + offset, size))
            {
                found = true;
                *node = ptr;
                break;
            }
        }

        ptr = ptr->Next;
    }

    if (!found) return Status_NotFound;

    return Status_Ok;
}

Status_t LinkedList_GetNode(LinkedList_t* list, uint16_t iter, LinkedListNode_t** node)
{
    if (list == NULL || node == NULL) return Status_NullPtr;
    if (STATUS_CHECK(list->Status)) return STATUS_PASS_UP(list->Status);
    if (iter > list->Size) return Status_IndexOOR;

    uint16_t count = iter;
    LinkedListNode_t* ptr = list->Head;

    while (iter--)
    {
        ptr = ptr->Next;
    }

    *node = ptr;

    return Status_Ok;
}

Status_t LinkedList_NumNodes(LinkedList_t* list, uint16_t* numNodes)
{
    if (list == NULL || numNodes == NULL) return Status_NullPtr;
    if (STATUS_CHECK(list->Status)) return STATUS_PASS_UP(list->Status);

    *numNodes = list->Size;

    return Status_Ok;
}

Status_t LinkedList_StartIter(LinkedList_t* list, LinkedListIterator_t* node)
{
    if (list == NULL || node == NULL) return Status_NullPtr;
    if (STATUS_CHECK(list->Status)) return STATUS_PASS_UP(list->Status);

    node->Rev = list->Rev;
    node->Node = list->Head;

    if (node->Node == NULL) return Status_BufferEmpty;

    return Status_Ok;
}

Status_t LinkedList_Next(LinkedList_t* list, LinkedListIterator_t* node)
{
    if (list == NULL || node == NULL) return Status_NullPtr;
    if (STATUS_CHECK(list->Status)) return STATUS_PASS_UP(list->Status);
    if (list->Rev != node->Rev) return Status_InvalidState;

    node->Node = node->Node->Next;

    if (node->Node == NULL) return Status_EOF;

    return Status_Ok;
}