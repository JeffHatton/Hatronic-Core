#include "src/Status.h"
#include "/var/lib/gems/2.7.0/gems/ceedling-0.31.1/vendor/unity/src/unity.h"








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

        uint32_t statusMasked = (cases[i].Statusx & (0x0000FFFF));

        UnityAssertEqualNumber((UNITY_INT)(UNITY_INT8 )((cases[i].ExpectedResult)), (UNITY_INT)(UNITY_INT8 )((statusMasked)), (

       ((void *)0)

       ), (UNITY_UINT)(34), UNITY_DISPLAY_STYLE_HEX8);

    }

}



void test_Status_NeedToImplement(void)

{



}
