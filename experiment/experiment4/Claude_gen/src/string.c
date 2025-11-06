#include "string_c.h"
#include <string.h>

#define BLOCK_SIZE 31

// 块结构
typedef struct Block {
    struct Block *next;
    unsigned char size;
    char data[BLOCK_SIZE];
} Block;

// 字符串结构
struct String {
    Block *head;
    Block *tail;
    size_t length;
    char *cached_cstr;  // 缓存的C字符串
};

// ==================== 辅助函数 ====================

static Block* block_create(void) {
    Block *b = (Block*)malloc(sizeof(Block));
    if (b) {
        b->next = NULL;
        b->size = 0;
    }
    return b;
}

static void block_destroy_all(Block *head) {
    while (head) {
        Block *temp = head;
        head = head->next;
        free(temp);
    }
}

static void invalidate_cache(String s) {
    if (s && s->cached_cstr) {
        free(s->cached_cstr);
        s->cached_cstr = NULL;
    }
}

// ==================== 生命周期管理 ====================

String str_create(void) {
    String s = (String)malloc(sizeof(struct String));
    if (!s) return NULL;
    
    s->head = s->tail = NULL;
    s->length = 0;
    s->cached_cstr = NULL;
    return s;
}

String str_create_from(const char *cstr) {
    if (!cstr) return NULL;
    
    String s = str_create();
    if (!s) return NULL;
    
    if (!str_assign(s, cstr)) {
        str_destroy(&s);
        return NULL;
    }
    return s;
}

String str_clone(const String s) {
    if (!s) return NULL;
    
    String clone = str_create();
    if (!clone) return NULL;
    
    Block *curr = s->head;
    while (curr) {
        Block *new_block = block_create();
        if (!new_block) {
            str_destroy(&clone);
            return NULL;
        }
        
        new_block->size = curr->size;
        memcpy(new_block->data, curr->data, curr->size);
        
        if (!clone->head) {
            clone->head = clone->tail = new_block;
        } else {
            clone->tail->next = new_block;
            clone->tail = new_block;
        }
        
        curr = curr->next;
    }
    
    clone->length = s->length;
    return clone;
}

void str_destroy(String *s) {
    if (!s || !*s) return;
    
    block_destroy_all((*s)->head);
    invalidate_cache(*s);
    free(*s);
    *s = NULL;
}

// ==================== 基本属性 ====================

size_t str_length(const String s) {
    return s ? s->length : 0;
}

bool str_empty(const String s) {
    return !s || s->length == 0;
}

// ==================== 赋值和拷贝 ====================

bool str_assign(String s, const char *cstr) {
    if (!s || !cstr) return false;
    
    str_clear(s);
    
    size_t len = strlen(cstr);
    if (len == 0) return true;
    
    for (size_t i = 0; i < len; i++) {
        if (!str_push_back(s, cstr[i])) {
            return false;
        }
    }
    
    return true;
}

bool str_assign_str(String s, const String src) {
    if (!s || !src) return false;
    if (s == src) return true;
    
    str_clear(s);
    return str_append_str(s, src);
}

bool str_copy(String dest, const String src) {
    return str_assign_str(dest, src);
}

// ==================== 访问 ====================

char str_at(const String s, size_t index) {
    if (!s || index >= s->length) return '\0';
    
    Block *curr = s->head;
    size_t pos = 0;
    
    while (curr) {
        if (pos + curr->size > index) {
            return curr->data[index - pos];
        }
        pos += curr->size;
        curr = curr->next;
    }
    
    return '\0';
}

const char* str_c_str(const String s) {
    if (!s) return "";
    if (s->length == 0) return "";
    
    if (s->cached_cstr) {
        return s->cached_cstr;
    }
    
    s->cached_cstr = (char*)malloc(s->length + 1);
    if (!s->cached_cstr) return "";
    
    size_t pos = 0;
    Block *curr = s->head;
    while (curr) {
        memcpy(s->cached_cstr + pos, curr->data, curr->size);
        pos += curr->size;
        curr = curr->next;
    }
    s->cached_cstr[s->length] = '\0';
    
    return s->cached_cstr;
}

// ==================== 比较 ====================

int str_compare(const String s1, const String s2) {
    if (!s1 && !s2) return 0;
    if (!s1) return -1;
    if (!s2) return 1;
    
    Block *b1 = s1->head;
    Block *b2 = s2->head;
    size_t i1 = 0, i2 = 0;
    
    while (b1 && b2) {
        char c1 = b1->data[i1];
        char c2 = b2->data[i2];
        
        if (c1 != c2) {
            return (unsigned char)c1 - (unsigned char)c2;
        }
        
        i1++;
        i2++;
        
        if (i1 >= b1->size) {
            b1 = b1->next;
            i1 = 0;
        }
        if (i2 >= b2->size) {
            b2 = b2->next;
            i2 = 0;
        }
    }
    
    if (s1->length < s2->length) return -1;
    if (s1->length > s2->length) return 1;
    return 0;
}

bool str_equals(const String s1, const String s2) {
    return str_compare(s1, s2) == 0;
}

// ==================== 修改操作 ====================

void str_clear(String s) {
    if (!s) return;
    
    block_destroy_all(s->head);
    s->head = s->tail = NULL;
    s->length = 0;
    invalidate_cache(s);
}

