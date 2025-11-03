#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_LEN 256
#define MAX_TOKENS 128

//================ 工具函数 ================
int is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

int priority(char op) {
    switch (op) {
        case '+':
        case '-': return 1;
        case '*':
        case '/': return 2;
        case '^': return 3;
        default: return 0;
    }
}

//================ 栈实现 ================
typedef struct {
    char data[MAX_LEN];
    int top;
} CharStack;

void init_charstack(CharStack *s) { s->top = -1; }
int empty_charstack(CharStack *s) { return s->top == -1; }
char top_char(CharStack *s) { return s->data[s->top]; }
void push_char(CharStack *s, char c) { s->data[++s->top] = c; }
char pop_char(CharStack *s) { return s->data[s->top--]; }

//================ 数字栈 ================
typedef struct {
    double data[MAX_LEN];
    int top;
} NumStack;

void init_numstack(NumStack *s) { s->top = -1; }
int empty_numstack(NumStack *s) { return s->top == -1; }
void push_num(NumStack *s, double val) { s->data[++s->top] = val; }
double pop_num(NumStack *s) { return s->data[s->top--]; }

//================ 中缀 → 后缀 ================
int infix_to_postfix(const char *expr, char output[][MAX_LEN]) {
    CharStack optr;
    init_charstack(&optr);

    int k = 0;
    int i = 0;
    while (expr[i] != '\0') {
        if (isspace(expr[i])) { i++; continue; }

        if (isdigit(expr[i])) {
            char num[64] = {0};
            int j = 0;
            while (isdigit(expr[i])) {
                num[j++] = expr[i++];
            }
            strcpy(output[k++], num);
        }
        else if (expr[i] == '(') {
            push_char(&optr, '(');
            i++;
        }
        else if (expr[i] == ')') {
            while (!empty_charstack(&optr) && top_char(&optr) != '(') {
                sprintf(output[k++], "%c", pop_char(&optr));
            }
            if (!empty_charstack(&optr)) pop_char(&optr); // 弹出 '('
            i++;
        }
        else if (is_operator(expr[i])) {
            while (!empty_charstack(&optr) && 
                   priority(top_char(&optr)) >= priority(expr[i])) {
                sprintf(output[k++], "%c", pop_char(&optr));
            }
            push_char(&optr, expr[i]);
            i++;
        }
        else {
            printf("非法字符: %c\n", expr[i]);
            exit(1);
        }
    }

    while (!empty_charstack(&optr)) {
        sprintf(output[k++], "%c", pop_char(&optr));
    }

    output[k][0] = '\0';
    return k; // 返回 token 数量
}

//================ 后缀求值 ================
double operate(double a, char op, double b) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        case '^': return pow(a, b);
        default: return 0;
    }
}

double eval_postfix(char postfix[][MAX_LEN]) {
    NumStack s;
    init_numstack(&s);

    for (int i = 0; postfix[i][0] != '\0'; i++) {
        if (isdigit(postfix[i][0])) {
            push_num(&s, atof(postfix[i]));
        } else {
            double b = pop_num(&s);
            double a = pop_num(&s);
            push_num(&s, operate(a, postfix[i][0], b));
        }
    }
    return pop_num(&s);
}

//================ 主函数 ================
int main() {
    char expr[MAX_LEN];
    char postfix[MAX_TOKENS][MAX_LEN];

    printf("请输入中缀表达式（例如 (3+5)*2-6/3 ）: ");
    fgets(expr, MAX_LEN, stdin);
    expr[strcspn(expr, "\n")] = '\0'; // 去除换行

    int token_count = infix_to_postfix(expr, postfix);

    printf("后缀表达式: ");
    for (int i = 0; i < token_count; i++) {
        printf("%s ", postfix[i]);
    }
    printf("\n");

    double result = eval_postfix(postfix);
    printf("计算结果: %.2f\n", result);

    return 0;
}