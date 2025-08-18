// main.c

#include "bst.h"
#include <stdio.h>


/*

 * ���������� (BST) ģ�鹹��

������ ��. �춨��ʯ�������ͼ�빫���ӿ� (./bst.h)
��   ��
��   ������ 1. ���塰�ͻ����Ĺ����䣺����ָ������
��   ��   ������ a. CompareFunc  (���⣺Ϊʲô��Ҫ��������BST�ġ���ꡱ�������˴�С��ϵ)
��   ��   ������ b. VisitFunc    (���⣺����Ϊ�˱���ʱ�ܶ�ÿ���ڵ��������ġ����ӡ�)
��   ��
��   ������ 2. ��ơ���͸���������棺�ṹ����APIԭ��
��       ������ a. typedef struct BST BST; (���⣺��Ϣ���ص�ħ�����û�ֻ֪������֪����)
��       ������ b. ����API����ԭ�� (��һ����ÿ�����������ã������������û��ġ���ͬ��)
��           ������ bst_create()
��           ������ bst_destroy()
��           ������ bst_insert()
��           ������ bst_remove()
��           ������ bst_search()
��           ������ bst_traverse()
��
������ ��. ��Ǽܣ�ʵ�ֺ��Ľṹ��򵥹������� (./bst.c)
��   ��
��   ������ 1. �����ڲ��ṹ������˽�нṹ��
��   ��   ������ a. Node          (���⣺�������Ļ���ש��)
��   ��   ������ b. BST (struct)  (���⣺�������ġ����ԡ���������һ��)
��   ��
��   ������ 2. ʵ�֡����족�롰������
��   ��   ������ a. bst_create()  (���⣺�����ڴ棬��ʼ�������ԡ�)
��   ��   ������ b. bst_destroy() (���⣺��ΪCEO���������������񣬵����幤��Ҫ��ί�С�������)
��   ��       ������ (������ _destroy_recursive ������)
��   ��
��   ������ 3. ʵ�֡��򵥡��Ĺ�������
��       ������ a. bst_search()  (���⣺������򵥵Ĳ��ң��õ���ʵ�֣��߼�ֱ��)
��       ������ b. bst_is_empty(), bst_get_size() (���⣺��Щ�Ǽ򵥵�״̬��ѯ)
��       ������ c. bst_traverse() (���⣺��ΪCEO��������������񣬾��幤����ί�С�������)
��           ������ (������ _traverse_recursive ������)
��
������ ��. ���Ѫ�⣺ʵ�ָ��ӵ�˽�еݹ��߼� (./bst.c �ĺ���)
    ��
    ������ 1. ʵ�֡����롱�ĵݹ��߼� (CEO -> ����)
    ��   ������ a. bst_insert() (��ϰ���ع�CEO��ְ��)
    ��   ������ b. _insert_recursive() (���뽲�⣺���ž�����εݹ���ҵ�λ��)
    ��       ������ b.1. ��׼��� (Base Case): �ҵ���λ�������ڵ�
    ��       ������ b.2. �ݹ鲽��: �Ƚϴ�С�����������߻���������
    ��
    ������ 2. ʵ�֡�ɾ�����ĵݹ��߼� (���ѵĲ��֣��ֲ�����)
    ��   ������ a. bst_remove() (��ϰ���ع�CEO��ְ��)
    ��   ������ b. _remove_recursive() (���뽲�⣺��λ������������)
    ��       ������ b.1. ���ҽ׶�: �ݹ���ҵ�Ҫɾ���Ľڵ�
    ��       ������ b.2. ɾ���׶� - ���1: Ҷ�ӽڵ� (���)
    ��       ������ b.3. ɾ���׶� - ���2: ֻ��һ���ӽڵ�Ľڵ�
    ��       ������ b.4. ɾ���׶� - ���3: �������ӽڵ�Ľڵ�
    ��           ������ (�˴���Ҫ�Ƚ��� _find_min_recursive ��������)
    ��
    ������ 3. ʵ�֡��������롰���١��ĵݹ��߼� (��Ϊ�ݹ����Ĺ���)
        ������ a. _traverse_recursive() (���뽲�⣺ǰ���С���������ĵݹ�ʵ��)
        ������ b. _destroy_recursive() (���뽲�⣺���������������ʱ������)
 */

