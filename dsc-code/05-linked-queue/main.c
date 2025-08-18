// main.c

#include "generic_linked_queue.h"
#include <stdio.h>

// ����һ���ṹ�����ڲ���
typedef struct {
    int id;
    char name[16];
} Person;

void print_person(const Person* p) {
    if (p) printf("Person(id: %d, name: \"%s\")", p->id, p->name);
}

void print_queue_status(const Queue* q, const char* title) {
    printf("--- %s ---\n", title);
    if (!q) {
        printf("����Ϊ NULL\n\n");
        return;
    }
    printf("��С: %zu, ��? %s\n", queue_get_size(q), queue_is_empty(q) ? "��" : "��");

    Person peeked_person;
    if (queue_peek(q, &peeked_person)) {
        printf("��ͷ��: ");
        print_person(&peeked_person);
        printf("\n");
    }
    else {
        printf("��ͷΪ�ա�\n");
    }
    printf("\n");
}


int main() {
    // 1. ����һ��Person���͵���ʽ����
    Queue* q = queue_create(sizeof(Person));
    print_queue_status(q, "1. ��ʼ״̬");

    // 2. ��ӵ�һ��Ԫ��
    Person p1 = { 101, "Alice" };
    printf("���: "); print_person(&p1); printf("\n");
    queue_enqueue(q, &p1);
    print_queue_status(q, "2. ��ӵ�һ��Ԫ�غ�");

    // 3. ��Ӹ���Ԫ��
    Person p2 = { 102, "Bob" };
    Person p3 = { 103, "Charlie" };
    printf("���: "); print_person(&p2); printf("\n");
    queue_enqueue(q, &p2);
    printf("���: "); print_person(&p3); printf("\n");
    queue_enqueue(q, &p3);
    print_queue_status(q, "3. ������Ӻ�");

    // 4. ����һ��Ԫ��
    Person dequeued_person;
    if (queue_dequeue(q, &dequeued_person)) {
        printf("���ӳɹ�: ");
        print_person(&dequeued_person);
        printf("\n");
    }
    print_queue_status(q, "4. ����һ��Ԫ�غ�");

    // 5. ȫ�����ӣ����Զ��б�յ��߼�
    printf("������ʣ��Ԫ�س���...\n");
    while (!queue_is_empty(q)) {
        if (queue_dequeue(q, &dequeued_person)) {
            printf("����: ");
            print_person(&dequeued_person);
            printf("\n");
        }
    }
    print_queue_status(q, "5. ȫ�����Ӻ�");

    // 6. ���ԶԿն��н��в���
    printf("���ԶԿն��г���...\n");
    if (!queue_dequeue(q, &dequeued_person)) {
        printf("����ʧ�ܣ�����Ԥ�ڡ�\n\n");
    }

    // 7. ���ٶ���
    queue_destroy(&q);
    printf("���������٣�ָ��Ϊ: %s\n", q == NULL ? "NULL" : "Not NULL");

    return 0;
}