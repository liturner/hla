#include "RTI/encoding/BasicDataElements.h"

#include "RTI/VariableLengthData.h"

#include <gtest/gtest.h>

#include "MIM/EnumeratedDataTypes.h"

#pragma region HLAboolean

TEST(HLAboolean, InitialiseTrue) {
    ASSERT_EQ(true, rti1516e::HLAboolean(true));
    ASSERT_EQ(true, rti1516e::HLAtrue);
}

TEST(HLAboolean, InitialiseFalse) {
    ASSERT_EQ(false, rti1516e::HLAboolean(false));
    ASSERT_EQ(false, rti1516e::HLAfalse);
}

TEST(HLAboolean, HashDiffers) {
    ASSERT_NE(rti1516e::HLAboolean(false).hash(), rti1516e::HLAboolean(true).hash());
}

TEST(HLAboolean, TrueEncoding) {
    rti1516e::HLAboolean myBool(true);
    rti1516e::VariableLengthData myData = myBool.encode();
    auto rawData = reinterpret_cast<int*>(const_cast<void *>(myData.data()));
    auto encodedValue = *rawData;
#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    ASSERT_EQ(encodedValue, 1);
#else
    ASSERT_EQ(encodedValue, 16777216);
#endif
}

TEST(HLAboolean, FalseEncoding) {
    rti1516e::HLAboolean myBool(false);
    rti1516e::VariableLengthData myData = myBool.encode();
    auto rawData = reinterpret_cast<unsigned int*>(const_cast<void *>(myData.data()));
    auto encodedValue = *rawData;
    ASSERT_EQ(encodedValue, 0);
}

TEST(HLAboolean, TrueDecoding) {
    char bigEndianOne[4] {0, 0, 0, 1};
    rti1516e::HLAboolean myBool(false);
    rti1516e::VariableLengthData myData(bigEndianOne, 4);
    myBool.decode(myData);
    ASSERT_TRUE(myBool);
}

TEST(HLAboolean, FalseDecoding) {
    char bigEndianZero[4] {0, 0, 0, 0};
    rti1516e::HLAboolean myBool(true);
    rti1516e::VariableLengthData myData(bigEndianZero, 4);
    myBool.decode(myData);
    ASSERT_FALSE(myBool);
}

#pragma endregion

#pragma region HLAbyte

TEST(HLAbyte, Initialisation) {
    ASSERT_EQ(1, rti1516e::HLAbyte(1));
    ASSERT_EQ(2, rti1516e::HLAbyte(2));
    ASSERT_EQ(5, rti1516e::HLAbyte(5));
    ASSERT_EQ(42, rti1516e::HLAbyte(42));
}

TEST(HLAbyte, Assignment) {
    rti1516e::HLAbyte token;
    token = 4;
    ASSERT_EQ(4, token);
    token = 34;
    ASSERT_EQ(34, token);
}

TEST(HLAbyte, Usage) {
    rti1516e::HLAbyte token(42);
    char aChar = token;
    int anInt = token;
    unsigned int aUint = token;
    ASSERT_EQ(42, aChar);
    ASSERT_EQ(42, anInt);
    ASSERT_EQ(42, aUint);
}

#pragma endregion


#pragma region HLAfloat32BE

TEST(HLAfloat32BE, Initialisation) {
    ASSERT_EQ(1, rti1516e::HLAfloat32BE(1));
    ASSERT_EQ(345.765f, rti1516e::HLAfloat32BE(345.765f));
    ASSERT_EQ(35.765f, rti1516e::HLAfloat32BE(35.765));
}

TEST(HLAfloat32BE, EcodeDecode) {
    constexpr float floatOfTruth(42.1337f);
    const rti1516e::HLAfloat32BE origionalHlaFloat(floatOfTruth);
    const rti1516e::VariableLengthData endodedData(origionalHlaFloat.encode());
    rti1516e::HLAfloat32BE decodedHlaFloat;
    decodedHlaFloat.decode(endodedData);

    ASSERT_EQ(floatOfTruth, origionalHlaFloat);
    ASSERT_EQ(floatOfTruth, decodedHlaFloat);
}

#pragma endregion

#pragma region HLAfloat64BE

TEST(HLAfloat64BE, Initialisation) {
    ASSERT_EQ(1, rti1516e::HLAfloat32BE(1));
    ASSERT_EQ(345.765f, rti1516e::HLAfloat32BE(345.765f));
    ASSERT_EQ(35.765f, rti1516e::HLAfloat32BE(35.765));
}

TEST(HLAfloat64BE, EcodeDecode) {
    constexpr double doubleOfTruth(42.1337);
    const rti1516e::HLAfloat64BE origionalHlaDouble(doubleOfTruth);
    const rti1516e::VariableLengthData endodedData(origionalHlaDouble.encode());
    rti1516e::HLAfloat64BE decodedHlaDouble;
    decodedHlaDouble.decode(endodedData);

    ASSERT_EQ(doubleOfTruth, origionalHlaDouble);
    ASSERT_EQ(doubleOfTruth, decodedHlaDouble);
}

#pragma endregion


