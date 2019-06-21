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
    char expectedMsgContents = 0b00000000;

    auto result = msg.getMsgStream();
    char expected[] = { msgId, expectedMsgContents, '\0' };
    for (unsigned int i = 0; i < totalSizeOfMessage; i++) {
        TEST_ASSERT_EQUAL(expected[i], result[i]);
    }
}

void humidity_negative1(void) {
    auto msg = ::Msg::Humidity::MsgHumidity(-1.0);
    char expectedMsgContents = 0b00000000;

    auto result = msg.getMsgStream();
    char expected[] = { msgId, expectedMsgContents, '\0' };
    for (unsigned int i = 0; i < totalSizeOfMessage; i++) {
        TEST_ASSERT_EQUAL(expected[i], result[i]);
    }
}

void humidity_100(void) {
    auto msg = ::Msg::Humidity::MsgHumidity(100.0);
    char expectedMsgContents = 0b01100100;

    auto result = msg.getMsgStream();
    char expected[] = { msgId, expectedMsgContents, '\0' };
    for (unsigned int i = 0; i < totalSizeOfMessage; i++) {
        TEST_ASSERT_EQUAL(expected[i], result[i]);
    }
}

void humidity_255(void) {
    auto msg = ::Msg::Humidity::MsgHumidity(255.0);
    const char expectedMsgContents = 0b01100100;

    auto result = msg.getMsgStream();
    const char expected[] = { msgId, expectedMsgContents, '\0' };
    for (unsigned int i = 0; i < totalSizeOfMessage; i++) {
        TEST_ASSERT_EQUAL(expected[i], result[i]);
    }
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