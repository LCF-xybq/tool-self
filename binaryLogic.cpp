#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <stack>
using namespace std;

char str[256];
unsigned int num[10];
int cnt = 0;
unsigned int result = 0;
char Hex[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

template<class T>
class STACK {
    private:
    stack<T> s;

    public:
        void pop() {s.pop();}

        T top() {return s.top();}

        void push(T a) {s.push(a);}

        bool empty() {return s.empty();}

        int size() {return s.size();}

        void clear() {
            while (!s.empty()) s.pop();
        }
};

STACK<char> op;
STACK<char> val;

void error()
{
    cout << "Input is wrong!" << endl;
    exit(1);
}

bool numOrNot(char ch)
{
    if ((ch <= '9' && ch >= '0') || (ch <= 'z' && ch >= 'a')) return true;

    return false;
}

void toLower()
{
    for (int i = 0; str[i]; i++) {
        if (isupper(str[i])) str[i] = tolower(str[i]);
    }
}

void initStr()
{
    char temp[256];
    int i = 0, j = 0;
    for (; str[i]; i++) {
        char ch = str[i];
        if (ch == ' ') continue;
        if (ch == '~' || ch == '|' || ch =='&' || ch == '^' || ch == '(' || ch == ')') temp[j++] = ch;
        else if (ch == '<' || ch == '>') {
            temp[j++] = ch;
            int p = i + 2;
            while (str[p] <'0' || str[p] > '9') p++;
            unsigned int move = 0;
            while (str[p] <= '9' && str[p] >= '0') {
                move = move * 10 + (str[p] - '0');
                p++;
            }
            num[cnt++] = move;
            temp[j++] = (cnt - 1) + '0';
            i = p - 1;
        }
        else if (ch == '0') {
            if (str[i+1] && str[i+1] == 'x') {
                int k = i + 2;
                for (; str[k]; k++) {
                    if (!numOrNot(str[k])) break;
                }
                int len = k - i - 2;
                int step = len;
                k = i + 2;
                unsigned int number = 0;
                while(step--) {
                    unsigned int powRes = (unsigned int)pow(16, len-1);
                    if (str[k] >= '0' && str[k] <= '9') {
                        number += (str[k] - '0') * powRes;
                    } else {
                        number += (str[k] - 'a' + 10) * powRes;
                    }
                    len--;
                    k++;
                }
                num[cnt++] = number;
                temp[j++] = (cnt - 1) + '0';
            }
        }
    }
    temp[j] = '\0';
    // for (int p = 0; p < cnt; p++) cout << "num: " << num[p] << endl;
    cnt = 0;
    memcpy(str, temp, 256);
}

void opStack(char ope, int pri1)
{
    while (!op.empty()) {
        char ch = op.top();
        if (ch == '(') break;
        int pri2 = 0;
        if (ch == '~') pri2 = 1;
        else if (ch == '<' || ch == '>') pri2 = 2;
        else if (ch == '&') pri2 = 3;
        else if (ch == '^') pri2 = 4;
        else pri2 = 5;

        if (pri1 >= pri2) { op.pop(); val.push(ch);}
        else break;
    }
    op.push(ope);
}

void valStack()
{
    while (!op.empty()) {
        char c = op.top();
        op.pop();
        if (c == '(') break;
        else val.push(c);
    }
}

void solve()
{
    for (int i = 0; str[i]; i++) {
        char ch = str[i];
        switch (ch) {
            case '<' :
            case '>' :
                opStack(ch, 2);
                break;
            case '~' :
                opStack(ch, 1);
                break;
            case '&' :
                opStack(ch, 3);
                break;
            case '^' :
                opStack(ch, 4);
                break;
            case '|' :
                opStack(ch, 5);
                break;
            case '(' :
                op.push(ch);
                break;
            case ')' :
                valStack();
                break;
            default :
                val.push(ch);
                break;
        }
    }
    while (!op.empty()) {
        val.push(op.top());
        op.pop();
    }

    while (!val.empty()) {
        op.push(val.top());
        val.pop();
    }

    STACK<unsigned int> res;
    while (!op.empty()) {
        char ch = op.top();
        op.pop();
        if (ch <= '9' && ch >= '0') res.push(num[ch - '0']);
        else {
            unsigned int a = 0;
            unsigned int b = 0;
            switch (ch) {
                case '~' :
                    if (res.empty()) error();
                    a = ~(res.top());
                    res.pop();
                    res.push(a);
                    break;
                case '<' :
                    if (res.size() < 2) error();
                    a = res.top(); res.pop();
                    b = res.top(); res.pop();
                    b = b << a;
                    res.push(b);
                    break;
                case '>' :
                    if (res.size() < 2) error();
                    a = res.top(); res.pop();
                    b = res.top(); res.pop();
                    b = b >> a;
                    res.push(b);
                    break;
                case '&' :
                    if (res.size() < 2) error();
                    a = res.top(); res.pop();
                    b = res.top(); res.pop();
                    b = b & a;
                    res.push(b);
                    break;
                case '^' :
                    if (res.size() < 2) error();
                    a = res.top(); res.pop();
                    b = res.top(); res.pop();
                    b = b ^ a;
                    res.push(b);
                    break;
                case '|' :
                    if (res.size() < 2) error();
                    a = res.top(); res.pop();
                    b = res.top(); res.pop();
                    b = b | a;
                    res.push(b);
                    break;
                default:
                    break;
            }
        }
    }
    if (res.size() == 1) result = res.top();
}

void printRes()
{
    cout << "\nDEC: " << result << endl;
    unsigned int a = result;

    int buff[8];
    int i = 0;
    cout << "HEX: 0x";
    while (a > 0) {
        buff[i++] = a % 16;
        a = a / 16;
    }
    for (i = i - 1; i >= 0; i--) {
        cout << Hex[buff[i]];
    }
    cout << endl;

    cout << endl;
    int format[8] = {3, 7, 11, 15, 19, 23, 27, 31};
    int f = 0;
    while (f < 8) {
        int space = 0;
        if (format[f] != 7 && format[f] != 11) {
            while (space++ < 3) { cout << " ";}
            cout << format[f++];
        } else {
            cout << "    7    11";
            f = 3;
        }
    }
    cout << endl;

    int bin[32];
    for (int k = 0; k < 32; k++) bin[k] = 2;
    i = 0;
    a = result;
    while (a > 0) {
        bin[i++] = a % 2;
        a = a / 2;
    }

    int cnt = 0;
    for (i = 31; i >= 0; i--) {
        if (bin[i] == 0 || bin[i] == 2) cout << '0';
        else cout << '1';
        cnt++;

        if (cnt % 4 == 0) cout << " ";
    }

    cout << endl;
}

int main()
{
    while (cin.getline(str, 256)) {
        toLower();
        // cout << str << endl;
        initStr();
        // cout << str << endl;
        solve();
        printRes();
        op.clear();
        val.clear();
        cout << "-------------------------------------------\n\n";
    }

    return 0;
}
