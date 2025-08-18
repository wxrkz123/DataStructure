#include <stdio.h>
#include <stdlib.h>
#include <string.h> // ��Ҫ strcmp �� strdup
#include "avl_tree.h"

// =================================================================
// ============== ���� 1: ������ԭ main �������ݣ� =================
// =================================================================

// ���ڲ��ԵıȽϺ������Ƚ�������
int compare_int(const void* a, const void* b, void* context) {
    (void)context; // context �ڴ�ʾ����δʹ��
    int int_a = *(const int*)a;
    int int_b = *(const int*)b;
    if (int_a < int_b) return -1;
    if (int_a > int_b) return 1;
    return 0;
}

// �ͷ��������ݵĻص�����
void free_int(void* data, void* context) {
    (void)context; // context �ڴ�ʾ����δʹ��
    free(data);
}

// ����ʱ��ӡ�����Ļص�����
void print_int(void* data, void* context) {
    (void)context; // context �ڴ�ʾ����δʹ��
    printf("%d ", *(int*)data);
}

// ������������̬����һ������
int* create_int(int value) {
    int* p = (int*)malloc(sizeof(int));
    if (p) {
        *p = value;
    }
    return p;
}

void print_line() {
    printf("----------------------------------------\n");
}

void run_integer_test() {
    printf("===== Running Integer Test =====\n");
    // ����AVL��
    AVLTree* tree = avl_create(compare_int, free_int, NULL);
    if (!tree) {
        fprintf(stderr, "Failed to create AVL tree.\n");
        return;
    }

    printf("AVL Tree created successfully.\n");
    printf("Is tree empty? %s\n", avl_is_empty(tree) ? "Yes" : "No");
    print_line();

    // --- ������� ---
    printf("Inserting elements: 10, 20, 30, 40, 50, 25\n");
    int values_to_insert[] = { 10, 20, 30, 40, 50, 25 };
    for (size_t i = 0; i < sizeof(values_to_insert) / sizeof(int); ++i) {
        avl_insert(tree, create_int(values_to_insert[i]));
    }

    printf("Tree size: %zu\n", avl_size(tree));
    printf("Tree height: %d\n", avl_height(tree));
    printf("Is tree valid? %s\n", avl_validate(tree) ? "Yes" : "No");
    print_line();

    // --- �������� ---
    printf("In-order traversal:   ");
    avl_traverse_inorder(tree, print_int, NULL);
    printf("\n");

    // --- ɾ������ ---
    printf("Deleting element 40...\n");
    int key_to_delete = 40;
    avl_delete(tree, &key_to_delete);
    printf("In-order after deleting 40: ");
    avl_traverse_inorder(tree, print_int, NULL);
    printf("\n");
    printf("Is tree valid? %s\n", avl_validate(tree) ? "Yes" : "No");
    print_line();

    // --- ���²��� ---
    printf("Updating element...\n");
    int old_key = 50;
    int* new_key_ptr = create_int(55);
    printf("Updating %d to %d...\n", old_key, *new_key_ptr);
    bool updated = avl_update(tree, &old_key, new_key_ptr);
    if (updated) {
        printf("Update successful.\n");
        printf("In-order after update: ");
        avl_traverse_inorder(tree, print_int, NULL);
        printf("\n");
    }
    else {
        printf("Update failed.\n");
        free(new_key_ptr); // �������ʧ�ܣ���Ҫ�ͷ������ݵ��ڴ�
    }
    print_line();

    // --- ���ٲ��� ---
    printf("Destroying the integer tree...\n");
    avl_destroy(tree);
    printf("Integer tree destroyed.\n");
}


// =================================================================
// ============== ���� 2: Student �ࣨ���� Context�� ================
// =================================================================

// Student "��"
typedef struct {
    int id;
    char* name;
} Student;

