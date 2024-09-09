#include <gtest/gtest.h>

#include "TT/math.h"

TEST(Basic, EcodeDecode) {
    Vector4 vec1;
    Vector4 vec2;
    ASSERT_EQ(vec1, vec2);

    vec1[0] = 1;
    ASSERT_NE(vec1, vec2);
}
