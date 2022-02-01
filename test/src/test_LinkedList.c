#ifdef TEST

#include "unity.h"

#include "Array.h"
#include "LinkedList.h"
#include "string.h"

struct Data_t {
    void* Data;
    uint16_t Size;
};

struct TestS_t {
    uint16_t value0;
    uint16_t value1;
    uint16_t value2;
    uint16_t value3;
};

LinkedListNode_t nodes[4];
struct Data_t data[4];

uint16_t size;

uint8_t value0 = 1;
struct TestS_t value1 = {2, 3, 5, 6};
struct TestS_t value2 = {7, 8, 9, 10};
uint32_t value3 = 6;

void setUp(void)
{
    data[0].Data = &value0;
    data[0].Size = sizeof(value0);
    data[1].Data = &value1;
    data[1].Size = sizeof(value1);
    data[2].Data = &value2;
    data[2].Size = sizeof(value2);
    data[3].Data = &value3;
    data[3].Size = sizeof(value3);
}

void tearDown(void)
{
}

void test_LinkedList_Basic(void)
{
    LinkedList_t list = {.Status = Status_NotInitialized};

    Status_t ret = LinkedList_Init(&list);
    TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);

    // Populate list and check count as it goes
    for (size_t i = 0; i < ARRAY_SIZE(nodes); i++)
    {
        ret = LinkedList_Append(&list, nodes + i, data[i].Data, data[i].Size);
        TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);

        ret = LinkedList_NumNodes(&list, &size);
        TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);
        TEST_ASSERT_EQUAL_INT(i + 1, size);
    }
    
    ret = LinkedList_Append(&list, nodes, data[0].Data, data[0].Size);
    TEST_ASSERT_EQUAL_HEX32(Status_AlreadyInBuffer, ret);

    // Remove 2 nodes and check count
    ret = LinkedList_RemoveData(&list, &value1, sizeof(value1));
    TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);
    ret = LinkedList_RemoveNode(&list, nodes + 2);
    TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);
        
    ret = LinkedList_NumNodes(&list, &size);
    TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);
    TEST_ASSERT_EQUAL_INT(ARRAY_SIZE(nodes) - 2, size);

    // Fill the list in a different order
    ret = LinkedList_Append(&list, nodes + 2, data[2].Data, data[2].Size);
    TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);
    
    ret = LinkedList_Append(&list, nodes + 1, data[1].Data, data[1].Size);
    TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);

    //
    // Get each node and verify order and values are correct
    //

    uint32_t testValue;
    struct TestS_t testStruct;
    LinkedListNode_t* testNode;
    ret = LinkedList_GetNode(&list, 0, &testNode);
    TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);
    TEST_ASSERT_EQUAL_INT(sizeof(value0), testNode->Size);
    testValue = *(uint8_t*)testNode->Data;
    TEST_ASSERT_EQUAL_INT(value0, testValue);

    ret = LinkedList_GetNode(&list, 1, &testNode);
    TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);
    TEST_ASSERT_EQUAL_INT(sizeof(value3), testNode->Size);
    testValue = *(uint32_t*)testNode->Data;
    TEST_ASSERT_EQUAL_INT(value3, testValue);

    ret = LinkedList_GetNode(&list, 2, &testNode);
    TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);
    TEST_ASSERT_EQUAL_INT(sizeof(struct TestS_t), testNode->Size);
    testStruct = *(struct TestS_t*)testNode->Data;
    TEST_ASSERT_EQUAL_MEMORY(&value2, &testStruct, sizeof(struct TestS_t));

    ret = LinkedList_GetNode(&list, 3, &testNode);
    TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);
    TEST_ASSERT_EQUAL_INT(sizeof(struct TestS_t), testNode->Size);
    testStruct = *(struct TestS_t*)testNode->Data;
    TEST_ASSERT_EQUAL_MEMORY(&value1, &testStruct, sizeof(struct TestS_t));

    // Clear list and verify it worked
    ret = LinkedList_Reset(&list);
    TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);

    ret = LinkedList_NumNodes(&list, &size);
    TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);
    TEST_ASSERT_EQUAL_INT(0, size);
    TEST_ASSERT_EQUAL_PTR(NULL, list.Head);
}

