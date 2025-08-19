#include "bplus_tree.h"

#include <assert.h>



 /* ========================================================================= */

 /*                           INTERNAL STRUCTURES                             */

 /* ========================================================================= */



 /* B+ Tree node structure */

struct bplus_node {

    bool is_leaf;

    int num_keys;

    void** keys;

	// keys[0] to keys[num_keys-1] are valid

    // keys[1] -》 指向第二个关键字

    union {

        bplus_node_t** children;  /* For internal nodes */

        void** values;             /* For leaf nodes */

    } data;

    bplus_node_t* next;           /* Next leaf (for leaf nodes only) */

    bplus_node_t* parent;         /* Parent node reference */

};



/* Context structure - encapsulates all state and configuration */

struct bplus_context {

    /* Configuration */

    int degree;

	int max_keys; // 2 * degree - 1

	int min_keys; //degree - 1



    /* Function pointers for generic operations */

    bplus_compare_fn compare;

    bplus_alloc_fn alloc;

    bplus_free_fn free;

    bplus_key_clone_fn key_clone;

    bplus_key_free_fn key_free;

    bplus_value_clone_fn value_clone;

    bplus_value_free_fn value_free;



    /* User data passed to callbacks */

    void* user_data;



    /* Statistics */

    size_t node_count;

    size_t key_count;

    size_t height;



    /* Memory pool (optional optimization) - not implemented */

    void* memory_pool;

    size_t pool_size;

};



/* B+ Tree structure */

struct bplus_tree {

    bplus_node_t* root;

    bplus_context_t* ctx;

};



/* Iterator structure */

struct bplus_iterator {

    bplus_tree_t* tree;

    bplus_node_t* current_node;

    int current_index;

    bool finished;

};



/* ========================================================================= */

/*                          STATIC HELPER FUNCTIONS                          */

/* ========================================================================= */



/* --- Default Callbacks --- */



static void* default_alloc(size_t size, void* user_data) {

    (void)user_data;

    return calloc(1, size);

}



static void default_free(void* ptr, void* user_data) {

    (void)user_data;

    free(ptr);

}



static void* default_clone(const void* ptr, void* user_data) {

    (void)user_data;

    return (void*)ptr;

}



static void default_free_noop(void* ptr, void* user_data) {

    (void)ptr;

    (void)user_data;

}



/* --- Node Management --- */



static bplus_node_t* bplus_node_create(bplus_context_t* ctx, bool is_leaf) {

    bplus_node_t* node = ctx->alloc(sizeof(bplus_node_t), ctx->user_data);

    if (!node) return NULL;



    node->is_leaf = is_leaf;

    node->num_keys = 0;

    node->keys = ctx->alloc(sizeof(void*) * (ctx->max_keys + 1), ctx->user_data); // +1 for temporary overflow



    if (is_leaf) {

        node->data.values = ctx->alloc(sizeof(void*) * (ctx->max_keys + 1), ctx->user_data); // +1 for temporary overflow

    }

    else {

        node->data.children = ctx->alloc(sizeof(bplus_node_t*) * (ctx->max_keys + 2), ctx->user_data); // +1 for temporary overflow

    }



    if (!node->keys || (!node->data.values && is_leaf) || (!node->data.children && !is_leaf)) {

        // Handle allocation failure

        if (node->keys) ctx->free(node->keys, ctx->user_data);

        if (node->data.values) ctx->free(node->data.values, ctx->user_data);

        if (node->data.children) ctx->free(node->data.children, ctx->user_data);

        ctx->free(node, ctx->user_data);

        return NULL;

    }



    node->next = NULL;

    node->parent = NULL;



    ctx->node_count++;

    return node;

}



static void bplus_node_destroy(bplus_context_t* ctx, bplus_node_t* node) {

    if (!node) return;



    for (int i = 0; i < node->num_keys; i++) {

        ctx->key_free(node->keys[i], ctx->user_data);

        if (node->is_leaf) {

            ctx->value_free(node->data.values[i], ctx->user_data);

        }

    }



    if (!node->is_leaf) {

        for (int i = 0; i <= node->num_keys; i++) {

            bplus_node_destroy(ctx, node->data.children[i]);

        }

        ctx->free(node->data.children, ctx->user_data);

    }

    else {

        ctx->free(node->data.values, ctx->user_data);

    }



    ctx->free(node->keys, ctx->user_data);

    ctx->free(node, ctx->user_data);

    ctx->node_count--;

}





