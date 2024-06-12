#include "SerialLoop.h"
#include <unity.h>

void setUp(void)
{
}

void tearDown(void)
{
}

uint8_t encode(KeyDirection direction, uint8_t speed)
{
    TEST_ASSERT_TRUE(speed < 32);
    return (static_cast<uint8_t>(direction) << 5) | speed;
}

void test_decode(void)
{
    KeyDirection direction;
    uint8_t speed;

    char message = encode(KeyDirection::RIGHT, 0);
    TEST_ASSERT_EQUAL_INT(0b01100000, message);

    decode(encode(KeyDirection::RIGHT, 0), direction, speed);
    TEST_ASSERT_EQUAL_INT(KeyDirection::RIGHT, direction);
    TEST_ASSERT_EQUAL_INT(0, speed);

    decode(encode(KeyDirection::FORWARD, 31), direction, speed);
    TEST_ASSERT_EQUAL_INT(KeyDirection::FORWARD, direction);
    TEST_ASSERT_EQUAL_INT(31, speed);

    decode(encode(KeyDirection::LEFT, 18), direction, speed);
    TEST_ASSERT_EQUAL_INT(KeyDirection::LEFT, direction);
    TEST_ASSERT_EQUAL_INT(18, speed);

    decode(encode(KeyDirection::BACKWARD, 1), direction, speed);
    TEST_ASSERT_EQUAL_INT(KeyDirection::BACKWARD, direction);
    TEST_ASSERT_EQUAL_INT(1, speed);

    // Stop
    decode(encode(KeyDirection::STOP, 1), direction, speed);
    TEST_ASSERT_EQUAL_INT(KeyDirection::STOP, direction);
    TEST_ASSERT_EQUAL_INT(1, speed);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_decode);
    UNITY_END();

    return 0;
}
