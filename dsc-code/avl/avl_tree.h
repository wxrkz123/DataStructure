#pragma once
#include <stddef.h>
#include <stdbool.h>

// 前向声明
typedef struct AVLNode AVLNode;
typedef struct AVLTree AVLTree;

// 比较函数类型：返回值 < 0 表示 a < b，= 0 表示 a = b，> 0 表示 a > b
typedef int (*AVLCompareFunc)(const void* a, const void* b, void* context);

// 遍历回调函数类型
typedef void (*AVLTraverseFunc)(void* data, void* context);

// 释放数据的回调函数类型
typedef void (*AVLFreeFunc)(void* data, void* context);

// 创建AVL树
AVLTree* avl_create(AVLCompareFunc compare, AVLFreeFunc free_func, void* context);

// 销毁AVL树
void avl_destroy(AVLTree* tree);

// 插入元素（如果已存在则返回false）
bool avl_insert(AVLTree* tree, void* data);

// 删除元素（返回是否成功删除）
bool avl_delete(AVLTree* tree, const void* data);

// 查找元素
void* avl_find(const AVLTree* tree, const void* data);

// 更新元素（先删除旧的，再插入新的）
bool avl_update(AVLTree* tree, const void* old_data, void* new_data);

// 获取树的大小
size_t avl_size(const AVLTree* tree);

// 检查树是否为空
bool avl_is_empty(const AVLTree* tree);

// 中序遍历
void avl_traverse_inorder(const AVLTree* tree, AVLTraverseFunc func, void* context);

// 前序遍历
void avl_traverse_preorder(const AVLTree* tree, AVLTraverseFunc func, void* context);

// 后序遍历
void avl_traverse_postorder(const AVLTree* tree, AVLTraverseFunc func, void* context);

// 获取树的高度
int avl_height(const AVLTree* tree);

// 验证AVL树的平衡性（用于调试）
bool avl_validate(const AVLTree* tree);