// --- �û�����ıȽϺͷ��ʺ��� ---
// �Ƚ���������
int compare_int(const void* a, const void* b) {
    int int_a = *(const int*)a;
    int int_b = *(const int*)b;
    if (int_a < int_b) return -1;
    if (int_a > int_b) return 1;
    return 0;
}

// ���Ǽ���дһ���ȽϺ������Ƚ�������Student��id�������
// int compare_student_by_id(const void* a, const void* b) {
//     const Student* student_a = (const Student*)a;
//     const Student* student_b = (const Student*)b;
//     if (student_a->id < student_b->id) return -1;
//     if (student_a->id > student_b->id) return 1;
//	 return 0;
// }

// ��ӡһ������
void visit_int(const void* data) {
    printf("%d ", *(const int*)data);
}

void print_in_order(const BST* bst) {
    printf("������� (����): ");
    bst_traverse(bst, IN_ORDER, visit_int);
    printf("\n");
}

int main() {
    printf("--- �ִ�C���Է��Ͷ���������ʵ�� ---\n");
    BST* bst = bst_create(sizeof(int), compare_int);

    // 1. ����ڵ�
    printf("\n1. ����ڵ�: 20, 10, 30, 5, 15, 25, 40, 3, 7\n");
    int values[] = { 20, 10, 30, 5, 15, 25, 40, 3, 7 };
    for (size_t i = 0; i < sizeof(values) / sizeof(int); i++) {
        bst_insert(bst, &values[i]);
    }
    print_in_order(bst);
    printf("��ǰ��С: %zu\n", bst_get_size(bst));

    // 2. �����ڵ�
    printf("\n2. �����ڵ�...\n");
    int key_to_find = 15;
    printf("���� %d: %s\n", key_to_find, bst_search(bst, &key_to_find) ? "�ҵ�" : "δ�ҵ�");
    key_to_find = 99;
    printf("���� %d: %s\n", key_to_find, bst_search(bst, &key_to_find) ? "�ҵ�" : "δ�ҵ�");

    // 3. ɾ������ - ���1: Ҷ�ӽڵ�
    printf("\n3. ɾ��Ҷ�ӽڵ� (7)...\n");
    int key_to_remove = 7;
    bst_remove(bst, &key_to_remove);
    print_in_order(bst);

    // 4. ɾ������ - ���2: ֻ��һ���ӽڵ�Ľڵ�
    // ������ɾ�� 3���� 5 ֻ��һ�����ӽڵ㡣Ȼ����ɾ�� 5��
    printf("\n4. ɾ��ֻ��һ���ӽڵ�Ľڵ� (5)...\n");
    key_to_remove = 3;
    bst_remove(bst, &key_to_remove); // ��ɾ��3
    key_to_remove = 5;
    bst_remove(bst, &key_to_remove); // ��ɾ5����ʱ5Ӧ��ֻ��һ������
    print_in_order(bst);

    // 5. ɾ������ - ���3: �������ӽڵ�Ľڵ�
    printf("\n5. ɾ���������ӽڵ�Ľڵ� (10)...\n");
    key_to_remove = 10;
    bst_remove(bst, &key_to_remove);
    print_in_order(bst);

    // 6. ɾ�����ڵ�
    printf("\n6. ɾ�����ڵ� (20)...\n");
    key_to_remove = 20;
    bst_remove(bst, &key_to_remove);
    print_in_order(bst);
    printf("��ǰ��С: %zu\n", bst_get_size(bst));

    // 7. ������
    printf("\n7. ������...\n");
    bst_destroy(&bst);
    printf("�������٣�ָ��Ϊ: %s\n", bst == NULL ? "NULL" : "OK");

    return 0;
}