
/* Построить упорядоченное дерево для хранения и поиска элементов с
уникальными строковыми ключами и вещественными значениями. Ключ –
непустая последовательность латинских букв не более чем из 6 символов.
Ключи упорядочиваются лексикографически.
1. AVL-дерево

Предусмотреть возможность ввода команд из текстового файла в формате
<номер операции> [<ключ>] [<значение >]
Необходимость параметров в квадратных скобках определяется номером
операции.

Хотя бы один тестовый пример должен включать работу с деревом не менее
чем из 20 узлов
Предусмотреть также вывод в текстовый файл в формате :
<команда из входного файла>
<её результат>
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "avl_tree.h"

static void setup_console_encoding(void) {
#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif
}

static void print_separator(FILE *out) {
    fprintf(out, "--------------------------\n");
}

static void execute_operation(AVLTree *tree, int op, int n, const char key[KEY_SIZE], double value, FILE *out) {
    switch (op) {
        case 1: // insert: 1 key value
            if (n == 3) {
                insert(tree, (char *)key, value);
                fprintf(out, "OK\n");
            } else {
                fprintf(out, "ERR: format for op=1 is: 1 <key> <value>\n");
            }
            print_separator(out);
            break;

        case 2: // delete: 2 key
            if (n >= 2) {
                delete_elem(tree, (char *)key);
                fprintf(out, "OK\n");
            } else {
                fprintf(out, "ERR: format for op=2 is: 2 <key>\n");
            }
            print_separator(out);
            break;

        case 3: // print: 3
            if (n >= 1) {
                print(tree, out);
            } else {
                fprintf(out, "ERR: format for op=3 is: 3\n");
            }
            print_separator(out);
            break;

        case 4: // find: 4 key
            if (n >= 2) {
                find(tree, (char *)key, out);
                fprintf(out, "OK\n");
            } else {
                fprintf(out, "ERR: format for op=4 is: 4 <key>\n");
            }
            print_separator(out);
            break;

        default:
            fprintf(out, "ERR: unknown operation %d\n", op);
            print_separator(out);
            break;
    }
}

static void process_numeric_command_line(AVLTree *tree, const char *line, FILE *out, int echo_input_line) {
    int op = 0;
    char key[KEY_SIZE] = {0};
    double value = 0.0;
    int n = sscanf(line, "%d %6[A-Za-z] %lf", &op, key, &value);

    if (echo_input_line) {
        fputs(line, out);
        if (line[0] != '\0' && line[strlen(line) - 1] != '\n') {
            fprintf(out, "\n");
        }
    }

    execute_operation(tree, op, n, key, value, out);
}

static int run_file_mode(const char *input_path, const char *output_path) {
    FILE *in = fopen(input_path, "r");
    if (!in) {
        perror("fopen input");
        return 1;
    }

    FILE *out = fopen(output_path, "w");
    if (!out) {
        perror("fopen output");
        fclose(in);
        return 1;
    }

    AVLTree *tree = init_avl_tree();
    if (!tree) {
        fclose(in);
        fclose(out);
        return 1;
    }
    char line[256];

    while (fgets(line, sizeof(line), in)) {
        process_numeric_command_line(tree, line, out, 1);
    }

    destroy_avl_tree(tree);
    fclose(in);
    fclose(out);
    return 0;
}

static void print_help(void) {
    printf("Commands:\n");
    printf("  insert <key> <value>\n");
    printf("  delete <key>\n");
    printf("  find <key>\n");
    printf("  print\n");
    printf("  help\n");
    printf("  exit\n");
    printf("Also supports numeric format from file: 1/2/3/4.\n");
}

static int run_interactive_mode(void) {
    AVLTree *tree = init_avl_tree();
    if (!tree) {
        return 1;
    }
    char line[256];

    printf("Interactive AVL CLI. Type 'help' for commands.\n");
    while (1) {
        char cmd[16] = {0};
        char key[KEY_SIZE] = {0};
        double value = 0.0;
        int n = 0;

        printf("> ");
        if (!fgets(line, sizeof(line), stdin)) {
            break;
        }

        if (line[0] == '\n' || line[0] == '\0') {
            continue;
        }

        n = sscanf(line, "%15s %6[A-Za-z] %lf", cmd, key, &value);

        if (n >= 1 && strcmp(cmd, "exit") == 0) {
            break;
        }
        if (n >= 1 && strcmp(cmd, "help") == 0) {
            print_help();
            continue;
        }
        if (n >= 1 && strcmp(cmd, "insert") == 0) {
            execute_operation(tree, 1, n, key, value, stdout);
            continue;
        }
        if (n >= 1 && strcmp(cmd, "delete") == 0) {
            execute_operation(tree, 2, n, key, value, stdout);
            continue;
        }
        if (n >= 1 && strcmp(cmd, "print") == 0) {
            execute_operation(tree, 3, 1, key, value, stdout);
            continue;
        }
        if (n >= 1 && strcmp(cmd, "find") == 0) {
            execute_operation(tree, 4, n, key, value, stdout);
            continue;
        }

        process_numeric_command_line(tree, line, stdout, 0);
    }

    destroy_avl_tree(tree);
    return 0;
}

static void print_usage(const char *program_name) {
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "  %s --interactive\n", program_name);
    fprintf(stderr, "  %s --file <input.txt> <output.txt>\n", program_name);
    fprintf(stderr, "  %s <input.txt> <output.txt>  (backward-compatible mode)\n", program_name);
}

int main(int argc, char **argv) {
    setup_console_encoding();

    if (argc == 1) {
        return run_interactive_mode();
    }

    if (argc == 2 && strcmp(argv[1], "--interactive") == 0) {
        return run_interactive_mode();
    }

    if (argc == 4 && strcmp(argv[1], "--file") == 0) {
        return run_file_mode(argv[2], argv[3]);
    }

    if (argc == 3) {
        return run_file_mode(argv[1], argv[2]);
    }

    print_usage(argv[0]);
    return 1;
}

