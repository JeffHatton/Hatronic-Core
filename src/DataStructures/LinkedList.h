/**
 * @file LinkedList.h
 * @author Jeff Hatton
 * @brief Singlely linked list
 * @date 2022-01-30
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef LinkedList_H
#define LinkedList_H

#include "Status.h"

// Exported Types ************************************

typedef struct LinkedListNode_s {
    uint16_t Size;
    void* Data;
    struct LinkedListNode_s* Next;
} LinkedListNode_t;

typedef struct {
    uint16_t Size;
    uint16_t Rev;
    LinkedListNode_t* Head;
    Status_t Status;    
} LinkedList_t;

typedef struct {
    uint16_t Rev;
    LinkedListNode_t* Node;
} LinkedListIterator_t;

// Exported Constants ************************************

// Exported Objects **************************************

// Exported Macros ***************************************

// Exported Functions ************************************

/**
 * @brief Init the linked list
 * 
 * @param list 
 * @return Status_t 
 */
Status_t LinkedList_Init(LinkedList_t* list);

/**
 * @brief Reset the linked list
 * 
 * @param list 
 * @return Status_t 
 */
Status_t LinkedList_Reset(LinkedList_t* list);

/**
 * @brief Append a node to the list. Will return an error if the node is already linked.
 * 
 * @param list The list to apppend too
 * @param node The node to append
 * @param data data for the node to append
 * @param size The size of the data to append
 * @return Status_t 
 */
Status_t LinkedList_Append(LinkedList_t* list, LinkedListNode_t* node, void* data, uint16_t size);

/**
 * @brief Remove a node from the list
 * 
 * @param list The list to remove from
 * @param node The node to remove
 * @return Status_t 
 */
Status_t LinkedList_RemoveNode(LinkedList_t* list, LinkedListNode_t* node);

/**
 * @brief Remove an element from the list by its index
 * 
 * @param list
 * @param iter The index to remove from 
 * @return Status_t 
 */
Status_t LinkedList_RemoveAt(LinkedList_t* list, uint16_t iter);

/**
 * @brief Remove a node from the the list by its data
 * 
 * @param list 
 * @param data The data to remove
 * @param size The size of the data to remove
 * @return Status_t 
 */
Status_t LinkedList_RemoveData(LinkedList_t* list, void* data, uint16_t size);

/**
 * @brief Get a node by its data.
 * 
 * @param list
 * @param data The data to search for 
 * @param size The size of the data
 * @param node Buffer for the found node
 * @return Status_t 
 */
Status_t LinkedList_FindByData(LinkedList_t* list, void* data, uint16_t size, LinkedListNode_t** node);

/**
 * @brief Get a node by its index
 * 
 * @param list 
 * @param iter The index
 * @param node Buffer for the node
 * @return Status_t 
 */
Status_t LinkedList_GetNode(LinkedList_t* list, uint16_t iter, LinkedListNode_t** node);

/**
 * @brief Get the number of nodes in the list
 * 
 * @param list 
 * @param numNodes Buffer for the node
 * @return Status_t 
 */
Status_t LinkedList_NumNodes(LinkedList_t* list, uint16_t* numNodes);

/**
 * @brief Start an iterator for the list
 * 
 * @param list 
 * @param node 
 * @return Status_t 
 */
Status_t LinkedList_StartIter(LinkedList_t* list, LinkedListIterator_t* node);

/**
 * @brief Move to the next node in the list. Must have called StartIter before this method.
 * NOTE: will throw an error if the list has changed since the StartIter call
 * 
 * @param list 
 * @param node 
 * @return Status_t 
 */
Status_t LinkedList_Next(LinkedList_t* list, LinkedListIterator_t* node);

//TODO foreach. iteration
//TODO find by member in struct

// END LinkedList_H
#endif