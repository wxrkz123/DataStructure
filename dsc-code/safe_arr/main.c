
// main.c

#include "safe_array.h"
#include <stdio.h>

void plain_c_array_example() {
    printf("--- 1. ��ͨC�����Σ���� ---\n");
    int normal_array[5] = { 10, 20, 30, 40, 50 };

    printf("���Զ�ȡԽ������ 10...\n");
    // ���д����ڱ���ʱ���ܲ��ᱨ��������ʱ���ȡ��δ֪���ڴ�����ֵ��
    // ����ֱ�ӵ��³������������ǡ�����ȫ����
    int garbage = normal_array[10];
    printf("��ȡ��������ֵ: %d (����δ������Ϊ!)\n", garbage);

	// update the value at index 2
	normal_array[2] = 99; // ����д��

   

    // MyIntArr* myarr = normal_array_create(5);
    
	// bool res = update_arr(myarr, 2, 99);
    // ��װ
    printf("����д��Խ������ -5...\n");
    // ��ȶ�ȡ��Σ�գ�������Ⱦ�����������ֵ��ڴ棬��������׷�ٵ�bug��
    // normal_array[-5] = 999; // (ȡ��ע�����п��ܻᵼ�³������)
    printf("д������������ƻ����ڴ棡\n\n");


	printf("��ͨC�����ʹ����Σ�յģ���Ϊ��û���κΰ�ȫ��顣\n");

    
}

void safe_array_example() {
    printf("--- 2. ʹ�����ǵġ���ȫ���顱 ---\n");

	// SafeArray my_array = { .data = NULL, .capacity = 0 };

    // API ����ԭ��
    // C���Ժ����� Cԭ��


    // ����һ������Ϊ5�İ�ȫ����
    SafeArray* sa = sarray_create(5);
    if (!sa) {
        printf("������ȫ����ʧ�ܡ�\n");
        return;
    }
    printf("��ȫ���鴴���ɹ���\n");
    sarray_print(sa); // ��ʼӦΪ0�������ֵ��ȡ����ϵͳ��




    // ��ȫ������ֵ
    printf("\n��ȫ������ֵ...\n");
    for (size_t i = 0; i < sarray_get_capacity(sa); i++) {
        sarray_set(sa, i, (i + 1) * 11);
    }
    sarray_print(sa);

    // ���԰�ȫ��д��Խ������
    printf("\n���԰�ȫ��д��Խ������ 10...\n");
    if (sarray_set(sa, 10, 999)) {
        printf("д��ɹ� (��Ӧ����!)\n");
    }
    else {
        printf("д��ʧ�ܣ���Ϊ����Խ�硣����ȫ��\n");
    }
    sarray_print(sa); // ��������û�б��ƻ�

    // ��ȫ�ػ�ȡֵ
    printf("\n��ȫ�ػ�ȡֵ...\n");
    int value;
    if (sarray_get(sa, 3, &value)) {
        printf("���� 3 ��ֵΪ: %d\n", value);
    }

    // ���԰�ȫ�ض�ȡԽ������
    printf("\n���԰�ȫ�ض�ȡԽ������ 5...\n");

    // SafeArray* sa = sarray_get(xxx);

    // sa->data = "123";


    if (sarray_get(sa, 5, &value)) {
        printf("��ȡ����ֵ: %d (��Ӧ����!)\n", value);
    }
    else {
        printf("��ȡʧ�ܣ���Ϊ����Խ�硣����ȫ��\n");
    }

    sarray_destroy(&sa);
    printf("\n��ȫ���������١�\n");
}

int main() {
    plain_c_array_example();
    safe_array_example();

    return 0;
}