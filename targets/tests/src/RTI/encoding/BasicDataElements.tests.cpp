#include "RTI/encoding/BasicDataElements.h"

#include "RTI/VariableLengthData.h"

#include <gtest/gtest.h>

#include "MIM/EnumeratedDataTypes.h"

#define IMPLEMENT_BASIC_TESTS(ENCODABLE_TYPE, RAW_TYPE, VALUE_1, VALUE_2)\
TEST(ENCODABLE_TYPE, Initialise) {    \
    ASSERT_EQ(VALUE_1, ENCODABLE_TYPE(VALUE_1));\
    ASSERT_EQ(VALUE_2, ENCODABLE_TYPE(VALUE_2));\
}\
\
TEST(ENCODABLE_TYPE, EncodeDecode) {\
    constexpr RAW_TYPE valueOfTruth(VALUE_1);\
    const ENCODABLE_TYPE origionalHlaValue(valueOfTruth);\
    const VariableLengthData endodedData(origionalHlaValue.encode());\
    ENCODABLE_TYPE decodedHlaValue;\
    decodedHlaValue.decode(endodedData);\
    ASSERT_EQ(valueOfTruth, origionalHlaValue);\
    ASSERT_EQ(valueOfTruth, decodedHlaValue);\
}\
\

namespace rti1516e {
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAASCIIchar, char )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAASCIIstring, std::string )
    IMPLEMENT_BASIC_TESTS(HLAboolean, bool, true, false)
    IMPLEMENT_BASIC_TESTS(HLAbyte, Octet, 42, 24)
    IMPLEMENT_BASIC_TESTS(HLAfloat32BE, float, 42.1337f, 1337.42f)
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAfloat32LE, float )
    IMPLEMENT_BASIC_TESTS(HLAfloat64BE, double, 42.1337, 1337.42)
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAfloat64LE, double )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAinteger16LE, Integer16 )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAinteger16BE, Integer16 )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAinteger32BE, Integer32 )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAinteger32LE, Integer32 )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAinteger64BE, Integer64 )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAinteger64LE, Integer64 )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAoctet, Octet )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAoctetPairBE, OctetPair )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAoctetPairLE, OctetPair )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAunicodeChar, wchar_t )
    //IMPLEMENT_ENCODING_HELPER_CLASS( HLAunicodeString, std::wstring )
}

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


