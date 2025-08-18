// hash_table.c
#define _CRT_SECURE_NO_WARNINGS // ����ʹ�� strcpy �Ⱥ���
#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Private Structure Definitions ---

// ��ϣ���еĽڵ㣬���ڹ���ÿ��Ͱ�е�����
typedef struct Node {
    char* key;           // ָ��̬����ļ��ַ���
    char* value;         // ָ��̬�����ֵ�ַ���
    struct Node* next;   // ָ�������е���һ���ڵ�
} Node;

// ��ϣ���ʵ�ʹ���ṹ
struct HashTable {
    Node** buckets;      // ָ��һ��ָ�����飬ÿ��ָ����һ��Ͱ�������ͷ�ڵ㣩
    size_t capacity;     // Ͱ������
    size_t size;         // ��ǰ�洢�ļ�ֵ������
};

// --- Static Helper Functions (Private) ---

// ��ϣ���������ַ���ת��Ϊһ���� [0, capacity-1] ��Χ�ڵ�����
// ��������HTMLʾ���еĹ�ϣ�����߼���ͬ��
// һ���򵥵��ַ���͹�ϣ�����������ڽ�ѧ��С��ģ���ݡ�
// ע�⣺ʵ��Ӧ����Ӧʹ�ø����ӵĹ�ϣ�����Լ��ٳ�ͻ��
static size_t hash_function(const char* key, size_t capacity) {
    unsigned long hash_value = 0;
    for (size_t i = 0; key[i] != '\0'; i++) {
        hash_value += key[i];
    }
    return hash_value % capacity;
}

// --- Public API Implementations ---

HashTable* ht_create(size_t capacity) {
    if (capacity == 0) return NULL;
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    if (!ht) return NULL;

    // ΪͰ��������ڴ棬����һ��ָ�����飬������ sizeof(Node*)
    ht->buckets = (Node**)calloc(capacity, sizeof(Node*));
    if (!ht->buckets) {
        free(ht);
        return NULL;
    }
    // `calloc` �Ὣ����Ͱ��ͷָ���ʼ��Ϊ NULL�����Ǳ���ġ�

    ht->capacity = capacity;
    ht->size = 0;
    return ht;
}

void ht_destroy(HashTable** p_ht) {
    if (!p_ht || !*p_ht) return;
    HashTable* ht = *p_ht;

    // ����ÿһ��Ͱ
    for (size_t i = 0; i < ht->capacity; i++) {
        // ����Ͱ�е������ͷ�ÿһ���ڵ�
        Node* current = ht->buckets[i];
      
        // ���ӷ����Լ�飬ȷ�� to_free->key �� to_free->value �� NULL �����ͷ�
        while (current != NULL) {
            Node* to_free = current;
            current = current->next;
            if (to_free->key) {
                free(to_free->key);   // �ͷż��Ŀ���
            }
            if (to_free->value) {
                free(to_free->value); // �ͷ�ֵ�Ŀ���
            }
            free(to_free);        // �ͷŽڵ㱾��
        }
    }
    free(ht->buckets); // �ͷ�Ͱ����
    free(ht);          // �ͷŹ�ϣ��ṹ��
    *p_ht = NULL;      // ��ֹ����ָ��
}

bool ht_set(HashTable* ht, const char* key, const char* value) {
    /*
     * ��������: [��ҵ��] ��������һ����ֵ�ԡ�
     * ------------------------- ��ѧ���� -------------------------
     * ���� capacity = 10, hash("name") = 4, hash("mane") = 4 (��ϣ��ͻ)
     *
     * ���� 1: ���� ("name", "Alice")
     *   - �����ϣ�õ����� 4��
     *   - Ͱ 4 Ϊ�գ�ֱ�Ӵ���һ���½ڵ㲢���롣
     *   - Ͱ 4: -> [("name", "Alice")] -> NULL
     *
     * ���� 2: ���� ("mane", "Bob")
     *   - �����ϣ�õ����� 4��
     *   - Ͱ 4 ��Ϊ�գ���������δ�ҵ��� "mane"��
     *   - �����½ڵ㣬��������뵽����ͷ�� (��򵥵ķ�ʽ)��
     *   - Ͱ 4: -> [("mane", "Bob")] -> [("name", "Alice")] -> NULL
     *
     * ���� 3: ���� ("name", "Amy")
     *   - �����ϣ�õ����� 4��
     *   - ����Ͱ 4 ������ʹ�� strcmp �ҵ��� "name"��
     *   - �ͷž�ֵ "Alice" ���ڴ棬�������ڴ沢���� "Amy"��
     *   - Ͱ 4: -> [("mane", "Bob")] -> [("name", "Amy")] -> NULL
     * -----------------------------------------------------------
    */

    // ���� 1: [�����Ա��] ���������������Ƿ���Ч��
    if (!ht || !key || !value) return false;

    // ���� 2: ������Ĺ�ϣֵ��ȷ���������ĸ�Ͱ��
    size_t index = hash_function(key, ht->capacity);

    // ���� 3: ����Ͱ�е����������Ƿ��Ѿ����ڡ�
    Node* current = ht->buckets[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            // --- ���A: ���Ѵ��ڣ�ִ�С����¡����� ---
            // Ϊ�˷�ֹ�ڴ�й©�����ͷž�ֵ���ڴ档
            free(current->value);
            // Ϊ��ֵ�����ڴ沢������
            current->value = (char*)malloc(strlen(value) + 1);
            if (!current->value) return false; // �ڴ����ʧ��
            strcpy(current->value, value);
            return true; // ���³ɹ�����������
        }
        current = current->next;
    }

    // --- ���B: �������� (ѭ�����궼δ�ҵ�)��ִ�С����롱���� ---
    // ���� 4: ����һ���µĽڵ㡣
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) return false;

    // Ϊ����ֵ����������ڴ沢������������ϣ��Ͳ��������ⲿ�����ָ���������ڡ�
    new_node->key = (char*)malloc(strlen(key) + 1);
    new_node->value = (char*)malloc(strlen(value) + 1);

    // ��׳�Լ�飺����κ�һ��mallocʧ�ܣ�����Ҫ�����ѷ�����ڴ档
    if (!new_node->key || !new_node->value) {
        free(new_node->key); // free(NULL)�ǰ�ȫ��
        free(new_node->value);
        free(new_node);
        return false;
    }
    strcpy(new_node->key, key);
    strcpy(new_node->value, value);

    // ���� 5: ���½ڵ���뵽�����ͷ����
    // ������򵥸�Ч��������뷽ʽ��
    new_node->next = ht->buckets[index]; // �½ڵ�� next ָ��ǰͰ�ĵ�һ���ڵ�
    ht->buckets[index] = new_node;       // ����Ͱ��ͷָ�룬ʹ��ָ������½ڵ�

    // ���� 6: ���¹�ϣ����ܴ�С��
    ht->size++;
    return true; // ����ɹ�
}

