#include "blockchain.h"
#include <string.h>

#define BLOCK_SIZE 31

typedef struct Block
{
    struct Block *next;
    unsigned char size;
    char data[BLOCK_SIZE];
} Block;

struct String
{
    Block *head;
    Block *tail;
    size_t length;
};

static Block *block_create(void)
{
    Block *b = (Block *)malloc(sizeof(Block));
    if (b)
    {
        b->next = NULL;
        b->size = 0;
    }
    return b;
}

static void block_destroy_all(Block *head)
{
    while (head)
    {
        Block *temp = head;
        head = head->next;
        free(temp);
    }
}

//-----Lifecycle Management------

String str_create(void)
{
    String str = (String)malloc(sizeof(struct String));

    if (!str)
        return NULL;

    str->head = str->tail = NULL;
    str->length = 0;

    return str;
}

String str_create_from(const char *cstr)
{
    if (!cstr)
        return NULL;
    String s = str_create();
    if (!s)
        return NULL;
    int i = 0;
    while (cstr[i] != '\0')
    {
        if (!str_push_back(s, cstr[i++]))
        {
            str_destroy(&s);
            return NULL;
        }
    }

    return s;
}

void str_destroy(String *s)
{
    if (!s || !*s)
        return;
    block_destroy_all((*s)->head);
    free(*s);
}

//-----Basic Properties-----

size_t str_length(const String s)
{
    return s ? s->length : 0;
}

bool str_empty(const String s)
{
    return str_length(s) ? false : true;
}

//-----Access-----

char str_at(const String s, size_t index)
{
    if (!s || index >= s->length)
    {
        return '\0';
    }

    Block *current = s->head;
    size_t pos = 0;

    while (current)
    {
        if (pos + current->size > index)
        {
            return current->data[index - pos];
        }
        pos += current->size;
        current = current->next;
    }

    return '\0';
}

//-----Modification Operations-----

void str_clear(String s)
{
    if (!s)
        return;

    block_destroy_all(s->head);
    s->length = 0;
    s->head = s->tail = NULL;
}

bool str_push_back(String s, char c)
{
    if (!s)
        return false;

    if (!s->tail || s->tail->size == BLOCK_SIZE)
    {
        Block *new_block = block_create();
        if (new_block)
        {
            if (s->head)
            {
                s->tail->next = new_block;
                s->tail = new_block;
            }
            else
            {
                s->head = s->tail = new_block;
            }
            // new_block->next = NULL;
        }
        else
        {
            return false;
        }
    }
    s->tail->data[s->tail->size++] = c;
    s->length++;
    // s->tail->next = NULL;
    return true;
}

bool str_append_str(String s, const String other)
{
    if (!s || !other)
        return false;

    Block *curr = other->head;
    while (curr)
    {
        for (size_t i = 0; i < curr->size; i++)
        {
            if (!str_push_back(s, curr->data[i]))
            {
                return false;
            }
        }
        curr = curr->next;
    }
    return true;
}

bool str_insert(String s, size_t pos, const String t)
{
    if (!s || !t || pos > s->length)
        return false;

    String temp = str_create();
    if (!temp)
        return false;

    // 复制前半部分
    for (size_t i = 0; i < pos; i++)
    {
        str_push_back(temp, str_at(s, i));
    }

    // 插入t
    str_append_str(temp, t);

    // 复制后半部分
    for (size_t i = pos; i < s->length; i++)
    {
        str_push_back(temp, str_at(s, i));
    }

    str_clear(s);
    str_append_str(s, temp);
    str_destroy(&temp);

    return true;
}

