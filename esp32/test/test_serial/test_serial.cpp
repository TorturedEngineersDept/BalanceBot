#include "SerialLoop.h"
#include <unity.h>

void setUp(void)
{
}

void tearDown(void)
{
}

uint8_t encode(Direction direction, uint8_t speed)
{
    return (static_cast<int>(direction) << 6) | speed;
}

void test_decode(void)
{
    Direction direction;
    uint8_t speed;

    decode(encode(Direction::RIGHT, 0), direction, speed);
    TEST_ASSERT_EQUAL_INT(Direction::RIGHT, direction);
    TEST_ASSERT_EQUAL_INT(0, speed);

    decode(encode(Direction::FORWARD, 63), direction, speed);
    TEST_ASSERT_EQUAL_INT(Direction::FORWARD, direction);
    TEST_ASSERT_EQUAL_INT(63, speed);

    decode(encode(Direction::LEFT, 32), direction, speed);
    TEST_ASSERT_EQUAL_INT(Direction::LEFT, direction);
    TEST_ASSERT_EQUAL_INT(32, speed);

    decode(encode(Direction::BACKWARD, 1), direction, speed);
    TEST_ASSERT_EQUAL_INT(Direction::BACKWARD, direction);
    TEST_ASSERT_EQUAL_INT(1, speed);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_decode);
    UNITY_END();

    return 0;
}
