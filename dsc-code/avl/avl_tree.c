#include "avl_tree.h"
#include <stdlib.h>
#include <assert.h>

// AVL节点结构
struct AVLNode {
	void* data;
	AVLNode* left;
	AVLNode* right;
	int height;
};

// AVL树结构

/*
* 结构体 AVLTree 用于表示 AVL 树。
* 它包含以下成员：
* - root: 指向树的根节点。
* - compare: 比较函数，用于比较两个数据元素。
* - free_func: 释放数据的回调函数，用于在删除节点时释放内存。
* - context: 上下文指针，用于传递额外信息给比较函数和释放函数。
* - size: 树中元素的数量。
*/
struct AVLTree {
	AVLNode* root;
	AVLCompareFunc compare;
	AVLFreeFunc free_func;
	void* context;
	size_t size;
};

// 内部函数声明
static AVLNode* node_create(void* data);
static void node_destroy(AVLNode* node, AVLFreeFunc free_func, void* context);
static int node_height(const AVLNode* node);
static void node_update_height(AVLNode* node);
static int node_balance_factor(const AVLNode* node);
static AVLNode* node_rotate_left(AVLNode* node);
static AVLNode* node_rotate_right(AVLNode* node);
static AVLNode* node_balance(AVLNode* node);
static AVLNode* node_insert(AVLNode* node, void* data, AVLCompareFunc compare, void* context, bool* inserted);
static AVLNode* node_delete(AVLNode* node, const void* data, AVLCompareFunc compare, void* context, AVLFreeFunc free_func, bool* deleted);
static AVLNode* node_find_min(AVLNode* node);
static void* node_find(const AVLNode* node, const void* data, AVLCompareFunc compare, void* context);
static void node_traverse_inorder(const AVLNode* node, AVLTraverseFunc func, void* context);
static void node_traverse_preorder(const AVLNode* node, AVLTraverseFunc func, void* context);
static void node_traverse_postorder(const AVLNode* node, AVLTraverseFunc func, void* context);
static bool node_validate(const AVLNode* node, AVLCompareFunc compare, void* context, const void* min, const void* max);

// 创建AVL树
AVLTree* avl_create(AVLCompareFunc compare, AVLFreeFunc free_func, void* context) {
	assert(compare != NULL);

	AVLTree* tree = (AVLTree*)malloc(sizeof(AVLTree));
	if (tree == NULL) {
		return NULL;
	}

	tree->root = NULL;
	tree->compare = compare;
	tree->free_func = free_func;
	tree->context = context;
	tree->size = 0;

	return tree;
}

// 销毁AVL树
void avl_destroy(AVLTree* tree) {
	if (tree == NULL) {
		return;
	}

	node_destroy(tree->root, tree->free_func, tree->context);
	free(tree);
}

// 插入元素
bool avl_insert(AVLTree* tree, void* data) {
	assert(tree != NULL);
	assert(data != NULL);

	bool inserted = false;
	tree->root = node_insert(tree->root, data, tree->compare, tree->context, &inserted);

	if (inserted) {
		tree->size++;
	}

	return inserted;
}

// 删除元素
bool avl_delete(AVLTree* tree, const void* data) {
	assert(tree != NULL);
	assert(data != NULL);

	bool deleted = false;
	tree->root = node_delete(tree->root, data, tree->compare, tree->context, tree->free_func, &deleted);

	if (deleted) {
		tree->size--;
	}

	return deleted;
}

// 查找元素
void* avl_find(const AVLTree* tree, const void* data) {
	assert(tree != NULL);
	assert(data != NULL);

	return node_find(tree->root, data, tree->compare, tree->context);
}

// 更新元素
bool avl_update(AVLTree* tree, const void* old_data, void* new_data) {
	assert(tree != NULL);
	assert(old_data != NULL);
	assert(new_data != NULL);

	// 如果新旧数据相等，直接返回成功
	if (tree->compare(old_data, new_data, tree->context) == 0) {
		return true;
	}

	// 先查找旧数据是否存在
	if (avl_find(tree, old_data) == NULL) {
		return false;
	}

	// 删除旧数据
	avl_delete(tree, old_data);

	// 插入新数据
	return avl_insert(tree, new_data);
}

