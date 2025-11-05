#include <gtest/gtest.h>
#include <fstream>

extern "C" {
#include "../include/utils.h"
}


TEST(Test, LoadFileSuccess) {
    Student *students = nullptr;
    int count = 0;
    StatusCode status = load_students("../tests/test_data.txt", &students, &count);
    
    EXPECT_EQ(status, SUCCESS);
    EXPECT_EQ(count, 5);
    EXPECT_NE(students, nullptr);
    
    if (students) {
        EXPECT_EQ(students[0].id, 1);
        EXPECT_STREQ(students[0].first_name, "John");
        EXPECT_STREQ(students[0].last_name, "Smith");
        EXPECT_STREQ(students[0].group, "CS101");
        
        free_students(students, count);
    }
}

TEST(Test, nonexistentFile) {
    Student *students = nullptr;
    int count = 0;
    StatusCode status = load_students("nonexistent.txt", &students, &count);
    
    EXPECT_EQ(status, OPEN_FILE_ERROR);
    EXPECT_EQ(students, nullptr);
}

TEST(Test, ValidateName) {
    EXPECT_TRUE(validate_name("John"));
    EXPECT_TRUE(validate_name("Alice"));
    EXPECT_FALSE(validate_name("John123"));
    EXPECT_FALSE(validate_name(""));
    EXPECT_FALSE(validate_name(nullptr));
}

TEST(Test, ValidateGroup) {
    EXPECT_TRUE(validate_group("CS101"));
    EXPECT_TRUE(validate_group("GroupA"));
    EXPECT_TRUE(validate_group("123"));
    EXPECT_TRUE(validate_group("212BV"));
    EXPECT_FALSE(validate_group(""));
    EXPECT_FALSE(validate_group(nullptr));
}

TEST(Test, Average) {
    unsigned char grades[] = {80, 90, 70, 85, 95};
    double avg = calculate_average(grades);
    EXPECT_NEAR(avg, 84.0, EPS);
    
    EXPECT_EQ(calculate_average(nullptr), 0.0);
}

TEST(Test, FindByIdSuccess) {
    Student *students = nullptr;
    int count = 0;
    load_students("../tests/test_data.txt", &students, &count);
    
    Student *found = nullptr;
    int found_count = 0;
    StatusCode status = find_by_id(students, count, 2, &found, &found_count);
    
    EXPECT_EQ(status, SUCCESS);
    EXPECT_EQ(found_count, 1);
    EXPECT_NE(found, nullptr);
    
    if (found) {
        EXPECT_EQ(found[0].id, 2);
        EXPECT_STREQ(found[0].first_name, "Jane");
        free_students(found, found_count);
    }
    
    free_students(students, count);
}

TEST(Test, FindByIdNotFound) {
    Student *students = nullptr;
    int count = 0;
    load_students("../tests/test_data.txt", &students, &count);
    
    Student *found = nullptr;
    int found_count = 0;
    StatusCode status = find_by_id(students, count, 999, &found, &found_count);
    
    EXPECT_EQ(status, NOT_FOUND);
    EXPECT_EQ(found, nullptr);
    
    free_students(students, count);
}

TEST(Test, FindByLastNameSuccess) {
    Student *students = nullptr;
    int count = 0;
    load_students("../tests/test_data.txt", &students, &count);
    
    Student *found = nullptr;
    int found_count = 0;
    StatusCode status = find_by_last_name(students, count, "Doe", &found, &found_count);
    
    EXPECT_EQ(status, SUCCESS);
    EXPECT_EQ(found_count, 1);
    EXPECT_NE(found, nullptr);
    
    if (found) {
        EXPECT_STREQ(found[0].last_name, "Doe");
        free_students(found, found_count);
    }
    
    free_students(students, count);
}

TEST(Test, FindByLastNotFound) {
    Student *students = nullptr;
    int count = 0;
    load_students("../tests/test_data.txt", &students, &count);
    
    Student *found = nullptr;
    int found_count = 0;
    StatusCode status = find_by_last_name(students, count, "Bbb", &found, &found_count);
    
    EXPECT_EQ(status, NOT_FOUND);
    EXPECT_EQ(found_count, 0);
    EXPECT_EQ(found, nullptr);

    free_students(students, count);
}

TEST(Test, FindByFirstNameSuccess) {
    Student *students = nullptr;
    int count = 0;
    load_students("../tests/test_data.txt", &students, &count);
    
    Student *found = nullptr;
    int found_count = 0;
    StatusCode status = find_by_first_name(students, count, "Bob", &found, &found_count);
    
    EXPECT_EQ(status, SUCCESS);
    EXPECT_EQ(found_count, 1);
    EXPECT_NE(found, nullptr);
    
    if (found) {
        EXPECT_STREQ(found[0].first_name, "Bob");
        free_students(found, found_count);
    }
    
    free_students(students, count);
}

TEST(Test, FindByFirstNameNotFound) {
    Student *students = nullptr;
    int count = 0;
    load_students("../tests/test_data.txt", &students, &count);
    
    Student *found = nullptr;
    int found_count = 0;
    StatusCode status = find_by_first_name(students, count, "GOL", &found, &found_count);
    
    EXPECT_EQ(status, NOT_FOUND);
    EXPECT_EQ(found_count, 0);
    EXPECT_EQ(found, nullptr);
    
    free_students(students, count);
}

