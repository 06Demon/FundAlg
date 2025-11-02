#include <stdio.h>
#include <string.h>
#include "status.h"
#include "utils.h"

#ifdef _WIN32
#include <windows.h>
#endif

void clear_input() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

void handle_status(StatusCode status) {
    if (status != STATUS_OK && status != STATUS_ERROR_NOT_FOUND) {
        printf("Ошибка: %s\n", status_message(status));
    }
}

void print_menu() {
    printf("\nВыберите функцию: \n");
    printf("1. memchr\n");
    printf("2. memcmp\n");
    printf("3. memcpy\n");
    printf("4. memset\n");
    printf("5. strncat\n");
    printf("6. strchr\n");
    printf("7. strncmp\n");
    printf("8. strncpy\n");
    printf("9. strcspn\n");
    printf("10. strerror\n");
    printf("11. strlen\n");
    printf("12. strpbrk\n");
    printf("13. strrchr\n");
    printf("14. strstr\n");
    printf("15. strtok\n");
    printf("0. Выход\n");
    printf("------------------\n");
}

int main(void) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    
    int choice = -1;

    while (1) {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            clear_input();
            printf("Некорректный ввод. Попробуйте снова.\n");
            continue;
        }
        clear_input();
        
        if (choice == 0) {
            printf("Выход из программы...\n");
            break;
        }

        StatusCode status = STATUS_OK;
        
        switch (choice) {
            case 1: { // memchr
                char str[256];
                char c;
                size_t n;
                printf("Введите строку: ");
                if (fgets(str, sizeof(str), stdin) == NULL) {
                    printf("Ошибка чтения строки\n");
                    break;
                }
                str[strcspn(str, "\n")] = 0;
                
                printf("Введите символ для поиска: ");
                if (scanf("%c", &c) != 1) {
                    printf("Ошибка чтения символа\n");
                    clear_input();
                    break;
                }
                
                printf("Введите количество байт для поиска (n): ");
                if (scanf("%zu", &n) != 1) {
                    printf("Ошибка чтения числа\n");
                    clear_input();
                    break;
                }
                clear_input();
                
                void *res;
                status = my_memchr(str, c, n, &res);
                if (status == STATUS_OK && res != NULL) {
                    int index = (int)((char *)res - str);
                    printf("Результат: %d\n", index);
                } else if (status == STATUS_ERROR_NOT_FOUND) {
                    printf("Результат: не найдено\n");
                } else {
                    handle_status(status);
                }
                break;
            }

            case 2: { // memcmp
                char s1[256], s2[256];
                size_t n;
                printf("Введите первую строку: ");
                if (fgets(s1, sizeof(s1), stdin) == NULL) {
                    printf("Ошибка чтения строки\n");
                    break;
                }
                s1[strcspn(s1, "\n")] = 0;
                
                printf("Введите вторую строку: ");
                if (fgets(s2, sizeof(s2), stdin) == NULL) {
                    printf("Ошибка чтения строки\n");
                    break;
                }
                s2[strcspn(s2, "\n")] = 0;
                
                printf("Введите n: ");
                if (scanf("%zu", &n) != 1) {
                    printf("Ошибка чтения числа\n");
                    clear_input();
                    break;
                }
                clear_input();
                
                int result;
                status = my_memcmp(s1, s2, n, &result);
                if (status == STATUS_OK) {
                    printf("Результат сравнения: %d\n", result);
                } else {
                    handle_status(status);
                }
                break;
            }

            case 3: { // memcpy
                char src[256], dest[256] = {0};
                size_t n;
                printf("Введите источник (source): ");
                if (fgets(src, sizeof(src), stdin) == NULL) {
                    printf("Ошибка чтения строки\n");
                    break;
                }
                src[strcspn(src, "\n")] = 0;
                
                printf("Введите количество байт для копирования (n): ");
                if (scanf("%zu", &n) != 1) {
                    printf("Ошибка чтения числа\n");
                    clear_input();
                    break;
                }
                clear_input();
                
                if (n >= sizeof(dest)) {
                    printf("Ошибка: n больше размера буфера\n");
                    break;
                }
                
                status = my_memcpy(dest, src, n);
                if (status == STATUS_OK) {
                    dest[n] = '\0';
                    printf("Результат копирования: %s\n", dest);
                } else {
                    handle_status(status);
                }
                break;
            }

            case 4: { // memset
                char buf[256];
                int c;
                size_t n;
                printf("Введите исходную строку: ");
                if (fgets(buf, sizeof(buf), stdin) == NULL) {
                    printf("Ошибка чтения строки\n");
                    break;
                }
                buf[strcspn(buf, "\n")] = 0;
                
                printf("Введите символ для заполнения: ");
                c = getchar();
                
                printf("Введите количество байт (n): ");
                if (scanf("%zu", &n) != 1) {
                    printf("Ошибка чтения числа\n");
                    clear_input();
                    break;
                }
                clear_input();
                
                
                size_t str_len = strlen(buf);
                status = my_memset(buf, c, n);
                if (status == STATUS_OK) {
                    buf[str_len] = '\0';
                    printf("Результат: %s\n", buf);
                } else {
                    handle_status(status);
                }
                break;
            }

            case 5: { // strncat
                char dest[256] = {0}, src[256];
                size_t n;
                printf("Введите строку назначения (dest): ");
                if (fgets(dest, sizeof(dest), stdin) == NULL) {
                    printf("Ошибка чтения строки\n");
                    break;
                }
                dest[strcspn(dest, "\n")] = 0;
                
                printf("Введите строку источника (src): ");
                if (fgets(src, sizeof(src), stdin) == NULL) {
                    printf("Ошибка чтения строки\n");
                    break;
                }
                src[strcspn(src, "\n")] = 0;
                
                printf("Введите количество символов для добавления (n): ");
                if (scanf("%zu", &n) != 1) {
                    printf("Ошибка чтения числа\n");
                    clear_input();
                    break;
                }
                clear_input();
                
                status = my_strncat(dest, sizeof(dest), src, n);
                if (status == STATUS_OK) {
                    printf("Результат: %s\n", dest);
                } else {
                    handle_status(status);
                }
                break;
            }

            case 6: { // strchr
                char str[256], c;
                printf("Введите строку: ");
                if (fgets(str, sizeof(str), stdin) == NULL) {
                    printf("Ошибка чтения строки\n");
                    break;
                }
                str[strcspn(str, "\n")] = 0;
                
                printf("Введите символ для поиска: ");
                if (scanf("%c", &c) != 1) {
                    printf("Ошибка чтения символа\n");
                    clear_input();
                    break;
                }
                clear_input();
                
                char *res;
                status = my_strchr(str, c, &res);
                if (status == STATUS_OK && res != NULL) {
                    int index = (int)(res - str);
                    printf("Результат: %d\n", index);
                } else if (status == STATUS_ERROR_NOT_FOUND) {
                    printf("Результат: не найдено\n");
                } else {
                    handle_status(status);
                }
                break;
            }

            case 7: { // strncmp
                char s1[256], s2[256];
                size_t n;
                printf("Введите первую строку: ");
                if (fgets(s1, sizeof(s1), stdin) == NULL) {
                    printf("Ошибка чтения строки\n");
                    break;
                }
                s1[strcspn(s1, "\n")] = 0;
                
                printf("Введите вторую строку: ");
                if (fgets(s2, sizeof(s2), stdin) == NULL) {
                    printf("Ошибка чтения строки\n");
                    break;
                }
                s2[strcspn(s2, "\n")] = 0;
                
                printf("Введите n: ");
                if (scanf("%zu", &n) != 1) {
                    printf("Ошибка чтения числа\n");
                    clear_input();
                    break;
                }
                clear_input();
                
                int result;
                status = my_strncmp(s1, s2, n, &result);
                if (status == STATUS_OK) {
                    printf("Результат сравнения: %d\n", result);
                } else {
                    handle_status(status);
                }
                break;
            }

            case 8: { // strncpy
                char src[256], dest[256] = {0};
                size_t n;
                printf("Введите строку источника (src): ");
                if (fgets(src, sizeof(src), stdin) == NULL) {
                    printf("Ошибка чтения строки\n");
                    break;
                }
                src[strcspn(src, "\n")] = 0;
                
                printf("Введите количество символов (n): ");
                if (scanf("%zu", &n) != 1) {
                    printf("Ошибка чтения числа\n");
                    clear_input();
                    break;
                }
                clear_input();
                
                status = my_strncpy(dest, sizeof(dest), src, n);
                if (status == STATUS_OK) {
                    printf("Результат копирования: %s\n", dest);
                } else {
                    handle_status(status);
                }
                break;
            }

            case 9: { // strcspn
                char s1[256], s2[256];
                printf("Введите первую строку (s1): ");
                if (fgets(s1, sizeof(s1), stdin) == NULL) {
                    printf("Ошибка чтения строки\n");
                    break;
                }
                s1[strcspn(s1, "\n")] = 0;
                
                printf("Введите вторую строку (s2): ");
                if (fgets(s2, sizeof(s2), stdin) == NULL) {
                    printf("Ошибка чтения строки\n");
                    break;
                }
                s2[strcspn(s2, "\n")] = 0;
                
                size_t result;
                status = my_strcspn(s1, s2, &result);
                if (status == STATUS_OK) {
                    printf("Длина сегмента без совпадений: %zu\n", result);
                } else {
                    handle_status(status);
                }
                break;
            }

            case 10: { // strerror
                int err;
                printf("Введите номер ошибки: ");
                if (scanf("%d", &err) != 1) {
                    printf("Ошибка чтения числа\n");
                    clear_input();
                    break;
                }
                clear_input();
                
                const char *result;
                status = my_strerror(err, &result);
                if (status == STATUS_OK) {
                    printf("Сообщение об ошибке: %s\n", result);
                } else {
                    handle_status(status);
                }
                break;
            }

            case 11: { // strlen
                char str[256];
                printf("Введите строку: ");
                if (fgets(str, sizeof(str), stdin) == NULL) {
                    printf("Ошибка чтения строки\n");
                    break;
                }
                str[strcspn(str, "\n")] = 0;
                
                size_t result;
                status = my_strlen(str, &result);
                if (status == STATUS_OK) {
                    printf("Длина строки: %zu\n", result);
                } else {
                    handle_status(status);
                }
                break;
            }

            case 12: { // strpbrk
                char s1[256], s2[256];
                printf("Введите первую строку (s1): ");
                if (fgets(s1, sizeof(s1), stdin) == NULL) {
                    printf("Ошибка чтения строки\n");
                    break;
                }
                s1[strcspn(s1, "\n")] = 0;
                
                printf("Введите вторую строку (s2): ");
                if (fgets(s2, sizeof(s2), stdin) == NULL) {
                    printf("Ошибка чтения строки\n");
                    break;
                }
                s2[strcspn(s2, "\n")] = 0;
                
                char *res;
                status = my_strpbrk(s1, s2, &res);
                if (status == STATUS_OK && res != NULL) {
                    printf("Результат: символ '%c', подстрока: %s\n", *res, res);
                } else if (status == STATUS_ERROR_NOT_FOUND) {
                    printf("Результат: не найдено\n");
                } else {
                    handle_status(status);
                }
                break;
            }

            case 13: { // strrchr
                char str[256], c;
                printf("Введите строку: ");
                if (fgets(str, sizeof(str), stdin) == NULL) {
                    printf("Ошибка чтения строки\n");
                    break;
                }
                str[strcspn(str, "\n")] = 0;
                
                printf("Введите символ для поиска: ");
                if (scanf("%c", &c) != 1) {
                    printf("Ошибка чтения символа\n");
                    clear_input();
                    break;
                }
                clear_input();
                
                char *res;
                status = my_strrchr(str, c, &res);
                if (status == STATUS_OK && res != NULL) {
                    int index = (int)(res - str);
                    printf("Результат: %d\n", index);
                } else if (status == STATUS_ERROR_NOT_FOUND) {
                    printf("Результат: не найдено\n");
                } else {
                    handle_status(status);
                }
                break;
            }

            case 14: { // strstr
                char hay[256], needle[256];
                printf("Введите строку, в которой ищем (haystack): ");
                if (fgets(hay, sizeof(hay), stdin) == NULL) {
                    printf("Ошибка чтения строки\n");
                    break;
                }
                hay[strcspn(hay, "\n")] = 0;
                
                printf("Введите подстроку для поиска (needle): ");
                if (fgets(needle, sizeof(needle), stdin) == NULL) {
                    printf("Ошибка чтения строки\n");
                    break;
                }
                needle[strcspn(needle, "\n")] = 0;
                
                char *res;
                status = my_strstr(hay, needle, &res);
                if (status == STATUS_OK && res != NULL) {
                    int index = (int)(res - hay);
                    printf("Результат: %d\n", index);
                } else if (status == STATUS_ERROR_NOT_FOUND) {
                    printf("Результат: не найдено\n");
                } else {
                    handle_status(status);
                }
                break;
            }

            case 15: { // strtok
                char text[256], delim[32];
                printf("Введите исходную строку: ");
                fgets(text, sizeof(text), stdin);
                text[strcspn(text, "\n")] = 0;
                printf("Введите разделители: ");
                fgets(delim, sizeof(delim), stdin);
                delim[strcspn(delim, "\n")] = 0;

                printf("Результат разбиения:\n");
                char *token = my_strtok(text, delim);
                while (token) {
                    printf("  \"%s\"\n", token);
                    token = my_strtok(NULL, delim);
                }
                break;
            }

            default:
                printf("Неверный выбор. Повторите попытку.\n");
                break;
        }
    }
    
    return 0;
}