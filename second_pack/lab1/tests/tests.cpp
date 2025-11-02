#include <gtest/gtest.h>
extern "C" 
{
#include "../include/utils.h"
}

TEST(lab1Test, FinalRepresentationBase8) {
    int results[5];
    StatusCode status = check_final_representation(8, 1e-9, 5, results, 0.5, 0.25, 1.0/3.0, 1.0/5.0, 3.0/7.0);
    ASSERT_EQ(status, SUCCESS);
    EXPECT_EQ(results[0], 1);
    EXPECT_EQ(results[1], 1);
    EXPECT_EQ(results[2], 0);
    EXPECT_EQ(results[3], 0);
    EXPECT_EQ(results[4], 0);
}

TEST(lab1Test, FinalRepresentationBase10) {
    int results[3];
    StatusCode status = check_final_representation(10, 1e-9, 3, results, 0.5, 0.25, 0.3333);
    ASSERT_EQ(status, SUCCESS);
    EXPECT_EQ(results[0], 1);
    EXPECT_EQ(results[1], 1);
    EXPECT_EQ(results[2], 1);
}

TEST(lab1Test, FinalRepresentationBase2) {
    int results[2];
    StatusCode status = check_final_representation(2, 1e-9, 2, results, 0.5, 0.3);
    ASSERT_EQ(status, SUCCESS);
    EXPECT_EQ(results[0], 1);
    EXPECT_EQ(results[1], 0);
}

TEST(lab1Test, FinalRepresentationBase5) {
    int results[1];
    StatusCode status = check_final_representation(5, 1e-9, 1, results, 0.2);
    ASSERT_EQ(status, SUCCESS);
    EXPECT_EQ(results[0], 1);
}

TEST(lab1Test, InvalidBase1) {
    int results[1];
    StatusCode status = check_final_representation(1, 1e-9, 1, results, 0.5);
    EXPECT_EQ(status, INVALID_BASE);
}

TEST(lab1Test, InvalidBase37) {
    int results[1];
    StatusCode status = check_final_representation(37, 1e-9, 1, results, 0.5);
    EXPECT_EQ(status, INVALID_BASE);
}