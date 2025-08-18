#define _CRT_SECURE_NO_WARNINGS

#include "Node.h"
#include <stdio.h>
#include <string.h>

// 1. ���塰��ӡ���ص�
void print_student(const void* data) {
    const Student* s = (const Student*)data;
    printf("{ID: %d, ����: %s, ����: %d}", s->id, s->name, s->age);
}

// 2. ����һ���򵥵ġ��Ƚϡ��ص� (ֻ��ID�Ƚ�)
int compare_by_id(const void* a, const void* b, void* context) {
    // �����������Ҫ����������ģ����� context ����������
    // (void)context; // ��ȷ��ʾ�������ⲻʹ������������������������

    const Student* s_a = (const Student*)a;
    const Student* target_s = (const Student*)b;
    return s_a->id == target_s->id ? 0 : 1; // ����0��ʾ���
}

// 3. ����һ�����ӵġ��Ƚϡ��ص� (��Ҫʹ��������)
//    �����Ľṹ�壬���ڴ���������
typedef struct {
    int min_age_required;
} SearchContext;

int compare_by_id_and_min_age(const void* a, const void* b, void* context) {
    const Student* s_a = (const Student*)a;
    const Student* target_s = (const Student*)b;
    SearchContext* ctx = (SearchContext*)context;

    // ���ӵıȽ��߼�
    if (s_a->id == target_s->id && s_a->age >= ctx->min_age_required) {
        return 0; // �����������㣬��Ϊ���
    }
    return 1; // �������
}

// 4. ���塰�ͷ�Data�ڲ���Դ���Ļص� (������Ϊ�գ�������ʾ)
void free_student_data(void* data) {
    // ��� Student �ṹ���ڵ� name �� char* �������� malloc ���䣬
    // �˴�����Ҫ free(((Student*)data)->name);
}



int main(void) {
    Node* head = NULL;

    // --- ��ʼ������ ---
    printf("--- 1. ��ʼ������ ---\n");
    Student students[] = {
        {101, "Alice", 22},
        {102, "Bob", 19},
        {103, "Carol", 25},
        {104, "David", 19}
    };
    for (int i = 0; i < 4; ++i) {
        appendNode(&head, students[i]);
    }
    printList(head, print_student);

    // --- �򵥲�����ɾ�� ---
    printf("\n--- 2. ɾ��ѧ��Ϊ103��ѧ��(Carol) ---\n");
    Student target_carol = { 103, "", 0 };
    deleteNode(&head, &target_carol, compare_by_id, NULL); // �򵥱Ƚϣ�����Ҫ������
    printList(head, print_student);

    // --- ���Ӳ�������� ---
    printf("\n--- 3. ����ѧ��Ϊ104�����䲻С��20���ѧ�� ---\n");
    SearchContext ctx_fail = { 20 }; // ���������ģ���С����20
    Student target_david = { 104, "", 0 };
    Node* found = findNode(head, &target_david, compare_by_id_and_min_age, &ctx_fail);
    if (found) {
        printf("�ҵ��ˣ�\n");
    }
    else {

        // test
        printf("û�ҵ� (��ΪDavidֻ��19��)��\n");
    }

    printf("\n--- 4. ����ѧ��Ϊ101�����䲻С��20���ѧ���������� ---\n");
    SearchContext ctx_success = { 20 }; // ���������ģ���С����20
    Student target_alice = { 101, "", 0 };
    Student new_alice_data = { 101, "Alicia", 23 };
    updateNode(head, &target_alice, new_alice_data, compare_by_id_and_min_age, &ctx_success);
    printList(head, print_student);

    //// --- ���� ---
    //printf("\n--- 5. �ͷ������ڴ� ---\n");
    //freeList(&head, free_student_data);
    //printf("��������ա�\n");
    //printList(head, print_student);

    return 0;
}