// 获取树的大小
size_t avl_size(const AVLTree* tree) {
	assert(tree != NULL);
	return tree->size;
}

// 检查树是否为空
bool avl_is_empty(const AVLTree* tree) {
	assert(tree != NULL);
	return tree->size == 0;
}

// 中序遍历
void avl_traverse_inorder(const AVLTree* tree, AVLTraverseFunc func, void* context) {
	assert(tree != NULL);
	assert(func != NULL);

	node_traverse_inorder(tree->root, func, context);
}

// 前序遍历
void avl_traverse_preorder(const AVLTree* tree, AVLTraverseFunc func, void* context) {
	assert(tree != NULL);
	assert(func != NULL);

	node_traverse_preorder(tree->root, func, context);
}

// 后序遍历
void avl_traverse_postorder(const AVLTree* tree, AVLTraverseFunc func, void* context) {
	assert(tree != NULL);
	assert(func != NULL);

	node_traverse_postorder(tree->root, func, context);
}

// 获取树的高度
int avl_height(const AVLTree* tree) {
	assert(tree != NULL);
	return node_height(tree->root);
}

// 验证AVL树的平衡性
bool avl_validate(const AVLTree* tree) {
	assert(tree != NULL);
	return node_validate(tree->root, tree->compare, tree->context, NULL, NULL);
}

// 内部函数实现

// 创建节点
static AVLNode* node_create(void* data) {
	AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
	if (node == NULL) {
		return NULL;
	}

	node->data = data;
	node->left = NULL;
	node->right = NULL;
	node->height = 1;

	return node;
}

// 销毁节点及其子树
static void node_destroy(AVLNode* node, AVLFreeFunc free_func, void* context) {
	if (node == NULL) {
		return;
	}

	node_destroy(node->left, free_func, context);
	node_destroy(node->right, free_func, context);

	if (free_func != NULL) {
		free_func(node->data, context);
	}

	free(node);
}

// 获取节点高度
static int node_height(const AVLNode* node) {
	return node ? node->height : 0;
}

// 更新节点高度
static void node_update_height(AVLNode* node) {
	if (node == NULL) {
		return;
	}

	int left_height = node_height(node->left);
	int right_height = node_height(node->right);

	// 高度为子节点高度的最大值加1
	// 1 + max(left_height, right_height);
	node->height = 1 + (left_height > right_height ? left_height : right_height);
}

// 计算平衡因子
static int node_balance_factor(const AVLNode* node) {
	// 平衡因子 = 左子树高度 - 右子树高度

	return node ? node_height(node->left) - node_height(node->right) : 0;
}

// 左旋
static AVLNode* node_rotate_left(AVLNode* node) {
	assert(node != NULL);
	assert(node->right != NULL);

	AVLNode* new_root = node->right;
	node->right = new_root->left;
	new_root->left = node;

	node_update_height(node);
	node_update_height(new_root);

	return new_root;
}

// 右旋
static AVLNode* node_rotate_right(AVLNode* node) {
	assert(node != NULL);
	assert(node->left != NULL);

	AVLNode* new_root = node->left;
	node->left = new_root->right;
	new_root->right = node;

	node_update_height(node);
	node_update_height(new_root);

	return new_root;
}

// 平衡节点
static AVLNode* node_balance(AVLNode* node) {
	if (node == NULL) {
		return NULL;
	}

	node_update_height(node);
	int balance = node_balance_factor(node);

	// 左重
	if (balance > 1) {
		// 左右情况
		if (node_balance_factor(node->left) < 0) {
			node->left = node_rotate_left(node->left);
		}
		// 左左情况
		return node_rotate_right(node);
	}

	// 右重
	if (balance < -1) {
		// 右左情况
		if (node_balance_factor(node->right) > 0) {
			node->right = node_rotate_right(node->right);
		}
		// 右右情况
		return node_rotate_left(node);
	}

	return node;
}

