#include <gtest/gtest.h>
extern "C" {
#include "../include/utils.h"
}

TEST(Tests, Zero) {
    char *result = nullptr;
    EXPECT_EQ(decimal_to_base2r(0, 2, &result), SUCCESS);
    EXPECT_STREQ(result, "0");
    free(result);
}

TEST(Tests, Base2) {
    char *result = nullptr;
    EXPECT_EQ(decimal_to_base2r(10, 1, &result), SUCCESS);
    EXPECT_STREQ(result, "1010");
    free(result);
}

TEST(Tests, Base4) {
    char *result = nullptr;
    EXPECT_EQ(decimal_to_base2r(10, 2, &result), SUCCESS);
    EXPECT_STREQ(result, "22");
    free(result);
}

TEST(Tests, Base16) {
    char *result = nullptr;
    EXPECT_EQ(decimal_to_base2r(255, 4, &result), SUCCESS);
    EXPECT_STREQ(result, "FF");
    free(result);
}

TEST(Tests, Base32) {
    char *result = nullptr;
    EXPECT_EQ(decimal_to_base2r(333, 5, &result), SUCCESS);
    EXPECT_STREQ(result, "AD");
    free(result);
}

TEST(Tests, InvalidBase) {
    char *result = nullptr;
    EXPECT_EQ(decimal_to_base2r(10, 0, &result), ERROR_INVALID_BASE);
    EXPECT_EQ(decimal_to_base2r(10, 6, &result), ERROR_INVALID_BASE);
}

TEST(Tests, NegativeNumber) {
    char *result = nullptr;
    EXPECT_EQ(decimal_to_base2r(-5, 2, &result), ERROR_NEGATIVE_NUMBER);
}

TEST(Tests, MemoryAllocation) {
    char *result = nullptr;
    EXPECT_EQ(decimal_to_base2r(1000, 3, &result), SUCCESS);
    free(result);
}