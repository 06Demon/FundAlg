#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/utils.h"

#ifdef _WIN32
#include <windows.h>
#endif

static void clear_input(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static void menu_header(void) {
    printf("\n------------MENU------------\n");
    printf(" 1. %%Ro  - int -> Roman numerals\n");
    printf(" 2. %%Zr  - unsigned int -> Zeckendorf representation\n");
    printf(" 3. %%Cv  - int -> base-N (lowercase)\n");
    printf(" 4. %%CV  - int -> base-N (UPPERCASE)\n");
    printf(" 5. %%to  - string (base N) -> decimal (lowercase)\n");
    printf(" 6. %%TO  - string (base N) -> decimal (UPPERCASE)\n");
    printf(" 7. %%mi  - memory dump (int)\n");
    printf(" 8. %%mu  - memory dump (unsigned int)\n");
    printf(" 9. %%md  - memory dump (double)\n");
    printf("10. %%mf  - memory dump (float)\n");
    printf("11. Test my_oversprintf example\n");
    printf("12. Test my_overfprintf (output to file)\n");
    printf(" 0. Exit\n");
    printf("------------------------------\n");
    printf("Enter your choose: ");
}

static void handle_status(StatusCode status) {
    switch (status) {
        case SUCCESS:
            break;
        case NULL_POINTER_ERROR:
            printf("Ошибка: нулевой указатель\n");
            break;
        case MEMORY_ALLOCATION_ERROR:
            printf("Ошибка: не удалось выделить память\n");
            break;
        case INVALID_BASE:
            printf("Ошибка: неверное основание системы счисления\n");
            break;
        case INVALID_FORMAT:
            printf("Ошибка: неверный формат\n");
            break;
        case BUFFER_OVERFLOW:
            printf("Ошибка: переполнение буфера\n");
            break;
        default:
            printf("Ошибка: неизвестная ошибка\n");
            break;
    }
}

int main(void) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    int choice;
    char buf[512];

    while (1) {
        menu_header();
        if (scanf("%d", &choice) != 1) {
            clear_input();
            continue;
        }
        clear_input();

        switch (choice) {
            case 0:
                printf("Выход.\n");
                return 0;

            case 1: {
                int val;
                printf("Введите целое число: ");
                if (scanf("%d", &val) != 1) {
                    printf("Ошибка ввода числа\n");
                    clear_input();
                    break;
                }
                clear_input();
                StatusCode status = my_oversprintf(buf, sizeof(buf), "%Ro", val);
                handle_status(status);
                if (status == SUCCESS) {
                    printf("Римское представление: %s\n", buf);
                }
                break;
            }

            case 2: {
                unsigned int val;
                printf("Введите неотрицательное целое число: ");
                if (scanf("%u", &val) != 1) {
                    printf("Ошибка ввода числа\n");
                    clear_input();
                    break;
                }
                clear_input();
                StatusCode status = my_oversprintf(buf, sizeof(buf), "%Zr", val);
                handle_status(status);
                if (status == SUCCESS) {
                    printf("Представление Цекендорфа: %s\n", buf);
                }
                break;
            }

            case 3:
            case 4: {
                int val, base;
                printf("Введите целое число: ");
                if (scanf("%d", &val) != 1) {
                    printf("Ошибка ввода числа\n");
                    clear_input();
                    break;
                }
                clear_input();

                printf("Введите основание системы счисления (2-36): ");
                if (scanf("%d", &base) != 1) {
                    printf("Ошибка ввода основания\n");
                    clear_input();
                    break;
                }
                clear_input();
                StatusCode status;
                if (choice == 3)
                    status = my_oversprintf(buf, sizeof(buf), "%Cv", val, base);
                else
                    status = my_oversprintf(buf, sizeof(buf), "%CV", val, base);
                handle_status(status);
                if (status == SUCCESS) {
                    printf("Результат: %s\n", buf);
                }
                break;
            }

            case 5:
            case 6: {
                char str[128];
                int base;
                printf("Введите строковое представление числа: ");
                if (scanf("%127s", str) != 1) {
                    printf("Ошибка ввода строки\n");
                    clear_input();
                    break;
                }
                printf("Введите исходное основание системы счисления (2-36): ");
                if (scanf("%d", &base) != 1) {
                    printf("Ошибка ввода основания\n");
                    clear_input();
                    break;
                }
                clear_input();
                StatusCode status;
                if (choice == 5)
                    status = my_oversprintf(buf, sizeof(buf), "%to", str, base);
                else
                    status = my_oversprintf(buf, sizeof(buf), "%TO", str, base);
                handle_status(status);
                if (status == SUCCESS) {
                    printf("Десятичное представление: %s\n", buf);
                }
                break;
            }

            case 7: {
                int x;
                printf("Введите целое число: ");
                if (scanf("%d", &x) != 1) {
                    printf("Ошибка ввода числа\n");
                    clear_input();
                    break;
                }
                clear_input();
                StatusCode status = my_oversprintf(buf, sizeof(buf), "%mi", x);
                handle_status(status);
                if (status == SUCCESS) {
                    printf("Дамп памяти (int): %s\n", buf);
                }
                break;
            }

            case 8: {
                unsigned int x;
                printf("Введите беззнаковое число: ");
                if (scanf("%u", &x) != 1) {
                    printf("Ошибка ввода числа\n");
                    clear_input();
                    break;
                }
                clear_input();
                StatusCode status = my_oversprintf(buf, sizeof(buf), "%mu", x);
                handle_status(status);
                if (status == SUCCESS) {
                    printf("Дамп памяти (unsigned int): %s\n", buf);
                }
                break;
            }

            case 9: {
                double d;
                printf("Введите число типа double: ");
                if (scanf("%lf", &d) != 1) {
                    printf("Ошибка ввода числа\n");
                    clear_input();
                    break;
                }
                clear_input();
                StatusCode status = my_oversprintf(buf, sizeof(buf), "%md", d);
                handle_status(status);
                if (status == SUCCESS) {
                    printf("Дамп памяти (double): %s\n", buf);
                }
                break;
            }

            case 10: {
                float f;
                printf("Введите число типа float: ");
                if (scanf("%f", &f) != 1) {
                    printf("Ошибка ввода числа\n");
                    clear_input();
                    break;
                }
                clear_input();
                StatusCode status = my_oversprintf(buf, sizeof(buf), "%mf", f);
                handle_status(status);
                if (status == SUCCESS) {
                    printf("Дамп памяти (float): %s\n", buf);
                }
                break;
            }

            case 11: {
                int num = 42;
                unsigned int unum = 25;
                double dbl = 3.24;
                float flt = 2.71f;
                char s[256];

                printf("Комбинированный пример:\n");
                StatusCode status;
                
                status = my_oversprintf(s, sizeof(s), "num=%d, roman=%Ro, base16=%Cv, fib=%Zr\n", 
                                       num, num, num, 16, unum);
                handle_status(status);
                if (status == SUCCESS) printf("%s", s);
                
                status = my_oversprintf(s, sizeof(s), "to-decimal(num=1f, basse=16)=%to\n", "1f", 16);
                handle_status(status);
                if (status == SUCCESS) printf("%s", s);
                
                status = my_oversprintf(s, sizeof(s), "dump int(42)=%mi\n", num);
                handle_status(status);
                if (status == SUCCESS) printf("%s", s);

                status = my_oversprintf(s, sizeof(s), "dump unsigned int(25)=%mu\n", num);
                handle_status(status);
                if (status == SUCCESS) printf("%s", s);
                
                status = my_oversprintf(s, sizeof(s), "dump double(3.24)=%md\n", dbl);
                handle_status(status);
                if (status == SUCCESS) printf("%s", s);
                
                status = my_oversprintf(s, sizeof(s), "dump float(2.71f)=%mf\n", flt);
                handle_status(status);
                if (status == SUCCESS) printf("%s", s);
                break;
            }
            case 12: {
                printf("Тестирование my_overfprintf (вывод в файл):\n");
                printf("--------------------------------------------\n");
                
                FILE* file = fopen("output.txt", "w");
                if (file == NULL) {
                    printf("Ошибка: не удалось создать/открыть файл output.txt\n");
                    break;
                }
                
                StatusCode status;

                int num = 4;
                unsigned int gol = 61;
                double dbl = 3.24;
                float flt = 2.71f;
                
                status = my_overfprintf(file, "num: %d\n", num);
                handle_status(status);

                status = my_overfprintf(file, "Римские цифры: %Ro\n", num);
                handle_status(status);
                
                status = my_overfprintf(file, "Цекендорф: %Zr\n", num);
                handle_status(status);
                
                status = my_overfprintf(file, "Шестнадцатеричное: %Cv\n", num, 16);
                handle_status(status);
                
                status = my_overfprintf(file, "Из двоичной(101): %to\n", "101", 2);
                handle_status(status);
                
                status = my_overfprintf(file, "Дамп int(4): %mi\n", num);
                handle_status(status);

                status = my_overfprintf(file, "Дамп unsigned int(61): %mu\n", gol);
                handle_status(status);

                status = my_overfprintf(file, "Дамп double(3.24): %md\n", dbl);
                handle_status(status);

                status = my_overfprintf(file, "Дамп float(2.71f): %mf\n", flt);
                handle_status(status);
                
                printf("Результат записан в файл.\n");
                fclose(file);
                break;
            }

            default:
                printf("Неверный выбор.\n");
                break;
        }
    }
}