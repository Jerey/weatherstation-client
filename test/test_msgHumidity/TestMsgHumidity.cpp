#include <unity.h>
#include <MsgHumidity.h>

namespace {
    
const char msgId = 2;
const uint32_t totalSizeOfMessage = 3;

void messageID(void) {
    auto msg = ::Msg::Humidity::MsgHumidity(2.0);
    TEST_ASSERT_EQUAL(msgId, msg.getMsgID());
}

void humidity_0(void) {
    auto msg = ::Msg::Humidity::MsgHumidity(0.0);
    char expectedMsgContents = 0x00;

    const auto result = msg.getMsgStream();
    const char expected[] = { msgId, expectedMsgContents, '\0' };
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, result, Msg::Humidity::sizeOfMessage + 1);
}

void humidity_negative1(void) {
    auto msg = ::Msg::Humidity::MsgHumidity(-1.0);
    const char expectedMsgContents = 0x00;

    const auto result = msg.getMsgStream();
    const char expected[] = { msgId, expectedMsgContents, '\0' };
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, result, Msg::Humidity::sizeOfMessage + 1);
}

void humidity_100(void) {
    auto msg = ::Msg::Humidity::MsgHumidity(100.0);
    const char expectedMsgContents = 0x64;

    const auto result = msg.getMsgStream();
    char expected[] = { msgId, expectedMsgContents, '\0' };
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, result, Msg::Humidity::sizeOfMessage + 1);
}

void humidity_255(void) {
    auto msg = ::Msg::Humidity::MsgHumidity(255.0);
    const char expectedMsgContents = 0x64;

    const auto result = msg.getMsgStream();
    const char expected[] = { msgId, expectedMsgContents, '\0' };
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, result, Msg::Humidity::sizeOfMessage + 1);
}

} // anonymous namespace 

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(humidity_0);
    RUN_TEST(humidity_negative1);
    RUN_TEST(humidity_100);
    RUN_TEST(humidity_255);
    UNITY_END();
    return 0;
}