bool str_insert_char(String s, size_t pos, char c)
{
    if (!s || pos > s->length)
    {
        return false;
    }

    if (pos == s->length)
    {
        return str_push_back(s, c);
    }

    // 定位到目标块
    Block *current = s->head;
    Block *prev = NULL;
    size_t block_start = 0;

    while (current && block_start + current->size <= pos)
    {
        block_start += current->size;
        prev = current;
        current = current->next;
    }

    if (!current)
        return false;

    size_t block_pos = pos - block_start;

    // 如果当前块有空间，直接在块内插入
    if (current->size < BLOCK_SIZE)
    {
        // 块内后移字符
        for (size_t i = current->size; i > block_pos; i--)
        {
            current->data[i] = current->data[i - 1];
        }
        current->data[block_pos] = c;
        current->size++;
        s->length++;
        return true;
    }
    // 块满了，需要分裂或创建新块
    else
    {
        // 折半分裂
        unsigned char old_size = current->size;
        Block *new_block = block_create();
        current->size /= 2;
        new_block->size = old_size - current->size;
        new_block->next = current->next;
        current->next = new_block;

        // 复制旧块后半块给新块
        for (int i = 0; i < new_block->size; i++)
            new_block->data[i] = current->data[current->size + i];

        // 插入字符c，递归调用以在分裂后的块中插入
        if (str_insert_char(s, pos, c))
            return true; // 简单实现
        else
            return false;
    }
}

// Deletes 'len' characters from position 'pos' in the string 's'.
// Returns true if deletion is successful, false otherwise.
bool str_delete(String s, size_t pos, size_t len)
{
    if (!s || pos + len > s->length)
        return false;

    Block *current = s->head;
    Block *prev = NULL;
    size_t block_start = 0;
    // 定位到目标块
    while (current && block_start + current->size <= pos)
    {
        block_start += current->size;
        prev = current;
        current = current->next;
    }

    if (!current)
        return false;

    size_t to_delete = len;
    size_t block_pos = pos - block_start;
    while (current && to_delete > 0)
    {
        size_t can_delete = current->size - block_pos;
        // 块内删除
        if (can_delete > to_delete)
        {
            for (size_t i = block_pos; i < current->size - to_delete; i++)
            {
                current->data[i] = current->data[i + to_delete];
            }
            current->size -= to_delete;
            s->length -= to_delete;
            to_delete = 0;
        }
        // 块间删除
        else
        {
            // 整块删除
            if (block_pos == 0)
            {
                Block *temp = current;
                current = current->next;
                if (prev)
                {
                    prev->next = current;
                }
                else
                {
                    s->head = current;
                }
                free(temp);
            }
            // 删除当前块中，pos后全部字符
            else
            {
                current->size = block_pos;
                prev = current;
                current = current->next;
                block_pos = 0;
            }
            s->length -= can_delete;
            to_delete -= can_delete;
        }
    }

    // 修正tail指针
    Block *tail_finder = s->head;
    if (!tail_finder)
    {
        s->tail = NULL;
    }
    else
    {
        while (tail_finder->next)
        {
            tail_finder = tail_finder->next;
        }
        s->tail = tail_finder;
    }

    return true;
}

//-----String Operations-----
bool str_concat(String result, const String s1, const String s2)
{
    if (!result || !s1 || !s2)
        return false;

    str_clear(result);
    if (!str_append_str(result, s1))
        return false;
    if (!str_append_str(result, s2))
        return false;

    return true;
}

bool str_substring(String sub, const String s, size_t pos, size_t len)
{
    if (!sub || !s || pos >= s->length)
        return false;

    if (pos + len > s->length)
    {
        len = s->length - pos; // 自动调整
    }

    str_clear(sub);
    for (size_t i = 0; i < len; i++)
    {
        str_push_back(sub, str_at(s, pos + i));
    }
    return true;
}

static int *prefix_function(const String pattern)
{
    /*
    time complexity: O(m)
    space complexity: O(m)
    */
    int m = str_length(pattern);
    int *next = (int *)malloc(sizeof(int) * m);
    if (!next)
        return NULL;
    next[0] = 0;
    for (int i = 1; i < m; i++)
    {
        int j = next[i - 1];
        while (j > 0 && str_at(pattern, i) != str_at(pattern, j))
            j = next[j - 1];
        if (str_at(pattern, i) == str_at(pattern, j))
            j++;
        next[i] = j;
    }
    return next;
}

