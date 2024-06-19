#include "SerialLoop.h"
#include <unity.h>

void setUp(void)
{
}

void tearDown(void)
{
}

uint8_t encode(KeyDirection direction)
{
    return static_cast<uint8_t>(direction);
}

void test_decode(void)
{
    KeyDirection direction;

    char message = encode(KeyDirection::RIGHT);
    decode(message, direction);
    TEST_ASSERT_EQUAL_INT(KeyDirection::RIGHT, direction);

    message = encode(KeyDirection::LEFT);
    decode(message, direction);
    TEST_ASSERT_EQUAL_INT(KeyDirection::LEFT, direction);

    message = encode(KeyDirection::FORWARD);
    decode(message, direction);
    TEST_ASSERT_EQUAL_INT(KeyDirection::FORWARD, direction);

    message = encode(KeyDirection::BACKWARD);
    decode(message, direction);
    TEST_ASSERT_EQUAL_INT(KeyDirection::BACKWARD, direction);

    message = encode(KeyDirection::STOP);
    decode(message, direction);
    TEST_ASSERT_EQUAL_INT(KeyDirection::STOP, direction);
}

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_decode);
    UNITY_END();

    return 0;
}
