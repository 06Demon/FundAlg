#include <gtest/gtest.h>
#include <cstdio>
#include <cstring>
extern "C" {
#include "../include/utils.h"
}

TEST(Oversscanf, Ro) {
    const char *s1 = "MCMXCIV"; // 1994
    const char *s2 = "mcmxciv";
    int v1 = 0, v2 = 0;
    StatusCode r1 = my_oversscanf(s1, "%Ro", &v1);
    StatusCode r2 = my_oversscanf(s2, "%Ro", &v2);
    EXPECT_EQ(r1, SUCCESS);
    EXPECT_EQ(r2, SUCCESS);
    EXPECT_EQ(v1, 1994);
    EXPECT_EQ(v2, 1994);

    const char *s3 = "-";
    unsigned int out3 = 0;
    StatusCode r3 = my_oversscanf(s3, "%Ro", &out3);
    EXPECT_EQ(r3, INVALID_ROMAN);
    EXPECT_EQ(out3, 0);
}

TEST(Oversscanf, Zr) {
    const char *s0 = "1";
    unsigned int out0 = 123;
    StatusCode r0 = my_oversscanf(s0, "%Zr", &out0);
    EXPECT_EQ(r0, INVALID_ZECKENDORF);
    EXPECT_EQ(out0, 123);

    const char *s1 = "1011";
    unsigned int out1 = 0;
    StatusCode r1 = my_oversscanf(s1, "%Zr", &out1);
    EXPECT_EQ(r1, SUCCESS);
    EXPECT_EQ(out1, 4);

    const char *s2 = "0000000000011";
    unsigned int out2 = 0;
    StatusCode r2 = my_oversscanf(s2, "%Zr", &out2);
    EXPECT_EQ(r2, SUCCESS);
    EXPECT_EQ(out2, 233);

    const char *s3 = "0000000000010";
    unsigned int out3 = 0;
    StatusCode r3 = my_oversscanf(s3, "%Zr", &out3);
    EXPECT_EQ(r3, INVALID_ZECKENDORF);
    EXPECT_EQ(out3, 0);
}

TEST(Oversscanf, Cv) {
    int dest1 = 0;
    int dest2 = 0;
    const char *s1 = "1a";
    StatusCode r1 = my_oversscanf(s1, "%Cv", &dest1, 16);
    EXPECT_EQ(r1, SUCCESS);
    EXPECT_EQ(dest1, 26);

    const char *s2 = "1A";
    StatusCode r2 = my_oversscanf(s2, "%CV", &dest2, 16);
    EXPECT_EQ(r2, SUCCESS);
    EXPECT_EQ(dest2, 26);

    int dest3 = 0;
    const char *s3 = "42";
    StatusCode r3 = my_oversscanf(s3, "%Cv", &dest3, 8);
    EXPECT_EQ(r3, SUCCESS);
    EXPECT_EQ(dest3, 34);

    int dest4 = 0;
    const char *s4 = "52";
    StatusCode r4 = my_oversscanf(s4, "%CV", &dest4, 6);
    EXPECT_EQ(r4, SUCCESS);
    EXPECT_EQ(dest4, 32);
}

TEST(Overfscanf, ScanFile) {
    FILE *f = fopen("test_input.txt", "w+");
    ASSERT_NE(f, nullptr);
    fprintf(f, "MCM 1011 2a 2A\n");
    rewind(f);
    int rv = 0;
    unsigned int zr = 0;
    int cv_low = 0, cv_up = 0;
    StatusCode n = my_overfscanf(f, "%Ro %Zr %Cv %CV", &rv, &zr, &cv_low, 16, &cv_up, 16);
    EXPECT_EQ(n, SUCCESS);
    EXPECT_EQ(rv, 1900);
    EXPECT_EQ(zr, 4u);
    EXPECT_EQ(cv_low, 42);
    EXPECT_EQ(cv_up, 42);
    fclose(f);
}