int *str_find_all(const String s, const String pattern, size_t start_pos)
{
    /*
    time complexity: O(n+m+k)
    space complexity: O(n+m+k)
    */
    if (!s || !pattern || start_pos >= str_length(s))
        return NULL;

    String cur = str_create();
    if (!cur)
        return NULL;

    size_t n = str_length(s);
    size_t m = str_length(pattern);
    str_append_str(cur, pattern);      // O(m)
    str_push_back(cur, '#');           // O(1)
    str_append_str(cur, s);            // O(n)
    str_delete(cur, m + 1, start_pos); // 删除前start_pos个字符 O(n)
    int *next = prefix_function(cur);  // O(n+m)
    if (!next)
        return NULL;
    int *positions = (int *)malloc(sizeof(int) * (n + 1)); // 最多n个匹配位置
    if (!positions)
    {
        free(next);
        return NULL;
    }
    int count = 1;
    for (size_t i = m + 1; i < str_length(cur); i++)
    { // O(n)
        if ((size_t)next[i] == m)
        {
            positions[count++] = i - 2 * m + start_pos;
        }
    }
    positions[0] = count - 1; // 记录匹配数量
    free(next);
    str_destroy(&cur);
    return positions;
}

int str_find_first(const String s, const String pattern, size_t start_pos)
{
    if (!s || !pattern || start_pos >= str_length(s))
        return -1;
    int *positions = str_find_all(s, pattern, start_pos);
    if (!positions)
        return -1;
    int count = positions[0];
    if (count == 0)
    {
        free(positions);
        return -1;
    }
    int first_pos = positions[1];
    free(positions);
    return first_pos;
}

int str_find_char(const String s, char c, size_t start_pos)
{
    if (!s || start_pos >= str_length(s))
        return -1;
    for (size_t i = start_pos; i < str_length(s); i++)
    {
        if (str_at(s, i) == c)
            return i;
    }
    return -1;
}

bool str_replace_first(String s, const String old_str, const String new_str)
{
    if (!s || !old_str || !new_str)
        return false;
    int pos = str_find_first(s, old_str, 0);
    if (pos == -1)
        return false;
    size_t old_len = str_length(old_str);
    str_delete(s, pos, old_len);
    str_insert(s, pos, new_str);
    return true;
}

int str_replace_all(String s, const String old_str, const String new_str)
{
    if (!s || !old_str || !new_str)
        return -1;
    int *pos = str_find_all(s, old_str, 0);
    if (!pos || pos[0] == 0)
        return -1;
    size_t old_len = str_length(old_str);
    for (int i = pos[0]; i > 0; i--)
    {
        str_delete(s, pos[i], old_len);
        str_insert(s, pos[i], new_str);
    }
    free(pos);
    return pos[0];
}

//-----Output-----

void str_print(const String s, FILE *fp)
{
    if (!s || !fp)
        return;

    Block *current = s->head;
    while (current)
    {
        fwrite(current->data, sizeof(char), current->size, fp);
        current = current->next;
    }
}

void str_println(const String s, FILE *fp)
{
    str_print(s, fp);
    fprintf(fp, "\n");
}

void str_print_per_block(const String s, FILE *fp)
{
    if (!s || !fp)
        return;

    Block *current = s->head;
    int block_index = 0;
    while (current)
    {
        fprintf(fp, "Block %d: ", block_index++);
        fwrite(current->data, sizeof(char), current->size, fp);
        fprintf(fp, "\n");
        current = current->next;
    }
}

//-----const char* str_error_message(StrError error)-----

const char *str_error_message(StrError error)
{
    switch (error)
    {
    case STR_OK:
        return "Success";
    case STR_ALLOC_FAILED:
        return "Memory allocation failed";
    case STR_INVALID_PARAM:
        return "Invalid parameter";
    case STR_OUT_OF_RANGE:
        return "Index out of range";
    case STR_NOT_FOUND:
        return "Pattern not found";
    default:
        return "Unknown error";
    }
}
