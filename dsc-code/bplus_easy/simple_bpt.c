#include "simple_bpt.h"
#include <stdio.h>
#include <stdlib.h>

// --- Forward Declarations for Internal Functions ---
Node* create_node(int degree, bool is_leaf);
void insert_into_parent(BPlusTree* tree, Node* parent, int key, Node* new_node);
Node* find_leaf(Node* root, int key);

// --- Public API Implementation ---

BPlusTree* create_bplus_tree(int degree) {
    if (degree < 2) {
        // Degree 't' must be at least 2 for a B-Tree structure.
        // t=2 means max_keys=3, min_keys=1.
        return NULL;
    }
    BPlusTree* tree = (BPlusTree*)malloc(sizeof(BPlusTree));
    if (!tree) {
        return NULL;
    }
    tree->degree = degree;
    tree->root = create_node(degree, true);
    return tree;
}

// Helper for recursive destruction
void destroy_node(Node* node) {
    if (node == NULL) return;

    if (!node->is_leaf) {
        for (int i = 0; i <= node->num_keys; i++) {
            destroy_node(node->children[i]);
        }
        free(node->children);
    }
    else {
        free(node->values);
    }
    free(node->keys);
    free(node);
}

void destroy_bplus_tree(BPlusTree* tree) {
    if (tree) {
        destroy_node(tree->root);
        free(tree);
    }
}


void insert(BPlusTree* tree, int key, int value) {
    Node* leaf = find_leaf(tree->root, key);

    // Find insertion position
    int i = 0;
    while (i < leaf->num_keys && key > leaf->keys[i]) {
        i++;
    }

    // Shift keys and values to make space
    for (int j = leaf->num_keys; j > i; j--) {
        leaf->keys[j] = leaf->keys[j - 1];
        leaf->values[j] = leaf->values[j - 1];
    }

    // Insert new key and value
    leaf->keys[i] = key;
    leaf->values[i] = value;
    leaf->num_keys++;

    // Check if the node needs to be split
    // max_keys = 2 * t - 1
    if (leaf->num_keys > 2 * tree->degree - 1) {
        // 10, 20, 30, 40 L1
        Node* new_leaf = create_node(tree->degree, true); // L2
        int mid_point = (leaf->num_keys) / 2;

        // Copy second half to the new node
        for (int j = mid_point; j < leaf->num_keys; j++) {
            new_leaf->keys[new_leaf->num_keys] = leaf->keys[j];
            new_leaf->values[new_leaf->num_keys] = leaf->values[j];
            new_leaf->num_keys++;
        }

        // L2 [30, 40]

        // Update original node's key count
        leaf->num_keys = mid_point;
		// L1 [10, 20] L2 [30,40]

        // Link the leaves
        new_leaf->next = leaf->next;
        leaf->next = new_leaf;

        // Key to be promoted to parent
        int promoted_key = new_leaf->keys[0];

        insert_into_parent(tree, leaf->parent, promoted_key, new_leaf);
    }
}

int* search(BPlusTree* tree, int key) {
    Node* leaf = find_leaf(tree->root, key);
    for (int i = 0; i < leaf->num_keys; i++) {
        if (leaf->keys[i] == key) {
            // Return a pointer to the value
            return &leaf->values[i];
        }
    }
    return NULL;
}


// --- Internal Helper Function Implementation ---

Node* create_node(int degree, bool is_leaf) {
    Node* node = (Node*)calloc(1, sizeof(Node));
    node->is_leaf = is_leaf;
    // max_keys = 2*t - 1. We allocate +1 for temporary overflow during insertion.
    int max_keys = 2 * degree - 1;
    node->keys = (int*)malloc(sizeof(int) * (max_keys + 1));

    if (is_leaf) {
        node->values = (int*)malloc(sizeof(int) * (max_keys + 1));
        node->children = NULL;
    }
    else {
        node->values = NULL;
        // max_children = 2*t. We allocate +1 for temporary overflow.
        node->children = (Node**)malloc(sizeof(Node*) * (max_keys + 2));
    }
    return node;
}


Node* find_leaf(Node* node, int key) {
    while (!node->is_leaf) {
        int i = 0;
        while (i < node->num_keys && key >= node->keys[i]) {
            i++;
        }
        node = node->children[i];
    }
    return node;
}

// This function handles splitting and recursive insertion into parents.
void insert_into_parent(BPlusTree* tree, Node* node, int key, Node* new_node) {

    // [10,20] L1  [30,40] L2
    if (node == NULL) { // This means we split the root
        Node* new_root = create_node(tree->degree, false);
        new_root->keys[0] = key;
        new_root->children[0] = tree->root;
        new_root->children[1] = new_node;
        new_root->num_keys = 1;
        tree->root->parent = new_root;
        new_node->parent = new_root;
        tree->root = new_root;
        return;
    }

    // Find insertion position for the new key and child
    int i = 0;
    while (i < node->num_keys && key > node->keys[i]) {
        i++;
    }

    // Shift keys and children to make space
    for (int j = node->num_keys; j > i; j--) {
        node->keys[j] = node->keys[j - 1];
        node->children[j + 1] = node->children[j];
    }

    // Insert new key and child
    node->keys[i] = key;
    node->children[i + 1] = new_node;
    new_node->parent = node;
    node->num_keys++;

    // Check if the parent node also needs to be split
    if (node->num_keys > 2 * tree->degree - 1) {
        Node* new_parent = create_node(tree->degree, false);
        int mid_point = node->num_keys / 2;
        int promoted_key = node->keys[mid_point];

        // Copy second half to new node (internal node split)
        for (int j = mid_point + 1; j < node->num_keys; j++) {
            new_parent->keys[new_parent->num_keys] = node->keys[j];
            new_parent->children[new_parent->num_keys] = node->children[j];
            new_parent->children[new_parent->num_keys]->parent = new_parent;
            new_parent->num_keys++;
        }
        new_parent->children[new_parent->num_keys] = node->children[node->num_keys];
        new_parent->children[new_parent->num_keys]->parent = new_parent;

        node->num_keys = mid_point;

        // Recursively insert into grandparent
        insert_into_parent(tree, node->parent, promoted_key, new_parent);
    }
}


void print_tree(BPlusTree* tree) {
    if (!tree || !tree->root) return;

    Node* queue[100]; // Simple queue for level-order traversal
    int head = 0, tail = 0;
    int level_nodes = 1;

    queue[tail++] = tree->root;

    while (head < tail) {
        int next_level_nodes = 0;
        for (int i = 0; i < level_nodes; i++) {
            Node* current = queue[head++];
            printf("[");
            for (int j = 0; j < current->num_keys; j++) {
                printf("%d%s", current->keys[j], j == current->num_keys - 1 ? "" : "|");
            }
            printf("] ");

            if (!current->is_leaf) {
                for (int j = 0; j <= current->num_keys; j++) {
                    queue[tail++] = current->children[j];
                    next_level_nodes++;
                }
            }
        }
        printf("\n");
        level_nodes = next_level_nodes;
    }
    printf("--------------------------------\n");
}