TEST(Test, FindByGroupSuccess) {
    Student *students = nullptr;
    int count = 0;
    load_students("../tests/test_data.txt", &students, &count);
    
    Student *found = nullptr;
    int found_count = 0;
    StatusCode status = find_by_group(students, count, "CS101", &found, &found_count);
    
    EXPECT_EQ(status, SUCCESS);
    EXPECT_EQ(found_count, 2);
    EXPECT_NE(found, nullptr);
    
    if (found) {
        for (int i = 0; i < found_count; i++) {
            EXPECT_STREQ(found[i].group, "CS101");
        }
        free_students(found, found_count);
    }
    
    free_students(students, count);
}

TEST(Test, FindByGroupNotFound) {
    Student *students = nullptr;
    int count = 0;
    load_students("../tests/test_data.txt", &students, &count);
    
    Student *found = nullptr;
    int found_count = 0;
    StatusCode status = find_by_group(students, count, "NE_GOL1", &found, &found_count);
    
    EXPECT_EQ(status, NOT_FOUND);
    EXPECT_EQ(found_count, 0);
    EXPECT_EQ(found, nullptr);
    
    free_students(students, count);
}

TEST(Test, SortById) {
    Student *students = nullptr;
    int count = 0;
    load_students("../tests/test_data.txt", &students, &count);
    
    Student *temp = (Student*)malloc(count * sizeof(Student));
    for (int i = 0; i < count; i++) {
        temp[i] = students[i];
        temp[i].grades = (unsigned char*)malloc(GRADES_COUNT * sizeof(unsigned char));
        memcpy(temp[i].grades, students[i].grades, GRADES_COUNT * sizeof(unsigned char));
    }
    
    sort_students(temp, count, compare_id);
    
    EXPECT_EQ(temp[0].id, 1);
    EXPECT_EQ(temp[1].id, 2);
    EXPECT_EQ(temp[2].id, 3);
    EXPECT_EQ(temp[3].id, 4);
    EXPECT_EQ(temp[4].id, 5);
    
    free_students(temp, count);
    free_students(students, count);
}

TEST(Test, SortByLastName) {
    Student *students = nullptr;
    int count = 0;
    load_students("../tests/test_data.txt", &students, &count);
    
    Student *temp = (Student*)malloc(count * sizeof(Student));
    for (int i = 0; i < count; i++) {
        temp[i] = students[i];
        temp[i].grades = (unsigned char*)malloc(GRADES_COUNT * sizeof(unsigned char));
        memcpy(temp[i].grades, students[i].grades, GRADES_COUNT * sizeof(unsigned char));
    }
    
    sort_students(temp, count, compare_last_name);
    
    EXPECT_STREQ(temp[0].last_name, "Brown");
    EXPECT_STREQ(temp[1].last_name, "Davis");
    EXPECT_STREQ(temp[2].last_name, "Doe");
    EXPECT_STREQ(temp[3].last_name, "Johnson");
    EXPECT_STREQ(temp[4].last_name, "Smith");
    
    free_students(temp, count);
    free_students(students, count);
}

TEST(Test, SaveStudentById) {
    Student *students = nullptr;
    int count = 0;
    load_students("../tests/test_data.txt", &students, &count);
    
    StatusCode status = save_student_by_id(students, count, 1, "output1.txt");
    
    EXPECT_EQ(status, SUCCESS);
    
    std::ifstream file("output1.txt");
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    EXPECT_TRUE(content.find("John Smith") != std::string::npos);
    EXPECT_TRUE(content.find("CS101") != std::string::npos);
    
    free_students(students, count);
}

TEST(Test, SaveTopStudents) {
    Student *students = nullptr;
    int count = 0;
    load_students("../tests/test_data.txt", &students, &count);
    
    StatusCode status = save_top_students(students, count, "output2.txt");
    
    EXPECT_EQ(status, SUCCESS);
    
    std::ifstream file("output2.txt");
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    
    EXPECT_TRUE(content.find("Doe Jane") != std::string::npos);
    EXPECT_TRUE(content.find("Davis Mike") != std::string::npos);
    EXPECT_TRUE(content.find("Brown Alice") != std::string::npos);
    
    free_students(students, count);
}

TEST(Test, NullPointer) {
    Student *students = nullptr;
    int count = 0;
    
    EXPECT_EQ(load_students(nullptr, &students, &count), NULL_POINTER);
    EXPECT_EQ(find_by_id(nullptr, 0, 0, nullptr, nullptr), NULL_POINTER);
    EXPECT_EQ(save_student_by_id(nullptr, 0, 0, nullptr), NULL_POINTER);
    
    free_students(nullptr, 0);
}

TEST(Test, InvalidInput) {
    Student *students = nullptr;
    int count = 0;
    load_students("../tests/test_data.txt", &students, &count);
    
    Student *found = nullptr;
    int found_count = 0;
    
    EXPECT_EQ(find_by_id(students, 0, 1, &found, &found_count), INVALID_INPUT);
    EXPECT_EQ(find_by_last_name(students, count, "Invalid123", &found, &found_count), INVALID_NAME);
    EXPECT_EQ(find_by_group(students, count, "", &found, &found_count), INVALID_GROUP);
    
    free_students(students, count);
}

TEST(Test, CompareFunctions) {
    Student s1 = {1, "A", "Aa", "CS101", nullptr};
    Student s2 = {2, "B", "Bb", "CS102", nullptr};
    
    EXPECT_LT(compare_id(&s1, &s2), 0);
    EXPECT_LT(compare_last_name(&s1, &s2), 0);
    EXPECT_LT(compare_first_name(&s1, &s2), 0);
    EXPECT_LT(compare_group(&s1, &s2), 0);
}

TEST(Test, DuplicateId) {
    Student *students = nullptr;
    int count = 0;
    StatusCode status = load_students("../tests/duplicate.txt", &students, &count);
    
    EXPECT_EQ(status, DUPLICATE_ID);
    EXPECT_EQ(students, nullptr);
}
