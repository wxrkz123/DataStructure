#include "avl_tree.h"
#include <stdlib.h>
#include <assert.h>

// AVL�ڵ�ṹ
struct AVLNode {
	void* data;
	AVLNode* left;
	AVLNode* right;
	int height;
};

// AVL���ṹ

/*
* �ṹ�� AVLTree ���ڱ�ʾ AVL ����
* ���������³�Ա��
* - root: ָ�����ĸ��ڵ㡣
* - compare: �ȽϺ��������ڱȽ���������Ԫ�ء�
* - free_func: �ͷ����ݵĻص�������������ɾ���ڵ�ʱ�ͷ��ڴ档
* - context: ������ָ�룬���ڴ��ݶ�����Ϣ���ȽϺ������ͷź�����
* - size: ����Ԫ�ص�������
*/
struct AVLTree {
	AVLNode* root;
	AVLCompareFunc compare;
	AVLFreeFunc free_func;
	void* context;
	size_t size;
};

// �ڲ���������
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

// ����AVL��
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

// ����AVL��
void avl_destroy(AVLTree* tree) {
	if (tree == NULL) {
		return;
	}

	node_destroy(tree->root, tree->free_func, tree->context);
	free(tree);
}

// ����Ԫ��
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

// ɾ��Ԫ��
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

// ����Ԫ��
void* avl_find(const AVLTree* tree, const void* data) {
	assert(tree != NULL);
	assert(data != NULL);

	return node_find(tree->root, data, tree->compare, tree->context);
}

// ����Ԫ��
bool avl_update(AVLTree* tree, const void* old_data, void* new_data) {
	assert(tree != NULL);
	assert(old_data != NULL);
	assert(new_data != NULL);

	// ����¾�������ȣ�ֱ�ӷ��سɹ�
	if (tree->compare(old_data, new_data, tree->context) == 0) {
		return true;
	}

	// �Ȳ��Ҿ������Ƿ����
	if (avl_find(tree, old_data) == NULL) {
		return false;
	}

	// ɾ��������
	avl_delete(tree, old_data);

	// ����������
	return avl_insert(tree, new_data);
}

// ��ȡ���Ĵ�С
size_t avl_size(const AVLTree* tree) {
	assert(tree != NULL);
	return tree->size;
}

// ������Ƿ�Ϊ��
bool avl_is_empty(const AVLTree* tree) {
	assert(tree != NULL);
	return tree->size == 0;
}

// �������
void avl_traverse_inorder(const AVLTree* tree, AVLTraverseFunc func, void* context) {
	assert(tree != NULL);
	assert(func != NULL);

	node_traverse_inorder(tree->root, func, context);
}

// ǰ�����
void avl_traverse_preorder(const AVLTree* tree, AVLTraverseFunc func, void* context) {
	assert(tree != NULL);
	assert(func != NULL);

	node_traverse_preorder(tree->root, func, context);
}

// �������
void avl_traverse_postorder(const AVLTree* tree, AVLTraverseFunc func, void* context) {
	assert(tree != NULL);
	assert(func != NULL);

	node_traverse_postorder(tree->root, func, context);
}

// ��ȡ���ĸ߶�
int avl_height(const AVLTree* tree) {
	assert(tree != NULL);
	return node_height(tree->root);
}

// ��֤AVL����ƽ����
bool avl_validate(const AVLTree* tree) {
	assert(tree != NULL);
	return node_validate(tree->root, tree->compare, tree->context, NULL, NULL);
}

// �ڲ�����ʵ��

// �����ڵ�
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

// ���ٽڵ㼰������
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

// ��ȡ�ڵ�߶�
static int node_height(const AVLNode* node) {
	return node ? node->height : 0;
}

// ���½ڵ�߶�
static void node_update_height(AVLNode* node) {
	if (node == NULL) {
		return;
	}

	int left_height = node_height(node->left);
	int right_height = node_height(node->right);

	// �߶�Ϊ�ӽڵ�߶ȵ����ֵ��1
	// 1 + max(left_height, right_height);
	node->height = 1 + (left_height > right_height ? left_height : right_height);
}

