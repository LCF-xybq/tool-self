#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <stack>
using namespace std;

char str[256];
unsigned int num[100];
int cnt = 0;
unsigned int result = 0;
char Hex[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

void error()
{
    cout << "Input is wrong!" << endl;
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
        if (ch == '~' || ch == '|' || ch =='&' || ch == '(' || ch == ')') temp[j++] = ch;
        else if (ch == '0') {
            if (str[i+1] && str[i+1] == 'x') {
                int k = i + 2;
                for (; str[k]; k++) {
                    if (!numOrNot(str[k])) break;
                }
                int len = k - i - 2;
        //        cout << i + 2 << " " << len << endl;
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
            //    cout << number << endl;
                num[cnt++] = number;
                temp[j++] = (cnt - 1) + '0';
            }
        }
    }
    temp[j] = '\0';
    memcpy(str, temp, 256);
}

void solve()
{
    stack<char> op;
    stack<unsigned int> val;

    for (int i = 0; str[i]; i++) {
        char ch = str[i];
        if (ch <= '9' && ch >= '0') {
            val.push(num[ch - '0']);
            //cout << "who  " << num[ch - '0'] << endl;
        } else if (ch == '~' || ch == '('){
            op.push(ch);
        } else if (ch == '|' || ch == '&') {
            if (op.top() == '~') {
                op.pop();
                op.push(ch);
                if (val.empty()) error();
                unsigned int temp = ~(val.top());
                //cout << "~  " << temp;
                val.pop();
                val.push(temp);
            } else
                op.push(ch);
        } else if (ch == ')') {
            while (!op.empty() && op.top() != '(') {
                unsigned int a = val.top();
                val.pop();
                char opera = op.top();
                op.pop();
                if (opera == '~') {
                    a = ~a;
                    val.push(a);
                } else {
                    unsigned int b = val.top();
                    val.pop();
                    if (opera == '|') {
                        //cout <<"|  " << (a|b) << endl;
                        val.push(a | b);
                    } else {
                        //cout << "&  " << (a&b) << endl;
                        val.push(a & b);
                    }
                }
            }
            if (op.top() == '(') op.pop();
        }
    }

    while (!op.empty()) {
        char lastOp = op.top();
        op.pop();
        unsigned int val1 = val.top();
        val.pop();
        if (lastOp == '~') {
            val.push(~val1);
            //cout <<"~  " << ~val1 << endl;
        } else {
            unsigned int val2 = val.top();
            val.pop();
            if (lastOp == '|') {
                //cout <<"|  " << (val1|val2) << endl;
                val.push(val1 | val2);
            } else {
                //cout <<"&  " << (val1&val2) << endl;
                val.push(val1 & val2);
            }
        }
    }

    result = val.top();
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
        initStr();
        solve();
        printRes();
        cout << "-------------------------------------------\n\n";
    }
    
    return 0;
}
