#include "RTI/encoding/BasicDataElements.h"

#include <gtest/gtest.h>

TEST(HLAboolean, InitialiseTrue) {
    const rti1516e::HLAboolean myBoolean(true);
    ASSERT_EQ(true, myBoolean);
}

TEST(HLAboolean, InitialiseFalse) {
    const rti1516e::HLAboolean myBoolean(false);
    ASSERT_EQ(false, myBoolean);
}

TEST(HLAboolean, HashDiffers) {
    ASSERT_NE(rti1516e::HLAboolean(false).hash(), rti1516e::HLAboolean(true).hash());
}
