#include "BatteryModule.h"
#include <unity.h>

void setUp(void)
{
}

void tearDown(void)
{
}

void test_battery(void)
{
    // Over the table
    TEST_ASSERT_EQUAL_INT(100, BatteryModule::calcBatteryPercentage(20));

    // In the table
    TEST_ASSERT_EQUAL_INT(100, BatteryModule::calcBatteryPercentage(16.19));
    TEST_ASSERT_EQUAL_INT(100, BatteryModule::calcBatteryPercentage(16.18));
    TEST_ASSERT_EQUAL_INT(4, BatteryModule::calcBatteryPercentage(13.8));
    TEST_ASSERT_EQUAL_INT(0, BatteryModule::calcBatteryPercentage(13.5));

    // Under the table
    TEST_ASSERT_EQUAL_INT(0, BatteryModule::calcBatteryPercentage(13));
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_battery);
    UNITY_END();

    return 0;
}
