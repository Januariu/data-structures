#ifndef STRING_H
#define STRING_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * @file string.h
 * @brief Dynamic string library based on block-linked list
 *
 * This library implements an efficient dynamic string data structure using
 * a block-linked list for storage. Each block has a fixed size (31 bytes),
 * avoiding frequent memory reallocations.
 */

typedef struct String *String;

typedef enum
{
    STR_OK = 0,        /**< 操作成功 */
    STR_ALLOC_FAILED,  /**< 内存分配失败 */
    STR_INVALID_PARAM, /**< 无效的参数 */
    STR_OUT_OF_RANGE,  /**< 索引越界 */
    STR_NOT_FOUND      /**< 未找到目标 */
} StrError;

/* ========================================================================
 * Lifecycle Management
 * ======================================================================== */

/**
 * @brief Create an empty string
 *
 * @return Pointer to string object on success, NULL on failure
 *
 * @note Must call str_destroy() to free memory when done
 *
 * @code
 * String s = str_create();
 * if (s) {
 *     // Use the string...
 *     str_destroy(&s);
 * }
 * @endcode
 */
String str_create(void);

/**
 * @brief Create a String object from a C string
 *
 * @param cstr C-style string (null-terminated), must not be NULL
 * @return Pointer to string object on success, NULL on failure
 *
 * @note Copies the content of cstr, original string can be safely freed
 *
 * @code
 * String s = str_create_from("Hello World");
 * @endcode
 */
String str_create_from(const char *cstr);

/**
 * @brief Destroy string and free memory
 *
 * @param s Pointer to string pointer, must not be NULL
 *
 * @note After destruction, *s will be set to NULL to avoid dangling pointers
 * @note If *s is NULL, no operation is performed
 *
 * @code
 * String s = str_create_from("Hello");
 * str_destroy(&s);  // s automatically becomes NULL
 * @endcode
 */
void str_destroy(String *s);

/* ========================================================================
 * Basic Properties
 * ======================================================================== */

/**
 * @brief Get string length
 *
 * @param s String object
 * @return Number of characters in the string (excluding '\0'), returns 0 if s is NULL
 *
 * @note Time complexity: O(1)
 *
 * @code
 * String s = str_create_from("Hello");
 * printf("Length: %zu\n", str_length(s));  // Output: Length: 5
 * @endcode
 */
size_t str_length(const String s);

/**
 * @brief Check if string is empty
 *
 * @param s String object
 * @return true if empty or NULL, false if non-empty
 *
 * @code
 * String s = str_create();
 * if (str_empty(s)) {
 *     printf("String is empty\n");
 * }
 * @endcode
 */
bool str_empty(const String s);

/* ========================================================================
 * Access
 * ======================================================================== */

/**
 * @brief Access character at specified position
 *
 * @param s String object
 * @param index Character index (0-based)
 * @return Character at specified position, returns '\0' if out of bounds or s is NULL
 *
 * @note Time complexity: O(n), requires traversing block list
 * @warning Low efficiency for frequent random access, recommend using str_c_str() to get entire string
 *
 * @code
 * String s = str_create_from("Hello");
 * char c = str_at(s, 0);  // c = 'H'
 * @endcode
 */
char str_at(const String s, size_t index);

/* ========================================================================
 * Modification Operations
 * ======================================================================== */

/**
 * @brief Clear the string
 *
 * @param s String object
 *
 * @note Frees all blocks, length becomes 0
 * @note Object itself is not destroyed and can continue to be used
 *
 * @code
 * String s = str_create_from("Hello");
 * str_clear(s);  // Now an empty string
 * str_append(s, "World");  // Can be reused
 * @endcode
 */
void str_clear(String s);

/**
 * @brief Append another String object to end
 *
 * @param s Target string, must not be NULL
 * @param other String to append, must not be NULL
 * @return true on success, false on failure
 *
 * @code
 * String s1 = str_create_from("Hello");
 * String s2 = str_create_from(" World");
 * str_append_str(s1, s2);  // s1 becomes "Hello World"
 * @endcode
 */
bool str_append_str(String s, const String other);

/**
 * @brief Append single character to end
 *
 * @param s String object, must not be NULL
 * @param c Character to append
 * @return true on success, false on failure
 *
 * @note This is the most efficient append method, time complexity O(1)
 *
 * @code
 * String s = str_create_from("Hello");
 * str_push_back(s, '!');  // s becomes "Hello!"
 * @endcode
 */
bool str_push_back(String s, char c);

/**
 * @brief Insert string at specified position
 *
 * @param s Target string, must not be NULL
 * @param pos Insert position (0 <= pos <= length)
 * @param t String to insert, must not be NULL
 * @return true on success, false on failure
 *
 * @note pos=0 means insert at beginning, pos=length means insert at end
 * @note Time complexity: O(n)
 *
 * @code
 * String s = str_create_from("Hello World");
 * String insert = str_create_from("Beautiful ");
 * str_insert(s, 6, insert);  // s becomes "Hello Beautiful World"
 * @endcode
 */
bool str_insert(String s, size_t pos, const String t);

/**
 * @brief Insert a single character at specified position
 *
 * @param s String object, must not be NULL
 * @param pos Insert position (0 <= pos <= length)
 * @param c Character to insert
 * @return true on success, false on failure
 *
 * @note pos=0 means insert at beginning, pos=length means append to end
 * @note Time complexity: O(n)
 *
 * @code
 * String s = str_create_from("Helo");
 * str_insert_char(s, 3, 'l');  // s becomes "Hello"
 * @endcode
 */