// 插入节点
static AVLNode* node_insert(AVLNode* node,
	void* data, AVLCompareFunc compare,
	void* context, bool* inserted) {
	// 找到插入位置
	if (node == NULL) {
		*inserted = true;
		return node_create(data);
	}

	int cmp = compare(data, node->data, context);

	if (cmp < 0) {
		node->left = node_insert(node->left, data, compare, context, inserted);
	}
	else if (cmp > 0) {
		node->right = node_insert(node->right, data, compare, context, inserted);
	}
	else {
		// 元素已存在
		*inserted = false;
		return node;
	}

	// 重新平衡
	return node_balance(node);
}

// 查找最小节点
static AVLNode* node_find_min(AVLNode* node) {
	while (node != NULL && node->left != NULL) {
		node = node->left;
	}
	return node;
}

// 删除节点
static AVLNode* node_delete(AVLNode* node, const void* data, AVLCompareFunc compare, void* context, AVLFreeFunc free_func, bool* deleted) {
	if (node == NULL) {
		*deleted = false;
		return NULL;
	}

	int cmp = compare(data, node->data, context);

	if (cmp < 0) {
		node->left = node_delete(node->left, data, compare, context, free_func, deleted);
	}
	else if (cmp > 0) {
		node->right = node_delete(node->right, data, compare, context, free_func, deleted);
	}
	else {
		// 找到要删除的节点
		*deleted = true;

		if (node->left == NULL || node->right == NULL) {
			// 至少有一个子节点为空
			AVLNode* temp = node->left ? node->left : node->right;

			if (free_func != NULL) {
				free_func(node->data, context);
			}
			free(node);

			return temp;
		}
		else {
			// 两个子节点都存在
			AVLNode* min_right = node_find_min(node->right);

			// 交换数据
			void* temp_data = node->data;
			node->data = min_right->data;
			min_right->data = temp_data;

			// 删除后继节点
			node->right = node_delete(node->right, temp_data, compare, context, free_func, deleted);
		}
	}

	// 重新平衡
	return node_balance(node);
}

// 查找节点
static void* node_find(const AVLNode* node, const void* data, AVLCompareFunc compare, void* context) {
	if (node == NULL) {
		return NULL;
	}

	int cmp = compare(data, node->data, context);

	if (cmp < 0) {
		return node_find(node->left, data, compare, context);
	}
	else if (cmp > 0) {
		return node_find(node->right, data, compare, context);
	}
	else {
		return node->data;
	}
}

// 中序遍历
static void node_traverse_inorder(const AVLNode* node, AVLTraverseFunc func, void* context) {
	if (node == NULL) {
		return;
	}

	node_traverse_inorder(node->left, func, context);
	func(node->data, context);
	node_traverse_inorder(node->right, func, context);
}

// 前序遍历
static void node_traverse_preorder(const AVLNode* node, AVLTraverseFunc func, void* context) {
	if (node == NULL) {
		return;
	}

	func(node->data, context);
	node_traverse_preorder(node->left, func, context);
	node_traverse_preorder(node->right, func, context);
}

// 后序遍历
static void node_traverse_postorder(const AVLNode* node, AVLTraverseFunc func, void* context) {
	if (node == NULL) {
		return;
	}

	node_traverse_postorder(node->left, func, context);
	node_traverse_postorder(node->right, func, context);
	func(node->data, context);
}

// 验证节点
static bool node_validate(const AVLNode* node, 
	AVLCompareFunc compare, 
	void* context,
	const void* min, const void* max) {
	if (node == NULL) {
		return true;
	}

	// 检查BST性质
	if (min != NULL && compare(node->data, min, context) <= 0) {
		return false;
	}
	if (max != NULL && compare(node->data, max, context) >= 0) {
		return false;
	}

	// 检查平衡因子
	int balance = node_balance_factor(node);
	if (balance < -1 || balance > 1) {
		return false;
	}

	// 检查高度是否正确
	int expected_height = 1 + (node_height(node->left) > node_height(node->right) ?
		node_height(node->left) : node_height(node->right));
	if (node->height != expected_height) {
		return false;
	}

	// 递归检查子树
	return node_validate(node->left, compare, context, min, node->data) &&
		node_validate(node->right, compare, context, node->data, max);
}