/* --- Search and Split Logic --- */



static bplus_node_t* find_leaf(bplus_context_t* ctx, bplus_node_t* node, void* key) {

    int i;

    while (!node->is_leaf) {

        i = 0;

        while (i < node->num_keys && ctx->compare(key, node->keys[i], ctx->user_data) >= 0) {

            i++;

        }

        node = node->data.children[i];

    }

    return node;

}





static bplus_error_t bplus_insert_into_leaf(bplus_context_t* ctx, bplus_node_t* leaf, void* key, void* value) {

    int i = 0;

    while (i < leaf->num_keys && ctx->compare(key, leaf->keys[i], ctx->user_data) > 0) {

        i++;

    }



    // Check for duplicates before insertion

    if (i < leaf->num_keys&& ctx->compare(key, leaf->keys[i], ctx->user_data) == 0) {

        return BPLUS_ERROR_KEY_EXISTS;

    }



    for (int j = leaf->num_keys; j > i; j--) {

        leaf->keys[j] = leaf->keys[j - 1];

        leaf->data.values[j] = leaf->data.values[j - 1];

    }



    leaf->keys[i] = ctx->key_clone(key, ctx->user_data);

    leaf->data.values[i] = ctx->value_clone(value, ctx->user_data);

    leaf->num_keys++;

    ctx->key_count++;

    return BPLUS_SUCCESS;

}





static void bplus_insert_into_parent(bplus_context_t* ctx, bplus_node_t* left, void* key, bplus_node_t* right, bplus_tree_t* tree) {

    bplus_node_t* parent = left->parent;

    if (parent == NULL) {

        // Create a new root

        bplus_node_t* new_root = bplus_node_create(ctx, false);

        new_root->keys[0] = key;

        new_root->data.children[0] = left;

        new_root->data.children[1] = right;

        new_root->num_keys = 1;



        left->parent = new_root;

        right->parent = new_root;

        tree->root = new_root;

        ctx->height++;

        return;

    }



    int insert_index = 0;

    while (insert_index < parent->num_keys&& parent->data.children[insert_index] != left) {

        insert_index++;

    }



    for (int i = parent->num_keys; i > insert_index; i--) {

        parent->keys[i] = parent->keys[i - 1];

        parent->data.children[i + 1] = parent->data.children[i];

    }



    parent->keys[insert_index] = key;

    parent->data.children[insert_index + 1] = right;

    parent->num_keys++;



    if (parent->num_keys > ctx->max_keys) {

        // Split the parent

        bplus_node_t* new_parent_node = bplus_node_create(ctx, false);

        int mid_point = (ctx->max_keys + 1) / 2;



        void* promoted_key = parent->keys[mid_point - 1];



        new_parent_node->num_keys = ctx->max_keys - mid_point;

        for (int i = mid_point; i < ctx->max_keys; i++) {

            new_parent_node->keys[i - mid_point] = parent->keys[i];

        }

        for (int i = mid_point; i <= ctx->max_keys; i++) {

            new_parent_node->data.children[i - mid_point] = parent->data.children[i];

            new_parent_node->data.children[i - mid_point]->parent = new_parent_node;

        }



        parent->num_keys = mid_point - 1;

        new_parent_node->parent = parent->parent;



        bplus_insert_into_parent(ctx, parent, promoted_key, new_parent_node, tree);

    

        // TODO

    }

}





/* ========================================================================= */

/*                          PUBLIC API IMPLEMENTATION                        */

/* ========================================================================= */



bplus_context_t* bplus_context_create(int degree, bplus_compare_fn compare, void* user_data) {

    if (degree < BPLUS_MIN_DEGREE || degree > BPLUS_MAX_DEGREE || !compare) {

        return NULL;

    }



    bplus_context_t* ctx = calloc(1, sizeof(bplus_context_t));

    if (!ctx) return NULL;



    ctx->degree = degree;

    ctx->max_keys = 2 * degree - 1;

    ctx->min_keys = degree - 1;

    ctx->compare = compare;

    ctx->user_data = user_data;



    ctx->alloc = default_alloc;

    ctx->free = default_free;

    ctx->key_clone = default_clone;

    ctx->key_free = default_free_noop;

    ctx->value_clone = default_clone;

    ctx->value_free = default_free_noop;



    return ctx;

}



