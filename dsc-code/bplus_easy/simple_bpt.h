#pragma once

#include <stdbool.h>

// --- Structures ---

// Represents a node in the B+ Tree.
typedef struct Node {
    bool is_leaf;
    int num_keys;
    int* keys;

    // Simplified: Pointers for both children and values exist,
    // but only one is used depending on is_leaf.
    struct Node** children; // Used by internal nodes
    int* values;            // Used by leaf nodes

    struct Node* next;      // Used by leaf nodes for linked list
    struct Node* parent;    // Pointer to parent node
} Node;

// Represents the entire B+ Tree.
typedef struct BPlusTree {
    Node* root;
    int degree; // This is 't', the minimum degree.
} BPlusTree;

// --- Public API ---

// Creates a new B+ Tree with a given minimum degree 't'.
BPlusTree* create_bplus_tree(int degree);

// Destroys the tree and frees all associated memory.
void destroy_bplus_tree(BPlusTree* tree);

// Inserts a key-value pair into the tree.
void insert(BPlusTree* tree, int key, int value);

// Searches for a key and returns a pointer to its value.
// Returns NULL if the key is not found.
int* search(BPlusTree* tree, int key);

// Prints the tree structure for debugging and visualization.
void print_tree(BPlusTree* tree);
