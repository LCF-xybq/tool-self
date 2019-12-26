#include <iostream>
#include <cstdio>
using namespace std;

void notop(int cnt)
{
		
}

void orop(int cnt)
{
	
}

void andop(int cnt)
{
	
}

int main()
{
	char op;
	int cnt;
	cout << "Enter operation: ";
	cin >> op;
	cout << "Enter scale your want to show: ";
	cin >> cnt;

	if (op == '~') {
		notop(cnt);
	}
	else if (op == '|') {
		orop(cnt);
	}
	else {
		andop(cnt);
	}

	return 0;
}
