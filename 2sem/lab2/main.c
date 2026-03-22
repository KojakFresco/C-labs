// 5. Метод простой вставки
// 3. тип ключа: строковый
// хранение данных и ключей: отдельно
// минимальное число: 13
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sort.h"

#define N 10
#define MAX_LEN 50

int main() {
    char keys[N][MAX_LEN];
    char data[N][MAX_LEN];

    srand(time(NULL));

    const char *types[3] = {"sorted", "reversed", "random"};

    for (int t = 0; t < 3; t++) {
        printf("\n=== Тест: %s ===\n", types[t]);

        fill_table(keys, data, N, types[t]);

        printf("\nДо сортировки:\n");
        print_table(keys, data, N);

        insertion_sort(keys, data, N);

        printf("\nПосле сортировки:\n");
        print_table(keys, data, N);

        // Пример поиска
        char search_key[MAX_LEN];
        printf("\nВведите ключ для поиска (или 'exit' для следующего теста):\n");
        while (1) {
            printf("> ");
            scanf("%s", search_key);
            if (strcmp(search_key, "exit") == 0) break;

            int idx = binary_search(keys, N, search_key);
            if (idx != -1) {
                printf("Найден: %-20s | %-20s\n", keys[idx], data[idx]);
            } else {
                printf("Не найден\n");
            }
        }
    }

    return 0;
}