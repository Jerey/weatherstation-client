#include <unity.h>
#include <MsgTemperature.h>

namespace {
const char msgId = 1;
const uint32_t totalSizeOfMessage = 3;

void messageID(void) {
    auto msg = ::Msg::Temperature::MsgTemperature(1.0);
    TEST_ASSERT_EQUAL(1, msg.getMsgID());
}

void zero_temp(void) {
    auto msg = ::Msg::Temperature::MsgTemperature(0.0);
    char expectedMsgContents = 0b00000000;

    auto result = msg.getMsgStream();
    char expected[] = { msgId, expectedMsgContents, '\0' };
    for (unsigned int i = 0; i < totalSizeOfMessage; i++) {
        TEST_ASSERT_EQUAL(expected[i], result[i]);
    }
}

void maxPos_temp_withoutHalfBit_63(void) {
    auto msg = ::Msg::Temperature::MsgTemperature(63.0);
    char expectedMsgContents = 0b01111110;

    auto result = msg.getMsgStream();
    char expected[] = { msgId, expectedMsgContents, '\0' };
    for (unsigned int i = 0; i < totalSizeOfMessage; i++) {
        TEST_ASSERT_EQUAL(expected[i], result[i]);
    }
}

void maxPos_temp_withHalfBit_63_5(void) {
    auto msg = ::Msg::Temperature::MsgTemperature(63.5);
    char expectedMsgContents = 0b01111111;

    auto result = msg.getMsgStream();
    char expected[] = { msgId, expectedMsgContents, '\0' };
    for (unsigned int i = 0; i < totalSizeOfMessage; i++) {
        TEST_ASSERT_EQUAL(expected[i], result[i]);
    }
}

void maxPos_temp_withHalfBit_63_74(void) {
    auto msg = ::Msg::Temperature::MsgTemperature(63.74);
    char expectedMsgContents = 0b01111111;

    auto result = msg.getMsgStream();
    char expected[] = { msgId, expectedMsgContents, '\0' };
    for (unsigned int i = 0; i < totalSizeOfMessage; i++) {
        TEST_ASSERT_EQUAL(expected[i], result[i]);
    }
}

void toBigPosLowered_temp_withHalfBit_63_75(void) {
    auto msg = ::Msg::Temperature::MsgTemperature(63.75);
    char expectedMsgContents = 0b01111111;

    auto result = msg.getMsgStream();
    char expected[] = { msgId, expectedMsgContents, '\0' };
    for (unsigned int i = 0; i < totalSizeOfMessage; i++) {
        TEST_ASSERT_EQUAL(expected[i], result[i]);
    }
}

void toBigPosLowered_temp_HalfBit_100(void) {
    auto msg = ::Msg::Temperature::MsgTemperature(100.0);
    char expectedMsgContents = 0b01111111;

    auto result = msg.getMsgStream();
    char expected[] = { msgId, expectedMsgContents, '\0' };
    for (unsigned int i = 0; i < totalSizeOfMessage; i++) {
        TEST_ASSERT_EQUAL(expected[i], result[i]);
    }
}

void maxNeg_temp_withoutHalfBit_64(void) {
    auto msg = ::Msg::Temperature::MsgTemperature(-64.0);
    char expectedMsgContents = 0b10000000;


    auto result = msg.getMsgStream();
    char expected[] = { msgId, expectedMsgContents, '\0' };
    for (unsigned int i = 0; i < totalSizeOfMessage; i++) {
        TEST_ASSERT_EQUAL(expected[i], result[i]);
    }
}

void maxNeg_temp_withHalfBit_63_5(void) {
    auto msg = ::Msg::Temperature::MsgTemperature(-63.5);
    char expectedMsgContents = 0b10000001;

    auto result = msg.getMsgStream();
    char expected[] = { msgId, expectedMsgContents, '\0' };
    for (unsigned int i = 0; i < totalSizeOfMessage; i++) {
        TEST_ASSERT_EQUAL(expected[i], result[i]);
    }
}

void toBigNegLowered_temp_withHalfBit_64_24(void) {
    auto msg = ::Msg::Temperature::MsgTemperature(-64.24);
    char expectedMsgContents = 0b10000000;

    auto result = msg.getMsgStream();
    char expected[] = { msgId, expectedMsgContents, '\0' };
    for (unsigned int i = 0; i < totalSizeOfMessage; i++) {
        TEST_ASSERT_EQUAL(expected[i], result[i]);
    }
}

void toBigNegLowered_temp_HalfBit_100(void) {
    auto msg = ::Msg::Temperature::MsgTemperature(-100.0);
    char expectedMsgContents = 0b10000000;

    auto result = msg.getMsgStream();
    char expected[] = { msgId, expectedMsgContents, '\0' };
    for (unsigned int i = 0; i < totalSizeOfMessage; i++) {
        TEST_ASSERT_EQUAL(expected[i], result[i]);
    }
}

void minNeg_temp_withHalfBit_0_5(void) {
    auto msg = ::Msg::Temperature::MsgTemperature(-0.5);
    char expectedMsgContents = 0b11111111;

    auto result = msg.getMsgStream();
    char expected[] = { msgId, expectedMsgContents, '\0' };
    for (unsigned int i = 0; i < totalSizeOfMessage; i++) {
        TEST_ASSERT_EQUAL(expected[i], result[i]);
    }
}
} // anonymous namespace

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(messageID);
    RUN_TEST(zero_temp);
    RUN_TEST(maxPos_temp_withoutHalfBit_63);
    RUN_TEST(maxPos_temp_withHalfBit_63_5);
    RUN_TEST(maxPos_temp_withHalfBit_63_74);
    RUN_TEST(toBigPosLowered_temp_withHalfBit_63_75);
    RUN_TEST(toBigPosLowered_temp_HalfBit_100);
    RUN_TEST(maxNeg_temp_withoutHalfBit_64);
    RUN_TEST(maxNeg_temp_withHalfBit_63_5);
    RUN_TEST(toBigNegLowered_temp_withHalfBit_64_24);
    RUN_TEST(toBigNegLowered_temp_HalfBit_100);
    RUN_TEST(minNeg_temp_withHalfBit_0_5);
    UNITY_END();
    return 0;
}