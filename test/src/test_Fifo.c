#ifdef TEST

#include "unity.h"

#include "Fifo.h"
#include "StatusCodes.h"
#include "string.h"

void setUp(void)
{
    
}

void tearDown(void)
{
}


void test_Fifo_Wrap(void)
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

void test_Fifo_Fill(void)
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

        // memset(tempBuf2, 0, size);
        ret = Fifo_Get(&fifo, 8, tempBuf2);
        if (STATUS_CHECK(ret)) TEST_FAIL();

        TEST_ASSERT_EQUAL_HEX8_ARRAY(tempBuf + startIdx, tempBuf2, 8);
    }
}

void test_Fifo_ObjectMode(void)
{
    TEST_FAIL();
}

void test_Fifo_Peek(void)
{
    TEST_FAIL();
}

#endif // TEST
