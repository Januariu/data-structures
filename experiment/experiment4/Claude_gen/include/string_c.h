#ifndef STRING_H
#define STRING_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * @file string.h
 * @brief 基于块链表的动态字符串库
 * 
 * 本库实现了一个高效的动态字符串数据结构，使用块链表存储字符数据。
 * 每个块固定大小(31字节)，避免频繁的内存重新分配。
 */

/**
 * @brief 字符串类型(不透明指针)
 * 
 * 实际指向内部String结构体，隐藏实现细节。
 * 用户只能通过提供的API操作字符串对象。
 */
typedef struct String* String;

/**
 * @brief 错误码枚举
 */
typedef enum {
    STR_OK = 0,              /**< 操作成功 */
    STR_ALLOC_FAILED,        /**< 内存分配失败 */
    STR_INVALID_PARAM,       /**< 无效的参数 */
    STR_OUT_OF_RANGE,        /**< 索引越界 */
    STR_NOT_FOUND            /**< 未找到目标 */
} StrError;

/* ========================================================================
 * 生命周期管理
 * ======================================================================== */

/**
 * @brief 创建一个空字符串
 * 
 * @return 成功返回字符串对象指针，失败返回NULL
 * 
 * @note 使用完毕后必须调用str_destroy释放内存
 * 
 * @code
 * String s = str_create();
 * if (s) {
 *     // 使用字符串...
 *     str_destroy(&s);
 * }
 * @endcode
 */
String str_create(void);

/**
 * @brief 从C字符串创建String对象
 * 
 * @param cstr C风格字符串(以'\0'结尾)，不能为NULL
 * @return 成功返回字符串对象指针，失败返回NULL
 * 
 * @note 会拷贝cstr的内容，原字符串可以安全释放
 * 
 * @code
 * String s = str_create_from("Hello World");
 * @endcode
 */
String str_create_from(const char *cstr);

/**
 * @brief 深拷贝字符串
 * 
 * @param s 源字符串，不能为NULL
 * @return 成功返回新字符串对象，失败返回NULL
 * 
 * @note 创建一个完全独立的副本，修改其中一个不影响另一个
 * 
 * @code
 * String s1 = str_create_from("Hello");
 * String s2 = str_clone(s1);  // s2是独立副本
 * str_append(s2, " World");   // 只修改s2
 * @endcode
 */
String str_clone(const String s);

/**
 * @brief 销毁字符串并释放内存
 * 
 * @param s 指向字符串指针的指针，不能为NULL
 * 
 * @note 销毁后会将*s设为NULL，避免悬空指针
 * @note 如果*s为NULL，则不执行任何操作
 * 
 * @code
 * String s = str_create_from("Hello");
 * str_destroy(&s);  // s自动变成NULL
 * @endcode
 */
void str_destroy(String *s);

/* ========================================================================
 * 基本属性
 * ======================================================================== */

/**
 * @brief 获取字符串长度
 * 
 * @param s 字符串对象
 * @return 字符串中的字符数量(不包括'\0')，如果s为NULL返回0
 * 
 * @note 时间复杂度: O(1)
 * 
 * @code
 * String s = str_create_from("Hello");
 * printf("Length: %zu\n", str_length(s));  // 输出: Length: 5
 * @endcode
 */
size_t str_length(const String s);

/**
 * @brief 判断字符串是否为空
 * 
 * @param s 字符串对象
 * @return true=空字符串或NULL，false=非空
 * 
 * @code
 * String s = str_create();
 * if (str_empty(s)) {
 *     printf("String is empty\n");
 * }
 * @endcode
 */
bool str_empty(const String s);

/**
 * @brief 获取字符串容量(可选实现)
 * 
 * @param s 字符串对象
 * @return 当前已分配的总容量(字节数)
 * 
 * @note 容量 = 块数 × 块大小
 */
size_t str_capacity(const String s);

/* ========================================================================
 * 赋值和拷贝
 * ======================================================================== */

