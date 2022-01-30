#ifdef TEST

#include "stdbool.h"

#include "unity.h"

#include "Status.h"
#include "Array.h"

//TODO documentation

static uint16_t SuId;

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
        {0xF00FDEAD, 0xEAD},
        {0xDEADBEEF, 0xEEF},
        {0xF00FFFFF, 0xFFF},
        {0xFFFFFFFF, 0xFFF},
    };

    for (uint32_t i = 0; i < ARRAY_SIZE(cases); i++)
    {
        uint32_t statusMasked = STATUS_GET_CODE(cases[i].Statusx);
        TEST_ASSERT_EQUAL_HEX16(cases[i].ExpectedResult, statusMasked);
    }
}

typedef struct {
    Status_t Statusx;
    uint32_t ValueSET;
    uint32_t ExpectedResult;
} StatusSETTestCase_t;

typedef struct {
    Status_t Statusx;
    uint32_t Value0;
    uint32_t Value1;
    uint32_t ExpectedResult;
} Status2ValueTestCase_t;


void test_STATUS_SET_META(void)
{
    StatusSETTestCase_t cases[] = {
        {0xFFFFFFFF, 0x00, 0x00FFFFFF},
        {0x00000000, 0xFF, 0xFF000000},
        {0xFFFFFFFF, 0x0F, 0x0FFFFFFF},
        {0xFFFFFFFF, 0xF0, 0xF0FFFFFF},
        {0x00FFFFFF, 0x10, 0x10FFFFFF},
    };

    for (uint32_t i = 0; i < ARRAY_SIZE(cases); i++)
    {
        uint32_t statusMasked = STATUS_SET_META(cases[i].Statusx, cases[i].ValueSET);
        TEST_ASSERT_EQUAL_HEX32(cases[i].ExpectedResult, statusMasked);
    }   
}

void test_STATUS_SET_ID(void)
{
    StatusSETTestCase_t cases[] = {
        {0xFFFFFFFF, 0x000, 0xFF000FFF},
        {0x00000000, 0xFFF, 0x00FFF000},
        {0x00000000, 0x00F, 0x0000F000},
        {0x00000000, 0x0F0, 0x000F0000},
        {0x00000000, 0xF00, 0x00F00000},
    };

    for (uint32_t i = 0; i < ARRAY_SIZE(cases); i++)
    {
        uint32_t statusMasked = STATUS_SET_ID(cases[i].Statusx, cases[i].ValueSET);
        TEST_ASSERT_EQUAL_HEX32(cases[i].ExpectedResult, statusMasked);
    }   
}

void test_STATUS_SET_CODE(void)
{
    StatusSETTestCase_t cases[] = {
        {0xFFFFFFFF, 0x000, 0xFFFFF000},
        {0x00000000, 0xFFF, 0x00000FFF},
        {0x00000000, 0x00F, 0x0000000F},
        {0x00000000, 0x0F0, 0x000000F0},
        {0x00000000, 0xF00, 0x00000F00},
    };

    for (uint32_t i = 0; i < ARRAY_SIZE(cases); i++)
    {
        uint32_t statusMasked = STATUS_SET_CODE(cases[i].Statusx, cases[i].ValueSET);
        TEST_ASSERT_EQUAL_HEX32(cases[i].ExpectedResult, statusMasked);
    }   
}

void test_STATUS_PACK(void)
{
    StatusSETTestCase_t cases[] = {
        {0xFFFFFFFF, 0x000, 0xFF000FFF},
        {0x00000000, 0xFFF, 0x00FFF000},
        {0x00000000, 0x00F, 0x0000F000},
        {0x00000000, 0x0F0, 0x000F0000},
        {0x00000000, 0xF00, 0x00F00000},
    };

    for (uint32_t i = 0; i < ARRAY_SIZE(cases); i++)
    {
        SuId = cases[i].ValueSET;
        uint32_t statusMasked = STATUS_PACK(cases[i].Statusx);
        TEST_ASSERT_EQUAL_HEX32(cases[i].ExpectedResult, statusMasked);
    }   
}

