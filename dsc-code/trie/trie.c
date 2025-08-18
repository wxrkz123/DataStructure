// trie.c (Generic Version)

#include "trie.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define ALPHABET_SIZE 26

typedef struct TrieNode {
    // bool isEndOfWord;
    // ���ݲ��
    void* value; // ��void*�滻bool isEndOfWord��NULL��ʾ�ǵ��ʽ�β��

	// ָ�����飬�����ÿ������(0-25)�����������ָ����һ��TrieNode��ָ�롣
    struct TrieNode* children[ALPHABET_SIZE];
} TrieNode;

struct Trie {
    TrieNode* root;
    ValueDestroyer valueDestroyer; // �洢�û��ṩ����������
};

// --- ˽�к���ԭ�ͱ��ֲ��� ---
static TrieNode* TrieNode_Create(void);
static void TrieNode_DestroyRecursive(TrieNode* node, ValueDestroyer destroyer);
static int CharToIndex(char c);
static bool TrieNode_DeleteRecursiveHelper(TrieNode** pNode,
    const char* key,
    int depth,
    ValueDestroyer destroyer);

// --- ��������ʵ���޸� ---

Trie* Trie_Create(ValueDestroyer destroyer) {
    Trie* trie = (Trie*)malloc(sizeof(Trie));
    if (!trie) return NULL;

    trie->root = TrieNode_Create();
    if (!trie->root) {
        free(trie);
        return NULL;
    }
    trie->valueDestroyer = destroyer; // ������������
    return trie;
}

void Trie_Destroy(Trie* trie) {
    if (!trie) return;
    TrieNode_DestroyRecursive(trie->root, trie->valueDestroyer);
    free(trie);
}

bool Trie_Insert(Trie* trie, const char* key, void* value) {
    if (!trie || !trie->root || !key) return false;
    // ����NULLֵ��������ģ���Ϊ������NULL����Ƿǽ�β�ڵ�
    assert(value != NULL && "Cannot insert a NULL value into the Trie.");


	// crawlָ�����ڱ���Trie "����ָ��"
    TrieNode* crawl = trie->root;
    int len = strlen(key);

	// ����ÿ���ַ�������ת��Ϊ����������Trie�в���������Ӧ�Ľڵ�
    for (int i = 0; i < len; ++i) {
        int index = CharToIndex(key[i]);
        if (index == -1) return false;

        if (!crawl->children[index]) {
            crawl->children[index] = TrieNode_Create();
            if (!crawl->children[index]) return false;
        }

		// �ƶ�����һ���ڵ㣨�����Ƿ����µģ�
        crawl = crawl->children[index];
    }

    // ע�⣺��� crawl->value ֮ǰ�Ѿ���ֵ�������ֱ�Ӹ��ǡ�
    // һ�������ƵĿ���ܻ��ȵ���destroyer�ͷž�ֵ��Ϊ�򻯣��˴����û�����

	// �������ڵ��ֵ����Ϊ����ķ�NULLָ�루value��

    // te

    // not prefix

	// ������߼��ǣ����һ���ڵ��value��ΪNULL�����ʾ����һ�����ʵĽ�β��
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

		// ����ַ�������ĸ��Χ�ڣ�ֱ�ӷ���NULL
        if (index == -1 || !crawl->children[index]) {
            return NULL;
        }

		// ǰ������һ���ڵ�
        crawl = crawl->children[index];
    }

    // ���ش洢��ֵ��ָ�룬�������һ����β����ֵΪNULL�����򷵻�NULL��
    return crawl ? crawl->value : NULL;
}

// StartsWith��ʵ�ֻ�������
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
    if (!trie || !key || !*key) { // ���trie, key�Ϳ��ַ���
        return;
    }
    TrieNode_DeleteRecursiveHelper(&trie->root, key, 0, trie->valueDestroyer);
}

/**
 * @brief ���һ���ڵ��Ƿ���Ա�ɾ����
 * @param node Ҫ���Ľڵ㡣
 * @return ����ڵ�û��ֵ����û���ӽڵ㣬����true��
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



// --- ˽�к���ʵ���޸� ---

static TrieNode* TrieNode_Create(void) {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    if (node) {
        node->value = NULL; // ��ʼ��ΪNULL
        for (int i = 0; i < ALPHABET_SIZE; ++i) {
            node->children[i] = NULL;
        }
    }
    return node;
}

static void TrieNode_DestroyRecursive(TrieNode* node, ValueDestroyer destroyer) {
    if (!node) return;


	// �ݹ����������ӽڵ�
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        TrieNode_DestroyRecursive(node->children[i], destroyer);
    }

    // �ؼ�һ�������ͷŽڵ㱾��֮ǰ������Ƿ����û�������Ҫ�ͷ�
    if (node->value && destroyer) {
        destroyer(node->value);
    }
    free(node);
}

static int CharToIndex(char c) {
    if (c >= 'a' && c <= 'z') return c - 'a';
    return -1;
}

// ����ɾ��

/**
 * @brief �ݹ��ɾ������
 * @param pNode ָ��ǰ�ڵ�ָ���ָ�룬���������޸ĸ��ڵ��е�ָ�롣
 * @param key Ҫɾ���ļ���
 * @param depth ��ǰ�ݹ���ȡ�
 * @param destroyer �����ͷ�ֵ�ĺ�����
 * @return �����ǰ�ڵ���ɾ�����Ϊ�գ����Ա����ڵ㰲ȫ�Ƴ����򷵻�true��
 */
static bool TrieNode_DeleteRecursiveHelper(TrieNode** pNode, 
    const char* key, 
    int depth, 
    ValueDestroyer destroyer) {


    // **

	// TrieNode** pNode: &l->children['e'];
	// TrieNode* node = *pNode; : node ������'e'�ڵ�ĵ�ַ
   

	// ��ȡ��ǰ�ڵ�
    TrieNode* node = *pNode;

    if (!node) {
        return false; // ��������
    }

    // key[5] \0
    // ��������˼���ĩβ
    if (key[depth] == '\0') {
        // �������ڵ�ȷʵ��һ�����ʵĽ�β
        if (node->value) {
            if (destroyer) {
                destroyer(node->value);
            }
			// node->isEndOfWord = false; // ���ǵ��ʽ�β
            node->value = NULL; // ��ɾ�������Ϊ�ǽ�β
        }
        // �������ڵ������Ƿ���Ա�����ɾ��
        if (IsNodeEmpty(node)) {
            free(node);
            *pNode = NULL; // �����ڵ�ָ��˽ڵ��ָ����ΪNULL
            return true;
        }
        return false;
    }

    // �ݹ鵽��һ���ڵ�
    int index = CharToIndex(key[depth]);
    if (index == -1) return false; // ��Ч�ַ�

    if (TrieNode_DeleteRecursiveHelper(&node->children[index], key, depth + 1, destroyer)) {
        // ����ӽڵ㱻ɾ���ˣ��ٴμ�鵱ǰ�ڵ��Ƿ�Ҳ��Ϊ����
        if (IsNodeEmpty(node)) {
            free(node);
            *pNode = NULL;
            return true;
        }
    }

    return false;
}