void test_LinkedList_GetNode(void)
{
    LinkedList_t list = {.Status = Status_NotInitialized};

    Status_t ret = LinkedList_Init(&list);
    TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);

    // Populate list and check count as it goes
    for (size_t i = 0; i < ARRAY_SIZE(nodes); i++)
    {
        ret = LinkedList_Append(&list, nodes + i, data[i].Data, data[i].Size);
        TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);

        ret = LinkedList_NumNodes(&list, &size);
        TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);
        TEST_ASSERT_EQUAL_INT(i + 1, size);
    }

    LinkedListNode_t* node;
    ret = LinkedList_FindByMember(&list,
                                  sizeof(value1), 
                                  offsetof(struct TestS_t, value1),
                                  &value1.value1,
                                  sizeof(value1.value1),
                                  &node);
    TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);
    TEST_ASSERT_EQUAL_INT(sizeof(value1), node->Size);
    TEST_ASSERT_EQUAL_MEMORY(&value1, node->Data, node->Size);

    ret = LinkedList_FindByMember(&list,
                                  sizeof(value1), 
                                  offsetof(struct TestS_t, value2),
                                  &value1.value2,
                                  sizeof(value1.value2),
                                  &node);
    TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);
    TEST_ASSERT_EQUAL_INT(sizeof(value1), node->Size);
    TEST_ASSERT_EQUAL_MEMORY(&value1, node->Data, node->Size);

    uint16_t x = 100;
    ret = LinkedList_FindByMember(&list,
                                  sizeof(value1), 
                                  offsetof(struct TestS_t, value1),
                                  &x,
                                  sizeof(value1.value2),
                                  &node);
    TEST_ASSERT_EQUAL_HEX32(Status_NotFound, STATUS_GET_CODE(ret)); 
}

void test_LinkedList_Iter(void)
{
    LinkedList_t list = {.Status = Status_NotInitialized};
    
    Status_t ret = LinkedList_Init(&list);
    TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);

    LinkedListIterator_t iter;
    ret = LinkedList_StartIter(&list, &iter);
    TEST_ASSERT_EQUAL_HEX32(Status_BufferEmpty, STATUS_GET_CODE(ret));

    // Populate list and check count as it goes
    for (size_t i = 0; i < ARRAY_SIZE(nodes); i++)
    {
        ret = LinkedList_Append(&list, nodes + i, data[i].Data, data[i].Size);
        TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);
    }

    ret = LinkedList_StartIter(&list, &iter);
    TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);

    uint32_t testValue;
    struct TestS_t testStruct;
    TEST_ASSERT_EQUAL_INT(sizeof(value0), iter.Node->Size);
    testValue = *(uint8_t*)iter.Node->Data;
    TEST_ASSERT_EQUAL_INT(value0, testValue);

    ret = LinkedList_Next(&list, &iter);
    TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);
    TEST_ASSERT_EQUAL_INT(sizeof(struct TestS_t), iter.Node->Size);
    testStruct = *(struct TestS_t*)iter.Node->Data;
    TEST_ASSERT_EQUAL_MEMORY(&value1, &testStruct, sizeof(struct TestS_t));

    ret = LinkedList_Next(&list, &iter);
    TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);
    TEST_ASSERT_EQUAL_INT(sizeof(struct TestS_t), iter.Node->Size);
    testStruct = *(struct TestS_t*)iter.Node->Data;
    TEST_ASSERT_EQUAL_MEMORY(&value2, &testStruct, sizeof(struct TestS_t));

    ret = LinkedList_Next(&list, &iter);
    TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);
    TEST_ASSERT_EQUAL_INT(sizeof(value3), iter.Node->Size);
    testValue = *(uint32_t*)iter.Node->Data;
    TEST_ASSERT_EQUAL_INT(value3, testValue);

    ret = LinkedList_Next(&list, &iter);
    TEST_ASSERT_EQUAL_HEX32(Status_EOF, STATUS_GET_CODE(ret));

    //
    // test error if list changes
    //

    ret = LinkedList_StartIter(&list, &iter);
    TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);

    ret = LinkedList_RemoveAt(&list, 1);
    TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);

    ret = LinkedList_Next(&list, &iter);
    TEST_ASSERT_EQUAL_HEX32(Status_InvalidState, STATUS_GET_CODE(ret));
}

