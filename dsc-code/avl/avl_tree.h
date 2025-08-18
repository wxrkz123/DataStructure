#pragma once
#include <stddef.h>
#include <stdbool.h>

// ǰ������
typedef struct AVLNode AVLNode;
typedef struct AVLTree AVLTree;

// �ȽϺ������ͣ�����ֵ < 0 ��ʾ a < b��= 0 ��ʾ a = b��> 0 ��ʾ a > b
typedef int (*AVLCompareFunc)(const void* a, const void* b, void* context);

// �����ص���������
typedef void (*AVLTraverseFunc)(void* data, void* context);

// �ͷ����ݵĻص���������
typedef void (*AVLFreeFunc)(void* data, void* context);

// ����AVL��
AVLTree* avl_create(AVLCompareFunc compare, AVLFreeFunc free_func, void* context);

// ����AVL��
void avl_destroy(AVLTree* tree);

// ����Ԫ�أ�����Ѵ����򷵻�false��
bool avl_insert(AVLTree* tree, void* data);

// ɾ��Ԫ�أ������Ƿ�ɹ�ɾ����
bool avl_delete(AVLTree* tree, const void* data);

// ����Ԫ��
void* avl_find(const AVLTree* tree, const void* data);

// ����Ԫ�أ���ɾ���ɵģ��ٲ����µģ�
bool avl_update(AVLTree* tree, const void* old_data, void* new_data);

// ��ȡ���Ĵ�С
size_t avl_size(const AVLTree* tree);

// ������Ƿ�Ϊ��
bool avl_is_empty(const AVLTree* tree);

// �������
void avl_traverse_inorder(const AVLTree* tree, AVLTraverseFunc func, void* context);

// ǰ�����
void avl_traverse_preorder(const AVLTree* tree, AVLTraverseFunc func, void* context);

// �������
void avl_traverse_postorder(const AVLTree* tree, AVLTraverseFunc func, void* context);

// ��ȡ���ĸ߶�
int avl_height(const AVLTree* tree);

// ��֤AVL����ƽ���ԣ����ڵ��ԣ�
bool avl_validate(const AVLTree* tree);
