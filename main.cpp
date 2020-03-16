#include "pch.h"

int main(int argc, char *argv[])
{
	string param(argv[1]);
	Abcde abcde(param);
	Deep deep(param);
	Solution solution(abcde, deep);
	solution.run();
	return 0;
}