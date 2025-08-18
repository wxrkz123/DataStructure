// main.c - Test driver for the simple B+ Tree.
#include "simple_bpt.h"
#include <stdio.h>

int main() {
    printf("Creating a B+ Tree with degree t=3 (max keys=5, min keys=2).\n");
    // t=3 -> max keys = 2*3 - 1 = 5.
    // Our implementation uses max keys = 2*t-1, so degree is really 't'.
    BPlusTree* tree = create_bplus_tree(3);

    int keys_to_insert[] = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 15, 25, 35, 45, 5 };
    int num_keys = sizeof(keys_to_insert) / sizeof(keys_to_insert[0]);

    for (int i = 0; i < num_keys; i++) {
        printf("Inserting %d...\n", keys_to_insert[i]);
        insert(tree, keys_to_insert[i], keys_to_insert[i] * 10);
        print_tree(tree);
    }

    printf("\n--- Search Tests ---\n");
    int search_keys[] = { 50, 15, 99, 100 };
    for (int i = 0; i < 4; i++) {
        int key = search_keys[i];
        int* value_ptr = search(tree, key);
        if (value_ptr) {
            printf("Found key %d, value is %d.\n", key, *value_ptr);
        }
        else {
            printf("Key %d not found.\n", key);
        }
    }

    destroy_bplus_tree(tree);
    printf("\nTree destroyed.\n");

    return 0;
}