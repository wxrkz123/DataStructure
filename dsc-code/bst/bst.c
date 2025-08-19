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

// 私有辅助函数：创建一个新节点
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

// 私有辅助函数：递归销毁树（后序遍历）
static void _destroy_recursive(Node* node) {
    if (node == NULL) return;
    _destroy_recursive(node->left);
    _destroy_recursive(node->right);
    free(node->data);
    free(node);
}

// 私有辅助函数：递归插入新节点
static Node* _insert_recursive(BST* bst, Node* node, const void* data) {
    /*
     * 函数功能: 在以 `node` 为根的子树中递归地插入数据。
     * 返回值是关键: 它返回这棵子树的新根。大多数情况下根不变，
     * 但在插入到空子树时，新创建的节点将作为新根返回，
     * 其父节点的 left 或 right 指针将被正确赋值。
     *
     * 教学案例: 假设树中已有 {10, 5}。我们要插入 7。
     *            10
     *           /
     *          5
     * 1. 调用 _insert(bst, root(10), 7)。7 < 10，进入左子树。
     *    执行: root->left = _insert(bst, root->left(5), 7);
     * 2. 在步骤1的递归调用中: _insert(bst, node(5), 7)。7 > 5，进入右子树。
     *    执行: node->right = _insert(bst, node->right(NULL), 7);
     * 3. 在步骤2的递归调用中: _insert(bst, NULL, 7)。node 为 NULL，这是基本情况。
     *    创建一个新节点 [7]，并将其返回。
     * 4. 返回到步骤2: node->right (5的右子节点) 被赋值为新返回的节点 [7]。
     *    然后步骤2返回节点 [5] (它的根没变)。
     * 5. 返回到步骤1: root->left (10的左子节点) 被赋值为返回的节点 [5] (它的根也没变)。
     *    最终完成插入。
    */
    // 基本情况：如果当前节点为空，说明我们找到了插入位置。
    if (node == NULL) {
        bst->size++;
        return _create_node(data, bst->element_size);
    }

    int cmp = bst->compare(data, node->data);

	// compare(a, b) 
    // cmp 
	// cpm < 0 表示 a < b
    // cmp > 0 表示 a > b
    // cmp == 0 表示 a == b
    // 这里的逻辑是：如果新数据小于当前节点数据，就去左子树插入；如果大于，就去右子树插入。
	// 注意：如果 cmp == 0，说明数据已存在，我们什么都不做，直接返回原节点。

	// if (new data < current_node_data) {

    if (cmp < 0) {
        // 新数据比当前节点小，递归地插入到左子树
        node->left = _insert_recursive(bst, node->left, data);
        // node->left是什么？
        // node->left NULL

        // 20 tree insert 10
		// node->left = _insert_recursive(bst, node->left, data);
        // Node 20 address : 0x4000
		// node: 在当前栈帧中，参数node的值是一个pointer，指向当前节点(20)的地址。
		// data: 20 a pointer, => int 20 内存
        // left: NULL
		// right : NULL
		// data: 10 a pointer, => int 10 内存 等待被插入的值的内存地址
        // Call stack:
        // 调用栈
		// 1. _insert_recursive(bst, node(20), 10)
		//  => _insert_recursive(node=0x4000, data=&10)  栈顶
        //_insert_recursive(bst, node->left, data);
		// bst 传入BST的结构指针
		// node->left 读取当前的Node（地址0x4000）的left成员
		// data 传入指向整数10的指针（&10）
        // 2. 发起递归调用
        // PUSH
        // //  => _insert_recursive(node=NULL, data=&10)  栈顶
        // //  => _insert_recursive(node=0x4000, data=&10)  

    }
    else if (cmp > 0) {
        // 新数据比当前节点大，递归地插入到右子树
        node->right = _insert_recursive(bst, node->right, data);
    }
    // 如果 cmp == 0，说明数据已存在，我们什么都不做，直接返回原节点。

    return node; // 返回当前子树的根（可能已更新了其子节点）
}

