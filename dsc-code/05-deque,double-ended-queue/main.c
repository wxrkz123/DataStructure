#include "generic_deque.h"
#include <stdio.h>

void test_deque_as_queue() {
    printf("--- 1. ʹ��˫�˶���ģ�⡰��׼���С� ---\n");
    printf("(����: push_back + pop_front)\n");

    Deque* dq = deque_create(5, sizeof(int));
    int val;

    printf("Push Back: 10, 20, 30\n");
    for (val = 10; val <= 30; val += 10) deque_push_back(dq, &val);

    printf("��ǰ��С: %zu\n", deque_get_size(dq));

    printf("Pop Front (Ӧ��FIFO˳��):\n");
    while (!deque_is_empty(dq)) {
        deque_pop_front(dq, &val);
        printf(" -> %d\n", val);
    }
    printf("\n");
    deque_destroy(&dq);
}

void test_deque_as_stack() {
    printf("--- 2. ʹ��˫�˶���ģ�⡰ջ�� ---\n");
    printf("(����: push_front + pop_front)\n");

    Deque* dq = deque_create(5, sizeof(int));
    int val;

    printf("Push Front: 10, 20, 30\n");
    for (val = 10; val <= 30; val += 10) deque_push_front(dq, &val);

    printf("��ǰ��С: %zu\n", deque_get_size(dq));

    printf("Pop Front (Ӧ��LIFO˳��):\n");
    while (!deque_is_empty(dq)) {
        deque_pop_front(dq, &val);
        printf(" -> %d\n", val);
    }
    printf("\n");
    deque_destroy(&dq);
}

void test_mixed_operations() {
    printf("--- 3. ��ϲ������� ---\n");
    Deque* dq = deque_create(5, sizeof(char));
    char val;

    // A, B
    val = 'A'; deque_push_back(dq, &val);
    val = 'B'; deque_push_back(dq, &val);
    printf("Push Back 'A', 'B'. Front: 'A', Back: 'B'\n");

    // X, A, B
    val = 'X'; deque_push_front(dq, &val);
    printf("Push Front 'X'. Front: 'X', Back: 'B'\n");

    // X, A, B, C
    val = 'C'; deque_push_back(dq, &val);
    printf("Push Back 'C'. Front: 'X', Back: 'C'\n");

    printf("��ǰ���д�С: %zu. ����(��ͷ��β): X, A, B, C\n", deque_get_size(dq));

    deque_peek_front(dq, &val);
    printf("Peek Front: %c\n", val);
    deque_peek_back(dq, &val);
    printf("Peek Back: %c\n", val);

    deque_pop_back(dq, &val);
    printf("Pop Back: %c. ʣ��: X, A, B\n", val);

    deque_pop_front(dq, &val);
    printf("Pop Front: %c. ʣ��: A, B\n", val);

    deque_destroy(&dq);
}


int main(void) {
  
    test_deque_as_queue();
    test_deque_as_stack();
    test_mixed_operations();
    return 0;
}