// binary_search_tree.h
#pragma once

#include <stddef.h>
#include <stdbool.h>

// --- Opaque Pointer and Function Pointer Type Definitions ---

typedef struct BST BST;

/**
 * �ȽϺ���ָ�����͡�
 * @param a ָ���һ��Ԫ�ص�ָ�롣
 * @param b ָ��ڶ���Ԫ�ص�ָ�롣
 * @return > 0 ��� a > b��
 * @return < 0 ��� a < b��
 * @return 0   ��� a == b��
 */
typedef int (*CompareFunc)(const void* a, const void* b);

/**
 * ���ʺ���ָ�����ͣ����ڱ�����
 * @param data ָ��ڵ����ݵ�ָ�롣
 */
typedef void (*VisitFunc)(const void* data);

// ����˳���ö��
typedef enum {
    IN_ORDER,   // ������� (LNR -> �������)
    PRE_ORDER,  // ǰ����� (NLR)
    POST_ORDER  // ������� (LRN)
} TraverseOrder;


// --- Public API Prototypes ---

/**
 * @brief ����һ���µķ��Ͷ�����������
 *
 * @param element_size ÿ��Ԫ�صĴ�С���ֽڣ���
 * @param compare_func ���ڱȽ�Ԫ�صĺ���ָ�룬����BST�ĺ��ġ�
 * @return �ɹ�ʱ����ָ��������ָ�룬ʧ�ܷ��� NULL��
 */
BST* bst_create(size_t element_size, CompareFunc compare_func);

/**
 * @brief ����һ���������������ͷ����нڵ���ڴ档
 */
void bst_destroy(BST** p_bst);

/**
 * @brief �����в���һ����Ԫ�ء�
 * ���Ԫ���Ѵ��ڣ��򲻽����κβ�����
 */
bool bst_insert(BST* bst, const void* element_data);

/**
 * @brief �������Ƴ�һ��Ԫ�ء�
 */
bool bst_remove(BST* bst, const void* element_data);

/**
 * @brief ���һ��Ԫ���Ƿ���������С�
 */
bool bst_search(const BST* bst, const void* key);

/**
 * @brief ����ָ����˳�������������ÿ���ڵ�ִ�з��ʺ�����
 */
void bst_traverse(const BST* bst, TraverseOrder order, VisitFunc visit_func);

bool bst_is_empty(const BST* bst);
size_t bst_get_size(const BST* bst);
