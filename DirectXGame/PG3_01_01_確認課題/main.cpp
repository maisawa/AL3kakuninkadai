#include<iostream>
#include<Windows.h>

int main() {
	SetConsoleOutputCP(65001);
	char str[] = "�\";
	printf("%s", str);
	return 0;
}