/**
 * @brief 用C字符串赋值
 * 
 * @param s 目标字符串，不能为NULL
 * @param cstr C风格字符串，不能为NULL
 * @return 成功返回true，失败返回false
 * 
 * @note 会清空原内容，重新分配空间
 * 
 * @code
 * String s = str_create();
 * str_assign(s, "Hello World");
 * @endcode
 */
bool str_assign(String s, const char *cstr);

/**
 * @brief 用另一个String对象赋值
 * 
 * @param s 目标字符串，不能为NULL
 * @param src 源字符串，不能为NULL
 * @return 成功返回true，失败返回false
 * 
 * @note 如果s==src，直接返回true
 * 
 * @code
 * String s1 = str_create_from("Hello");
 * String s2 = str_create();
 * str_assign_str(s2, s1);  // s2变成"Hello"
 * @endcode
 */
bool str_assign_str(String s, const String src);

/**
 * @brief 拷贝字符串(等同于str_assign_str)
 * 
 * @param dest 目标字符串，不能为NULL
 * @param src 源字符串，不能为NULL
 * @return 成功返回true，失败返回false
 * 
 * @note 这是str_assign_str的别名，提供更直观的命名
 */
bool str_copy(String dest, const String src);

/* ========================================================================
 * 访问
 * ======================================================================== */

/**
 * @brief 访问指定位置的字符
 * 
 * @param s 字符串对象
 * @param index 字符索引(从0开始)
 * @return 指定位置的字符，越界或s为NULL时返回'\0'
 * 
 * @note 时间复杂度: O(n)，需要遍历块链表
 * @warning 频繁随机访问效率较低，建议使用str_c_str()获取整个字符串
 * 
 * @code
 * String s = str_create_from("Hello");
 * char c = str_at(s, 0);  // c = 'H'
 * @endcode
 */
char str_at(const String s, size_t index);

/**
 * @brief 获取C风格字符串(只读)
 * 
 * @param s 字符串对象
 * @return 指向以'\0'结尾的C字符串的指针，s为NULL或空时返回""
 * 
 * @warning 返回的指针在下次修改操作后可能失效，不要长期持有
 * @warning 不要修改返回的字符串内容
 * 
 * @note 使用缓存机制，重复调用很快(O(1))
 * @note 修改字符串后缓存会自动失效并重建
 * 
 * @code
 * String s = str_create_from("Hello");
 * printf("%s\n", str_c_str(s));  // 输出: Hello
 * 
 * // 错误用法:
 * const char *p = str_c_str(s);
 * str_append(s, " World");  // p已失效!
 * printf("%s\n", p);        // 危险!
 * @endcode
 */
const char* str_c_str(const String s);

/* ========================================================================
 * 比较
 * ======================================================================== */

/**
 * @brief 字典序比较两个字符串
 * 
 * @param s1 第一个字符串
 * @param s2 第二个字符串
 * @return 
 *   - < 0: s1 < s2
 *   - = 0: s1 == s2
 *   - > 0: s1 > s2
 * 
 * @note 如果两个都为NULL，返回0
 * @note 如果只有一个为NULL，NULL < 非NULL
 * 
 * @code
 * String s1 = str_create_from("apple");
 * String s2 = str_create_from("banana");
 * if (str_compare(s1, s2) < 0) {
 *     printf("apple comes before banana\n");
 * }
 * @endcode
 */
int str_compare(const String s1, const String s2);

/**
 * @brief 判断两个字符串是否相等
 * 
 * @param s1 第一个字符串
 * @param s2 第二个字符串
 * @return true=相等，false=不相等
 * 
 * @note 等价于 str_compare(s1, s2) == 0
 * 
 * @code
 * String s1 = str_create_from("Hello");
 * String s2 = str_create_from("Hello");
 * if (str_equals(s1, s2)) {
 *     printf("Strings are equal\n");
 * }
 * @endcode
 */
bool str_equals(const String s1, const String s2);

/* ========================================================================
 * 修改操作
 * ======================================================================== */

