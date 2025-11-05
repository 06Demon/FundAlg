#include <gtest/gtest.h>
extern "C" {
#include "../include/utils.h"
}

TEST(Test, BalancedSimple) {
    EXPECT_EQ(check_brackets("()"), SUCCESS);
    EXPECT_EQ(check_brackets("{}"), SUCCESS);
    EXPECT_EQ(check_brackets("[]"), SUCCESS);
    EXPECT_EQ(check_brackets("<>"), SUCCESS);
}

TEST(Test, BalancedWithText) {
    EXPECT_EQ(check_brackets("Hello, world!(gol) JSON - {}. list - [1, 2, 3, 4]. HTML </>"), SUCCESS);
    EXPECT_EQ(check_brackets("<TextTextText(TextTextTextTextTextText[1,2,3]).> TextTextTextText - {efgsdgfsdfg(gol)}"), SUCCESS);
}

TEST(Test, BalancedAll) {
    EXPECT_EQ(check_brackets("({[<>]})"), SUCCESS);
}

TEST(Test, UnbalancedMissingClose) {
    EXPECT_EQ(check_brackets("Hello, world!(gol) JSON - {}. list - [1, 2, 3, 4. HTML </>"), BRACKETS_NOT_BALANCED);
    EXPECT_EQ(check_brackets("({[]}"), BRACKETS_NOT_BALANCED);
}

TEST(Test, UnbalancedWrongOrder) {
    EXPECT_EQ(check_brackets("({[}])"), BRACKETS_NOT_BALANCED);
}

TEST(Test, UnbalancedMissingOpen) {
    EXPECT_EQ(check_brackets("Hello, world!(gol) JSON - {}. list - 1, 2, 3, 4]. HTML </>"), BRACKETS_NOT_BALANCED);
    EXPECT_EQ(check_brackets("({[]>})"), BRACKETS_NOT_BALANCED);
}

TEST(Test, Empty) {
    EXPECT_EQ(check_brackets(""), SUCCESS);
}

TEST(Test, Text) {
    EXPECT_EQ(check_brackets("Hello World"), SUCCESS);
}

TEST(Test, InvalidArgument) {
    EXPECT_EQ(check_brackets(nullptr), INVALID_ARGUMENT);
}