void test_STATUS_PACK_META(void)
{
    Status2ValueTestCase_t cases[] = {
        {0xFFFFFFFF, 0x000, 0x00, 0x00000FFF},
        {0x00000000, 0xFFF, 0xFF, 0xFFFFF000},
        {0x00000000, 0x00F, 0x0F, 0x0F00F000},
        {0x00000000, 0x0F0, 0xF0, 0xF00F0000},
        {0x00000000, 0xF00, 0xAB, 0xABF00000},
    };

    for (uint32_t i = 0; i < ARRAY_SIZE(cases); i++)
    {
        SuId = cases[i].Value0;
        uint32_t statusMasked = STATUS_PACK_META(cases[i].Statusx, cases[i].Value1);
        TEST_ASSERT_EQUAL_HEX32(cases[i].ExpectedResult, statusMasked);
    }   
}

void test_STATUS_PACK_META_SU(void)
{
    Status2ValueTestCase_t cases[] = {
        {0xFFFFFFFF, 0x000, 0x00, 0x00000FFF},
        {0x00000000, 0xFFF, 0xFF, 0xFFFFF000},
        {0x00000000, 0x00F, 0x0F, 0x0F00F000},
        {0x00000000, 0x0F0, 0xF0, 0xF00F0000},
        {0x00000000, 0xF00, 0xAB, 0xABF00000},
    };

    for (uint32_t i = 0; i < ARRAY_SIZE(cases); i++)
    {
        uint32_t statusMasked = STATUS_PACK_META_SU(cases[i].Statusx, cases[i].Value0, cases[i].Value1);
        TEST_ASSERT_EQUAL_HEX32(cases[i].ExpectedResult, statusMasked);
    }   
}

void test_STATUS_IS_CODE(void)
{
    StatusSETTestCase_t cases[] = {
        {0xFFFFFFFF, 0x000, false},
        {0xFFFFF000, 0xFFF, false},
        {0x00000FFF, 0x000, false},
        {0xFFFFFFFF, 0xFFF, true},
        {0x00000FFF, 0xFFF, true},
        {0x0000000F, 0x00F, true},
        {0x000000F0, 0x0F0, true},
        {0x00000F00, 0xF00, true},
    };

    for (uint32_t i = 0; i < ARRAY_SIZE(cases); i++)
    {
        bool statusMasked = STATUS_IS_CODE(cases[i].Statusx, cases[i].ValueSET);
        TEST_ASSERT_EQUAL_HEX8(cases[i].ExpectedResult, statusMasked);
    }   
}

void test_STATUS_GET_METADATA(void)
{
    StatusSETTestCase_t cases[] = {
        {0xFFFFFFFF, 0x0, 0xFF},
        {0xFF000000, 0x0, 0xFF},
        {0x00FFFFFF, 0x0, 0x00},
        {0x00000000, 0x0, 0x00},
        {0x0F000000, 0x0, 0x0F},
        {0xF0000000, 0x0, 0xF0},
    };

    for (uint32_t i = 0; i < ARRAY_SIZE(cases); i++)
    {
        uint32_t metaValue = STATUS_GET_METADATA(cases[i].Statusx);
        TEST_ASSERT_EQUAL_HEX8(cases[i].ExpectedResult, metaValue);
    }   
}


void test_STATUS_POP_METADATA(void)
{
    StatusSETTestCase_t cases[] = {
        {0xFF, 0x0, 0xFF},
        {0x00, 0x0, 0x00},
        {0xFF, 0x0, 0xFF},
        {0x00, 0x0, 0x00},
        {0x00, 0x0, 0x00},
        {0x00, 0x0, 0x00},
    };

    //TODO use object to set values
    for (uint32_t i = 0; i < ARRAY_SIZE(cases); i++)
    {
        StatusMetadataMask_t startObj;
        startObj.value = cases[i].Statusx;

        Status_t ret = STATUS_SET_META(0, startObj.value);
        StatusMetadataMask_t obj;
        STATUS_POP_METADATA(ret, obj);
        TEST_ASSERT_EQUAL_HEX8(cases[i].ExpectedResult, obj.value);
    }   
}

