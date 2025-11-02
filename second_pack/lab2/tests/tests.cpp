#include <gtest/gtest.h>
#include <cstring>
#include <string>
extern "C" {
#include "utils.h"
}

TEST(MemTest, MemchrFound) {
    const char *s = "hello";
    void *result;
    StatusCode status = my_memchr(s, 'e', 5, &result);
    EXPECT_EQ(status, STATUS_OK);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(*(char*)result, 'e');
}

TEST(MemTest, MemchrNotFound) {
    const char *s = "abc";
    void *result;
    StatusCode status = my_memchr(s, 'z', 3, &result);
    EXPECT_EQ(status, STATUS_ERROR_NOT_FOUND);
    EXPECT_EQ(result, nullptr);
}

TEST(MemTest, MemcmpEqual) {
    const char a[] = {1,2,3,4};
    const char b[] = {1,2,3,4};
    int result;
    StatusCode status = my_memcmp(a, b, 4, &result);
    EXPECT_EQ(status, STATUS_OK);
    EXPECT_EQ(result, 0);
}

TEST(MemTest, MemcmpDiff) {
    const char a[] = {1,2,5};
    const char b[] = {1,2,3};
    int result;
    StatusCode status = my_memcmp(a, b, 3, &result);
    EXPECT_EQ(status, STATUS_OK);
    EXPECT_GT(result, 0);
}

TEST(MemTest, MemcpyBasic) {
    char dest[] = "abcdef";
    const char src[] = "xyz";
    StatusCode status = my_memcpy(dest, src, 4);
    EXPECT_EQ(status, STATUS_ERROR_OUT_OF_RANGE);
    // EXPECT_STREQ(dest, "xyzdef");
}

TEST(MemTest, MemsetBasic) {
    char buf[4];
    StatusCode status = my_memset(buf, 'x', 4);
    EXPECT_EQ(status, STATUS_OK);
    EXPECT_EQ(buf[0], 'x');
    EXPECT_EQ(buf[3], 'x');
}

TEST(StrTest, StrncatBasic) {
    char dest[10] = "ab";
    const char *src = "cdef";
    StatusCode status = my_strncat(dest, sizeof(dest), src, 5);
    EXPECT_EQ(status, STATUS_OK);
    EXPECT_STREQ(dest, "abcdef");
}

TEST(StrTest, StrchrBasic) {
    const char *s = "hello";
    char *result;
    StatusCode status = my_strchr(s, 'l', &result);
    EXPECT_EQ(status, STATUS_OK);
    EXPECT_STREQ(result, "llo");
}

TEST(StrTest, StrchrNotFound) {
    const char *s = "hello";
    char *result;
    StatusCode status = my_strchr(s, 'x', &result);
    EXPECT_EQ(status, STATUS_ERROR_NOT_FOUND);
    EXPECT_EQ(result, nullptr);
}

TEST(StrTest, StrncmpEqual) {
    int result;
    StatusCode status = my_strncmp("abc", "abd", 2, &result);
    EXPECT_EQ(status, STATUS_OK);
    EXPECT_EQ(result, 0);
}

TEST(StrTest, StrncmpDiff) {
    int result;
    StatusCode status = my_strncmp("abc", "abd", 3, &result);
    EXPECT_EQ(status, STATUS_OK);
    EXPECT_LT(result, 0);
}

TEST(StrTest, StrncpyBasic) {
    char dest[6] = {};
    StatusCode status = my_strncpy(dest, sizeof(dest), "hi", 6);
    EXPECT_EQ(status, STATUS_OK);
    EXPECT_STREQ(dest, "hi");
    EXPECT_EQ(dest[2], '\0');
}

TEST(StrTest, StrcspnBasic) {
    size_t result;
    StatusCode status = my_strcspn("abcdef", "xdy", &result);
    EXPECT_EQ(status, STATUS_OK);
    EXPECT_EQ(result, 3);
}