// 私有辅助函数：查找子树中最小的节点（用于删除）
static Node* _find_min_recursive(Node* node) {
    // 在BST中，最左边的节点永远是最小的。
    while (node && node->left != NULL) {
        node = node->left;
    }
    return node;
}

// ======================================================================
// ====================  删除操作 - 教学核心区域  ======================
// ======================================================================
static Node* _remove_recursive(BST* bst, Node* node, const void* key) {
    /*
     * 函数功能: 在以 `node` 为根的子树中递归地删除值为 `key` 的节点。
     * 这是BST中最复杂的操作，有三种主要情况。
    */

    // 基本情况：如果节点为空，说明没找到要删除的键，直接返回。
    if (node == NULL) return NULL;

    // 步骤 1: 像搜索一样，找到要删除的节点。
    int cmp = bst->compare(key, node->data);
    if (cmp < 0) {
        node->left = _remove_recursive(bst, node->left, key);
    }
    else if (cmp > 0) {
        node->right = _remove_recursive(bst, node->right, key);
    }
    else {
        // 情况 1: 要删除的节点是叶子节点 (没有子节点)。
       
        if (node->left == NULL && node->right == NULL) {
            // --- 找到了要删除的节点 (node) ---
            bst->size--; // 先把size减掉
            free(node->data);
            free(node);
            return NULL; // 
        }

        // 情况 2: 要删除的节点只有一个子节点 (左或右)。
        
        if (node->left == NULL) {
            // --- 找到了要删除的节点 (node) ---
            bst->size--; // 先把size减掉
            Node* temp = node->right; // 保存右子节点的引用
            free(node->data);
            free(node);
            return temp; // 返回右子节点给父节点，完成“领养”。
        }
        else if (node->right == NULL) {
            // --- 找到了要删除的节点 (node) ---
            bst->size--; // 先把size减掉
            Node* temp = node->left; // 保存左子节点的引用
            free(node->data);
            free(node);
            return temp; // 返回左子节点给父节点。
        }

        // 情况 3: 要删除的节点有两个子节点 (最复杂的情况)。

        // 策略:
        //  a. 找到这个节点的“中序后继者”(In-order Successor)。
        //     中序后继者就是它右子树中最小的那个节点。
        //     对于节点10，它的右子树是 {15}，其中最小的节点就是 15。
        //     案例: 删除 20。右子树是 {30,25,40}，其中最小的节点是 25。
        //  b. 将后继者的“数据”复制到当前要删除的节点中。
        //     案例: 找到 10 的后继者 15。将 15 的数据拷贝到 10 的节点里。
        //     树在逻辑上变成了:
        //              15 (但物理上还是原来的10号节点)
        //            /    \
        //          5      15
        //         / \     
        //        3   7
        //  c. 现在问题转化为：从右子树中删除那个后继者。
        //     这非常巧妙，因为后继者(右子树的最小值)保证最多只有一个右孩子，
        //     所以删除它就转化为了上面的情况1或情况2，问题被简化了！
        //     案例: 从 10 的右子树 {15} 中删除 15。这是一个叶子节点删除(情况1)。

        // 步骤 a: 找到右子树的最小节点 (中序后继者)
        Node* successor = _find_min_recursive(node->right);

        // 步骤 b: 将后继者的数据拷贝到当前节点
        memcpy(node->data, successor->data, bst->element_size);

        // 步骤 c: 递归地从右子树中删除那个后继者
        node->right = _remove_recursive(bst, node->right, successor->data);
    }
    return node;
}


// 私有辅助函数：递归遍历
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
    return bst->size > old_size; // 如果size增加了，说明插入成功
}

bool bst_remove(BST* bst, const void* element_data) {
    if (!bst || !element_data) return false;
    size_t old_size = bst->size;
    bst->root = _remove_recursive(bst, bst->root, element_data);
    return bst->size < old_size; // 如果size减小了，说明删除成功
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