void test_STATUS_GET_ID(void)
{
    StatusSETTestCase_t cases[] = {
        {0xFFFFFFFF, 0x0, 0xFFF},
        {0x00FFF000, 0x0, 0xFFF},
        {0xFFFFF000, 0x0, 0xFFF},
        {0x00FFFFFF, 0x0, 0xFFF},
        {0xFF000FFF, 0x0, 0x000},
        {0x00000000, 0x0, 0x000},
        {0xFF000000, 0x0, 0x000},
        {0x00000FFF, 0x0, 0x000},
        {0x0000F000, 0x0, 0x00F},
        {0x000F0000, 0x0, 0x0F0},
        {0x00F00000, 0x0, 0xF00},
    };

    //TODO use object to set values
    for (uint32_t i = 0; i < ARRAY_SIZE(cases); i++)
    {
        uint32_t id = STATUS_GET_ID(cases[i].Statusx);
        TEST_ASSERT_EQUAL_HEX16(cases[i].ExpectedResult, id);
    }   
}

void test_STATUS_GET_CODE(void)
{
    StatusSETTestCase_t cases[] = {
        {0xFFFFFFFF, 0x0, 0xFFF},
        {0x00000FFF, 0x0, 0xFFF},
        {0xFFFFF000, 0x0, 0x000},
        {0x00000000, 0x0, 0x000},
        {0x0000000F, 0x0, 0x00F},
        {0x000000F0, 0x0, 0x0F0},
        {0x00000F00, 0x0, 0xF00},
        {0x00000BEF, 0x0, 0xBEF},
    };

    //TODO use object to set values
    for (uint32_t i = 0; i < ARRAY_SIZE(cases); i++)
    {
        uint32_t status = STATUS_GET_CODE(cases[i].Statusx);
        TEST_ASSERT_EQUAL_HEX16(cases[i].ExpectedResult, status);
    }   
}

void test_Status_Metadata(void)
{
    StatusMetadataMask_t startObj;
    startObj.Meta.Severity = StatusServerity_Error;

    Status_t ret = STATUS_SET_META(0x0, startObj.value);
    TEST_ASSERT_TRUE(STATUS_IS_ERROR(ret));
    TEST_ASSERT_FALSE(STATUS_IS_CU(ret));

    startObj.Meta.Severity = StatusServerity_Warn;
    ret = STATUS_SET_META(0x0, startObj.value);
    TEST_ASSERT_TRUE(STATUS_IS_WARN(ret));
    TEST_ASSERT_FALSE(STATUS_IS_FATAL(ret));

    startObj.Meta.Severity = StatusServerity_Fatal;
    ret = STATUS_SET_META(0x0, startObj.value);
    TEST_ASSERT_TRUE(STATUS_IS_FATAL(ret));
    TEST_ASSERT_FALSE(STATUS_IS_WARN(ret));

    startObj.Meta.Severity = StatusServerity_CU;
    ret = STATUS_SET_META(0x0, startObj.value);
    TEST_ASSERT_TRUE(STATUS_IS_CU(ret));
    TEST_ASSERT_FALSE(STATUS_IS_ERROR(ret));
    
    startObj.Meta.Handled = 0;
    startObj.Meta.Originating = 0;
    startObj.Meta.Severity = StatusServerity_CU;
    ret = STATUS_SET_META(0x0, startObj.value);
    TEST_ASSERT_FALSE(STATUS_IS_HANDLED(ret));
    TEST_ASSERT_FALSE(STATUS_IS_ORGINATOR(ret));

    startObj.Meta.Handled = 1;
    startObj.Meta.Originating = 0;
    startObj.Meta.Severity = StatusServerity_CU;
    ret = STATUS_SET_META(0x0, startObj.value);
    TEST_ASSERT_TRUE(STATUS_IS_HANDLED(ret));
    TEST_ASSERT_FALSE(STATUS_IS_ORGINATOR(ret));
    TEST_ASSERT_TRUE(STATUS_IS_CU(ret));

    startObj.Meta.Handled = 0;
    startObj.Meta.Originating = 1;
    startObj.Meta.Severity = StatusServerity_CU;
    ret = STATUS_SET_META(0x0, startObj.value);
    TEST_ASSERT_FALSE(STATUS_IS_HANDLED(ret));
    TEST_ASSERT_TRUE(STATUS_IS_ORGINATOR(ret));
    TEST_ASSERT_TRUE(STATUS_IS_CU(ret));

    startObj.Meta.Handled = 1;
    startObj.Meta.Originating = 1;
    startObj.Meta.Severity = StatusServerity_CU;
    ret = STATUS_SET_META(0x0, startObj.value);
    TEST_ASSERT_TRUE(STATUS_IS_HANDLED(ret));
    TEST_ASSERT_TRUE(STATUS_IS_ORGINATOR(ret));
    TEST_ASSERT_TRUE(STATUS_IS_CU(ret));

    ret = 0xFFFFFFFF & ~STATUS_META_HANDLED_MASK_PACKED;
    ret = STATUS_HANDLED(ret);
    TEST_ASSERT_EQUAL_HEX32(0xFFFFFFFF, ret);

    ret = 0x00000000;
    ret = STATUS_HANDLED(ret);
    TEST_ASSERT_EQUAL_HEX32(STATUS_META_HANDLED_MASK_PACKED, ret);

    ret = 0xFFFFFFFF & ~STATUS_META_ORIGINATING_MASK_PACKED;
    ret = STATUS_PASS_UP(ret);
    TEST_ASSERT_EQUAL_HEX32(0xFFFFFFFF, ret);

    ret = 0x00000000;
    ret = STATUS_PASS_UP(ret);
    TEST_ASSERT_EQUAL_HEX32(STATUS_META_ORIGINATING_MASK_PACKED, ret);
}