void test_LinkedList_Removal(void)
{
    LinkedList_t list = {.Status = Status_NotInitialized};

    Status_t ret = LinkedList_Init(&list);
    TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);

    uint16_t size;

    // Populate list and check count as it goes
    for (size_t i = 0; i < ARRAY_SIZE(nodes); i++)
    {
        ret = LinkedList_Append(&list, nodes + i, data[i].Data, data[i].Size);
        TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);
    }

    ret = LinkedList_RemoveAt(&list, ARRAY_SIZE(nodes) + 1);
    TEST_ASSERT_EQUAL_HEX32(Status_IndexOOR, STATUS_GET_CODE(ret));

    for (size_t i = 0; i < ARRAY_SIZE(nodes); i++)
    {
        ret = LinkedList_RemoveAt(&list, 0);
        TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);

        ret = LinkedList_NumNodes(&list, &size);
        TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);
        TEST_ASSERT_EQUAL_INT(ARRAY_SIZE(nodes) - (i + 1), size);
    }

    ret = LinkedList_RemoveAt(&list, 0);
    TEST_ASSERT_EQUAL_HEX32(Status_BufferEmpty, STATUS_GET_CODE(ret));

    for (size_t i = 0; i < ARRAY_SIZE(nodes); i++)
    {
        ret = LinkedList_Append(&list, nodes + i, data[i].Data, data[i].Size);
        TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);
    }

    LinkedListNode_t node;
    ret = LinkedList_RemoveNode(&list, &node);
    TEST_ASSERT_EQUAL_HEX32(Status_NotFound, STATUS_GET_CODE(ret));

    for (size_t i = 0; i < ARRAY_SIZE(nodes); i++)
    {
        ret = LinkedList_RemoveNode(&list, nodes + i);
        TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);

        ret = LinkedList_NumNodes(&list, &size);
        TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);
        TEST_ASSERT_EQUAL_INT(ARRAY_SIZE(nodes) - (i + 1), size);
    }

    ret = LinkedList_RemoveNode(&list, nodes);
    TEST_ASSERT_EQUAL_HEX32(Status_BufferEmpty, STATUS_GET_CODE(ret));

    for (size_t i = 0; i < ARRAY_SIZE(nodes); i++)
    {
        ret = LinkedList_Append(&list, nodes + i, data[i].Data, data[i].Size);
        TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);
    }

    uint32_t x = 0xFFFF;
    ret = LinkedList_RemoveData(&list, &x, sizeof(x));
    TEST_ASSERT_EQUAL_HEX32(Status_NotFound, STATUS_GET_CODE(ret));

    for (size_t i = 0; i < ARRAY_SIZE(nodes); i++)
    {
        ret = LinkedList_RemoveData(&list, data[i].Data, data[i].Size);
        TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);

        ret = LinkedList_NumNodes(&list, &size);
        TEST_ASSERT_EQUAL_HEX32(Status_Ok, ret);
        TEST_ASSERT_EQUAL_INT(ARRAY_SIZE(nodes) - (i + 1), size);
    }

    ret = LinkedList_RemoveData(&list, nodes[0].Data, nodes[0].Size);
    TEST_ASSERT_EQUAL_HEX32(Status_BufferEmpty, STATUS_GET_CODE(ret));
}

//TODO tests
/*
    Error cases

    add
        Add already linked node
        
*/


#endif // TEST