// �������������� Student
Student* create_student(int id, const char* name) {
    Student* s = (Student*)malloc(sizeof(Student));
    if (!s) return NULL;
    s->id = id;
    s->name = _strdup(name); // strdup ��̬�����ڴ沢�����ַ���
    if (!s->name) {
        free(s);
        return NULL;
    }
    return s;
}

// �ȽϺ��������� context ��������ʽ
int compare_student(const void* a, const void* b, void* context) {
    const Student* s_a = (const Student*)a;
    const Student* s_b = (const Student*)b;
    const char* sort_key = (const char*)context;

    // ���� context��һ���ַ����������Ƚ��ֶ�
    if (strcmp(sort_key, "name") == 0) {
        return strcmp(s_a->name, s_b->name);
    }

    // Ĭ�ϻ�ָ���� "id" �Ƚ�
    // if (strcmp(sort_key, "id") == 0)
    if (s_a->id < s_b->id) return -1;
    if (s_a->id > s_b->id) return 1;
    return 0;
}

// �ͷ� Student ����
void free_student(void* data, void* context) {
    (void)context;
    Student* s = (Student*)data;
    free(s->name); // ���ͷ��ڲ����ַ���
    free(s);       // ���ͷŽṹ�屾��
}

// ��ӡ Student ����
void print_student(void* data, void* context) {
    (void)context;
    const Student* s = (const Student*)data;
    printf("(%d, %s) ", s->id, s->name);
}

void run_student_test() {
    printf("===== Running Student Test (using context) =====\n");

    // --- Part 1: �� ID ���� ---
    printf("\n--- Sorting by ID ---\n");
    const char* sort_by_id = "id";
    AVLTree* student_tree_by_id = avl_create(compare_student, free_student, (void*)sort_by_id);

    printf("Inserting students...\n");
    avl_insert(student_tree_by_id, create_student(5, "Charlie"));
    avl_insert(student_tree_by_id, create_student(2, "Alice"));
    avl_insert(student_tree_by_id, create_student(8, "Eve"));
    avl_insert(student_tree_by_id, create_student(1, "Bob"));
    avl_insert(student_tree_by_id, create_student(6, "David"));

    printf("Tree size: %zu\n", avl_size(student_tree_by_id));
    printf("Is tree valid? %s\n", avl_validate(student_tree_by_id) ? "Yes" : "No");
    printf("In-order traversal (should be by ID): ");
    avl_traverse_inorder(student_tree_by_id, print_student, NULL);
    printf("\n");

    printf("Destroying student tree (sorted by ID)...\n");
    avl_destroy(student_tree_by_id);
    print_line();

    // --- Part 2: �� Name ���� ---
    printf("\n--- Sorting by Name ---\n");
    const char* sort_by_name = "name";
    AVLTree* student_tree_by_name = avl_create(compare_student, free_student, (void*)sort_by_name);

    printf("Inserting same students...\n");
    avl_insert(student_tree_by_name, create_student(5, "Charlie"));
    avl_insert(student_tree_by_name, create_student(2, "Alice"));
    avl_insert(student_tree_by_name, create_student(8, "Eve"));
    avl_insert(student_tree_by_name, create_student(1, "Bob"));
    avl_insert(student_tree_by_name, create_student(6, "David"));

    printf("Tree size: %zu\n", avl_size(student_tree_by_name));
    printf("Is tree valid? %s\n", avl_validate(student_tree_by_name) ? "Yes" : "No");
    printf("In-order traversal (should be by Name): ");
    avl_traverse_inorder(student_tree_by_name, print_student, NULL);
    printf("\n");

    printf("Destroying student tree (sorted by Name)...\n");
    avl_destroy(student_tree_by_name);
    printf("Student test finished.\n");
}


// =================================================================
// ===================== ��������� =================================
// =================================================================

int main() {
    run_integer_test();

    printf("\n\n================================================\n");
    printf("============== End of Integer Test =============\n");
    printf("================================================\n\n");

    run_student_test();

    return 0;
}