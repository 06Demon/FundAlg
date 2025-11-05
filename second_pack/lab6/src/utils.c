#include "../include/utils.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

StatusCode load_students(const char *filename, Student **students, int *count) {
    if (!filename || !students || !count) return NULL_POINTER;
    
    FILE *file = fopen(filename, "r");
    if (!file) return OPEN_FILE_ERROR;

    Student *temp = NULL;
    int capacity = 10;
    int size = 0;
    temp = malloc(capacity * sizeof(Student));
    if (!temp) {
        fclose(file);
        return MEMORY_ALLOCATION;
    }

    while (!feof(file)) {
        if (size >= capacity) {
            capacity *= 2;
            Student *new_temp = realloc(temp, capacity * sizeof(Student));
            if (!new_temp) {
                free_students(temp, size);
                fclose(file);
                return MEMORY_ALLOCATION;
            }
            temp = new_temp;
        }

        Student *s = &temp[size];
        s->grades = malloc(GRADES_COUNT * sizeof(unsigned char));
        if (!s->grades) {
            free_students(temp, size);
            fclose(file);
            return MEMORY_ALLOCATION;
        }

        if (fscanf(file, "%u %49s %49s %19s", &s->id, s->first_name, s->last_name, s->group) != 4) {
            free(s->grades);
            return INVALID_DATA;
        }

        for (int j = 0; j < size; j++) {
            if (temp[j].id == s->id) {
                free(s->grades);
                free_students(temp, size);
                fclose(file);
                return DUPLICATE_ID;
            }
        }

        for (int i = 0; i < GRADES_COUNT; i++) {
            unsigned grade;
            if (fscanf(file, "%u", &grade) != 1 || grade > 100) {
                free_students(temp, size);
                fclose(file);
                return INVALID_DATA;
            }
            s->grades[i] = (unsigned char)grade;
        }

        if (!validate_name(s->first_name) || !validate_name(s->last_name) || 
            !validate_group(s->group) || !validate_grades(s->grades)) {
            free_students(temp, size);
            fclose(file);
            return INVALID_DATA;
        }

        size++;
    }

    fclose(file);
    *students = temp;
    *count = size;
    return SUCCESS;
}

void free_students(Student *students, int count) {
    if (!students) return;
    
    for (int i = 0; i < count; i++) {
        free(students[i].grades);
    }
    free(students);
}

StatusCode find_by_id(const Student *students, int count, unsigned int id, Student **result, int *result_count) {
    if (!students || !result || !result_count) return NULL_POINTER;
    if (count <= 0) return INVALID_INPUT;

    Student *found = malloc(count * sizeof(Student));
    if (!found) return MEMORY_ALLOCATION;

    int found_count = 0;
    for (int i = 0; i < count; i++) {
        if (students[i].id == id) {
            found[found_count] = students[i];
            found[found_count].grades = malloc(GRADES_COUNT * sizeof(unsigned char));
            if (!found[found_count].grades) {
                for (int j = 0; j < found_count; j++) free(found[j].grades);
                free(found);
                return MEMORY_ALLOCATION;
            }
            memcpy(found[found_count].grades, students[i].grades, GRADES_COUNT * sizeof(unsigned char));
            found_count++;
        }
    }

    if (found_count == 0) {
        free(found);
        return NOT_FOUND;
    }

    *result = found;
    *result_count = found_count;
    return SUCCESS;
}

StatusCode find_by_last_name(const Student *students, int count, const char *last_name, Student **result, int *result_count) {
    if (!students || !last_name || !result || !result_count) return NULL_POINTER;
    if (count <= 0) return INVALID_INPUT;
    if (!validate_name(last_name)) return INVALID_NAME;

    Student *found = malloc(count * sizeof(Student));
    if (!found) return MEMORY_ALLOCATION;

    int found_count = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].last_name, last_name) == 0) {
            found[found_count] = students[i];
            found[found_count].grades = malloc(GRADES_COUNT * sizeof(unsigned char));
            if (!found[found_count].grades) {
                for (int j = 0; j < found_count; j++) free(found[j].grades);
                free(found);
                return MEMORY_ALLOCATION;
            }
            memcpy(found[found_count].grades, students[i].grades, GRADES_COUNT * sizeof(unsigned char));
            found_count++;
        }
    }

    if (found_count == 0) {
        free(found);
        return NOT_FOUND;
    }

    *result = found;
    *result_count = found_count;
    return SUCCESS;
}

