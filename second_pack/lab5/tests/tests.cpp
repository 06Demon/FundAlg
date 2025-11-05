#include <gtest/gtest.h>

extern "C" {
#include "../include/utils.h"
}

TEST(File1, File1) {
    const char* input_path = "../tests/inp/input1.txt";
    const char* output_path = "../tests/out/output1.txt";
    
    StatusCode result = process_file(input_path, output_path);
    EXPECT_EQ(result, SUCCESS);
}

TEST(File2, File2) {
    const char* input_path = "../tests/inp/input2.txt";
    const char* output_path = "../tests/out/output2.txt";
    
    StatusCode result = process_file(input_path, output_path);
    EXPECT_EQ(result, SUCCESS);
}

TEST(File3, File3) {
    const char* input_path = "../tests/inp/input3.txt";
    const char* output_path = "../tests/out/output3.txt";
    
    StatusCode result = process_file(input_path, output_path);
    EXPECT_EQ(result, SUCCESS);
}

TEST(File4, LargeWord) {
    const char* input_path = "../tests/inp/input4.txt";
    const char* output_path = "../tests/out/output4.txt";
    
    StatusCode result = process_file(input_path, output_path);
    EXPECT_EQ(result, ERROR_LARGE_WORD);
}

TEST(File5, Word80symbols) {
    const char* input_path = "../tests/inp/input5.txt";
    const char* output_path = "../tests/out/output5.txt";
    
    StatusCode result = process_file(input_path, output_path);
    EXPECT_EQ(result, SUCCESS);
}