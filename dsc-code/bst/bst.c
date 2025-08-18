// bst.c

#include "bst.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// --- Private Structure Definitions ---

typedef struct Node {
    void* data;
    struct Node* left;
    struct Node* right;
} Node;

struct BST {
    Node* root;
    size_t element_size;
    size_t size;
    CompareFunc compare;
};

// --- Static Helper Functions (Recursive Implementations) ---

// ˽�и�������������һ���½ڵ�
static Node* _create_node(const void* data, size_t element_size) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) return NULL;

    new_node->data = malloc(element_size);
    if (!new_node->data) {
        free(new_node);
        return NULL;
    }
    memcpy(new_node->data, data, element_size);
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

// ˽�и����������ݹ������������������
static void _destroy_recursive(Node* node) {
    if (node == NULL) return;
    _destroy_recursive(node->left);
    _destroy_recursive(node->right);
    free(node->data);
    free(node);
}

// ˽�и����������ݹ�����½ڵ�
static Node* _insert_recursive(BST* bst, Node* node, const void* data) {
    /*
     * ��������: ���� `node` Ϊ���������еݹ�ز������ݡ�
     * ����ֵ�ǹؼ�: ����������������¸������������¸����䣬
     * ���ڲ��뵽������ʱ���´����Ľڵ㽫��Ϊ�¸����أ�
     * �丸�ڵ�� left �� right ָ�뽫����ȷ��ֵ��
     *
     * ��ѧ����: ������������ {10, 5}������Ҫ���� 7��
     *            10
     *           /
     *          5
     * 1. ���� _insert(bst, root(10), 7)��7 < 10��������������
     *    ִ��: root->left = _insert(bst, root->left(5), 7);
     * 2. �ڲ���1�ĵݹ������: _insert(bst, node(5), 7)��7 > 5��������������
     *    ִ��: node->right = _insert(bst, node->right(NULL), 7);
     * 3. �ڲ���2�ĵݹ������: _insert(bst, NULL, 7)��node Ϊ NULL�����ǻ��������
     *    ����һ���½ڵ� [7]�������䷵�ء�
     * 4. ���ص�����2: node->right (5�����ӽڵ�) ����ֵΪ�·��صĽڵ� [7]��
     *    Ȼ����2���ؽڵ� [5] (���ĸ�û��)��
     * 5. ���ص�����1: root->left (10�����ӽڵ�) ����ֵΪ���صĽڵ� [5] (���ĸ�Ҳû��)��
     *    ������ɲ��롣
    */
    // ��������������ǰ�ڵ�Ϊ�գ�˵�������ҵ��˲���λ�á�
    if (node == NULL) {
        bst->size++;
        return _create_node(data, bst->element_size);
    }

    int cmp = bst->compare(data, node->data);

	// compare(a, b) 
    // cmp 
	// cpm < 0 ��ʾ a < b
    // cmp > 0 ��ʾ a > b
    // cmp == 0 ��ʾ a == b
    // ������߼��ǣ����������С�ڵ�ǰ�ڵ����ݣ���ȥ���������룻������ڣ���ȥ���������롣
	// ע�⣺��� cmp == 0��˵�������Ѵ��ڣ�����ʲô��������ֱ�ӷ���ԭ�ڵ㡣

	// if (new data < current_node_data) {

    if (cmp < 0) {
        // �����ݱȵ�ǰ�ڵ�С���ݹ�ز��뵽������
        node->left = _insert_recursive(bst, node->left, data);
        // node->left��ʲô��
        // node->left NULL

        // 20 tree insert 10
		// node->left = _insert_recursive(bst, node->left, data);
        // Node 20 address : 0x4000
		// node: �ڵ�ǰջ֡�У�����node��ֵ��һ��pointer��ָ��ǰ�ڵ�(20)�ĵ�ַ��
		// data: 20 a pointer, => int 20 �ڴ�
        // left: NULL
		// right : NULL
		// data: 10 a pointer, => int 10 �ڴ� �ȴ��������ֵ���ڴ��ַ
        // Call stack:
        // ����ջ
		// 1. _insert_recursive(bst, node(20), 10)
		//  => _insert_recursive(node=0x4000, data=&10)  ջ��
        //_insert_recursive(bst, node->left, data);
		// bst ����BST�Ľṹָ��
		// node->left ��ȡ��ǰ��Node����ַ0x4000����left��Ա
		// data ����ָ������10��ָ�루&10��
        // 2. ����ݹ����
        // PUSH
        // //  => _insert_recursive(node=NULL, data=&10)  ջ��
        // //  => _insert_recursive(node=0x4000, data=&10)  

    }
    else if (cmp > 0) {
        // �����ݱȵ�ǰ�ڵ�󣬵ݹ�ز��뵽������
        node->right = _insert_recursive(bst, node->right, data);
    }
    // ��� cmp == 0��˵�������Ѵ��ڣ�����ʲô��������ֱ�ӷ���ԭ�ڵ㡣

    return node; // ���ص�ǰ�����ĸ��������Ѹ��������ӽڵ㣩
}

// ˽�и���������������������С�Ľڵ㣨����ɾ����
static Node* _find_min_recursive(Node* node) {
    // ��BST�У�����ߵĽڵ���Զ����С�ġ�
    while (node && node->left != NULL) {
        node = node->left;
    }
    return node;
}

