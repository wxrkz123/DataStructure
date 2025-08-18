#include "bplus_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

 /* ========================================================================= */
 /*                          EXAMPLE HELPER FUNCTIONS                         */
 /* ========================================================================= */

 /* Integer comparison function */
static int int_compare(const void* a, const void* b, void* user_data) {
    (void)user_data;
    int va = *(int*)a;
    int vb = *(int*)b;
    return (va > vb) - (va < vb);
}

/* Integer key/value clone function (for deep copies) */
static void* int_clone(const void* key, void* user_data) {
    (void)user_data;
    int* new_key = malloc(sizeof(int));
    if (new_key) {
        *new_key = *(int*)key;
    }
    return new_key;
}

/* Integer key/value free function (for deep copies) */
static void int_free(void* key, void* user_data) {
    (void)user_data;
    free(key);
}

/* Print integer key */
static void print_int(void* key) {
    if (key) {
        printf("%d", *(int*)key);
    }
    else {
        printf("NULL");
    }
}

/* ========================================================================= */
/*                                 TEST SUITE                                */
/* ========================================================================= */

void run_bplus_tree_tests(void) {
    printf("=== B+ Tree Test Suite ===\n\n");

    /* Create context with degree 3 and deep-copy functions */
    bplus_context_t* ctx = bplus_context_create(3, int_compare, NULL);
    bplus_context_set_key_functions(ctx, int_clone, int_free);
    bplus_context_set_value_functions(ctx, int_clone, int_free);
    assert(ctx != NULL);

    /* Create tree */
    bplus_tree_t* tree = bplus_tree_create(ctx);
    assert(tree != NULL);

    printf("--- Test 1: Insertions ---\n");
    int keys[] = { 10, 20, 5, 6, 12, 30, 7, 17, 3, 8, 15, 25, 35, 40, 45 };
    int n = sizeof(keys) / sizeof(keys[0]);

    for (int i = 0; i < n; i++) {
        bplus_error_t err = bplus_tree_insert(tree, &keys[i], &keys[i]);
        printf("Inserted key=%d ... ", keys[i]);
        assert(err == BPLUS_SUCCESS);
        printf("OK\n");
    }

    printf("\nTree size: %zu (expected: %d)\n", bplus_tree_size(tree), n);
    assert(bplus_tree_size(tree) == (size_t)n);

    printf("\nTree structure after insertions:\n");
    bplus_tree_print(tree, print_int);

    printf("\n--- Test 2: Search for existing keys ---\n");
    for (int i = 0; i < n; i++) {
        int* found = bplus_tree_search(tree, &keys[i]);
        printf("Searching for key=%d ... ", keys[i]);
        assert(found != NULL);
        assert(*found == keys[i]);
        printf("Found value=%d. OK\n", *found);
    }

    printf("\n--- Test 3: Search for non-existing keys ---\n");
    int non_existing[] = { 1, 100, 50 };
    for (int i = 0; i < 3; i++) {
        void* found = bplus_tree_search(tree, &non_existing[i]);
        printf("Searching for key=%d ... ", non_existing[i]);
        assert(found == NULL);
        printf("Not found (as expected). OK\n");
    }

    printf("\n--- Test 4: Iterator Traversal ---\n");
    bplus_iterator_t* iter = bplus_iterator_create(tree);
    int count = 0;
    int prev_key = -1;
    while (bplus_iterator_has_next(iter)) {
        int* key;
        int* value;
        bplus_iterator_next(iter, (void**)&key, (void**)&value);
        printf("Iterator: key=%d, value=%d\n", *key, *value);
        if (count > 0) {
            assert(*key > prev_key); // Check order
        }
        prev_key = *key;
        count++;
    }
    printf("Iterator traversed %d elements. OK\n", count);
    assert(count == n);
    bplus_iterator_destroy(iter);

    printf("\n--- Test 5: Duplicate key insertion ---\n");
    int dup_key = 10;
    bplus_error_t err = bplus_tree_insert(tree, &dup_key, &dup_key);
    printf("Attempting to insert duplicate key %d ... ", dup_key);
    assert(err == BPLUS_ERROR_KEY_EXISTS);
    printf("Rejected (as expected). OK\n");

    // Deletion tests would go here if implemented
    // ...

    printf("\n--- Test 6: Clean up ---\n");
    bplus_tree_destroy(tree);
    bplus_context_destroy(ctx);
    printf("Tree and context destroyed successfully.\n");

    printf("\n=== All tests passed! ===\n");
}

int main(void) {
    run_bplus_tree_tests();
    return 0;
}