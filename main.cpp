#include "pch.h"

int main(int argc, char *argv[])
{
	Parametrs param;
	param.process_program_options(argc, argv);
	Abcde abcde(param.config_file);
	Deep deep(param.config_file);
	Solution solution(abcde, deep, param);
	solution.run_manager();
	return 0;
}