// ����ƽ������
static int node_balance_factor(const AVLNode* node) {
	// ƽ������ = �������߶� - �������߶�

	return node ? node_height(node->left) - node_height(node->right) : 0;
}

// ����
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

// ����
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

// ƽ��ڵ�
static AVLNode* node_balance(AVLNode* node) {
	if (node == NULL) {
		return NULL;
	}

	node_update_height(node);
	int balance = node_balance_factor(node);

	// ����
	if (balance > 1) {
		// �������
		if (node_balance_factor(node->left) < 0) {
			node->left = node_rotate_left(node->left);
		}
		// �������
		return node_rotate_right(node);
	}

	// ����
	if (balance < -1) {
		// �������
		if (node_balance_factor(node->right) > 0) {
			node->right = node_rotate_right(node->right);
		}
		// �������
		return node_rotate_left(node);
	}

	return node;
}

// ����ڵ�
static AVLNode* node_insert(AVLNode* node,
	void* data, AVLCompareFunc compare,
	void* context, bool* inserted) {
	// �ҵ�����λ��
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
		// Ԫ���Ѵ���
		*inserted = false;
		return node;
	}

	// ����ƽ��
	return node_balance(node);
}

// ������С�ڵ�
static AVLNode* node_find_min(AVLNode* node) {
	while (node != NULL && node->left != NULL) {
		node = node->left;
	}
	return node;
}

// ɾ���ڵ�
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
		// �ҵ�Ҫɾ���Ľڵ�
		*deleted = true;

		if (node->left == NULL || node->right == NULL) {
			// ������һ���ӽڵ�Ϊ��
			AVLNode* temp = node->left ? node->left : node->right;

			if (free_func != NULL) {
				free_func(node->data, context);
			}
			free(node);

			return temp;
		}
		else {
			// �����ӽڵ㶼����
			AVLNode* min_right = node_find_min(node->right);

			// ��������
			void* temp_data = node->data;
			node->data = min_right->data;
			min_right->data = temp_data;

			// ɾ����̽ڵ�
			node->right = node_delete(node->right, temp_data, compare, context, free_func, deleted);
		}
	}

	// ����ƽ��
	return node_balance(node);
}

// ���ҽڵ�
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

// �������
static void node_traverse_inorder(const AVLNode* node, AVLTraverseFunc func, void* context) {
	if (node == NULL) {
		return;
	}

	node_traverse_inorder(node->left, func, context);
	func(node->data, context);
	node_traverse_inorder(node->right, func, context);
}

// ǰ�����
static void node_traverse_preorder(const AVLNode* node, AVLTraverseFunc func, void* context) {
	if (node == NULL) {
		return;
	}

	func(node->data, context);
	node_traverse_preorder(node->left, func, context);
	node_traverse_preorder(node->right, func, context);
}

// �������
static void node_traverse_postorder(const AVLNode* node, AVLTraverseFunc func, void* context) {
	if (node == NULL) {
		return;
	}

	node_traverse_postorder(node->left, func, context);
	node_traverse_postorder(node->right, func, context);
	func(node->data, context);
}

// ��֤�ڵ�
static bool node_validate(const AVLNode* node, 
	AVLCompareFunc compare, 
	void* context,
	const void* min, const void* max) {
	if (node == NULL) {
		return true;
	}

	// ���BST����
	if (min != NULL && compare(node->data, min, context) <= 0) {
		return false;
	}
	if (max != NULL && compare(node->data, max, context) >= 0) {
		return false;
	}

	// ���ƽ������
	int balance = node_balance_factor(node);
	if (balance < -1 || balance > 1) {
		return false;
	}

	// ���߶��Ƿ���ȷ
	int expected_height = 1 + (node_height(node->left) > node_height(node->right) ?
		node_height(node->left) : node_height(node->right));
	if (node->height != expected_height) {
		return false;
	}

	// �ݹ�������
	return node_validate(node->left, compare, context, min, node->data) &&
		node_validate(node->right, compare, context, node->data, max);
}