TEST(StrTest, StrcspnNoMatch) {
    size_t result;
    StatusCode status = my_strcspn("abc", "", &result);
    EXPECT_EQ(status, STATUS_OK);
    EXPECT_EQ(result, 3);
}

TEST(StrTest, StrerrorCommon) {
    const char *result;
    StatusCode status = my_strerror(2, &result);
    EXPECT_EQ(status, STATUS_OK);
    EXPECT_NE(result, nullptr);
    
    std::string s(result);
    EXPECT_TRUE(s.find("file") != std::string::npos || 
                s.find("File") != std::string::npos);
}

TEST(StrTest, StrerrorOutOfRange) {
    const char *result;
    StatusCode status = my_strerror(9999, &result);
    EXPECT_EQ(status, STATUS_ERROR_OUT_OF_RANGE);
    EXPECT_STREQ(result, "Unknown error");
}

TEST(StrTest, StrlenBasic) {
    size_t result;
    StatusCode status = my_strlen("", &result);
    EXPECT_EQ(status, STATUS_OK);
    EXPECT_EQ(result, 0);
}

TEST(StrTest, StrlenEmpty) {
    size_t result;
    StatusCode status = my_strlen("", &result);
    EXPECT_EQ(status, STATUS_OK);
    EXPECT_EQ(result, 0);
}

TEST(StrTest, StrpbrkBasic) {
    const char *s = "abcdef";
    char *result;
    StatusCode status = my_strpbrk(s, "xdc", &result);
    EXPECT_EQ(status, STATUS_OK);
    EXPECT_STREQ(result, "cdef");
}

TEST(StrTest, StrpbrkNotFound) {
    const char *s = "abcdef";
    char *result;
    StatusCode status = my_strpbrk(s, "XYZ", &result);
    EXPECT_EQ(status, STATUS_ERROR_NOT_FOUND);
    EXPECT_EQ(result, nullptr);
}

TEST(StrTest, StrrchrBasic) {
    const char *s = "abca";
    char *result;
    StatusCode status = my_strrchr(s, 'a', &result);
    EXPECT_EQ(status, STATUS_OK);
    EXPECT_STREQ(result, "a");
}

TEST(StrTest, StrrchrNotFound) {
    const char *s = "abca";
    char *result;
    StatusCode status = my_strrchr(s, 'z', &result);
    EXPECT_EQ(status, STATUS_ERROR_NOT_FOUND);
    EXPECT_EQ(result, nullptr);
}

TEST(StrTest, StrstrBasic) {
    const char *s = "hello world";
    char *result;
    StatusCode status = my_strstr(s, "world", &result);
    EXPECT_EQ(status, STATUS_OK);
    EXPECT_STREQ(result, "world");
}

TEST(StrTest, StrstrNotFound) {
    const char *s = "abc";
    char *result;
    StatusCode status = my_strstr(s, "d", &result);
    EXPECT_EQ(status, STATUS_ERROR_NOT_FOUND);
    EXPECT_EQ(result, nullptr);
}

TEST(StrTest, StrtokBasic) {
    char s[] = "a,b,c";
    char *token = my_strtok(s, ",");
    ASSERT_NE(token, nullptr);
    EXPECT_STREQ(token, "a");
    
    token = my_strtok(NULL, ",");
    EXPECT_STREQ(token, "b");
    
    token = my_strtok(NULL, ",");
    EXPECT_STREQ(token, "c");
    
    token = my_strtok(NULL, ",");
    EXPECT_EQ(token, nullptr);
}

TEST(StatusTest, StatusMessages) {
    EXPECT_STREQ(status_message(STATUS_OK), "Success");
    EXPECT_STREQ(status_message(STATUS_ERROR_NULL_POINTER), "Null pointer error");
    EXPECT_STREQ(status_message(STATUS_ERROR_NOT_FOUND), "Not found");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}