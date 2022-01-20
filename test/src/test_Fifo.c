#ifdef TEST

#include "unity.h"

#include "Array.h"
#include "Fifo.h"
#include "StatusCodes.h"
#include "string.h"

void setUp(void)
{
    
}

void tearDown(void)
{
}


void test_Fifo_ByteWrap(void)
{
    uint8_t buf[13];
    uint8_t tempBuf[sizeof(buf) * 2];
    uint8_t tempBuf2[sizeof(buf) * 2];
    uint16_t size = sizeof(buf);

    Fifo_t fifo = {.Status = Status_NotInitialized};
    Status_t ret = Fifo_Init(&fifo, buf, size);
    TEST_ASSERT_EQUAL_HEX32(0, ret);

    ret = Fifo_SetAllowWrap(&fifo, true);
    TEST_ASSERT_EQUAL_HEX32(0, ret);

    // Fill temp buf for test
    for (int i = 0; i < size * 2; i++) tempBuf[i] = i;

    // Fill Buffer
    ret = Fifo_Put(&fifo, tempBuf, size - 1);
    TEST_ASSERT_EQUAL_HEX32(0, ret);

    ret = Fifo_Put(&fifo, tempBuf + (size - 1), (size - 1) / 2);
    TEST_ASSERT_EQUAL_HEX32(0, ret);

    uint16_t used;
    ret = Fifo_GetUsed(&fifo, &used);
    TEST_ASSERT_EQUAL_HEX16((size -1), used);

    ret = Fifo_Get(&fifo, (size - 1), tempBuf2);
    TEST_ASSERT_EQUAL_HEX32(0, ret);

    TEST_ASSERT_EQUAL_HEX8_ARRAY(tempBuf + ((size - 1) / 2), tempBuf2, size -1);
}

void test_Fifo_Stress(void)
{
    TEST_FAIL();
}

void test_Fifo_ByteFill(void)
{
    uint8_t buf[24];
    uint8_t tempBuf[24];
    uint8_t tempBuf2[24];
    uint16_t size = sizeof(buf);

    Fifo_t fifo = {.Status = Status_NotInitialized};
    Status_t ret = Fifo_Init(&fifo, buf, size);
    TEST_ASSERT_EQUAL_HEX32(0, ret);

    bool empty;
    ret = Fifo_IsEmpty(&fifo, &empty);
    TEST_ASSERT_EQUAL_HEX32(0, ret);
    TEST_ASSERT_TRUE(empty);

    // Fill temp buf for test
    for (int i = 0; i < size; i++) tempBuf[i] = i;
    
    //
    // Fill Buffer. Checking proper empty/full flags
    //

    ret = Fifo_Put(&fifo, tempBuf, size - 1);
    TEST_ASSERT_EQUAL_HEX32(0, ret);

    ret = Fifo_IsEmpty(&fifo, &empty);
    TEST_ASSERT_EQUAL_HEX32(0, ret);
    TEST_ASSERT_FALSE(empty);

    ret = Fifo_IsFull(&fifo, &empty);
    TEST_ASSERT_EQUAL_HEX32(0, ret);
    TEST_ASSERT_TRUE(empty);

    // Attempt to put a byte into the buffer
    ret = Fifo_Put(&fifo, tempBuf, 1);
    TEST_ASSERT_EQUAL_HEX32(Status_BufferFull, STATUS_GET_CODE(ret));

    //
    // Clear the whole buffer and verify it has been cleared
    //

    ret = Fifo_ResetByConsumer(&fifo);
    TEST_ASSERT_EQUAL_HEX32(0, ret);

    ret = Fifo_IsEmpty(&fifo, &empty);
    TEST_ASSERT_EQUAL_HEX32(0, ret);
    TEST_ASSERT_TRUE(empty);

    //
    // Setup and clear again to test clear by producer
    //

    ret = Fifo_Put(&fifo, tempBuf, size - 1);
    TEST_ASSERT_EQUAL_HEX32(0, ret);

    ret = Fifo_ResetByProducer(&fifo);
    TEST_ASSERT_EQUAL_HEX32(0, ret);

    ret = Fifo_IsEmpty(&fifo, &empty);
    TEST_ASSERT_EQUAL_HEX32(0, ret);
    TEST_ASSERT_TRUE(empty);    
}