bool str_insert_char(String s, size_t pos, char c);

/**
 * @brief Delete substring at specified position and length
 *
 * @param s String object, must not be NULL
 * @param pos Start position (0 <= pos < length)
 * @param len Number of characters to delete
 * @return true on success, false on failure
 *
 * @note If pos+len exceeds string length, deletes to end
 * @note Time complexity: O(n)
 *
 * @code
 * String s = str_create_from("Hello World");
 * str_delete(s, 5, 6);  // s becomes "Hello"
 * @endcode
 */
bool str_delete(String s, size_t pos, size_t len);

/* ========================================================================
 * String Operations
 * ======================================================================== */

/**
 * @brief Concatenate two strings
 *
 * @param result Result string (will be cleared), must not be NULL
 * @param s1 First string, must not be NULL
 * @param s2 Second string, must not be NULL
 * @return true on success, false on failure
 *
 * @note result = s1 + s2
 * @note result can be s1 or s2 itself
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
 * @brief Extract substring
 *
 * @param sub Result string (will be cleared), must not be NULL
 * @param s Source string, must not be NULL
 * @param pos Start position (0 <= pos < length)
 * @param len Substring length
 * @return true on success, false on failure
 *
 * @note Extracts range: [pos, pos+len)
 * @note If pos+len exceeds length, extracts to end
 *
 * @code
 * String s = str_create_from("Hello World");
 * String sub = str_create();
 * str_substring(sub, s, 0, 5);  // sub = "Hello"
 * @endcode
 */
bool str_substring(String sub, const String s, size_t pos, size_t len);

/**
 * @brief Find first occurrence of substring
 *
 * @param s Source string
 * @param pattern Pattern string to search for
 * @param start_pos Position to start searching from
 * @return Position index (>=0) if found, -1 if not found
 *
 * @note Uses naive string matching algorithm, time complexity O(n*m)
 *
 * @code
 * String s = str_create_from("Hello World");
 * String pattern = str_create_from("World");
 * int pos = str_find(s, pattern, 0);  // pos = 6
 * @endcode
 */
int str_find_first(const String s, const String pattern, size_t start_pos);

/**
 * @brief Find all occurrences of substring
 *
 * @param s Source string
 * @param pattern Pattern string to search for
 * @param start_pos Position to start searching from
 * @return Number of occurrences found (>=0)
 *
 * @note Uses naive string matching algorithm, time complexity O(n*m)
 *
 * @code
 * String s = str_create_from("ababcabc");
 * String pattern = str_create_from("abc");
 * int count = str_find_all(s, pattern, 0);  // count = 2
 * @endcode
 */
int *str_find_all(const String s, const String pattern, size_t start_pos);

/**
 * @brief Find first occurrence of character
 *
 * @param s Source string
 * @param c Character to search for
 * @param start_pos Position to start searching from
 * @return Position index (>=0) if found, -1 if not found
 *
 * @code
 * String s = str_create_from("Hello World");
 * int pos = str_find_char(s, 'W', 0);  // pos = 6
 * @endcode
 */
int str_find_char(const String s, char c, size_t start_pos);

/**
 * @brief Replace first occurrence of substring
 *
 * @param s Target string, must not be NULL
 * @param old_str Substring to be replaced, must not be NULL
 * @param new_str New substring, must not be NULL
 * @return true if replaced successfully, false if not found or failed
 *
 * @code
 * String s = str_create_from("foo bar foo");
 * String old = str_create_from("foo");
 * String new = str_create_from("qux");
 * str_replace_first(s, old, new);  // s becomes "qux bar foo"
 * @endcode
 */
bool str_replace_first(String s, const String old_str, const String new_str);

/**
 * @brief Replace all occurrences of substring
 *
 * @param s Target string, must not be NULL
 * @param old_str Substring to be replaced, must not be NULL and length > 0
 * @param new_str New substring, must not be NULL
 * @return Number of replacements made (>=0)
 *
 * @note If old_str length is 0, returns 0
 *
 * @code
 * String s = str_create_from("foo bar foo baz foo");
 * String old = str_create_from("foo");
 * String new = str_create_from("qux");
 * int count = str_replace_all(s, old, new);
 * // s becomes "qux bar qux baz qux", count=3
 * @endcode
 */
int str_replace_all(String s, const String old_str, const String new_str);

/* ========================================================================
 * Output
 * ======================================================================== */

/**
 * @brief Print string to file stream
 *
 * @param s String object
 * @param fp File pointer (such as stdout, stderr or a file)
 *
 * @note Does not print newline character
 *
 * @code
 * String s = str_create_from("Hello World");
 * str_print(s, stdout);  // Output to console
 * @endcode
 */
void str_print(const String s, FILE *fp);

/**
 * @brief Print string to file stream with newline
 *
 * @param s String object
 * @param fp File pointer
 *
 * @note Equivalent to str_print(s, fp) + fprintf(fp, "\n")
 *
 * @code
 * String s = str_create_from("Hello World");
 * str_println(s, stdout);  // Output: Hello World\n
 * @endcode
 */
void str_println(const String s, FILE *fp);

void str_print_per_block(const String s, FILE *fp);

/* ========================================================================
 * Error Handling
 * ======================================================================== */

/**
 * @brief Get error message for error code
 *
 * @param error Error code
 * @return Error description string (static string, no need to free)
 *
 * @code
 * StrError err = STR_ALLOC_FAILED;
 * printf("Error: %s\n", str_error_message(err));
 * // Output: Error: Memory allocation failed
 * @endcode
 */
const char *str_error_message(StrError error);

#endif // STRING_H