#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef void* string;


string str_init(void);
void str_destroy(string S);

bool str_assign(string S, const char *chars); // S <- chars
bool str_copy(string dest, string src);       // dest <- src

bool str_empty(string S);
int  str_compare(string S1, string S2);       // <0,=0,>0
int  str_length(string S);

void str_clear(string S);

bool str_concat(string result, string S1, string S2);
bool str_substring(string sub, string S, int pos, int len);
int  str_index(string S, string T, int pos);  // 返回T在S中首次出现位置
bool str_replace(string S, string T, string V);
bool str_insert(string S, int pos, string T);
bool str_delete(string S, int pos, int len);
bool str_push_back(string S, char c);

#endif //BLOCKCHAIN_H