void bplus_context_set_memory_functions(bplus_context_t* ctx, bplus_alloc_fn alloc_fn, bplus_free_fn free_fn) {

    if (!ctx) return;

    ctx->alloc = alloc_fn ? alloc_fn : default_alloc;

    ctx->free = free_fn ? free_fn : default_free;

}



void bplus_context_set_key_functions(bplus_context_t* ctx, bplus_key_clone_fn clone_fn, bplus_key_free_fn free_fn) {

    if (!ctx) return;

    ctx->key_clone = clone_fn ? clone_fn : default_clone;

    ctx->key_free = free_fn ? free_fn : default_free_noop;

}



void bplus_context_set_value_functions(bplus_context_t* ctx, bplus_value_clone_fn clone_fn, bplus_value_free_fn free_fn) {

    if (!ctx) return;

    ctx->value_clone = clone_fn ? clone_fn : default_clone;

    ctx->value_free = free_fn ? free_fn : default_free_noop;

}



void bplus_context_destroy(bplus_context_t* ctx) {

    if (!ctx) return;

    free(ctx);

}





bplus_tree_t* bplus_tree_create(bplus_context_t* ctx) {

    if (!ctx) return NULL;



    bplus_tree_t* tree = ctx->alloc(sizeof(bplus_tree_t), ctx->user_data);

    if (!tree) return NULL;



    tree->ctx = ctx;

    tree->root = bplus_node_create(ctx, true);

    if (!tree->root) {

        ctx->free(tree, ctx->user_data);

        return NULL;

    }



    ctx->height = 1;

    return tree;

}



void bplus_tree_destroy(bplus_tree_t* tree) {

    if (!tree) return;

    bplus_node_destroy(tree->ctx, tree->root);

    tree->ctx->free(tree, tree->ctx->user_data);

}





bplus_error_t bplus_tree_insert(bplus_tree_t* tree, void* key, void* value) {

    if (!tree || !key) return BPLUS_ERROR_INVALID_PARAM;

    bplus_context_t* ctx = tree->ctx;



    bplus_node_t* leaf = find_leaf(ctx, tree->root, key);



    if (leaf->num_keys < ctx->max_keys) {

        return bplus_insert_into_leaf(ctx, leaf, key, value);

    }



    // Leaf is full, must split

    bplus_node_t* new_leaf = bplus_node_create(ctx, true);

    void** temp_keys = ctx->alloc(sizeof(void*) * (ctx->max_keys + 1), ctx->user_data);

    void** temp_values = ctx->alloc(sizeof(void*) * (ctx->max_keys + 1), ctx->user_data);



    int insertion_point = 0;

    while (insertion_point < ctx->max_keys && ctx->compare(key, leaf->keys[insertion_point], ctx->user_data) > 0) {

        insertion_point++;

    }



    // Check for duplicates before splitting

    if (insertion_point < ctx->max_keys && ctx->compare(key, leaf->keys[insertion_point], ctx->user_data) == 0) {

        ctx->free(temp_keys, ctx->user_data);

        ctx->free(temp_values, ctx->user_data);

        bplus_node_destroy(ctx, new_leaf);

        return BPLUS_ERROR_KEY_EXISTS;

    }



    int i = 0, j = 0;

    while (i < leaf->num_keys) {

        if (j == insertion_point) j++;

        temp_keys[j] = leaf->keys[i];

        temp_values[j] = leaf->data.values[i];

        i++; j++;

    }

    temp_keys[insertion_point] = ctx->key_clone(key, ctx->user_data);

    temp_values[insertion_point] = ctx->value_clone(value, ctx->user_data);



    leaf->num_keys = 0;

    int split_point = (ctx->max_keys + 1) / 2;



    for (i = 0; i < split_point; i++) {

        leaf->keys[i] = temp_keys[i];

        leaf->data.values[i] = temp_values[i];

        leaf->num_keys++;

    }



    for (i = split_point; i < ctx->max_keys + 1; i++) {

        new_leaf->keys[i - split_point] = temp_keys[i];

        new_leaf->data.values[i - split_point] = temp_values[i];

        new_leaf->num_keys++;

    }



    ctx->free(temp_keys, ctx->user_data);

    ctx->free(temp_values, ctx->user_data);



    new_leaf->next = leaf->next;

    leaf->next = new_leaf;



    new_leaf->parent = leaf->parent;

    void* new_key_for_parent = ctx->key_clone(new_leaf->keys[0], ctx->user_data);



    ctx->key_count++;



    bplus_insert_into_parent(ctx, leaf, new_key_for_parent, new_leaf, tree);



    return BPLUS_SUCCESS;

}





