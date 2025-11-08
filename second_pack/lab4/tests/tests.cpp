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
    int count1 = 0, count2 = 0;
    StatusCode r1 = my_oversscanf(s1, &count1, "%Ro", &v1);
    StatusCode r2 = my_oversscanf(s2, &count2, "%Ro", &v2);
    EXPECT_EQ(r1, SUCCESS);
    EXPECT_EQ(r2, SUCCESS);
    EXPECT_EQ(count1, 1);
    EXPECT_EQ(count2, 1);
    EXPECT_EQ(v1, 1994);
    EXPECT_EQ(v2, 1994);

    const char *s3 = "123";
    unsigned int out3 = 0;
    int count3 = 0;
    StatusCode r3 = my_oversscanf(s3, &count3, "%Ro", &out3);
    EXPECT_EQ(r3, INVALID_ROMAN);
    EXPECT_EQ(count3, 0);
    EXPECT_EQ(out3, 0);
}

TEST(Oversscanf, Zr) {
    const char *s0 = "1";
    unsigned int out0 = 123;
    int count0 = 0;
    StatusCode r0 = my_oversscanf(s0, &count0, "%Zr", &out0);
    EXPECT_EQ(r0, INVALID_ZECKENDORF);
    EXPECT_EQ(count0, 0);
    EXPECT_EQ(out0, 123);

    const char *s1 = "1011";
    unsigned int out1 = 0;
    int count1 = 0;
    StatusCode r1 = my_oversscanf(s1, &count1, "%Zr", &out1);
    EXPECT_EQ(r1, SUCCESS);
    EXPECT_EQ(count1, 1);
    EXPECT_EQ(out1, 4);

    const char *s2 = "0000000000011";
    unsigned int out2 = 0;
    int count2 = 0;
    StatusCode r2 = my_oversscanf(s2, &count2, "%Zr", &out2);
    EXPECT_EQ(r2, SUCCESS);
    EXPECT_EQ(count2, 1);
    EXPECT_EQ(out2, 233);

    const char *s3 = "0000000000010";
    unsigned int out3 = 0;
    int count3 = 0;
    StatusCode r3 = my_oversscanf(s3, &count3, "%Zr", &out3);
    EXPECT_EQ(r3, INVALID_ZECKENDORF);
    EXPECT_EQ(count3, 0);
    EXPECT_EQ(out3, 0);

    const char *s4 = "101101011";
    unsigned int out4 = 0;
    int count4 = 0;
    StatusCode r4 = my_oversscanf(s4, &count4, "%Zr", &out4);
    EXPECT_EQ(r4, INVALID_ZECKENDORF);
    EXPECT_EQ(count4, 0);
    EXPECT_EQ(out4, 0);
}

TEST(Oversscanf, Cv) {
    int dest1 = 0;
    int dest2 = 0;
    const char *s1 = "1a";
    int count1 = 0, count2 = 0;
    StatusCode r1 = my_oversscanf(s1, &count1, "%Cv", &dest1, 16);
    EXPECT_EQ(r1, SUCCESS);
    EXPECT_EQ(count1, 1);
    EXPECT_EQ(dest1, 26);

    const char *s2 = "1A";
    StatusCode r2 = my_oversscanf(s2, &count2, "%CV", &dest2, 16);
    EXPECT_EQ(r2, SUCCESS);
    EXPECT_EQ(count2, 1);
    EXPECT_EQ(dest2, 26);

    int dest3 = 0;
    const char *s3 = "42";
    int count3 = 0;
    StatusCode r3 = my_oversscanf(s3, &count3, "%Cv", &dest3, 8);
    EXPECT_EQ(r3, SUCCESS);
    EXPECT_EQ(count3, 1);
    EXPECT_EQ(dest3, 34);

    int dest4 = 0;
    const char *s4 = "52";
    int count4 = 0;
    StatusCode r4 = my_oversscanf(s4, &count4, "%CV", &dest4, 6);
    EXPECT_EQ(r4, SUCCESS);
    EXPECT_EQ(count4, 1);
    EXPECT_EQ(dest4, 32);

    int dest5 = 0;
    const char *s5 = "88";
    int count5 = 0;
    StatusCode r5 = my_oversscanf(s5, &count5, "%CV", &dest5, 6);
    EXPECT_EQ(r5, INVALID_BASE);
    EXPECT_EQ(count5, 0);
    EXPECT_EQ(dest5, 0);

    int dest6 = 0;
    const char *s6 = "88";
    int count6 = 0;
    StatusCode r6 = my_oversscanf(s6, &count6, "%Cv", &dest6, 6);
    EXPECT_EQ(r6, INVALID_BASE);
    EXPECT_EQ(count6, 0);
    EXPECT_EQ(dest6, 0);
}

