#include "string_c.h"

int main(void) {

    printf("=== String Library Test ===\n\n");
    
    // 测试1: 创建和基本操作
    printf("Test 1: Create and basic operations\n");
    String s1 = str_create_from("Hello");
    printf("s1: \"%s\", length: %zu\n", str_c_str(s1), str_length(s1));
    
    str_append(s1, " World");
    printf("After append: \"%s\"\n", str_c_str(s1));
    
    str_push_back(s1, '!');
    printf("After push_back: \"%s\"\n\n", str_c_str(s1));
    
    // 测试2: 拷贝和比较
    printf("Test 2: Copy and compare\n");
    String s2 = str_clone(s1);
    printf("s2 (clone): \"%s\"\n", str_c_str(s2));
    printf("s1 == s2: %s\n", str_equals(s1, s2) ? "true" : "false");
    
    str_append(s2, " Extra");
    printf("After modify s2: \"%s\"\n", str_c_str(s2));
    printf("s1 vs s2: %d\n\n", str_compare(s1, s2));
    
    // 测试3: 子串和查找
    printf("Test 3: Substring and find\n");
    String sub = str_create();
    str_substring(sub, s1, 0, 5);
    printf("Substring(0, 5): \"%s\"\n", str_c_str(sub));
    
    int pos = str_find(s1, sub, 0);
    printf("Find \"Hello\" at position: %d\n", pos);
    
    int char_pos = str_find_char(s1, 'W', 0);
    printf("Find 'W' at position: %d\n\n", char_pos);
    
    // 测试4: 插入和删除
    printf("Test 4: Insert and delete\n");
    String s3 = str_create_from("Hello World");
    printf("Original: \"%s\"\n", str_c_str(s3));
    
    String insert_str = str_create_from(" Beautiful");
    str_insert(s3, 5, insert_str);
    printf("After insert: \"%s\"\n", str_c_str(s3));
    
    str_delete(s3, 5, 10);
    printf("After delete: \"%s\"\n\n", str_c_str(s3));
    
    // 测试5: 替换
    printf("Test 5: Replace\n");
    String s4 = str_create_from("foo bar foo baz foo");
    printf("Original: \"%s\"\n", str_c_str(s4));
    
    String old = str_create_from("foo");
    String new = str_create_from("qux");
    
    int replaced = str_replace_all(s4, old, new);
    printf("After replace all 'foo' with 'qux': \"%s\"\n", str_c_str(s4));
    printf("Replaced count: %d\n\n", replaced);
    
    // 测试6: 长字符串(测试块链表)
    printf("Test 6: Long string (block chain test)\n");
    String s5 = str_create();
    for (int i = 0; i < 100; i++) {
        str_append(s5, "0123456789");
    }
    printf("Created string with length: %zu\n", str_length(s5));
    printf("First 50 chars: %.50s...\n", str_c_str(s5));
    printf("Last 20 chars: ...%s\n\n", str_c_str(s5) + str_length(s5) - 20);
    
    // 清理
    str_destroy(&s1);
    str_destroy(&s2);
    str_destroy(&s3);
    str_destroy(&s4);
    str_destroy(&s5);
    str_destroy(&sub);
    str_destroy(&insert_str);
    str_destroy(&old);
    str_destroy(&new);
    
    printf("All tests completed!\n");
    return 0;
}