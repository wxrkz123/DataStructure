#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // 需要包含 string.h 来使用 strcpy

#include "DynamicArray.h"

// 定义我们自己的回调函数
// 1. 这是一个知道如何打印Student的函数
// 它的函数签名必须和函数指针的定义完全匹配
void print_student(const void* data) {
    // 接收到的data是一个void*指针，我们需要将它转换为我们真正的类型
    const Student* s_ptr = (const Student*)data;
    printf("Student: {id: %d, name: \"%s\"}", s_ptr->id, s_ptr->name);
}


int main() {
    printf("--- 测试 Student 动态数组 ---\n");
    DynamicArray* student_list = create_array(2);

    Student s1 = { 101, "Alice" };
    Student s2 = { 102, "Bob" };
    array_append(student_list, s1);
    array_append(student_list, s2);

    // 调用通用的 print_array，并把“如何打印学生”的函数传给它！
    printf("打印学生名单：\n");
    print_array(student_list, &print_student); // & 是可选的，但更清晰

    destroy_array(student_list);

    printf("\n\n--- 测试 Integer 动态数组 ---\n");
    // 为了运行这个测试，你需要临时将 DynamicArray.h 中的
    // typedef Student Data; 改回 typedef int Data; 然后重新编译
    /*
    DynamicArray* number_list = create_array(5);
    array_append(number_list, 10);
    array_append(number_list, 20);
    array_append(number_list, 30);

    // 同样调用那个万能的 print_array，但这次把“如何打印整数”的函数传给它！
    printf("打印数字列表：\n");
    print_array(number_list, &print_int);

    destroy_array(number_list);
    */

    return 0;
}