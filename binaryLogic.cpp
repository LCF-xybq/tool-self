#include <iostream>
#include <stack>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
using namespace std;

#define MAX_LEN 256
unsigned int num[10];
int cnt = 0; // count of num
unsigned int result = 0;
char Hex[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

stack<char> op;
stack<char> val;

int numOrNot(char ch) {
	return (ch <= '9' && ch >= '0') || (ch <= 'z' && ch >= 'a');
}

void toLower(char *str) {
	for (int i = 0; str[i]; i++) {
		if (isupper(str[i])) str[i] = tolower(str[i]);
	}
}

/*
 * if input expression like: 0x123321 | 0xefa123 & (0x123 << 12), we call it A
 * simplify A to 0|1&(2<<3), we call it expression B
 * in B, 0, 1, 2, 3 means index of array-'num'. In 'num', we store operands
 * In other words, num[0] = 0x123321; num[1] = 0xefa123; num[2] = 0x123; num[3] = 12
 * So, we only support for up to 10 operands
 */
void initStr(char *str) {
	char temp[MAX_LEN]; // store simplified expression
	int i = 0, j = 0;
	for (; str[i]; i++) {
		char ch = str[i];
		if (ch == ' ') continue;
		if (ch == '~' || ch == '|' || ch == '&' || \
			ch == '^' || ch == '(' || ch == ')') temp[j++] = ch;
		else if (ch == '<' || ch == '>') {
			temp[j++] = ch;
			int p = i + 2; // << >> jump
			while (!numOrNot(str[p])) p++;
			unsigned int move = 0;
			while (numOrNot(str[p])) {
				move = move * 10 + (str[p] - '0');
				p++;
			}
			num[cnt++] = move;
			temp[j++] = (cnt - 1) + '0';
			i = p - 1;
		}
		else if (ch == '0') {
			if (str[i+1] && str[i+1] == 'x') {
				int k = i + 2; // 0x jump
				for (; str[k]; k++) {
					if (!numOrNot(str[k])) break;
				}
				int len = k - i - 2; //digits of number
				int step = len;
				k = i + 2;
				unsigned int number = 0;
				while (step--) {
					unsigned int powRes = (unsigned int)pow(16, len - 1);
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
	cnt = 0;
	memcpy(str, temp, MAX_LEN);
}

/*
 * stack 'op' stores operations
 * if A's priority > B's priority, then push
 * else pop 'op' till A's > B's
 * After popping from 'op', then push it into 'val'
 */
void opStack(char ope, int pri1) {
	while (!op.empty()) {
		char ch = op.top();
		if (ch == '(') break;
		int pri2 = 0;
		if (ch == '~') pri2 = 1;
		else if (ch == '<' || ch == '>') pri2 = 2;
		else if (ch == '&') pri2 = 3;
		else if (ch == '^') pri2 = 4;
		else pri2 = 5;

		if (pri1 > pri2) {
			op.pop();
			val.push(ch);
		} else {
			break;
		}
	}
	op.push(ope);
}
// for ')'
void valStack() {
	while (!op.empty()) {
		char ch = op.top();
		op.pop();
		if (ch == '(') break;
		else val.push(ch);
	}
}

void solve(char* str) {
	for (int i = 0; str[i]; i++) {
		char ch = str[i];
		switch (ch) {
			case '<':
			case '>':
				opStack(ch, 2);
				break;
			case '~':
				opStack(ch, 1);
				break;
			case '&':
				opStack(ch, 3);
				break;
			case '^':
				opStack(ch, 4);
				break;
			case '|':
				opStack(ch, 5);
				break;
			case '(':
				op.push(ch);
				break;
			case ')':
				valStack();
				break;
			default:
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
	// now, postfix expression completed!
	// next, calculate!!
	stack<unsigned int> res;
	while (!op.empty()) {
		char ch = op.top();
		op.pop();
		if (numOrNot(ch)) res.push(num[ch - '0']);
		else {
			unsigned int a = 0;
			unsigned int b = 0;
			switch (ch) {
				case '~':
					if (res.empty()) printf("error\n");
					a = ~(res.top());
					res.pop();
					res.push(a);
					break;
				case '<':
					if (res.size() < 2) printf("error\n");
					a = res.top(); res.pop();
					b = res.top(); res.pop();
					b = b << a;
					res.push(b);
					break;
				case '>':
					if (res.size() < 2) printf("error\n");
					a = res.top(); res.pop();
					b = res.top(); res.pop();
					b = b >> a;
					res.push(b);
					break;
				case '&':
					if (res.size() < 2) printf("error\n");
					a = res.top(); res.pop();
					b = res.top(); res.pop();
					b = b & a;
					res.push(b);
					break;
				case '^':
					if (res.size() < 2) printf("error\n");
					a = res.top(); res.pop();
					b = res.top(); res.pop();
					b = b ^ a;
					res.push(b);
					break;
				case '|':
					if (res.size() < 2) printf("error\n");
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

void printRes(char *str) {
	printf("\nDEC: %u\n", result);
	unsigned int a = result;

	int buff[8];
	int i = 0;
	printf("HEX: 0x");
	while (a > 0) {
		buff[i++] = a % 16;
		a = a / 16;
	}
	for (i = i - 1; i >= 0; i--) {
		cout << Hex[buff[i]];
	}
	printf("\n\n");
	int format[8] = {31, 27, 23, 19, 15, 11, 7, 3};
	int f = 0, space = 0;
	while (f < 8) {
		if (format[f] > 7) {
			if (f != 0 && (space++) < 3) printf(" ");
			else {
				printf("%d", format[f++]);
				space = 0;
			}
		} else {
			printf("    7    3");
			break;
		}
	}
	printf("\n");

	int bin[32];
	for (int k = 0; k < 32; k++) bin[k] = 2;
	i = 0;
	a = result;
	while (a > 0) {
		bin[i++] = a % 2;
		a = a / 2;
	}

	printf(" ");
	int val = 0;
	for (i = 31; i >= 0; i--) {
		if (bin[i] == 0 || bin[i] == 2) printf("0");
		else printf("1");
		val++;

		if (val % 4 == 0) printf(" ");
	}
	printf("\n");
}

int main() {
	char str[MAX_LEN];
	
	while (fgets(str, MAX_LEN, stdin)) {
		int len = strlen(str);
		str[len-1] = '\0';
		toLower(str);
		initStr(str);
		solve(str);
		printRes(str);
		while (!op.empty()) op.pop();
		while (!val.empty()) val.pop();
		printf("-------------------------------------------\n\n");
	}
	
	return 0;
}