// NOTE: Deletion is complex and has been omitted for this example.

// A full implementation would require handling underflow, merging, and redistribution.

bplus_error_t bplus_tree_delete(bplus_tree_t* tree, void* key) {

    // Stub function

    (void)tree;

    (void)key;

    printf("WARN: bplus_tree_delete is not fully implemented.\n");

    return BPLUS_SUCCESS;

}





void* bplus_tree_search(bplus_tree_t* tree, void* key) {

    if (!tree || !key) return NULL;



    bplus_node_t* leaf = find_leaf(tree->ctx, tree->root, key);



    // Simple linear scan for small N; binary search for larger

    for (int i = 0; i < leaf->num_keys; i++) {

        if (tree->ctx->compare(key, leaf->keys[i], tree->ctx->user_data) == 0) {

            return leaf->data.values[i];

        }

    }

    return NULL;

}





bool bplus_tree_contains(bplus_tree_t* tree, void* key) {

    return bplus_tree_search(tree, key) != NULL;

}



size_t bplus_tree_size(const bplus_tree_t* tree) {

    return tree ? tree->ctx->key_count : 0;

}



bool bplus_tree_empty(const bplus_tree_t* tree) {

    return !tree || tree->ctx->key_count == 0;

}





bplus_iterator_t* bplus_iterator_create(bplus_tree_t* tree) {

    if (!tree) return NULL;



    bplus_iterator_t* iter = tree->ctx->alloc(sizeof(bplus_iterator_t), tree->ctx->user_data);

    if (!iter) return NULL;



    iter->tree = tree;

    iter->current_node = tree->root;

    while (iter->current_node && !iter->current_node->is_leaf) {

        iter->current_node = iter->current_node->data.children[0];

    }



    iter->current_index = 0;

    iter->finished = (!iter->current_node || iter->current_node->num_keys == 0);



    return iter;

}



bool bplus_iterator_has_next(bplus_iterator_t* iter) {

    return iter && !iter->finished;

}



void bplus_iterator_next(bplus_iterator_t* iter, void** key, void** value) {

    if (!iter || iter->finished) {

        if (key) *key = NULL;

        if (value) *value = NULL;

        return;

    }



    if (key) *key = iter->current_node->keys[iter->current_index];

    if (value) *value = iter->current_node->data.values[iter->current_index];



    iter->current_index++;



    if (iter->current_index >= iter->current_node->num_keys) {

        iter->current_node = iter->current_node->next;

        iter->current_index = 0;



        if (!iter->current_node) {

            iter->finished = true;

        }

    }

}



void bplus_iterator_destroy(bplus_iterator_t* iter) {

    if (!iter) return;

    iter->tree->ctx->free(iter, iter->tree->ctx->user_data);

}





static void bplus_print_node(bplus_node_t* node, int level, void (*print_key)(void*)) {

    if (!node) return;

    for (int i = 0; i < level; i++) printf("  ");



    printf("Level %d [%p]: ", level, (void*)node);

    for (int i = 0; i < node->num_keys; i++) {

        print_key(node->keys[i]);

        printf(" ");

    }

    printf("\n");



    if (!node->is_leaf) {

        for (int i = 0; i <= node->num_keys; i++) {

            bplus_print_node(node->data.children[i], level + 1, print_key);

        }

    }

}



void bplus_tree_print(bplus_tree_t* tree, void (*print_key)(void*)) {

    if (!tree || !print_key) return;

    printf("B+ Tree (height=%zu, nodes=%zu, keys=%zu):\n",

        tree->ctx->height, tree->ctx->node_count, tree->ctx->key_count);

    bplus_print_node(tree->root, 0, print_key);

}





// NOTE: Validation is complex and has been omitted for this example.

bool bplus_tree_validate(bplus_tree_t* tree) {

    // Stub function

    (void)tree;

    printf("WARN: bplus_tree_validate is not implemented.\n");

    return true;

}