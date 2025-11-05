#include "include/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#endif

void print_menu() {
    printf("\n---------------------------------\n");
    printf("1. Найти студента по ID\n");
    printf("2. Найти студента по фамилии\n");
    printf("3. Найти студента по имени\n");
    printf("4. Найти студентов по группе\n");
    printf("5. Сортировать студентов по ID\n");
    printf("6. Сортировать студентов по фамилии\n");
    printf("7. Сортировать студентов по имени\n");
    printf("8. Сортировать студентов по группе\n");
    printf("9. Сохранить информацию о студенте по ID в файл\n");
    printf("10. Сохранить лучших студентов в файл\n");
    printf("11. Показать всех студентов\n");
    printf("0. Выход\n");
    printf("Введите ваш выбор: ");
}

void display_students(const Student *students, int count) {
    if (!students || count <= 0) {
        printf("Нет студентов для отображения.\n");
        return;
    }
    
    printf("\n%-5s %-15s %-15s %-10s %s\n", "ID", "Имя", "Фамилия", "Группа", "Оценки");
    printf("------------------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-5u %-15s %-15s %-10s ", 
               students[i].id, students[i].first_name, students[i].last_name, students[i].group);
        for (int j = 0; j < GRADES_COUNT; j++) {
            printf("%3u", students[i].grades[j]);
        }
        printf(" (Ср: %.2f)\n", calculate_average(students[i].grades));
    }
}

void handle_find_by_id(Student *students, int count) {
    unsigned int id;
    printf("Введите ID студента: ");
    if (scanf("%u", &id) != 1) {
        printf("Ошибка: Неверный формат ID.\n");
        while (getchar() != '\n');
        return;
    }
    
    Student *found = NULL;
    int found_count = 0;
    StatusCode status = find_by_id(students, count, id, &found, &found_count);
    
    switch (status) {
        case SUCCESS:
            printf("Найдено %d студент(ов):\n", found_count);
            display_students(found, found_count);
            break;
        case NOT_FOUND:
            printf("Студент с ID %u не найден.\n", id);
            break;
        default:
            printf("Ошибка при поиске студента.\n");
    }
    
    if (found) free_students(found, found_count);
}

void handle_find_by_last_name(Student *students, int count) {
    char last_name[MAX_NAME_LEN];
    printf("Введите фамилию: ");
    if (scanf("%49s", last_name) != 1) {
        printf("Ошибка: Неверный ввод.\n");
        return;
    }
    
    Student *found = NULL;
    int found_count = 0;
    StatusCode status = find_by_last_name(students, count, last_name, &found, &found_count);
    
    switch (status) {
        case SUCCESS:
            printf("Найдено %d студент(ов):\n", found_count);
            display_students(found, found_count);
            break;
        case NOT_FOUND:
            printf("Студент с фамилией '%s' не найден.\n", last_name);
            break;
        case INVALID_NAME:
            printf("Ошибка: Неверный формат фамилии.\n");
            break;
        default:
            printf("Ошибка при поиске студента.\n");
    }
    
    if (found) free_students(found, found_count);
}

void handle_find_by_first_name(Student *students, int count) {
    char first_name[MAX_NAME_LEN];
    printf("Введите имя: ");
    if (scanf("%49s", first_name) != 1) {
        printf("Ошибка: Неверный ввод.\n");
        return;
    }
    
    Student *found = NULL;
    int found_count = 0;
    StatusCode status = find_by_first_name(students, count, first_name, &found, &found_count);
    
    switch (status) {
        case SUCCESS:
            printf("Найдено %d студент(ов):\n", found_count);
            display_students(found, found_count);
            break;
        case NOT_FOUND:
            printf("Студент с именем '%s' не найден.\n", first_name);
            break;
        case INVALID_NAME:
            printf("Ошибка: Неверный формат имени.\n");
            break;
        default:
            printf("Ошибка при поиске студента.\n");
    }
    
    if (found) free_students(found, found_count);
}

void handle_find_by_group(Student *students, int count) {
    char group[MAX_GROUP_LEN];
    printf("Введите группу: ");
    if (scanf("%19s", group) != 1) {
        printf("Ошибка: Неверный ввод.\n");
        return;
    }
    
    Student *found = NULL;
    int found_count = 0;
    StatusCode status = find_by_group(students, count, group, &found, &found_count);
    
    switch (status) {
        case SUCCESS:
            printf("Найдено %d студент(ов):\n", found_count);
            display_students(found, found_count);
            break;
        case NOT_FOUND:
            printf("Студенты в группе '%s' не найдены.\n", group);
            break;
        case INVALID_GROUP:
            printf("Ошибка: Неверный формат группы.\n");
            break;
        default:
            printf("Ошибка при поиске студентов.\n");
    }
    
    if (found) free_students(found, found_count);
}