TEST(Overfscanf, ScanFile) {
    FILE *f = fopen("test_input.txt", "w+");
    ASSERT_NE(f, nullptr);
    fprintf(f, "MCM 1011 2a 2A\n");
    rewind(f);
    int rv = 0;
    unsigned int zr = 0;
    int cv_low = 0, cv_up = 0;
    int count = 0;
    StatusCode status = my_overfscanf(f, &count, "%Ro %Zr %Cv %CV", &rv, &zr, &cv_low, 16, &cv_up, 16);
    EXPECT_EQ(status, SUCCESS);
    EXPECT_EQ(count, 4);
    EXPECT_EQ(rv, 1900);
    EXPECT_EQ(zr, 4u);
    EXPECT_EQ(cv_low, 42);
    EXPECT_EQ(cv_up, 42);
    fclose(f);
}

TEST(Standard, Integer) {
    const char *input_int = "42 -123 0 +999";
    int a, b, c, d;
    int count1 = 0;
    StatusCode status1 = my_oversscanf(input_int, &count1, "%d %d %d %d", &a, &b, &c, &d);
    EXPECT_EQ(status1, SUCCESS);
    EXPECT_EQ(count1, 4);
    EXPECT_EQ(a, 42);
    EXPECT_EQ(b, -123);
    EXPECT_EQ(c, 0);
    EXPECT_EQ(d, 999);

    FILE *f = fopen("test_int.txt", "w+");
    fprintf(f, "100 -200 300");
    rewind(f);
    int x, y, z;
    int count2 = 0;
    StatusCode status2 = my_overfscanf(f, &count2, "%d %d %d", &x, &y, &z);
    fclose(f);
    EXPECT_EQ(status2, SUCCESS);
    EXPECT_EQ(count2, 3);
    EXPECT_EQ(x, 100);
    EXPECT_EQ(y, -200);
    EXPECT_EQ(z, 300);
}

TEST(Standard, UnsignedInteger) {
    const char *input_uint = "42 123 999 0";
    unsigned int a, b, c, d;
    int count1 = 0;
    StatusCode status1 = my_oversscanf(input_uint, &count1, "%u %u %u %u", &a, &b, &c, &d);
    EXPECT_EQ(status1, SUCCESS);
    EXPECT_EQ(count1, 4);
    EXPECT_EQ(a, 42u);
    EXPECT_EQ(b, 123u);
    EXPECT_EQ(c, 999u);
    EXPECT_EQ(d, 0u);

    FILE *f = fopen("test_uint.txt", "w+");
    fprintf(f, "100 200 300");
    rewind(f);
    unsigned int x, y, z;
    int count2 = 0;
    StatusCode status2 = my_overfscanf(f, &count2, "%u %u %u", &x, &y, &z);
    fclose(f);
    EXPECT_EQ(status2, SUCCESS);
    EXPECT_EQ(count2, 3);
    EXPECT_EQ(x, 100u);
    EXPECT_EQ(y, 200u);
    EXPECT_EQ(z, 300u);
}

