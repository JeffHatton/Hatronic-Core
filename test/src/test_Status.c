#ifdef TEST

#include "unity.h"

#include "Status.h"

void setUp(void)
{
}

void tearDown(void)
{
}

typedef struct {
    Status_t Statusx;
    uint32_t ExpectedResult;
} StatusTestCase_t;
    
void test_Status_GetStatus(void)
{
    StatusTestCase_t cases[] = {
        {0xFFFF0000, 0},
        {0xFFFF000A, 0xA},
        {0xF00FDEAD, 0xDEAD},
        {0xDEADBEEF, 0xBEEF},
        {0xF00FFFFF, 0xFFFF},
        {0xFFFFFFFF, 0xFFFF},
    };

    for (uint32_t i = 0; i < sizeof(cases) / sizeof(cases[0]); i++)
    {
        uint32_t statusMasked = STATUS_GET_STATUS(cases[i].Statusx);
        TEST_ASSERT_EQUAL_HEX8(cases[i].ExpectedResult, statusMasked);
    }
}

#endif // TEST
