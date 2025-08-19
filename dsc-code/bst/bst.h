// binary_search_tree.h
#pragma once

#include <stddef.h>
#include <stdbool.h>

// --- Opaque Pointer and Function Pointer Type Definitions ---

typedef struct BST BST;

/**
 * 比较函数指针类型。
 * @param a 指向第一个元素的指针。
 * @param b 指向第二个元素的指针。
 * @return > 0 如果 a > b。
 * @return < 0 如果 a < b。
 * @return 0   如果 a == b。
 */
typedef int (*CompareFunc)(const void* a, const void* b);

/**
 * 访问函数指针类型，用于遍历。
 * @param data 指向节点数据的指针。
 */
typedef void (*VisitFunc)(const void* data);

// 遍历顺序的枚举
typedef enum {
    IN_ORDER,   // 中序遍历 (LNR -> 结果有序)
    PRE_ORDER,  // 前序遍历 (NLR)
    POST_ORDER  // 后序遍历 (LRN)
} TraverseOrder;


// --- Public API Prototypes ---

/**
 * @brief 创建一个新的泛型二叉搜索树。
 *
 * @param element_size 每个元素的大小（字节）。
 * @param compare_func 用于比较元素的函数指针，这是BST的核心。
 * @return 成功时返回指向新树的指针，失败返回 NULL。
 */
BST* bst_create(size_t element_size, CompareFunc compare_func);

/**
 * @brief 销毁一个二叉搜索树并释放所有节点和内存。
 */
void bst_destroy(BST** p_bst);

/**
 * @brief 在树中插入一个新元素。
 * 如果元素已存在，则不进行任何操作。
 */
bool bst_insert(BST* bst, const void* element_data);

/**
 * @brief 从树中移除一个元素。
 */
bool bst_remove(BST* bst, const void* element_data);

/**
 * @brief 检查一个元素是否存在于树中。
 */
bool bst_search(const BST* bst, const void* key);

/**
 * @brief 按照指定的顺序遍历树，并对每个节点执行访问函数。
 */
void bst_traverse(const BST* bst, TraverseOrder order, VisitFunc visit_func);

bool bst_is_empty(const BST* bst);
size_t bst_get_size(const BST* bst);
