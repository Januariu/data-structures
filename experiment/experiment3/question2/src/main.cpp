#include <iostream>
#include <stack>
#include <vector>
#include <string>
#include <cctype>
#include <sstream>
#include <cmath>
using namespace std;

// 判断是否为运算符
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

// 运算符优先级
int priority(char op) {
    switch (op) {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        case '^': return 3;
        default: return 0;
    }
}

// 中缀表达式 -> 后缀表达式
vector<string> infixToPostfix(const string& expr) {
    stack<char> optr;
    vector<string> output;
    string num;

    for (size_t i = 0; i < expr.size(); i++) {
        char ch = expr[i];

        if (isspace(ch)) continue; // 忽略空格

        if (isdigit(ch)) {
            num += ch; // 收集多位数
            // 如果下一个不是数字，就把整个数输出
            if (i == expr.size()-1 || !isdigit(expr[i+1])) {
                output.push_back(num);
                num.clear();
            }
        }
        else if (ch == '(') {
            optr.push(ch);
        }
        else if (ch == ')') {
            while (!optr.empty() && optr.top() != '(') {
                output.push_back(string(1, optr.top()));
                optr.pop();
            }
            if (!optr.empty()) optr.pop(); // 弹出左括号
        }
        else if (isOperator(ch)) {
            while (!optr.empty() && priority(optr.top()) >= priority(ch)) {
                output.push_back(string(1, optr.top()));
                optr.pop();
            }
            optr.push(ch);
        }
        else {
            cerr << "Invalid character: " << ch << endl;
            exit(1);
        }
    }

    while (!optr.empty()) {
        output.push_back(string(1, optr.top()));
        optr.pop();
    }

    return output;
}

// 计算 a op b
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

// 求后缀表达式值
double evalPostfix(const vector<string>& postfix) {
    stack<double> st;
    for (auto& token : postfix) {
        if (isdigit(token[0])) {
            st.push(stod(token));
        } else {
            double b = st.top(); st.pop();
            double a = st.top(); st.pop();
            st.push(operate(a, token[0], b));
        }
    }
    return st.top();
}

int main() {
    string expr;
    cout << "请输入中缀表达式（例如 (3+5)*2-6/3 ）: ";
    getline(cin, expr);

    auto postfix = infixToPostfix(expr);

    cout << "后缀表达式: ";
    for (auto& s : postfix) cout << s << ' ';
    cout << endl;

    double result = evalPostfix(postfix);
    cout << "计算结果: " << result << endl;

    return 0;
}