/**
 * @brief 清空字符串
 * 
 * @param s 字符串对象
 * 
 * @note 释放所有块，长度变为0
 * @note 对象本身不会被销毁，可以继续使用
 * 
 * @code
 * String s = str_create_from("Hello");
 * str_clear(s);  // 现在是空字符串
 * str_append(s, "World");  // 可以重新使用
 * @endcode
 */
void str_clear(String s);

/**
 * @brief 追加C字符串到末尾
 * 
 * @param s 目标字符串，不能为NULL
 * @param cstr 要追加的C字符串，不能为NULL
 * @return 成功返回true，失败返回false
 * 
 * @code
 * String s = str_create_from("Hello");
 * str_append(s, " World");  // s变成"Hello World"
 * @endcode
 */
bool str_append(String s, const char *cstr);

/**
 * @brief 追加另一个String对象到末尾
 * 
 * @param s 目标字符串，不能为NULL
 * @param other 要追加的字符串，不能为NULL
 * @return 成功返回true，失败返回false
 * 
 * @code
 * String s1 = str_create_from("Hello");
 * String s2 = str_create_from(" World");
 * str_append_str(s1, s2);  // s1变成"Hello World"
 * @endcode
 */
bool str_append_str(String s, const String other);

/**
 * @brief 追加单个字符到末尾
 * 
 * @param s 字符串对象，不能为NULL
 * @param c 要追加的字符
 * @return 成功返回true，失败返回false
 * 
 * @note 这是最高效的追加方式，时间复杂度O(1)
 * 
 * @code
 * String s = str_create_from("Hello");
 * str_push_back(s, '!');  // s变成"Hello!"
 * @endcode
 */
bool str_push_back(String s, char c);

/**
 * @brief 在指定位置插入字符串
 * 
 * @param s 目标字符串，不能为NULL
 * @param pos 插入位置(0 <= pos <= length)
 * @param t 要插入的字符串，不能为NULL
 * @return 成功返回true，失败返回false
 * 
 * @note pos=0表示插入到开头，pos=length表示插入到末尾
 * @note 时间复杂度: O(n)
 * 
 * @code
 * String s = str_create_from("Hello World");
 * String insert = str_create_from("Beautiful ");
 * str_insert(s, 6, insert);  // s变成"Hello Beautiful World"
 * @endcode
 */
bool str_insert(String s, size_t pos, const String t);

/**
 * @brief 删除指定位置和长度的子串
 * 
 * @param s 字符串对象，不能为NULL
 * @param pos 起始位置(0 <= pos < length)
 * @param len 要删除的字符数
 * @return 成功返回true，失败返回false
 * 
 * @note 如果pos+len超过字符串长度，删除到末尾
 * @note 时间复杂度: O(n)
 * 
 * @code
 * String s = str_create_from("Hello World");
 * str_delete(s, 5, 6);  // s变成"Hello"
 * @endcode
 */
bool str_delete(String s, size_t pos, size_t len);

/* ========================================================================
 * 字符串操作
 * ======================================================================== */

/**
 * @brief 拼接两个字符串
 * 
 * @param result 结果字符串(会被清空)，不能为NULL
 * @param s1 第一个字符串，不能为NULL
 * @param s2 第二个字符串，不能为NULL
 * @return 成功返回true，失败返回false
 * 
 * @note result = s1 + s2
 * @note result可以是s1或s2本身
 * 
 * @code
 * String s1 = str_create_from("Hello");
 * String s2 = str_create_from(" World");
 * String result = str_create();
 * str_concat(result, s1, s2);  // result = "Hello World"
 * @endcode
 */
bool str_concat(String result, const String s1, const String s2);

/**
 * @brief 提取子串
 * 
 * @param sub 结果字符串(会被清空)，不能为NULL
 * @param s 源字符串，不能为NULL
 * @param pos 起始位置(0 <= pos < length)
 * @param len 子串长度
 * @return 成功返回true，失败返回false
 * 
 * @note 提取区间: [pos, pos+len)
 * @note 如果pos+len超过长度，提取到末尾
 * 
 * @code
 * String s = str_create_from("Hello World");
 * String sub = str_create();
 * str_substring(sub, s, 0, 5);  // sub = "Hello"
 * @endcode
 */
