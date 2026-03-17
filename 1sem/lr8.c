#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "linked_list.h"

int main() {
    system("chcp 65001 > nul");
    srand(time(NULL));
    Node* linked_list = new_ll();

    printf("Меню команд:\n");
    printf("1 - stop\n2 - insert\n3 - print element\n4 - delete elem\n5 - print list\n6 - sort list\n");
    while (true) {
        int v;
        scanf("%d", &v);
        int d;
        bool flag = false;
        switch (v) {
            case 1:
                flag = true;
                break;
            case 2:
                char* str = (char*)malloc(256 * sizeof(char));
                scanf("%s %d", str, &d);
                linked_list = insert_to_ll(linked_list, str, d);
                printf("Элемент вставлен.\n");
                break;
            case 3:
                scanf("%d", &d);
                printf("Вывод элемента на позиции %d:\n", d);
                print_elem(linked_list, d);
                break;
            case 4:
                scanf("%d", &d);
                linked_list = delete_elem(linked_list, d);
                printf("Элемент на позиции %d удален.\n", d);
                break;
            case 5:
                printf("Текущий список:\n");
                print_ll(linked_list);
                break;
            case 6:
                linked_list = sort_ll(linked_list);
                printf("Список отсортирован.\n");
                break;
            default:
                printf("Неверный ввод. Попробуйте снова.\n");
        }
        if (flag) {
            break;
        }
    }
    return 0;
}