StatusCode find_by_first_name(const Student *students, int count, const char *first_name, Student **result, int *result_count) {
    if (!students || !first_name || !result || !result_count) return NULL_POINTER;
    if (count <= 0) return INVALID_INPUT;
    if (!validate_name(first_name)) return INVALID_NAME;

    Student *found = malloc(count * sizeof(Student));
    if (!found) return MEMORY_ALLOCATION;

    int found_count = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].first_name, first_name) == 0) {
            found[found_count] = students[i];
            found[found_count].grades = malloc(GRADES_COUNT * sizeof(unsigned char));
            if (!found[found_count].grades) {
                for (int j = 0; j < found_count; j++) free(found[j].grades);
                free(found);
                return MEMORY_ALLOCATION;
            }
            memcpy(found[found_count].grades, students[i].grades, GRADES_COUNT * sizeof(unsigned char));
            found_count++;
        }
    }

    if (found_count == 0) {
        free(found);
        return NOT_FOUND;
    }

    *result = found;
    *result_count = found_count;
    return SUCCESS;
}

StatusCode find_by_group(const Student *students, int count, const char *group, Student **result, int *result_count) {
    if (!students || !group || !result || !result_count) return NULL_POINTER;
    if (count <= 0) return INVALID_INPUT;
    if (!validate_group(group)) return INVALID_GROUP;

    Student *found = malloc(count * sizeof(Student));
    if (!found) return MEMORY_ALLOCATION;

    int found_count = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(students[i].group, group) == 0) {
            found[found_count] = students[i];
            found[found_count].grades = malloc(GRADES_COUNT * sizeof(unsigned char));
            if (!found[found_count].grades) {
                for (int j = 0; j < found_count; j++) free(found[j].grades);
                free(found);
                return MEMORY_ALLOCATION;
            }
            memcpy(found[found_count].grades, students[i].grades, GRADES_COUNT * sizeof(unsigned char));
            found_count++;
        }
    }

    if (found_count == 0) {
        free(found);
        return NOT_FOUND;
    }

    *result = found;
    *result_count = found_count;
    return SUCCESS;
}

void sort_students(Student *students, int count, int (*comparator)(const void*, const void*)) {
    if (!students || count <= 0 || !comparator) return;
    qsort(students, count, sizeof(Student), comparator);
}

int compare_id(const void *a, const void *b) {
    const Student *sa = (const Student*)a;
    const Student *sb = (const Student*)b;
    return (sa->id > sb->id) - (sa->id < sb->id);
}

int compare_last_name(const void *a, const void *b) {
    const Student *sa = (const Student*)a;
    const Student *sb = (const Student*)b;
    return strcmp(sa->last_name, sb->last_name);
}

int compare_first_name(const void *a, const void *b) {
    const Student *sa = (const Student*)a;
    const Student *sb = (const Student*)b;
    return strcmp(sa->first_name, sb->first_name);
}

int compare_group(const void *a, const void *b) {
    const Student *sa = (const Student*)a;
    const Student *sb = (const Student*)b;
    return strcmp(sa->group, sb->group);
}

StatusCode save_student_by_id(const Student *students, int count, unsigned int id, const char *filename) {
    if (!students || !filename) return NULL_POINTER;
    if (count <= 0) return INVALID_INPUT;

    FILE *file = fopen(filename, "w");
    if (!file) return OPEN_FILE_ERROR;

    StatusCode status = NOT_FOUND;
    for (int i = 0; i < count; i++) {
        if (students[i].id == id) {
            double avg = calculate_average(students[i].grades);
            fprintf(file, "Name: %s %s\nGroup: %s\nAverage grade: %.2lf\n", 
                   students[i].first_name, students[i].last_name, students[i].group, avg);
            status = SUCCESS;
            break;
        }
    }

    fclose(file);
    return status;
}

StatusCode save_top_students(const Student *students, int count, const char *filename) {
    if (!students || !filename) return NULL_POINTER;
    if (count == 0) return INVALID_DATA;

    double total_avg = 0;
    for (int i = 0; i < count; i++) {
        total_avg += calculate_average(students[i].grades);
    }
    total_avg /= count;

    FILE *file = fopen(filename, "w");
    if (!file) return OPEN_FILE_ERROR;

    int found_any = 0;
    for (int i = 0; i < count; i++) {
        double avg = calculate_average(students[i].grades);
        if (avg > total_avg + EPS) {
            fprintf(file, "%s %s (Average: %.2lf)\n", students[i].last_name, students[i].first_name, avg);
            found_any = 1;
        }
    }

    fclose(file);
    return found_any ? SUCCESS : NOT_FOUND;
}

double calculate_average(const unsigned char *grades) {
    if (!grades) return 0.0;
    
    double sum = 0;
    for (int i = 0; i < GRADES_COUNT; i++) {
        sum += grades[i];
    }
    return sum / GRADES_COUNT;
}

int validate_name(const char *name) {
    if (!name || strlen(name) == 0) return 0;
    
    for (size_t i = 0; i < strlen(name); i++) {
        if (!isalpha(name[i])) return 0;
    }
    return 1;
}

int validate_group(const char *group) {
    return group && strlen(group) > 0;
}

int validate_grades(const unsigned char *grades) {
    if (!grades) return 0;
    
    for (int i = 0; i < GRADES_COUNT; i++) {
        if (grades[i] > 100) return 0;
    }
    return 1;
}