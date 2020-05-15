#pragma once
#include "pch.h"


class Deep:public Model
{
private:
	pt::ptree propTree;

public:
	Deep();

	Deep(const string& param);

	double run();

	double parse_result(string output);

	void act_with_config_file();

	void prepare_tmp_deep_ini_file(Distribution::Thetha thetha, string exe_file, string param_exe_file);

	void create_tmp_deep_ini_file();

	string config_file;
	string tmp_config_file;
	string deep_exe;
	double error;
	int index_score;
	vector<int> num_string_when_add_param;// = 46; 46; 32
	vector<int> index_opt_param_in_command_line;// = 5; 7; 5
	vector<int> index_opt_param_in_vector_param;
};
