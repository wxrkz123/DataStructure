// trie.c (Generic Version)

#include "trie.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define ALPHABET_SIZE 26

typedef struct TrieNode {
    // bool isEndOfWord;
    // 数据插槽
    void* value; // 用void*替换bool isEndOfWord。NULL表示非单词结尾。

	// 指针数组，数组的每个格子(0-25)，都用来存放指向另一个TrieNode的指针。
    struct TrieNode* children[ALPHABET_SIZE];
} TrieNode;

struct Trie {
    TrieNode* root;
    ValueDestroyer valueDestroyer; // 存储用户提供的析构函数
};

// --- 私有函数原型保持不变 ---
static TrieNode* TrieNode_Create(void);
static void TrieNode_DestroyRecursive(TrieNode* node, ValueDestroyer destroyer);
static int CharToIndex(char c);
static bool TrieNode_DeleteRecursiveHelper(TrieNode** pNode,
    const char* key,
    int depth,
    ValueDestroyer destroyer);

// --- 公共函数实现修改 ---

Trie* Trie_Create(ValueDestroyer destroyer) {
    Trie* trie = (Trie*)malloc(sizeof(Trie));
    if (!trie) return NULL;

    trie->root = TrieNode_Create();
    if (!trie->root) {
        free(trie);
        return NULL;
    }
    trie->valueDestroyer = destroyer; // 保存析构函数
    return trie;
}

void Trie_Destroy(Trie* trie) {
    if (!trie) return;
    TrieNode_DestroyRecursive(trie->root, trie->valueDestroyer);
    free(trie);
}

bool Trie_Insert(Trie* trie, const char* key, void* value) {
    if (!trie || !trie->root || !key) return false;
    // 插入NULL值是无意义的，因为我们用NULL来标记非结尾节点
    assert(value != NULL && "Cannot insert a NULL value into the Trie.");


	// crawl指针用于遍历Trie "爬行指针"
    TrieNode* crawl = trie->root;
    int len = strlen(key);

	// 遍历每个字符，将其转换为索引，并在Trie中插入或查找相应的节点
    for (int i = 0; i < len; ++i) {
        int index = CharToIndex(key[i]);
        if (index == -1) return false;

        if (!crawl->children[index]) {
            crawl->children[index] = TrieNode_Create();
            if (!crawl->children[index]) return false;
        }

		// 移动到下一个节点（无论是否是新的）
        crawl = crawl->children[index];
    }

    // 注意：如果 crawl->value 之前已经有值，这里会直接覆盖。
    // 一个更完善的库可能会先调用destroyer释放旧值。为简化，此处由用户负责。

	// 将整个节点的值设置为传入的非NULL指针（value）

    // te

    // not prefix

	// 这里的逻辑是：如果一个节点的value不为NULL，则表示这是一个单词的结尾。
	// crawl->isEndOfWord = true;
    crawl->value = value;
    return true;
}

void* Trie_Search(const Trie* trie, const char* key) {
    if (!trie || !trie->root || !key) return NULL;

    const TrieNode* crawl = trie->root;
    int len = strlen(key);

    for (int i = 0; i < len; ++i) {
        int index = CharToIndex(key[i]);

		// 如果字符不在字母表范围内，直接返回NULL
        if (index == -1 || !crawl->children[index]) {
            return NULL;
        }

		// 前进到下一个节点
        crawl = crawl->children[index];
    }

    // 返回存储的值的指针，如果不是一个结尾（即值为NULL），则返回NULL。
    return crawl ? crawl->value : NULL;
}

// StartsWith的实现基本不变
bool Trie_StartsWith(const Trie* trie, const char* prefix) {
    if (!trie || !trie->root || !prefix) return false;
    const TrieNode* crawl = trie->root;
    int len = strlen(prefix);
    for (int i = 0; i < len; ++i) {
        int index = CharToIndex(prefix[i]);
        if (index == -1 || !crawl->children[index]) return false;
        crawl = crawl->children[index];
    }
    return crawl != NULL;
}

void Trie_Delete(Trie* trie, const char* key) {
    if (!trie || !key || !*key) { // 检查trie, key和空字符串
        return;
    }
    TrieNode_DeleteRecursiveHelper(&trie->root, key, 0, trie->valueDestroyer);
}

/**
 * @brief 检查一个节点是否可以被删除。
 * @param node 要检查的节点。
 * @return 如果节点没有值并且没有子节点，返回true。
 */
static bool IsNodeEmpty(TrieNode* node) {
    if (node->value) {
        return false;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            return false;
        }
    }
    return true;
}



// --- 私有函数实现修改 ---

static TrieNode* TrieNode_Create(void) {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    if (node) {
        node->value = NULL; // 初始化为NULL
        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            node->children[i] = NULL;
        }
    }
    return node;
}

static void TrieNode_DestroyRecursive(TrieNode* node, ValueDestroyer destroyer) {
    if (!node) return;


	// 递归销毁所有子节点
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        TrieNode_DestroyRecursive(node->children[i], destroyer);
    }

    // 关键一步：在释放节点本身之前，检查是否有用户数据需要释放
    if (node->value && destroyer) {
        destroyer(node->value);
    }
    free(node);
}

static int CharToIndex(char c) {
    if (c >= 'a' && c <= 'z') return c - 'a';
    return -1;
}

// 物理删除

/**
 * @brief 递归地删除键。
 * @param pNode 指向当前节点指针的指针，允许我们修改父节点中的指针。
 * @param key 要删除的键。
 * @param depth 当前递归深度。
 * @param destroyer 用于释放值的函数。
 * @return 如果当前节点在删除后变为空，可以被父节点安全移除，则返回true。
 */
static bool TrieNode_DeleteRecursiveHelper(TrieNode** pNode, 
    const char* key, 
    int depth, 
    ValueDestroyer destroyer) {


    // **

	// TrieNode** pNode: &l->children['e'];
	// TrieNode* node = *pNode; : node 现在是'e'节点的地址
   

	// 获取当前节点
    TrieNode* node = *pNode;

    if (!node) {
        return false; // 键不存在
    }

    // key[5] \0
    // 如果到达了键的末尾
    if (key[depth] == '\0') {
        // 如果这个节点确实是一个单词的结尾
        if (node->value) {
            if (destroyer) {
                destroyer(node->value);
            }
			// node->isEndOfWord = false; // 不是单词结尾
            node->value = NULL; // 懒删除：标记为非结尾
        }
        // 检查这个节点现在是否可以被物理删除
        if (IsNodeEmpty(node)) {
            free(node);
            *pNode = NULL; // 将父节点指向此节点的指针设为NULL
            return true;
        }
        return false;
    }

    // 递归到下一个节点
    int index = CharToIndex(key[depth]);
    if (index == -1) return false; // 无效字符

    if (TrieNode_DeleteRecursiveHelper(&node->children[index], key, depth + 1, destroyer)) {
        // 如果子节点被删除了，再次检查当前节点是否也变为空了
        if (IsNodeEmpty(node)) {
            free(node);
            *pNode = NULL;
            return true;
        }
    }

    return false;
}