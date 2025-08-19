#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4996) // VS2022 compatibility
#endif

 /* ========================================================================= */
 /*                              CONFIGURATION                                */
 /* ========================================================================= */

#define BPLUS_MIN_DEGREE 3 // 规定B+树的最小度数t不能小于3，这里是B+。
#define BPLUS_MAX_DEGREE 128
#define BPLUS_DEFAULT_DEGREE 4

/* ========================================================================= */
/*                              TYPE DEFINITIONS                             */
/* ========================================================================= */

/* Error codes returned by API functions */
typedef enum {
    BPLUS_SUCCESS = 0,
    BPLUS_ERROR_MEMORY = -1,
    BPLUS_ERROR_INVALID_PARAM = -2,
    BPLUS_ERROR_KEY_EXISTS = -3,
    BPLUS_ERROR_KEY_NOT_FOUND = -4,
    BPLUS_ERROR_TREE_EMPTY = -5
} bplus_error_t;

/* Forward declarations for opaque structures */
typedef struct bplus_node bplus_node_t;
typedef struct bplus_tree bplus_tree_t;
typedef struct bplus_context bplus_context_t;
typedef struct bplus_iterator bplus_iterator_t;

/* --- Callback Function Pointer Types for Generic Programming --- */

/* Function to compare two keys.
 * Should return:
 *  < 0 if a < b
 *  = 0 if a == b
 *  > 0 if a > b
 */
typedef int (*bplus_compare_fn)(const void* a, const void* b, void* user_data);

/* Memory allocation functions */
typedef void* (*bplus_alloc_fn)(size_t size, void* user_data);
typedef void (*bplus_free_fn)(void* ptr, void* user_data);

/* Key cloning and freeing functions */
typedef void* (*bplus_key_clone_fn)(const void* key, void* user_data);
typedef void (*bplus_key_free_fn)(void* key, void* user_data);

/* Value cloning and freeing functions */
typedef void* (*bplus_value_clone_fn)(const void* value, void* user_data);
typedef void (*bplus_value_free_fn)(void* value, void* user_data);

/* ========================================================================= */
/*                                PUBLIC API                                 */
/* ========================================================================= */

/* --- Context Management ---
 * The context holds all configuration and callback functions for a tree.
 */
bplus_context_t* bplus_context_create(
    int degree,
    bplus_compare_fn compare,
    void* user_data
);

void bplus_context_set_memory_functions(
    bplus_context_t* ctx,
    bplus_alloc_fn alloc_fn,
    bplus_free_fn free_fn
);

void bplus_context_set_key_functions(
    bplus_context_t* ctx,
    bplus_key_clone_fn clone_fn,
    bplus_key_free_fn free_fn
);

void bplus_context_set_value_functions(
    bplus_context_t* ctx,
    bplus_value_clone_fn clone_fn,
    bplus_value_free_fn free_fn
);

void bplus_context_destroy(bplus_context_t* ctx);


/* --- Tree Operations --- */
bplus_tree_t* bplus_tree_create(bplus_context_t* ctx);
void bplus_tree_destroy(bplus_tree_t* tree);
bplus_error_t bplus_tree_insert(bplus_tree_t* tree, void* key, void* value);
bplus_error_t bplus_tree_delete(bplus_tree_t* tree, void* key);
void* bplus_tree_search(bplus_tree_t* tree, void* key);
bool bplus_tree_contains(bplus_tree_t* tree, void* key);
size_t bplus_tree_size(const bplus_tree_t* tree);
bool bplus_tree_empty(const bplus_tree_t* tree);


/* --- Iterator Operations ---
 * Allows for ordered traversal of the tree.
 */
bplus_iterator_t* bplus_iterator_create(bplus_tree_t* tree);
bool bplus_iterator_has_next(bplus_iterator_t* iter);
void bplus_iterator_next(bplus_iterator_t* iter, void** key, void** value);
void bplus_iterator_destroy(bplus_iterator_t* iter);


/* --- Utility and Debugging Functions --- */
void bplus_tree_print(bplus_tree_t* tree, void (*print_key)(void*));
bool bplus_tree_validate(bplus_tree_t* tree);


#ifdef _MSC_VER
#pragma warning(pop)
#endif
