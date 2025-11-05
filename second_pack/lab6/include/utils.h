#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#define MAX_NAME_LEN 50
#define MAX_GROUP_LEN 20
#define GRADES_COUNT 5
#define EPS 1e-6

typedef struct {
    unsigned int id;
    char first_name[MAX_NAME_LEN];
    char last_name[MAX_NAME_LEN];
    char group[MAX_GROUP_LEN];
    unsigned char *grades;
} Student;

typedef enum {
    SUCCESS,
    MEMORY_ALLOCATION,
    OPEN_FILE_ERROR,
    INVALID_DATA,
    INVALID_INPUT,
    NOT_FOUND,
    INVALID_ID,
    INVALID_NAME,
    INVALID_GROUP,
    NULL_POINTER,
    DUPLICATE_ID 
} StatusCode;

StatusCode load_students(const char *filename, Student **students, int *count);
void free_students(Student *students, int count);

StatusCode find_by_id(const Student *students, int count, unsigned int id, Student **result, int *result_count);
StatusCode find_by_last_name(const Student *students, int count, const char *last_name, Student **result, int *result_count);
StatusCode find_by_first_name(const Student *students, int count, const char *first_name, Student **result, int *result_count);
StatusCode find_by_group(const Student *students, int count, const char *group, Student **result, int *result_count);

void sort_students(Student *students, int count, int (*comparator)(const void*, const void*));
int compare_id(const void *a, const void *b);
int compare_last_name(const void *a, const void *b);
int compare_first_name(const void *a, const void *b);
int compare_group(const void *a, const void *b);

StatusCode save_student_by_id(const Student *students, int count, unsigned int id, const char *filename);
StatusCode save_top_students(const Student *students, int count, const char *filename);

double calculate_average(const unsigned char *grades);
int validate_name(const char *name);
int validate_group(const char *group);
int validate_grades(const unsigned char *grades);

#endif