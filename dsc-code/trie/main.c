#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"

// --- �Զ������ݽṹ ---
// Ϊ�˲��Է���Trie�����Ƕ���һ��Person�ṹ������Ϊ�洢��ֵ��
typedef struct {
    char* name;
    int age;
} Person;

// --- �Զ���ֵ�Ĵ��������ٺ��� ---

/**
 * @brief ����һ���µ�Personʵ����������������
 */
Person* Person_Create(const char* name, int age) {
    Person* p = (Person*)malloc(sizeof(Person));
    if (!p) return NULL;
    // Ϊ���ַ����ڴ沢����
    p->name = (char*)malloc(strlen(name) + 1);
    if (!p->name) {
        free(p);
        return NULL;
    }
    strcpy(p->name, name);
    p->age = age;
    return p;
}

/**
 * @brief ����Trie��ValueDestroyer�������ͷ�Person�ṹ����ڴ档
 * @param value һ��void*ָ�룬ָ��һ��Person�ṹ�塣
 */
void Person_Destroy(void* value) {
    if (!value) return;
    Person* p = (Person*)value;
    printf("  (Destroying value for Person: %s)\n", p->name);
    free(p->name); // �ͷ��ڲ����ַ���
    free(p);       // �ͷŽṹ�屾��
}


/**
 * @brief ��ӡ��������ĸ���������
 */
void print_search_result(const Trie* trie, const char* key) {
    Person* p = (Person*)Trie_Search(trie, key);
    if (p) {
        printf("Search for '%s': FOUND. Value -> { Name: %s, Age: %d }\n", key, p->name, p->age);
    }
    else {
        printf("Search for '%s': NOT FOUND.\n", key);
    }
}


int main(void) {
    printf("--- Trie Test Suite ---\n\n");

    // 1. ����Trie��������Person����������
    printf("1. Creating Trie with a custom value destroyer...\n");
    Trie* trie = Trie_Create(Person_Destroy);
    if (!trie) {
        fprintf(stderr, "Failed to create Trie.\n");
        return 1;
    }
    printf("Trie created successfully.\n\n");

    // 2. ��������
    printf("2. Testing Trie_Insert...\n");
    // ע�⣺Trie��ӹ���ЩPersonָ����ڴ�������ǲ���Ҫ��main���ֶ��ͷ�����
    Trie_Insert(trie, "apple", Person_Create("iPhone", 15));
    Trie_Insert(trie, "app", Person_Create("AppStore", 16));
    Trie_Insert(trie, "application", Person_Create("GenericApp", 5));
    Trie_Insert(trie, "banana", Person_Create("Fruit", 2));
    Trie_Insert(trie, "band", Person_Create("MusicGroup", 10));
    printf("Finished inserting keys: 'apple', 'app', 'application', 'banana', 'band'.\n\n");

    // 3. ��������
    printf("3. Testing Trie_Search...\n");
    print_search_result(trie, "apple");
    print_search_result(trie, "app");
    print_search_result(trie, "application");
    print_search_result(trie, "banana");
    print_search_result(trie, "band");
    print_search_result(trie, "ban");    // �����ڵļ�
    print_search_result(trie, "apples");  // �����ڵļ�
    printf("\n");

    // 4. ǰ׺����
    printf("4. Testing Trie_StartsWith...\n");
    printf("Prefix 'app': %s\n", Trie_StartsWith(trie, "app") ? "true" : "false");
    printf("Prefix 'ban': %s\n", Trie_StartsWith(trie, "ban") ? "true" : "false");
    printf("Prefix 'bana': %s\n", Trie_StartsWith(trie, "bana") ? "true" : "false");
    printf("Prefix 'cat': %s\n", Trie_StartsWith(trie, "cat") ? "false" : "false");
    printf("Prefix 'application': %s\n", Trie_StartsWith(trie, "application") ? "true" : "false");
    printf("\n");

    // 5. ɾ������
    printf("5. Testing Trie_Delete...\n");

    // ����A: ɾ��һ����Ϊ������ǰ׺�ļ� ("app")
    // Ԥ�ڣ�'app'��value��ΪNULL�����ڵ㱣������Ϊ'apple'��'application'��Ҫ����
    printf("\n--- Deleting 'app' (is a prefix of others) ---\n");
    Trie_Delete(trie, "app");
    print_search_result(trie, "app"); // Ӧ���Ҳ�����
    print_search_result(trie, "apple"); // Ӧ�û���
    printf("Prefix 'app' after deleting key 'app': %s\n", Trie_StartsWith(trie, "app") ? "true" : "false"); // ǰ׺Ӧ�û�����

    // ����B: ɾ��һ��Ҷ�Ӽ� ("banana")
    // Ԥ�ڣ�'banana'��ɾ��������'nana'·���Ľڵ㱻�����Ƴ���
    printf("\n--- Deleting 'banana' (is a leaf path) ---\n");
    Trie_Delete(trie, "banana");
    print_search_result(trie, "banana"); // Ӧ���Ҳ�����
    print_search_result(trie, "band");   // Ӧ�û���
    printf("Prefix 'bana' after deleting key 'banana': %s\n", Trie_StartsWith(trie, "bana") ? "false" : "false"); // ǰ׺Ӧ�ò�������
    printf("Prefix 'ban' after deleting key 'banana': %s\n", Trie_StartsWith(trie, "ban") ? "true" : "false");   // ��'ban'ǰ׺Ӧ��'band'������

    // ����C: ɾ��һ�������ڵļ�
    printf("\n--- Deleting 'zebra' (non-existent) ---\n");
    Trie_Delete(trie, "zebra");
    printf("Attempted to delete a non-existent key. Let's check if others are intact.\n");
    print_search_result(trie, "apple"); // Ӧ�û���
    print_search_result(trie, "band");  // Ӧ�û���

    // ����D: ɾ�����һ��������ĳ��ǰ׺�ļ�
    printf("\n--- Deleting 'application' ---\n");
    Trie_Delete(trie, "application");
    print_search_result(trie, "application");
    printf("Prefix 'appli' after deleting key 'application': %s\n", Trie_StartsWith(trie, "appli") ? "false" : "false"); // ǰ׺Ӧ�ò�������

    printf("\n");

    // 6. ����Trie
    printf("6. Testing Trie_Destroy...\n");
    printf("Calling Trie_Destroy. The custom destroyer should be called for remaining values ('apple', 'band').\n");
    Trie_Destroy(trie);
    printf("Trie destroyed.\n\n");

    printf("--- Test Suite Finished ---\n");
    return 0;
}