#include <gtest/gtest.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdarg>
#include <fstream>


extern "C" {
#include "../include/utils.h"
}

TEST(OverSprintfTests, Ro) {
    char buf[256];
    StatusCode status = my_oversprintf(buf, sizeof(buf), "X=%Ro", 1999);
    EXPECT_EQ(status, SUCCESS);
    EXPECT_STREQ(buf, "X=MCMXCIX");
    
    status = my_oversprintf(buf, sizeof(buf), "%Ro", -4);
    EXPECT_EQ(status, SUCCESS);
    EXPECT_STREQ(buf, "-IV");
    
    status = my_oversprintf(buf, sizeof(buf), "%Ro", 0);
    EXPECT_EQ(status, SUCCESS);
    EXPECT_STREQ(buf, "0");
}

TEST(OverSprintfTests, Zr) {
    char buf[256];
    StatusCode status;
    
    status = my_oversprintf(buf, sizeof(buf), "Zr: %Zr", 0u);
    EXPECT_EQ(status, SUCCESS);
    EXPECT_STREQ(buf, "Zr: 01");
    
    status = my_oversprintf(buf, sizeof(buf), "%Zr", 1u);
    EXPECT_EQ(status, SUCCESS);
    EXPECT_STREQ(buf, "11");
    
    status = my_oversprintf(buf, sizeof(buf), "%Zr", 2u);
    EXPECT_EQ(status, SUCCESS);
    EXPECT_STREQ(buf, "011");
    
    status = my_oversprintf(buf, sizeof(buf), "%Zr", 4u);
    EXPECT_EQ(status, SUCCESS);
    EXPECT_STREQ(buf, "1011");
}

TEST(OverSprintfTests, CvCV) {
    char buf[256];
    StatusCode status;
    
    status = my_oversprintf(buf, sizeof(buf), "%Cv", 255, 16);
    EXPECT_EQ(status, SUCCESS);
    EXPECT_STREQ(buf, "ff");
    
    status = my_oversprintf(buf, sizeof(buf), "%CV", 255, 16);
    EXPECT_EQ(status, SUCCESS);
    EXPECT_STREQ(buf, "FF");
    
    status = my_oversprintf(buf, sizeof(buf), "%Cv", -15, 2);
    EXPECT_EQ(status, SUCCESS);
    EXPECT_STREQ(buf, "-1111");
    
    status = my_oversprintf(buf, sizeof(buf), "%Cv", 10, 8);
    EXPECT_EQ(status, SUCCESS);
    EXPECT_STREQ(buf, "12");

    status = my_oversprintf(buf, sizeof(buf), "%Cv", 102, 38);
    EXPECT_EQ(status, SUCCESS);
    EXPECT_STREQ(buf, "102");
}

TEST(OverSprintfTests, toTO) {
    char buf[256];
    StatusCode status;
    
    status = my_oversprintf(buf, sizeof(buf), "%to", "ff", 16);
    EXPECT_EQ(status, SUCCESS);
    EXPECT_STREQ(buf, "255");
    
    status = my_oversprintf(buf, sizeof(buf), "%to", "123", 1000);
    EXPECT_EQ(status, SUCCESS);
    EXPECT_STREQ(buf, "123");
    
    status = my_oversprintf(buf, sizeof(buf), "%to", "1010", 2);
    EXPECT_EQ(status, SUCCESS);
    EXPECT_STREQ(buf, "10");

    status = my_oversprintf(buf, sizeof(buf), "%to", "66", 5);
    EXPECT_EQ(status, INVALID_BASE);
    EXPECT_STREQ(buf, "10");

    status = my_oversprintf(buf, sizeof(buf), "%TO", "FF", 16);
    EXPECT_EQ(status, SUCCESS);
    EXPECT_STREQ(buf, "255");

    status = my_oversprintf(buf, sizeof(buf), "%TO", "2234", 5);
    EXPECT_EQ(status, SUCCESS);
    EXPECT_STREQ(buf, "319");

    status = my_oversprintf(buf, sizeof(buf), "%TO", "77", 4);
    EXPECT_EQ(status, INVALID_BASE);
    EXPECT_STREQ(buf, "319");

    status = my_oversprintf(buf, sizeof(buf), "%TO", "102", 38);
    EXPECT_EQ(status, SUCCESS);
    EXPECT_STREQ(buf, "102");
}