// ======================================================================
// ====================  ɾ������ - ��ѧ��������  ======================
// ======================================================================
static Node* _remove_recursive(BST* bst, Node* node, const void* key) {
    /*
     * ��������: ���� `node` Ϊ���������еݹ��ɾ��ֵΪ `key` �Ľڵ㡣
     * ����BST����ӵĲ�������������Ҫ�����
    */

    // �������������ڵ�Ϊ�գ�˵��û�ҵ�Ҫɾ���ļ���ֱ�ӷ��ء�
    if (node == NULL) return NULL;

    // ���� 1: ������һ�����ҵ�Ҫɾ���Ľڵ㡣
    int cmp = bst->compare(key, node->data);
    if (cmp < 0) {
        node->left = _remove_recursive(bst, node->left, key);
    }
    else if (cmp > 0) {
        node->right = _remove_recursive(bst, node->right, key);
    }
    else {
        // ��� 1: Ҫɾ���Ľڵ���Ҷ�ӽڵ� (û���ӽڵ�)��
       
        if (node->left == NULL && node->right == NULL) {
            // --- �ҵ���Ҫɾ���Ľڵ� (node) ---
            bst->size--; // �Ȱ�size����
            free(node->data);
            free(node);
            return NULL; // 
        }

        // ��� 2: Ҫɾ���Ľڵ�ֻ��һ���ӽڵ� (�����)��
        
        if (node->left == NULL) {
            // --- �ҵ���Ҫɾ���Ľڵ� (node) ---
            bst->size--; // �Ȱ�size����
            Node* temp = node->right; // �������ӽڵ������
            free(node->data);
            free(node);
            return temp; // �������ӽڵ�����ڵ㣬��ɡ���������
        }
        else if (node->right == NULL) {
            // --- �ҵ���Ҫɾ���Ľڵ� (node) ---
            bst->size--; // �Ȱ�size����
            Node* temp = node->left; // �������ӽڵ������
            free(node->data);
            free(node);
            return temp; // �������ӽڵ�����ڵ㡣
        }

        // ��� 3: Ҫɾ���Ľڵ��������ӽڵ� (��ӵ����)��

        // ����:
        //  a. �ҵ�����ڵ�ġ��������ߡ�(In-order Successor)��
        //     �������߾���������������С���Ǹ��ڵ㡣
        //     ���ڽڵ�10�������������� {15}��������С�Ľڵ���� 15��
        //     ����: ɾ�� 20���������� {30,25,40}��������С�Ľڵ��� 25��
        //  b. ������ߵġ����ݡ����Ƶ���ǰҪɾ���Ľڵ��С�
        //     ����: �ҵ� 10 �ĺ���� 15���� 15 �����ݿ����� 10 �Ľڵ��
        //     �����߼��ϱ����:
        //              15 (�������ϻ���ԭ����10�Žڵ�)
        //            /    \
        //          5      15
        //         / \     
        //        3   7
        //  c. ��������ת��Ϊ������������ɾ���Ǹ�����ߡ�
        //     ��ǳ������Ϊ�����(����������Сֵ)��֤���ֻ��һ���Һ��ӣ�
        //     ����ɾ������ת��Ϊ����������1�����2�����ⱻ���ˣ�
        //     ����: �� 10 �������� {15} ��ɾ�� 15������һ��Ҷ�ӽڵ�ɾ��(���1)��

        // ���� a: �ҵ�����������С�ڵ� (��������)
        Node* successor = _find_min_recursive(node->right);

        // ���� b: ������ߵ����ݿ�������ǰ�ڵ�
        memcpy(node->data, successor->data, bst->element_size);

        // ���� c: �ݹ�ش���������ɾ���Ǹ������
        node->right = _remove_recursive(bst, node->right, successor->data);
    }
    return node;
}


// ˽�и����������ݹ����
static void _traverse_recursive(const Node* node, VisitFunc visit, TraverseOrder order) {
    if (node == NULL) return;

    if (order == PRE_ORDER) visit(node->data);
    _traverse_recursive(node->left, visit, order);
    if (order == IN_ORDER) visit(node->data);
    _traverse_recursive(node->right, visit, order);
    if (order == POST_ORDER) visit(node->data);
}

// --- Public API Implementations ---

BST* bst_create(size_t element_size, CompareFunc compare_func) {
    if (element_size == 0 || !compare_func) return NULL;
    BST* bst = (BST*)malloc(sizeof(BST));
    if (!bst) return NULL;
    bst->root = NULL;
    bst->element_size = element_size;
    bst->size = 0;
    bst->compare = compare_func;
    return bst;
}

void bst_destroy(BST** p_bst) {
    if (p_bst && *p_bst) {
        _destroy_recursive((*p_bst)->root);
        free(*p_bst);
        *p_bst = NULL;
    }
}

bool bst_insert(BST* bst, const void* element_data) {
    if (!bst || !element_data) return false;
    size_t old_size = bst->size;
    bst->root = _insert_recursive(bst, bst->root, element_data);
    return bst->size > old_size; // ���size�����ˣ�˵������ɹ�
}

bool bst_remove(BST* bst, const void* element_data) {
    if (!bst || !element_data) return false;
    size_t old_size = bst->size;
    bst->root = _remove_recursive(bst, bst->root, element_data);
    return bst->size < old_size; // ���size��С�ˣ�˵��ɾ���ɹ�
}

bool bst_search(const BST* bst, const void* key) {
    if (!bst || !key) return false;
    Node* current = bst->root;
    while (current != NULL) {
        int cmp = bst->compare(key, current->data);
        if (cmp == 0) return true;
        if (cmp < 0) current = current->left;
        else current = current->right;
    }
    return false;
}

void bst_traverse(const BST* bst, TraverseOrder order, VisitFunc visit_func) {
    if (bst && visit_func) {
        _traverse_recursive(bst->root, visit_func, order);
    }
}

bool bst_is_empty(const BST* bst) { return !bst || bst->size == 0; }
size_t bst_get_size(const BST* bst) { return bst ? bst->size : 0; }