void test_STATUS_CHECK(void)
{
    StatusSETTestCase_t cases[] = {
        {0x00000000, 0x0, false},
        {0xFFFFFFFF, 0x0, true},
        {0x0000000F, 0x0, true},
        {0x000000F0, 0x0, true},
        {0x00000F00, 0x0, true},
        {0x0000F000, 0x0, true},
        {0x000F0000, 0x0, true},
        {0x00F00000, 0x0, true},
        {0x0F000000, 0x0, true},
        {0xF0000000, 0x0, true},
        {0xDEADBEEF, 0x0, true},
    };

    for (uint32_t i = 0; i < ARRAY_SIZE(cases); i++)
    {
        bool statusMasked = STATUS_CHECK(cases[i].Statusx);
        TEST_ASSERT_EQUAL_HEX8(cases[i].ExpectedResult, statusMasked);
    }  
}

void test_STATUS_CHECK_SEV(void)
{
    StatusMetadataMask_t startObj;
    startObj.Meta.Severity = StatusServerity_Error;

    Status_t ret = STATUS_SET_META(0x0, startObj.value);
    TEST_ASSERT_TRUE(STATUS_CHECK_SEV(ret, StatusServerity_Error));

    ret = STATUS_SET_META(0xF, startObj.value);
    TEST_ASSERT_TRUE(STATUS_CHECK_SEV(ret, StatusServerity_Error));
    TEST_ASSERT_FALSE(STATUS_CHECK_SEV(ret, StatusServerity_CU));

    startObj.Meta.Severity = StatusServerity_Warn;
    ret = STATUS_SET_META(0x0, startObj.value);
    TEST_ASSERT_TRUE(STATUS_CHECK_SEV(ret, StatusServerity_Warn));

    ret = STATUS_SET_META(0xF, startObj.value);
    TEST_ASSERT_TRUE(STATUS_CHECK_SEV(ret, StatusServerity_Warn));
    TEST_ASSERT_FALSE(STATUS_CHECK_SEV(ret, StatusServerity_Fatal));

    startObj.Meta.Severity = StatusServerity_Fatal;
    ret = STATUS_SET_META(0x0, startObj.value);
    TEST_ASSERT_TRUE(STATUS_CHECK_SEV(ret, StatusServerity_Fatal));

    ret = STATUS_SET_META(0xF, startObj.value);
    TEST_ASSERT_TRUE(STATUS_CHECK_SEV(ret, StatusServerity_Fatal));
    TEST_ASSERT_FALSE(STATUS_CHECK_SEV(ret, StatusServerity_Warn));

    startObj.Meta.Severity = StatusServerity_CU;
    ret = STATUS_SET_META(0x0, startObj.value);
    TEST_ASSERT_TRUE(STATUS_CHECK_SEV(ret, StatusServerity_CU));


    ret = STATUS_SET_META(0xF, startObj.value);
    TEST_ASSERT_TRUE(STATUS_CHECK_SEV(ret, StatusServerity_CU));
    TEST_ASSERT_FALSE(STATUS_CHECK_SEV(ret, StatusServerity_Error));
}

#endif // TEST
