#ifndef STRING_C_H
#define STRING_C_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct String* String;

typedef enum {
    STR_OK = 0,
    STR_ALLOC_FAILED,
    STR_INVALID_PARAM,
    STR_OUT_OF_RANGE,
    STR_NOT_FOUND
} StrError;

// 生命周期管理
String str_create(void);
String str_create_from(const char *cstr);
String str_clone(const String s);
void str_destroy(String *s);

// 基本属性
size_t str_length(const String s);
bool str_empty(const String s);

// 赋值和拷贝
bool str_assign(String s, const char *cstr);
bool str_assign_str(String s, const String src);
bool str_copy(String dest, const String src);

// 访问
char str_at(const String s, size_t index);
const char* str_c_str(const String s);

// 比较
int str_compare(const String s1, const String s2);
bool str_equals(const String s1, const String s2);

// 修改操作
void str_clear(String s);
bool str_append(String s, const char *cstr);
bool str_append_str(String s, const String other);
bool str_push_back(String s, char c);
bool str_insert(String s, size_t pos, const String t);
bool str_delete(String s, size_t pos, size_t len);

// 字符串操作
bool str_concat(String result, const String s1, const String s2);
bool str_substring(String sub, const String s, size_t pos, size_t len);
int str_find(const String s, const String pattern, size_t start_pos);
int str_find_char(const String s, char c, size_t start_pos);
bool str_replace_first(String s, const String old_str, const String new_str);
int str_replace_all(String s, const String old_str, const String new_str);

// 输出
void str_print(const String s, FILE *fp);
void str_println(const String s, FILE *fp);

// 错误处理
const char* str_error_message(StrError error);

#endif // STRING_H