void handle_sort_students(Student *students, int count, int choice) {
    int (*comparator)(const void*, const void*) = NULL;
    char *sort_type = "";
    
    switch (choice) {
        case 5: comparator = compare_id; sort_type = "ID"; break;
        case 6: comparator = compare_last_name; sort_type = "фамилии"; break;
        case 7: comparator = compare_first_name; sort_type = "имени"; break;
        case 8: comparator = compare_group; sort_type = "группе"; break;
        default: return;
    }
    
    if (comparator) {
        sort_students(students, count, comparator);
        printf("Студенты отсортированы по %s.\n", sort_type);
        display_students(students, count);
    }
}

void handle_save_student_by_id(Student *students, int count, const char *trace_file) {
    if (!trace_file) {
        printf("Ошибка: Файл для сохранения не указан.\n");
        return;
    }
    
    unsigned int id;
    printf("Введите ID студента: ");
    if (scanf("%u", &id) != 1) {
        printf("Ошибка: Неверный формат ID.\n");
        while (getchar() != '\n');
        return;
    }
    
    StatusCode status = save_student_by_id(students, count, id, trace_file);
    
    switch (status) {
        case SUCCESS:
            printf("Информация о студенте сохранена в '%s'.\n", trace_file);
            break;
        case NOT_FOUND:
            printf("Студент с ID %u не найден.\n", id);
            break;
        case OPEN_FILE_ERROR:
            printf("Ошибка: Не удалось открыть файл '%s'.\n", trace_file);
            break;
        default:
            printf("Ошибка сохранения информации о студенте.\n");
    }
}

void handle_save_top_students(Student *students, int count, const char *trace_file) {
    if (!trace_file) {
        printf("Ошибка: Файл для сохранения не указан.\n");
        return;
    }
    
    StatusCode status = save_top_students(students, count, trace_file);
    
    switch (status) {
        case SUCCESS:
            printf("Лучшие студенты сохранены в '%s'.\n", trace_file);
            break;
        case NOT_FOUND:
            printf("Лучшие студенты не найдены (все имеют средний или ниже среднего балл).\n");
            break;
        case OPEN_FILE_ERROR:
            printf("Ошибка: Не удалось открыть файл '%s'.\n", trace_file);
            break;
        default:
            printf("Ошибка сохранения лучших студентов.\n");
    }
}

int main(int argc, char *argv[]) {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif

    if (argc < 2) {
        printf("Использование: %s <файл_данных> [файл_вывода]\n", argv[0]);
        printf("Пример: %s students.txt output.txt\n", argv[0]);
        return 1;
    }

    Student *students = NULL;
    int count = 0;
    
    printf("Загрузка студентов из '%s'\n", argv[1]);
    StatusCode status = load_students(argv[1], &students, &count);
    
    switch (status) {
        case SUCCESS:
            printf("Успешно загружено %d студентов.\n", count);
            break;
        case OPEN_FILE_ERROR:
            printf("Ошибка: Не удалось открыть файл '%s'.\n", argv[1]);
            return 1;
        case MEMORY_ALLOCATION:
            printf("Ошибка: Не удалось выделить память.\n");
            return 1;
        case INVALID_DATA:
            printf("Ошибка: Неверные данные в файле '%s'.\n", argv[1]);
            return 1;
        case INVALID_INPUT:
            printf("Ошибка: Неверные входные данные.\n");
            return 1;
        case DUPLICATE_ID:
            printf("Ошибка: Обнаружены дубликаты ID в файле '%s'.\n", argv[1]);
            return 1;
        default:
            printf("Ошибка загрузки студентов.\n");
            return 1;
    }

    const char *trace_file = (argc >= 3) ? argv[2] : NULL;
    
    int choice;
    do {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            printf("Ошибка: Неверный ввод.\n");
            while (getchar() != '\n');
            continue;
        }
        
        switch (choice) {
            case 1: handle_find_by_id(students, count); break;
            case 2: handle_find_by_last_name(students, count); break;
            case 3: handle_find_by_first_name(students, count); break;
            case 4: handle_find_by_group(students, count); break;
            case 5: case 6: case 7: case 8: 
                handle_sort_students(students, count, choice); 
                break;
            case 9: 
                handle_save_student_by_id(students, count, trace_file); 
                break;
            case 10: 
                handle_save_top_students(students, count, trace_file); 
                break;
            case 11: 
                display_students(students, count); 
                break;
            case 0: 
                printf("Выход...\n"); 
                break;
            default: 
                printf("Неверный выбор. Пожалуйста, попробуйте снова.\n");
        }
    } while (choice != 0);

    free_students(students, count);
    return 0;
}