#include<iostream>
#include<Windows.h>

int main() {
	SetConsoleOutputCP(65001);
	char str[] = "ƒ\";
	printf("%s", str);
	return 0;
}