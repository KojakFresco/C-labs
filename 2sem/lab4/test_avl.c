#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "avl_tree.h"

static int g_tests_run = 0;
static int g_tests_failed = 0;

static int max_int(int a, int b) {
    return a > b ? a : b;
}

static void copy_key(char dst[KEY_SIZE], const char *src) {
    strncpy(dst, src, KEY_SIZE - 1);
    dst[KEY_SIZE - 1] = '\0';
}

static void insert_key(AVLTree *tree, const char *key, double value) {
    char key_buf[KEY_SIZE];
    copy_key(key_buf, key);
    insert(tree, key_buf, value);
}

static void delete_key(AVLTree *tree, const char *key) {
    char key_buf[KEY_SIZE];
    copy_key(key_buf, key);
    delete_elem(tree, key_buf);
}

static int validate_subtree(const AVLNode *node, const char *min_key, const char *max_key, int *count) {
    if (!node) {
        return 0;
    }

    if (min_key != NULL && strcmp(node->key, min_key) <= 0) {
        return -1;
    }
    if (max_key != NULL && strcmp(node->key, max_key) >= 0) {
        return -1;
    }

    int left_h = validate_subtree(node->left, min_key, node->key, count);
    if (left_h < 0) {
        return -1;
    }

    int right_h = validate_subtree(node->right, node->key, max_key, count);
    if (right_h < 0) {
        return -1;
    }

    int expected_h = 1 + max_int(left_h, right_h);
    int balance = right_h - left_h;

    if (node->height != expected_h) {
        return -1;
    }
    if (balance < -1 || balance > 1) {
        return -1;
    }

    (*count)++;
    return expected_h;
}

static int validate_tree(AVLTree *tree, int expected_count, int max_allowed_height) {
    int count = 0;
    int height = validate_subtree(tree->root, NULL, NULL, &count);
    if (height < 0) {
        return 0;
    }
    if (count != expected_count) {
        return 0;
    }
    if (height > max_allowed_height) {
        return 0;
    }
    return 1;
}

static int capture_find_output(AVLTree *tree, const char *key, char *buffer) {
    char key_buf[KEY_SIZE];
    copy_key(key_buf, key);

    FILE *tmp = tmpfile();
    if (!tmp) {
        return 0;
    }

    find(tree, key_buf, tmp);
    fflush(tmp);
    rewind(tmp);

    size_t read_size = fread(buffer, 1, 255, tmp);
    buffer[read_size] = '\0';
    fclose(tmp);
    return 1;
}

static int capture_print_output(AVLTree *tree, char *buffer) {
    FILE *tmp = tmpfile();
    if (!tmp) {
        return 0;
    }

    print(tree, tmp);
    fflush(tmp);
    rewind(tmp);

    size_t read_size = fread(buffer, 1, 2047, tmp);
    buffer[read_size] = '\0';
    fclose(tmp);
    return 1;
}

static void report_result(const char *name, int ok) {
    g_tests_run++;
    if (ok) {
        printf("[PASS] %s\n", name);
    } else {
        printf("[FAIL] %s\n", name);
        g_tests_failed++;
    }
}

static void test_insert_and_find_existing(void) {
    AVLTree *tree = init_avl_tree();
    insert_key(tree, "abc", 10.5);

    char out[256];
    int ok = capture_find_output(tree, "abc", out);
    ok = ok && strstr(out, "found") != NULL;
    ok = ok && strstr(out, "abc") != NULL;
    ok = ok && strstr(out, "10.500000") != NULL;
    ok = ok && validate_tree(tree, 1, 1);

    report_result("insert + find existing", ok);
    destroy_avl_tree(tree);
}

static void test_find_missing(void) {
    AVLTree *tree = init_avl_tree();
    insert_key(tree, "abc", 1.0);
    insert_key(tree, "abd", 2.0);

    char out[256];
    int ok = capture_find_output(tree, "zzz", out);
    ok = ok && strstr(out, "not found") != NULL;
    ok = ok && validate_tree(tree, 2, 2);

    report_result("find missing key", ok);
    destroy_avl_tree(tree);
}

static void test_delete_leaf(void) {
    AVLTree *tree = init_avl_tree();
    insert_key(tree, "m", 1.0);
    insert_key(tree, "c", 2.0);
    insert_key(tree, "t", 3.0);
    insert_key(tree, "a", 4.0);

    delete_key(tree, "a");

    char out[256];
    int ok = capture_find_output(tree, "a", out);
    ok = ok && strstr(out, "not found") != NULL;
    ok = ok && validate_tree(tree, 3, 3);

    report_result("delete leaf node", ok);
    destroy_avl_tree(tree);
}

static void test_delete_node_with_two_children(void) {
    AVLTree *tree = init_avl_tree();
    insert_key(tree, "m", 1.0);
    insert_key(tree, "c", 2.0);
    insert_key(tree, "t", 3.0);
    insert_key(tree, "a", 4.0);
    insert_key(tree, "e", 5.0);
    insert_key(tree, "p", 6.0);
    insert_key(tree, "w", 7.0);

    delete_key(tree, "m");

    char out[256];
    int ok = capture_find_output(tree, "m", out);
    ok = ok && strstr(out, "not found") != NULL;
    ok = ok && validate_tree(tree, 6, 4);

    report_result("delete node with two children", ok);
    destroy_avl_tree(tree);
}

static void test_large_tree_twenty_nodes(void) {
    static const char *keys[] = {
        "m", "f", "t", "b", "h", "p", "v", "a", "c", "g",
        "i", "n", "r", "u", "x", "d", "e", "j", "k", "l"
    };
    static const double values[] = {
        13, 6, 20, 2, 8, 16, 22, 1, 3, 7,
        9, 14, 18, 21, 24, 4, 5, 10, 11, 12
    };

    AVLTree *tree = init_avl_tree();
    for (size_t i = 0; i < sizeof(keys) / sizeof(keys[0]); ++i) {
        insert_key(tree, keys[i], values[i]);
    }

    char out[2048];
    int ok = validate_tree(tree, 20, 6);
    ok = ok && capture_print_output(tree, out);
    ok = ok && strstr(out, "m") != NULL;
    ok = ok && strstr(out, "a") != NULL;
    ok = ok && strstr(out, "x") != NULL;

    char find_out[256];
    ok = ok && capture_find_output(tree, "j", find_out);
    ok = ok && strstr(find_out, "found") != NULL;
    ok = ok && strstr(find_out, "10.000000") != NULL;

    report_result("large tree with 20 nodes", ok);
    destroy_avl_tree(tree);
}

int main(void) {
    test_insert_and_find_existing();
    test_find_missing();
    test_delete_leaf();
    test_delete_node_with_two_children();
    test_large_tree_twenty_nodes();

    printf("\nSummary: %d/%d tests passed\n", g_tests_run - g_tests_failed, g_tests_run);
    return g_tests_failed == 0 ? 0 : 1;
}
