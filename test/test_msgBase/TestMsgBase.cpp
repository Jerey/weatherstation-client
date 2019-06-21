#include <unity.h>
#include <MsgBase.h>

namespace {

void checkDataAndID(void) {
    const char id = 1;
    const uint32_t msgSize = 3;
    const char data[msgSize - 1] = {2, 3};

    auto msg = ::Msg::Common::MsgBase<msgSize>(id, data);
    TEST_ASSERT_EQUAL(id, msg.getMsgID());
    auto result = msg.getMsgStream();
    char expected[msgSize];
    expected[0] = id;
    std::copy(data, data, expected + 1);
    for(uint32_t i = 0; i < msgSize; i++){
        TEST_ASSERT_EQUAL(*expected + i, *result + i);
    }
}

void checkDataAndId_longMsg(void){
    const char id = 100;
    const uint32_t msgSize = 6;
    const char data[msgSize - 1] = {2, 3, 4, 5, 6};

    auto msg = ::Msg::Common::MsgBase<msgSize>(id, data);
    TEST_ASSERT_EQUAL(id, msg.getMsgID());
    auto result = msg.getMsgStream();
    char expected[msgSize];
    expected[0] = id;
    std::copy(data, data, expected + 1);
    for(uint32_t i = 0; i < msgSize; i++){
        TEST_ASSERT_EQUAL(*expected + i, *result + i);
    }
}

} // anonymous namespace 

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(checkDataAndID);
    RUN_TEST(checkDataAndId_longMsg);
    UNITY_END();
    return 0;
}