bool str_push_back(String s, char c) {
    if (!s) return false;
    
    invalidate_cache(s);
    
    if (!s->tail || s->tail->size >= BLOCK_SIZE) {
        Block *new_block = block_create();
        if (!new_block) return false;
        
        if (!s->head) {
            s->head = s->tail = new_block;
        } else {
            s->tail->next = new_block;
            s->tail = new_block;
        }
    }
    
    s->tail->data[s->tail->size++] = c;
    s->length++;
    return true;
}

bool str_append(String s, const char *cstr) {
    if (!s || !cstr) return false;
    
    size_t len = strlen(cstr);
    for (size_t i = 0; i < len; i++) {
        if (!str_push_back(s, cstr[i])) {
            return false;
        }
    }
    return true;
}

bool str_append_str(String s, const String other) {
    if (!s || !other) return false;
    
    Block *curr = other->head;
    while (curr) {
        for (size_t i = 0; i < curr->size; i++) {
            if (!str_push_back(s, curr->data[i])) {
                return false;
            }
        }
        curr = curr->next;
    }
    return true;
}

bool str_insert(String s, size_t pos, const String t) {
    if (!s || !t || pos > s->length) return false;
    
    String temp = str_create();
    if (!temp) return false;
    
    // 复制前半部分
    for (size_t i = 0; i < pos; i++) {
        str_push_back(temp, str_at(s, i));
    }
    
    // 插入t
    str_append_str(temp, t);
    
    // 复制后半部分
    for (size_t i = pos; i < s->length; i++) {
        str_push_back(temp, str_at(s, i));
    }
    
    str_clear(s);
    str_append_str(s, temp);
    str_destroy(&temp);
    
    return true;
}

bool str_delete(String s, size_t pos, size_t len) {
    if (!s || pos >= s->length) return false;
    
    if (pos + len > s->length) {
        len = s->length - pos;
    }
    
    String temp = str_create();
    if (!temp) return false;
    
    // 复制前半部分
    for (size_t i = 0; i < pos; i++) {
        str_push_back(temp, str_at(s, i));
    }
    
    // 复制后半部分
    for (size_t i = pos + len; i < s->length; i++) {
        str_push_back(temp, str_at(s, i));
    }
    
    str_clear(s);
    str_append_str(s, temp);
    str_destroy(&temp);
    
    return true;
}

// ==================== 字符串操作 ====================

bool str_concat(String result, const String s1, const String s2) {
    if (!result || !s1 || !s2) return false;
    
    str_clear(result);
    return str_append_str(result, s1) && str_append_str(result, s2);
}

bool str_substring(String sub, const String s, size_t pos, size_t len) {
    if (!sub || !s || pos >= s->length) return false;
    
    if (pos + len > s->length) {
        len = s->length - pos;
    }
    
    str_clear(sub);
    for (size_t i = pos; i < pos + len; i++) {
        if (!str_push_back(sub, str_at(s, i))) {
            return false;
        }
    }
    
    return true;
}

int str_find(const String s, const String pattern, size_t start_pos) {
    if (!s || !pattern || pattern->length == 0) return -1;
    if (start_pos >= s->length) return -1;
    if (pattern->length > s->length - start_pos) return -1;
    
    for (size_t i = start_pos; i <= s->length - pattern->length; i++) {
        bool match = true;
        for (size_t j = 0; j < pattern->length; j++) {
            if (str_at(s, i + j) != str_at(pattern, j)) {
                match = false;
                break;
            }
        }
        if (match) return (int)i;
    }
    
    return -1;
}

int str_find_char(const String s, char c, size_t start_pos) {
    if (!s || start_pos >= s->length) return -1;
    
    for (size_t i = start_pos; i < s->length; i++) {
        if (str_at(s, i) == c) {
            return (int)i;
        }
    }
    return -1;
}

bool str_replace_first(String s, const String old_str, const String new_str) {
    if (!s || !old_str || !new_str) return false;
    
    int pos = str_find(s, old_str, 0);
    if (pos < 0) return false;
    
    str_delete(s, pos, old_str->length);
    return str_insert(s, pos, new_str);
}

int str_replace_all(String s, const String old_str, const String new_str) {
    if (!s || !old_str || !new_str || old_str->length == 0) return 0;
    
    int count = 0;
    size_t pos = 0;
    
    while (pos < s->length) {
        int found = str_find(s, old_str, pos);
        if (found < 0) break;
        
        str_delete(s, found, old_str->length);
        str_insert(s, found, new_str);
        
        pos = found + new_str->length;
        count++;
    }
    
    return count;
}

// ==================== 输出 ====================

void str_print(const String s, FILE *fp) {
    if (!s || !fp) return;
    fprintf(fp, "%s", str_c_str(s));
}

void str_println(const String s, FILE *fp) {
    if (!s || !fp) return;
    fprintf(fp, "%s\n", str_c_str(s));
}

// ==================== 错误处理 ====================

const char* str_error_message(StrError error) {
    switch (error) {
        case STR_OK: return "Success";
        case STR_ALLOC_FAILED: return "Memory allocation failed";
        case STR_INVALID_PARAM: return "Invalid parameter";
        case STR_OUT_OF_RANGE: return "Index out of range";
        case STR_NOT_FOUND: return "Pattern not found";
        default: return "Unknown error";
    }
}