const char* ht_get(const HashTable* ht, const char* key) {
    /*
     * ��������: [��ҵ��] ���ݼ����Ҷ�Ӧ��ֵ��
     * ��ѧ����: �� `ht_set` ���� 3 ��״̬�²��� "name"��
     *   - Ͱ 4: -> [("mane", "Bob")] -> [("name", "Amy")] -> NULL
     *   - �����ϣ("name")�õ����� 4��
     *   - ����Ͱ 4 ������
     *   - ��һ���ڵ� "mane" != "name"��
     *   - �ڶ����ڵ� "name" == "name"���ҵ���
     *   - ����ָ���ڲ�ֵ "Amy" ��ָ�롣
    */
    if (!ht || !key) return NULL;

    size_t index = hash_function(key, ht->capacity);

    Node* current = ht->buckets[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->value; // �ҵ�������ָ���ڲ�ֵ��ָ��
        }
        current = current->next;
    }

    return NULL; // ������������δ�ҵ�
}

bool ht_remove(HashTable* ht, const char* key) {
    /*
     * ��������: [��ҵ��] ���ݼ�ɾ��һ����ֵ�ԡ�
     * ��ѧ����: �� `ht_set` ���� 3 ��״̬��ɾ�� "mane"��
     *   - Ͱ 4: -> [("mane", "Bob")] -> [("name", "Amy")] -> NULL
     *   - �����ϣ("mane")�õ����� 4��
     *   - ����������Ҫһ�� `prev` ָ��������ɾ����
     *   - �ҵ� "mane" ʱ������ͷ�ڵ� (prev=NULL)��
     *   - ��Ͱ��ͷָ�� `buckets[4]` ָ�� "mane" ����һ���ڵ� ("name")��
     *   - �ͷ� "mane" �ڵ�ļ���ֵ�ͽڵ㱾��
     *   - ����Ͱ 4: -> [("name", "Amy")] -> NULL
     * 
	 * [Ͱ�����] -> Alice -> Bob -> Charile -> David -> NULL
	 * ht->buckets[index] = Alice
	 * current = Alice
     * 
     */
    if (!ht || !key) return false;

    size_t index = hash_function(key, ht->capacity);

    Node* current = ht->buckets[index];
    Node* prev = NULL; // �ؼ�: ��������ǰ��ڵ�� "ǰ��" ָ��

    // �����������Ҫɾ���Ľڵ�
    while (current != NULL && strcmp(current->key, key) != 0) {
        prev = current;
        current = current->next;
    }

    // ��� current Ϊ NULL��˵��������Ҳû�ҵ������
    if (current == NULL) {
        return false;
    }

    // --- �ҵ���Ҫɾ���Ľڵ� (current) ---
    if (prev == NULL) {
        // ���A: Ҫɾ�����������ͷ�ڵ�
        ht->buckets[index] = current->next;
    }
    else {
        // ���B: Ҫɾ������������м��β���ڵ�
        prev->next = current->next;
    }

    // �ͷű�ɾ���ڵ�������ڴ�
    free(current->key);
    free(current->value);
    free(current);

    ht->size--;
    return true; // ɾ���ɹ�
}

void ht_print(const HashTable* ht) {
    if (!ht) {
        printf("HashTable is NULL.\n");
        return;
    }
    printf("--- HashTable (capacity: %zu, size: %zu) ---\n", ht->capacity, ht->size);
    for (size_t i = 0; i < ht->capacity; i++) {
        printf("Bucket[%zu]: ", i);
        Node* current = ht->buckets[i];
        if (current == NULL) {
            printf("-> NULL\n");
        }
        else {
            while (current != NULL) {
                printf("-> [K:\"%s\", V:\"%s\"] ", current->key, current->value);
                current = current->next;
            }
            printf("-> NULL\n");
        }
    }
    printf("------------------------------------------\n\n");
}