#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // ��Ҫ���� string.h ��ʹ�� strcpy

#include "DynamicArray.h"

// ���������Լ��Ļص�����
// 1. ����һ��֪����δ�ӡStudent�ĺ���
// ���ĺ���ǩ������ͺ���ָ��Ķ�����ȫƥ��
void print_student(const void* data) {
    // ���յ���data��һ��void*ָ�룬������Ҫ����ת��Ϊ��������������
    const Student* s_ptr = (const Student*)data;
    printf("Student: {id: %d, name: \"%s\"}", s_ptr->id, s_ptr->name);
}


int main() {
    printf("--- ���� Student ��̬���� ---\n");
    DynamicArray* student_list = create_array(2);

    Student s1 = { 101, "Alice" };
    Student s2 = { 102, "Bob" };
    array_append(student_list, s1);
    array_append(student_list, s2);

    // ����ͨ�õ� print_array�����ѡ���δ�ӡѧ�����ĺ�����������
    printf("��ӡѧ��������\n");
    print_array(student_list, &print_student); // & �ǿ�ѡ�ģ���������

    destroy_array(student_list);

    printf("\n\n--- ���� Integer ��̬���� ---\n");
    // Ϊ������������ԣ�����Ҫ��ʱ�� DynamicArray.h �е�
    // typedef Student Data; �Ļ� typedef int Data; Ȼ�����±���
    /*
    DynamicArray* number_list = create_array(5);
    array_append(number_list, 10);
    array_append(number_list, 20);
    array_append(number_list, 30);

    // ͬ�������Ǹ����ܵ� print_array������ΰѡ���δ�ӡ�������ĺ�����������
    printf("��ӡ�����б�\n");
    print_array(number_list, &print_int);

    destroy_array(number_list);
    */

    return 0;
}