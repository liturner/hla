#include "RTI/encoding/BasicDataElements.h"

#include "RTI/VariableLengthData.h"

#include <gtest/gtest.h>

TEST(HLAboolean, InitialiseTrue) {
    ASSERT_EQ(true, rti1516e::HLAboolean(true));
}

TEST(HLAboolean, InitialiseFalse) {
    ASSERT_EQ(false, rti1516e::HLAboolean(false));
}

TEST(HLAboolean, HashDiffers) {
    ASSERT_NE(rti1516e::HLAboolean(false).hash(), rti1516e::HLAboolean(true).hash());
}

TEST(HLAboolean, TrueEncoding) {
    rti1516e::HLAboolean myBool(true);
    rti1516e::VariableLengthData myData = myBool.encode();
    auto rawData = reinterpret_cast<unsigned int*>(const_cast<void *>(myData.data()));
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