void test_Fifo_ByteMode(void)
{
    uint8_t buf[24];
    uint8_t tempBuf[24];
    uint8_t tempBuf2[24];
    uint16_t size = sizeof(buf);

    Fifo_t fifo = {.Status = Status_NotInitialized};
    Status_t ret = Fifo_Init(&fifo, buf, size);
    TEST_ASSERT_EQUAL_HEX32(0, ret);

    // Fill temp buf for test
    for (int i = 0; i < size; i++) tempBuf[i] = i;

    // Test that when the buffer rolls over 
    // indices there are no errors
    for (int i = 0; i < (size / 8) * 3; i++)
    {
        uint16_t startIdx = (i * 8) % size;
        ret = Fifo_Put(&fifo, tempBuf + startIdx, 8);
        if (STATUS_CHECK(ret)) TEST_FAIL();

        ret = Fifo_Get(&fifo, 8, tempBuf2);
        if (STATUS_CHECK(ret)) TEST_FAIL();

        TEST_ASSERT_EQUAL_HEX8_ARRAY(tempBuf + startIdx, tempBuf2, 8);
    }
}

void test_Fifo_ObjectFill(void)
{
    struct TestCaseObj_t {
        uint8_t V0;
        uint16_t V1;
        uint8_t V2;
        uint32_t V3;
    };

    struct TestCaseObj_t TestCases[4];

    for (size_t i = 0; i < ARRAY_SIZE(TestCases); i++)
    {
        TestCases[i].V0 = 0 + (i * 4);
        TestCases[i].V1 = 1 + (i * 4);
        TestCases[i].V2 = 2 + (i * 4);
        TestCases[i].V3 = 3 + (i * 4);
    }
    
    uint8_t buf[sizeof(TestCases) + ARRAY_SIZE(TestCases) * 2 + 1];

    Fifo_t fifo = {.Status = Status_NotInitialized};
    Status_t ret = Fifo_Init(&fifo, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_HEX32(0, ret);

    bool empty;
    ret = Fifo_IsEmpty(&fifo, &empty);
    TEST_ASSERT_EQUAL_HEX32(0, ret);
    TEST_ASSERT_TRUE(empty);
    
    //
    // Fill Buffer. Checking proper empty/full flags
    //

    for (size_t i = 0; i < ARRAY_SIZE(TestCases); i++)
    {
        ret = Fifo_PutObj(&fifo, (uint8_t*)&TestCases[i], sizeof(struct TestCaseObj_t));
        TEST_ASSERT_EQUAL_HEX32(0, ret);
    }

    ret = Fifo_IsEmpty(&fifo, &empty);
    TEST_ASSERT_EQUAL_HEX32(0, ret);
    TEST_ASSERT_FALSE(empty);

    ret = Fifo_IsFull(&fifo, &empty);
    TEST_ASSERT_EQUAL_HEX32(0, ret);
    TEST_ASSERT_TRUE(empty);

    // Attempt to put an object into the buffer
    ret = Fifo_PutObj(&fifo, (uint8_t*)&TestCases[0], sizeof(struct TestCaseObj_t));
    TEST_ASSERT_EQUAL_HEX32(Status_BufferFull, STATUS_GET_CODE(ret));

    //
    // Clear the whole buffer and verify it has been cleared
    //

    ret = Fifo_ResetByConsumer(&fifo);
    TEST_ASSERT_EQUAL_HEX32(0, ret);

    ret = Fifo_IsEmpty(&fifo, &empty);
    TEST_ASSERT_EQUAL_HEX32(0, ret);
    TEST_ASSERT_TRUE(empty);

    //
    // Setup and clear again to test clear by producer
    //

    for (size_t i = 0; i < ARRAY_SIZE(TestCases); i++)
    {
        ret = Fifo_PutObj(&fifo, (uint8_t*)&TestCases[i], sizeof(struct TestCaseObj_t));
        TEST_ASSERT_EQUAL_HEX32(0, ret);
    }

    ret = Fifo_ResetByProducer(&fifo);
    TEST_ASSERT_EQUAL_HEX32(0, ret);

    ret = Fifo_IsEmpty(&fifo, &empty);
    TEST_ASSERT_EQUAL_HEX32(0, ret);
    TEST_ASSERT_TRUE(empty);
}

void test_Fifo_WrapObject(void)
{
    struct TestCaseObj_t {
        uint8_t V0;
        uint16_t V1;
        uint8_t V2;
        uint32_t V3;
    };

    struct TestCaseObj_t TestCases[4];

    for (size_t i = 0; i < ARRAY_SIZE(TestCases); i++)
    {
        TestCases[i].V0 = 0 + (i * 4);
        TestCases[i].V1 = 1 + (i * 4);
        TestCases[i].V2 = 2 + (i * 4);
        TestCases[i].V3 = 3 + (i * 4);
    }
    
    uint8_t buf[sizeof(TestCases) + ARRAY_SIZE(TestCases) * 2 + 1];

    Fifo_t fifo = {.Status = Status_NotInitialized};
    Status_t ret = Fifo_Init(&fifo, buf, sizeof(buf) - (2 + sizeof(struct TestCaseObj_t)));
    TEST_ASSERT_EQUAL_HEX32(0, ret);

    ret = Fifo_SetAllowWrap(&fifo, true);
    TEST_ASSERT_EQUAL_HEX32(0, ret);
    
    for (size_t i = 0; i < ARRAY_SIZE(TestCases); i++)
    {
        ret = Fifo_PutObj(&fifo, (uint8_t*)&TestCases[i], sizeof(struct TestCaseObj_t));
        TEST_ASSERT_EQUAL_HEX32(0, ret);
    }

    uint16_t bufferSize = sizeof(struct TestCaseObj_t);
    for (size_t i = 1; i < ARRAY_SIZE(TestCases); i++)
    {
        struct TestCaseObj_t obj;
        ret = Fifo_GetObj(&fifo, &bufferSize, (uint8_t*)&obj);
        TEST_ASSERT_EQUAL_HEX32(0, ret);

        TEST_ASSERT_EQUAL_INT(obj.V0, TestCases[i].V0);
        TEST_ASSERT_EQUAL_INT(obj.V1, TestCases[i].V1);
        TEST_ASSERT_EQUAL_INT(obj.V2, TestCases[i].V2);
        TEST_ASSERT_EQUAL_INT(obj.V3, TestCases[i].V3);
    }
}

void test_Fifo_ObjectMode(void)
{
    struct TestCaseObj_t {
        uint8_t V0;
        uint16_t V1;
        uint8_t V2;
        uint32_t V3;
    };

    struct TestCaseObj_t TestCases[4];

    for (size_t i = 0; i < ARRAY_SIZE(TestCases); i++)
    {
        TestCases[i].V0 = 0 + (i * 4);
        TestCases[i].V1 = 1 + (i * 4);
        TestCases[i].V2 = 2 + (i * 4);
        TestCases[i].V3 = 3 + (i * 4);
    }
    
    uint8_t buf[sizeof(TestCases) + ARRAY_SIZE(TestCases) * 2 + 1];

    Fifo_t fifo = {.Status = Status_NotInitialized};
    Status_t ret = Fifo_Init(&fifo, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_HEX32(0, ret);

    for (size_t i = 0; i < ARRAY_SIZE(TestCases); i++)
    {
        ret = Fifo_PutObj(&fifo, (uint8_t*)&TestCases[i], sizeof(struct TestCaseObj_t));
        TEST_ASSERT_EQUAL_HEX32(0, ret);
    }

    bool full;
    ret = Fifo_IsFull(&fifo, &full);
    TEST_ASSERT_EQUAL_HEX32(0, ret);
    TEST_ASSERT_TRUE(full);

    // Test if the buffer is too small that the correct errors are returned
    struct TestCaseObj_t obj;
    uint16_t bufferSize = 2;
    ret = Fifo_GetObj(&fifo, &bufferSize, (uint8_t*)&obj);
    TEST_ASSERT_EQUAL_HEX32(Status_BufferTooSmall, STATUS_GET_CODE(ret));
    TEST_ASSERT_EQUAL_INT(sizeof(struct TestCaseObj_t), bufferSize);

    // Check to make sure the buffer is still full
    ret = Fifo_IsFull(&fifo, &full);
    TEST_ASSERT_EQUAL_HEX32(0, ret);
    TEST_ASSERT_TRUE(full);

    for (size_t i = 0; i < ARRAY_SIZE(TestCases); i++)
    {
        ret = Fifo_GetObj(&fifo, &bufferSize, (uint8_t*)&obj);
        TEST_ASSERT_EQUAL_HEX32(0, ret);

        TEST_ASSERT_EQUAL_INT(obj.V0, 0 + (i * 4));
        TEST_ASSERT_EQUAL_INT(obj.V1, 1 + (i * 4));
        TEST_ASSERT_EQUAL_INT(obj.V2, 2 + (i * 4));
        TEST_ASSERT_EQUAL_INT(obj.V3, 3 + (i * 4));
    }
}

void test_Fifo_IncorrectStateErrors(void)
{
        struct TestCaseObj_t {
        uint8_t V0;
        uint16_t V1;
        uint8_t V2;
        uint32_t V3;
    };

    struct TestCaseObj_t TestCases[4];

    for (size_t i = 0; i < ARRAY_SIZE(TestCases); i++)
    {
        TestCases[i].V0 = 0 + (i * 4);
        TestCases[i].V1 = 1 + (i * 4);
        TestCases[i].V2 = 2 + (i * 4);
        TestCases[i].V3 = 3 + (i * 4);
    }
    
    uint8_t buf[sizeof(TestCases) + ARRAY_SIZE(TestCases) * 2 + 1];

    Fifo_t fifo = {.Status = Status_NotInitialized};
    Status_t ret = Fifo_Init(&fifo, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_HEX32(0, ret);

    ret = Fifo_Put(&fifo, (uint8_t*)&ret, 1);
    TEST_ASSERT_EQUAL_HEX32(0, ret);

    ret = Fifo_PutObj(&fifo, (uint8_t*)&TestCases[0], sizeof(struct TestCaseObj_t));
    TEST_ASSERT_EQUAL_HEX32(Status_InvalidState, STATUS_GET_CODE(ret));
    
    Fifo_t fifo2 = {.Status = Status_NotInitialized};
    ret = Fifo_Init(&fifo2, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_HEX32(0, ret);

    ret = Fifo_PutObj(&fifo2, (uint8_t*)&TestCases[0], sizeof(struct TestCaseObj_t));    
    TEST_ASSERT_EQUAL_HEX32(0, ret);

    ret = Fifo_Put(&fifo2, (uint8_t*)&ret, 1);
    TEST_ASSERT_EQUAL_HEX32(Status_InvalidState, STATUS_GET_CODE(ret));
}

void test_Fifo_PeekByte(void)
{
    uint8_t buf[25];
    uint8_t tempBuf[25];
    uint8_t tempBuf2[25];
    uint16_t size = sizeof(buf);

    Fifo_t fifo = {.Status = Status_NotInitialized};
    Status_t ret = Fifo_Init(&fifo, buf, size);
    TEST_ASSERT_EQUAL_HEX32(0, ret);

    // Fill temp buf for test
    for (int i = 0; i < size; i++) tempBuf[i] = i;

    //
    // Fill buff and then peak entire buf
    //
    ret = Fifo_Put(&fifo, tempBuf, size - 1);
    TEST_ASSERT_EQUAL_HEX32(0, ret);

    ret = Fifo_Peek(&fifo, 0, size - 1, tempBuf2);
    TEST_ASSERT_EQUAL_HEX32(0, ret);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(tempBuf, tempBuf2, size - 1);

    bool full;
    ret = Fifo_IsFull(&fifo, &full);
    TEST_ASSERT_EQUAL_HEX32(0, ret);
    TEST_ASSERT_TRUE(full);

    //
    // Have Head wrap around the end of the buff and test again 
    //

    ret = Fifo_Pop(&fifo, (size - 1) / 2);
    TEST_ASSERT_EQUAL_HEX32(0, ret);

    ret = Fifo_Put(&fifo, tempBuf, (size - 1) / 2);
    TEST_ASSERT_EQUAL_HEX32(0, ret);

    ret = Fifo_Peek(&fifo, 0, size - 1, tempBuf2);
    TEST_ASSERT_EQUAL_HEX32(0, ret);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(tempBuf + (size - 1) / 2, tempBuf2, (size - 1) / 2);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(tempBuf, tempBuf2 + (size - 1) / 2, (size - 1) / 2);

    // Check offset
    ret = Fifo_Peek(&fifo, (size - 1) / 2, (size - 1) / 2, tempBuf2);
    TEST_ASSERT_EQUAL_HEX32(0, ret);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(tempBuf, tempBuf2, (size - 1) / 2);
}

void test_Fifo_PeekObj(void)
{
    struct TestCaseObj_t {
        uint8_t V0;
        uint8_t V1;
        uint16_t V2;
        uint32_t V3;
    };

    struct TestCaseObj_t TestCases[4];
    struct TestCaseObj_t obj;

    for (size_t i = 0; i < ARRAY_SIZE(TestCases); i++)
    {
        TestCases[i].V0 = 0 + (i * 4);
        TestCases[i].V1 = 1 + (i * 4);
        TestCases[i].V2 = 2 + (i * 4);
        TestCases[i].V3 = 3 + (i * 4);
    }
    
    uint8_t buf[sizeof(TestCases) + ARRAY_SIZE(TestCases) * 2 + 1];

    Fifo_t fifo = {.Status = Status_NotInitialized};
    Status_t ret = Fifo_Init(&fifo, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_HEX32(0, ret);

    //
    // Fill buff and then peak entire buf
    //

    for (size_t i = 0; i < ARRAY_SIZE(TestCases); i++)
    {
        ret = Fifo_PutObj(&fifo, (uint8_t*)&TestCases[i], sizeof(struct TestCaseObj_t));
        TEST_ASSERT_EQUAL_HEX32(0, ret);
    }

    for (size_t i = 0; i < ARRAY_SIZE(TestCases); i++)
    {
        uint16_t size = sizeof(obj);
        ret = Fifo_PeekObj(&fifo, i, &size, (uint8_t*)&obj);
        TEST_ASSERT_EQUAL_HEX32(0, ret);
        TEST_ASSERT_EQUAL_MEMORY(&TestCases[i], &obj, sizeof(obj));
    }

    bool full;
    ret = Fifo_IsFull(&fifo, &full);
    TEST_ASSERT_EQUAL_HEX32(0, ret);
    TEST_ASSERT_TRUE(full);

    //
    // Have Head wrap around the end of the buff and test again 
    //

    ret = Fifo_Pop(&fifo, 2);
    TEST_ASSERT_EQUAL_HEX32(0, ret);

    for (size_t i = 0; i < ARRAY_SIZE(TestCases) / 2; i++)
    {
        ret = Fifo_PutObj(&fifo, (uint8_t*)&TestCases[i], sizeof(struct TestCaseObj_t));
        TEST_ASSERT_EQUAL_HEX32(0, ret);
    }

    ret = Fifo_IsFull(&fifo, &full);
    TEST_ASSERT_EQUAL_HEX32(0, ret);
    TEST_ASSERT_TRUE(full);

    for (size_t i = 0; i < ARRAY_SIZE(TestCases); i++)
    {
        uint16_t size = sizeof(obj);
        ret = Fifo_PeekObj(&fifo, i, &size, (uint8_t*)&obj);
        TEST_ASSERT_EQUAL_HEX32(0, ret);
        TEST_ASSERT_EQUAL_MEMORY(&TestCases[(i + 2) % ARRAY_SIZE(TestCases)], &obj, sizeof(obj));
    }
}

void test_Fifo_GetNumObjs(void)
{
    struct TestCaseObj_t {
        uint8_t V0;
        uint16_t V1;
        uint8_t V2;
        uint32_t V3;
    };

    struct TestCaseObj_t TestCases[4];

    for (size_t i = 0; i < ARRAY_SIZE(TestCases); i++)
    {
        TestCases[i].V0 = 0 + (i * 4);
        TestCases[i].V1 = 1 + (i * 4);
        TestCases[i].V2 = 2 + (i * 4);
        TestCases[i].V3 = 3 + (i * 4);
    }
    
    uint8_t buf[sizeof(TestCases) + ARRAY_SIZE(TestCases) * 2 + 1];

    Fifo_t fifo = {.Status = Status_NotInitialized};
    Status_t ret = Fifo_Init(&fifo, buf, sizeof(buf));
    TEST_ASSERT_EQUAL_HEX32(0, ret);

    for (size_t i = 0; i < ARRAY_SIZE(TestCases); i++)
    {
        ret = Fifo_PutObj(&fifo, (uint8_t*)&TestCases[i], sizeof(struct TestCaseObj_t));
        TEST_ASSERT_EQUAL_HEX32(0, ret);
    }

    uint16_t count;
    ret = Fifo_GetNumObjs(&fifo, &count);
    TEST_ASSERT_EQUAL_HEX32(0, ret);
    TEST_ASSERT_EQUAL_INT(ARRAY_SIZE(TestCases), count);

    ret = Fifo_Pop(&fifo, 2);
    TEST_ASSERT_EQUAL_HEX32(0, ret);

    ret = Fifo_GetNumObjs(&fifo, &count);
    TEST_ASSERT_EQUAL_HEX32(0, ret);
    TEST_ASSERT_EQUAL_INT((ARRAY_SIZE(TestCases) - 2), count);

    //
    // Refill buffer then pop elements 1 at a time checking count each time
    //

    for (size_t i = 0; i < 2; i++)
    {
        ret = Fifo_PutObj(&fifo, (uint8_t*)&TestCases[i], sizeof(struct TestCaseObj_t));
        TEST_ASSERT_EQUAL_HEX32(0, ret);
    }

    for (size_t i = 0; i < ARRAY_SIZE(TestCases); i++)
    {
        ret = Fifo_Pop(&fifo, 1);
        TEST_ASSERT_EQUAL_HEX32(0, ret);

        ret = Fifo_GetNumObjs(&fifo, &count);
        TEST_ASSERT_EQUAL_HEX32(0, ret);
        TEST_ASSERT_EQUAL_INT((ARRAY_SIZE(TestCases) - 1) - i, count);
    }
}

//TODO test error cases

#endif // TEST
