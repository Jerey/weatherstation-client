#include <iostream>
#include <unity.h>
#include <MsgBase.h>

namespace {

void checkMessageID(void) {
    const uint32_t msgSize = 2;
    const char data[msgSize - 1] = {2};

    for (uint32_t i = 0; i < 3; i++){
        const auto msg = ::Msg::Common::MsgBase<msgSize>(i, data);
        TEST_ASSERT_EQUAL(i, msg.getMsgID());
    }
}

void checkData_shortMessage(void) {
    const char id = 1;
    const uint32_t msgSize = 3;
    const char data[msgSize - 1] = {2, 3};

    const auto msg = ::Msg::Common::MsgBase<msgSize>(id, data);
    TEST_ASSERT_EQUAL(id, msg.getMsgID());

    const auto result = msg.getMsgStream();
    const char expected[msgSize] = {id, data[0], data[1]};
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, result, msgSize);
}

void checkData_longMessage(void){
    const char id = 100;
    const uint32_t msgSize = 6;
    const char data[msgSize - 1] = {0x02, 0x03, 0x04, 0x05, 0x06};

    const auto msg = ::Msg::Common::MsgBase<msgSize>(id, data);
    TEST_ASSERT_EQUAL(id, msg.getMsgID());

    const auto result = msg.getMsgStream();
    const char expected[msgSize] = {id, data[0], data[1], data[2], data[3], data[4]};
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, result, msgSize);
}

} // anonymous namespace 

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(checkMessageID);
    RUN_TEST(checkData_shortMessage);
    RUN_TEST(checkData_longMessage);
    UNITY_END();
    return 0;
}