TEST(OverSprintfTests, MemDumpInts) {
    char buf[1024];
    StatusCode status;
    
    int x = 0x01020304;
    status = my_oversprintf(buf, sizeof(buf), "%mi", x);
    EXPECT_EQ(status, SUCCESS);
    EXPECT_GT(strlen(buf), 0);
    
    unsigned int ux = 0xAABBCCDDu;
    status = my_oversprintf(buf, sizeof(buf), "%mu", ux);
    EXPECT_EQ(status, SUCCESS);
    EXPECT_GT(strlen(buf), 0);
}

TEST(OverSprintfTests, MemDumpFloatDouble) {
    char buf[1024];
    StatusCode status;
    
    float f = 1.5f;
    status = my_oversprintf(buf, sizeof(buf), "%mf", f);
    EXPECT_EQ(status, SUCCESS);
    EXPECT_GT(strlen(buf), 0);
    
    double d = 3.125;
    status = my_oversprintf(buf, sizeof(buf), "%md", d);
    EXPECT_EQ(status, SUCCESS);
    EXPECT_GT(strlen(buf), 0);
}

TEST(OverSprintfTests, ErrorConditions) {
    char buf[256];
    StatusCode status;
    
    status = my_oversprintf(NULL, sizeof(buf), "%Ro", 10);
    EXPECT_EQ(status, NULL_POINTER_ERROR);
    
    status = my_oversprintf(buf, 0, "%Ro", 10);
    EXPECT_EQ(status, BUFFER_OVERFLOW);
    
    status = my_oversprintf(buf, sizeof(buf), NULL, 10);
    EXPECT_EQ(status, NULL_POINTER_ERROR);
    
    status = my_oversprintf(buf, 1, "%Ro", 1999);
    EXPECT_EQ(status, BUFFER_OVERFLOW);
}

TEST(OverSprintfTests, CombinedFormats) {
    char buf[1024];
    StatusCode status;
    
    status = my_oversprintf(buf, sizeof(buf), "Roman: %Ro, Zeck: %Zr, Hex: %Cv", 42, 25u, 255, 16);
    EXPECT_EQ(status, SUCCESS);
    EXPECT_GT(strlen(buf), 0);
    
    status = my_oversprintf(buf, sizeof(buf), "Dec: %to, Mem: %mi", "FF", 16, 100);
    EXPECT_EQ(status, SUCCESS);
    EXPECT_GT(strlen(buf), 0);
}

TEST(OverSprintfTests, Standard) {
    char buf[256];
    StatusCode status;
    
    status = my_oversprintf(buf, sizeof(buf), "Int: %d, String: %s", 42, "test");
    EXPECT_EQ(status, SUCCESS);
    EXPECT_STREQ(buf, "Int: 42, String: test");

    
    status = my_oversprintf(buf, sizeof(buf), "Value: %i", 123);
    EXPECT_EQ(status, SUCCESS);
    EXPECT_STREQ(buf, "Value: 123");

    status = my_oversprintf(buf, sizeof(buf), "|%5d|%-5d|%05d|", 42, 42, 42);
    EXPECT_EQ(status, SUCCESS);
    EXPECT_STREQ(buf, "|   42|42   |00042|");
    
    status = my_oversprintf(buf, sizeof(buf), "Hex: %x, Float: %.2f", 255, 3.14159);
    EXPECT_EQ(status, SUCCESS);
    EXPECT_STREQ(buf, "Hex: ff, Float: 3.14");

    status = my_oversprintf(buf, sizeof(buf), "HEX: %X", 255);
    EXPECT_EQ(status, SUCCESS);
    EXPECT_STREQ(buf, "HEX: FF");

    status = my_oversprintf(buf, sizeof(buf), "Unsigned: %u", 4294u);
    EXPECT_EQ(status, SUCCESS);
    EXPECT_STREQ(buf, "Unsigned: 4294");

    status = my_oversprintf(buf, sizeof(buf), "Octal: %o", 64);
    EXPECT_EQ(status, SUCCESS);
    EXPECT_STREQ(buf, "Octal: 100");

    status = my_oversprintf(buf, sizeof(buf), "Char: %c %c", 'A', 66);
    EXPECT_EQ(status, SUCCESS);
    EXPECT_STREQ(buf, "Char: A B");

    int x = 42;
    void* ptr = &x;
    
    status = my_oversprintf(buf, sizeof(buf), "Pointer: %p", ptr);
    EXPECT_EQ(status, SUCCESS);
    printf("%s", buf);
    EXPECT_TRUE(strstr(buf, "Pointer: 0") != nullptr);

    status = my_oversprintf(buf, sizeof(buf), "Percent: %% complete");
    EXPECT_EQ(status, SUCCESS);
    EXPECT_STREQ(buf, "Percent: % complete");
}