TEST(Standard, Character) {
    const char *input_char = "A b C";
    char a, b, c;
    int count1 = 0;
    StatusCode status1 = my_oversscanf(input_char, &count1, "%c %c %c", &a, &b, &c);
    EXPECT_EQ(status1, SUCCESS);
    EXPECT_EQ(count1, 3);
    EXPECT_EQ(a, 'A');
    EXPECT_EQ(b, 'b');
    EXPECT_EQ(c, 'C');

    FILE *f = fopen("test_char.txt", "w+");
    fprintf(f, "X y Z");
    rewind(f);
    char x, y, z;
    int count2 = 0;
    StatusCode status2 = my_overfscanf(f, &count2, "%c %c %c", &x, &y, &z);
    fclose(f);
    EXPECT_EQ(status2, SUCCESS);
    EXPECT_EQ(count2, 3);
    EXPECT_EQ(x, 'X');
    EXPECT_EQ(y, 'y');
    EXPECT_EQ(z, 'Z');
}

TEST(Standard, String) {
    const char *input_str = "Hello World Test";
    char str1[20], str2[20], str3[20];
    int count1 = 0;
    StatusCode status1 = my_oversscanf(input_str, &count1, "%s %s %s", str1, str2, str3);
    EXPECT_EQ(status1, SUCCESS);
    EXPECT_EQ(count1, 3);
    EXPECT_STREQ(str1, "Hello");
    EXPECT_STREQ(str2, "World");
    EXPECT_STREQ(str3, "Test");

    FILE *f = fopen("test_str.txt", "w+");
    fprintf(f, "Programming C Language");
    rewind(f);
    char s1[20], s2[20], s3[20];
    int count2 = 0;
    StatusCode status2 = my_overfscanf(f, &count2, "%s %s %s", s1, s2, s3);
    fclose(f);
    EXPECT_EQ(status2, SUCCESS);
    EXPECT_EQ(count2, 3);
    EXPECT_STREQ(s1, "Programming");
    EXPECT_STREQ(s2, "C");
    EXPECT_STREQ(s3, "Language");
}

TEST(Standard, Mixed) {
    const char *input_mixed = "42 3.14 Hello X";
    int a;
    float b;
    char str[20];
    char ch;
    int count1 = 0;
    StatusCode status1 = my_oversscanf(input_mixed, &count1, "%d %f %s %c", &a, &b, str, &ch);
    EXPECT_EQ(status1, SUCCESS);
    EXPECT_EQ(count1, 4);
    EXPECT_EQ(a, 42);
    EXPECT_FLOAT_EQ(b, 3.14f);
    EXPECT_STREQ(str, "Hello");
    EXPECT_EQ(ch, 'X');

    FILE *f = fopen("test_mixed.txt", "w+");
    fprintf(f, "100 2.718 Test Y");
    rewind(f);
    int x;
    float y;
    char s[20];
    char c;
    int count2 = 0;
    StatusCode status2 = my_overfscanf(f, &count2, "%d %f %s %c", &x, &y, s, &c);
    fclose(f);
    EXPECT_EQ(status2, SUCCESS);
    EXPECT_EQ(count2, 4);
    EXPECT_EQ(x, 100);
    EXPECT_FLOAT_EQ(y, 2.718f);
    EXPECT_STREQ(s, "Test");
    EXPECT_EQ(c, 'Y');
}

TEST(Standard, Special) {
    const char *empty = "";
    int a;
    int count1 = 0;
    StatusCode status1 = my_oversscanf(empty, &count1, "%d", &a);
    EXPECT_EQ(status1, SCAN_ERROR);
    EXPECT_EQ(count1, 0);

    const char *partial = "123";
    int x, y;
    int count2 = 0;
    StatusCode status2 = my_oversscanf(partial, &count2, "%d %d", &x, &y);
    EXPECT_EQ(status2, SUCCESS);
    EXPECT_EQ(count2, 1);
    EXPECT_EQ(x, 123);

    const char *wrong_type = "abc";
    int num;
    int count3 = 0;
    StatusCode status3 = my_oversscanf(wrong_type, &count3, "%d", &num);
    EXPECT_EQ(status3, SCAN_ERROR);
    EXPECT_EQ(count3, 0);

    const char *with_spaces = "   42   hello   ";
    int n;
    char text[20];
    int count4 = 0;
    StatusCode status4 = my_oversscanf(with_spaces, &count4, "%d %s", &n, text);
    EXPECT_EQ(status4, SUCCESS);
    EXPECT_EQ(count4, 2);
    EXPECT_EQ(n, 42);
    EXPECT_STREQ(text, "hello");
}