bool str_substring(String sub, const String s, size_t pos, size_t len);

/**
 * @brief 查找子串首次出现的位置
 * 
 * @param s 源字符串
 * @param pattern 要查找的模式串
 * @param start_pos 开始查找的位置
 * @return 找到返回位置索引(>=0)，未找到返回-1
 * 
 * @note 使用朴素字符串匹配算法，时间复杂度O(n*m)
 * 
 * @code
 * String s = str_create_from("Hello World");
 * String pattern = str_create_from("World");
 * int pos = str_find(s, pattern, 0);  // pos = 6
 * @endcode
 */
int str_find(const String s, const String pattern, size_t start_pos);

/**
 * @brief 查找字符首次出现的位置
 * 
 * @param s 源字符串
 * @param c 要查找的字符
 * @param start_pos 开始查找的位置
 * @return 找到返回位置索引(>=0)，未找到返回-1
 * 
 * @code
 * String s = str_create_from("Hello World");
 * int pos = str_find_char(s, 'W', 0);  // pos = 6
 * @endcode
 */
int str_find_char(const String s, char c, size_t start_pos);

/**
 * @brief 替换第一次出现的子串
 * 
 * @param s 目标字符串，不能为NULL
 * @param old_str 要被替换的子串，不能为NULL
 * @param new_str 新的子串，不能为NULL
 * @return 成功替换返回true，未找到或失败返回false
 * 
 * @code
 * String s = str_create_from("foo bar foo");
 * String old = str_create_from("foo");
 * String new = str_create_from("qux");
 * str_replace_first(s, old, new);  // s变成"qux bar foo"
 * @endcode
 */
bool str_replace_first(String s, const String old_str, const String new_str);

/**
 * @brief 替换所有出现的子串
 * 
 * @param s 目标字符串，不能为NULL
 * @param old_str 要被替换的子串，不能为NULL且长度>0
 * @param new_str 新的子串，不能为NULL
 * @return 返回替换的次数(>=0)
 * 
 * @note 如果old_str长度为0，返回0
 * 
 * @code
 * String s = str_create_from("foo bar foo baz foo");
 * String old = str_create_from("foo");
 * String new = str_create_from("qux");
 * int count = str_replace_all(s, old, new);  
 * // s变成"qux bar qux baz qux", count=3
 * @endcode
 */
int str_replace_all(String s, const String old_str, const String new_str);

/* ========================================================================
 * 输出
 * ======================================================================== */

/**
 * @brief 打印字符串到文件流
 * 
 * @param s 字符串对象
 * @param fp 文件指针(如stdout, stderr或文件)
 * 
 * @note 不会打印换行符
 * 
 * @code
 * String s = str_create_from("Hello World");
 * str_print(s, stdout);  // 输出到控制台
 * @endcode
 */
void str_print(const String s, FILE *fp);

/**
 * @brief 打印字符串到文件流并换行
 * 
 * @param s 字符串对象
 * @param fp 文件指针
 * 
 * @note 等价于 str_print(s, fp) + fprintf(fp, "\n")
 * 
 * @code
 * String s = str_create_from("Hello World");
 * str_println(s, stdout);  // 输出: Hello World\n
 * @endcode
 */
void str_println(const String s, FILE *fp);

/* ========================================================================
 * 错误处理
 * ======================================================================== */

/**
 * @brief 获取错误码对应的描述信息
 * 
 * @param error 错误码
 * @return 错误描述字符串(静态字符串，不需要释放)
 * 
 * @code
 * StrError err = STR_ALLOC_FAILED;
 * printf("Error: %s\n", str_error_message(err));
 * // 输出: Error: Memory allocation failed
 * @endcode
 */
const char* str_error_message(StrError error);

#endif // STRING_H