TEST(OverSprintfTests, StandardSpecial) {
    char buf[256];
    StatusCode status;
    
    status = my_oversprintf(buf, sizeof(buf), "");
    EXPECT_EQ(status, SUCCESS);
    EXPECT_STREQ(buf, "");
    
    status = my_oversprintf(buf, sizeof(buf), "Just plain text");
    EXPECT_EQ(status, SUCCESS);
    EXPECT_STREQ(buf, "Just plain text");
    
    status = my_oversprintf(buf, sizeof(buf), "%d%d%d%d%d", 1, 2, 3, 4, 5);
    EXPECT_EQ(status, SUCCESS);
    EXPECT_STREQ(buf, "12345");
}

TEST(OverSprintfTests, StandardError) {
    char buf[256];
    StatusCode status;
    
    status = my_oversprintf(NULL, sizeof(buf), "Test %d", 42);
    EXPECT_EQ(status, NULL_POINTER_ERROR);
    
    status = my_oversprintf(buf, sizeof(buf), NULL, 42);
    EXPECT_EQ(status, NULL_POINTER_ERROR);
    
    status = my_oversprintf(buf, 5, "Long string %d", 12345);
    EXPECT_EQ(status, BUFFER_OVERFLOW);
}

TEST(OverFprintfTests, All){
    FILE* file = fopen("output1.txt", "w");
    
    StatusCode status;

    int num = 5;
    unsigned int gol = 121;

    status = my_overfprintf(file, "num: %d\n", num);
    EXPECT_EQ(status, SUCCESS);

    status = my_overfprintf(file, "Римские цифры: %Ro\n", num);
    EXPECT_EQ(status, SUCCESS);
    
    status = my_overfprintf(file, "Цекендорф: %Zr\n", num);
    EXPECT_EQ(status, SUCCESS);

    status = my_overfprintf(file, "Шестнадцатеричное: %Cv\n", num, 16);
    EXPECT_EQ(status, SUCCESS);

    status = my_overfprintf(file, "Из двоичной(num=1011): %to\n", "1011", 2);
    EXPECT_EQ(status, SUCCESS);
    
    status = my_overfprintf(file, "Дамп int(5): %mi\n", num);
    EXPECT_EQ(status, SUCCESS);

    status = my_overfprintf(file, "Дамп unsigned int(121): %mu\n", gol);
    EXPECT_EQ(status, SUCCESS);

    status = my_overfprintf(file, "Дамп double(3.14): %md\n", 3.14);
    EXPECT_EQ(status, SUCCESS);

    status = my_overfprintf(file, "Дамп float(2.48f): %mf\n", 2.48f);
    EXPECT_EQ(status, SUCCESS);

    fclose(file);


    std::ifstream file_r("output1.txt");
    std::string content((std::istreambuf_iterator<char>(file_r)), std::istreambuf_iterator<char>());
    file_r.close();

    EXPECT_TRUE(content.find("num: 5") != std::string::npos);
    EXPECT_TRUE(content.find("Римские цифры: V") != std::string::npos);
    EXPECT_TRUE(content.find("Цекендорф: 00011") != std::string::npos);
    EXPECT_TRUE(content.find("Шестнадцатеричное: 5") != std::string::npos);
    EXPECT_TRUE(content.find("Из двоичной(num=1011): 11") != std::string::npos);
    EXPECT_TRUE(content.find("Дамп int(5): 00000101 00000000 00000000 00000000") != std::string::npos);
    EXPECT_TRUE(content.find("Дамп unsigned int(121): 01111001 00000000 00000000 00000000") != std::string::npos);
    EXPECT_TRUE(content.find("Дамп double(3.14): 00011111 10000101 11101011 01010001 10111000 00011110 00001001 01000000") != std::string::npos);
    EXPECT_TRUE(content.find("Дамп float(2.48f): 01010010 10111000 00011110 01000000") != std::string::npos);
}

TEST(OverFprintfTests, OnlyStandard) {
    const char* filename = "test_output_std.txt";
    FILE* file = fopen(filename, "w");
    ASSERT_NE(file, nullptr);
    
    StatusCode status = my_overfprintf(file, "Std: %d %s %.1f %c %%", 100, "output", 2.7, 'Z');
    EXPECT_EQ(status, SUCCESS);
    
    fclose(file);
    
    FILE* read_file = fopen(filename, "r");
    ASSERT_NE(read_file, nullptr);
    
    char buffer[256];
    fgets(buffer, sizeof(buffer), read_file);
    fclose(read_file);
    
    EXPECT_STREQ(buffer, "Std: 100 output 2.7 Z %");
    
    remove(filename);
}

