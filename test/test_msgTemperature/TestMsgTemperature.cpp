#include <unity.h>
#include <MsgTemperature.h>

namespace {
const char msgId = 1;
const uint32_t totalSizeOfMessage = 3;

void messageID(void) {
    const auto msg = ::Msg::Temperature::MsgTemperature(1.0);
    TEST_ASSERT_EQUAL(1, msg.getMsgID());
}

void zero_temp(void) {
    const auto msg = ::Msg::Temperature::MsgTemperature(0.0);
    const char expectedMsgContents = 0b00000000;

    const auto result = msg.getMsgStream();
    const char expected[] = { msgId, expectedMsgContents, '\0' };
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, result, ::Msg::Temperature::sizeOfMessage + 1);
}

void maxPos_temp_withoutHalfBit_63(void) {
    const auto msg = ::Msg::Temperature::MsgTemperature(63.0);
    const char expectedMsgContents = 0b01111110;

    const auto result = msg.getMsgStream();
    const char expected[] = { msgId, expectedMsgContents, '\0' };
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, result, ::Msg::Temperature::sizeOfMessage + 1);
}

void maxPos_temp_withHalfBit_63_5(void) {
    const auto msg = ::Msg::Temperature::MsgTemperature(63.5);
    const char expectedMsgContents = 0b01111111;

    const auto result = msg.getMsgStream();
    const char expected[] = { msgId, expectedMsgContents, '\0' };
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, result, ::Msg::Temperature::sizeOfMessage + 1);
}

void maxPos_temp_withHalfBit_63_74(void) {
    const auto msg = ::Msg::Temperature::MsgTemperature(63.74);
    const char expectedMsgContents = 0b01111111;

    const auto result = msg.getMsgStream();
    const char expected[] = { msgId, expectedMsgContents, '\0' };
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, result, ::Msg::Temperature::sizeOfMessage + 1);
}

void toBigPosLowered_temp_withHalfBit_63_75(void) {
    const auto msg = ::Msg::Temperature::MsgTemperature(63.75);
    const char expectedMsgContents = 0b01111111;

    const auto result = msg.getMsgStream();
    const char expected[] = { msgId, expectedMsgContents, '\0' };
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, result, ::Msg::Temperature::sizeOfMessage + 1);
}

void toBigPosLowered_temp_HalfBit_100(void) {
    const auto msg = ::Msg::Temperature::MsgTemperature(100.0);
    const char expectedMsgContents = 0b01111111;

    const auto result = msg.getMsgStream();
    const char expected[] = { msgId, expectedMsgContents, '\0' };
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, result, ::Msg::Temperature::sizeOfMessage + 1);
}

void maxNeg_temp_withoutHalfBit_64(void) {
    const auto msg = ::Msg::Temperature::MsgTemperature(-64.0);
    const char expectedMsgContents = 0b10000000;


    const auto result = msg.getMsgStream();
    const char expected[] = { msgId, expectedMsgContents, '\0' };
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, result, ::Msg::Temperature::sizeOfMessage + 1);
}

void maxNeg_temp_withHalfBit_63_5(void) {
    const auto msg = ::Msg::Temperature::MsgTemperature(-63.5);
    const char expectedMsgContents = 0b10000001;

    const auto result = msg.getMsgStream();
    const char expected[] = { msgId, expectedMsgContents, '\0' };
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, result, ::Msg::Temperature::sizeOfMessage + 1);
}

void toBigNegLowered_temp_withHalfBit_64_24(void) {
    const auto msg = ::Msg::Temperature::MsgTemperature(-64.24);
    const char expectedMsgContents = 0b10000000;

    const auto result = msg.getMsgStream();
    const char expected[] = { msgId, expectedMsgContents, '\0' };
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, result, ::Msg::Temperature::sizeOfMessage + 1);
}

void toBigNegLowered_temp_HalfBit_100(void) {
    const auto msg = ::Msg::Temperature::MsgTemperature(-100.0);
    const char expectedMsgContents = 0b10000000;

    const auto result = msg.getMsgStream();
    const char expected[] = { msgId, expectedMsgContents, '\0' };
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, result, ::Msg::Temperature::sizeOfMessage + 1);
}

void minNeg_temp_withHalfBit_0_5(void) {
    const auto msg = ::Msg::Temperature::MsgTemperature(-0.5);
    const char expectedMsgContents = 0b11111111;

    const auto result = ::Msg::Temperature::MsgTemperature(-0.5).getMsgStream();
    const char expected[] = { msgId, expectedMsgContents, '\0' };
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected, result, ::Msg::Temperature::sizeOfMessage + 1);
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