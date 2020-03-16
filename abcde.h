#pragma once
#include "pch.h"

//#define DEEP_FILE "deep_test.ini"
//#define OUT_DEEP_FILE "deep_test.ini-deep-output"
//#define N 3
class Abcde: public Model
{
public:
	Abcde() {}
	Abcde(const string &param)
	{
		config_file = param;
		init_posterior();
		act_with_config_file();
		error = new double[count_iter];
	}

	void act_with_config_file() 
	{
		boost::property_tree::ptree pt;
		boost::property_tree::ini_parser::read_ini(config_file, pt);
		count_thread =  stoi(pt.get<std::string>("data.count_thread"));
		deep_ini_file = pt.get<std::string>("data.name_deep_ini_file");
		deep_exe = pt.get<std::string>("data.name_exe_file");
		optimizing_model_exe = pt.get<std::string>("data.name_command_exe_file");
		param_opt_model = pt.get<std::string>("data.param_command_exe_file");
		eps = stod(pt.get<std::string>("data.eps"));
		t = stoi(pt.get<std::string>("data.t"));
	}
	string config_file;
	string deep_ini_file;
	string deep_exe;
	string optimizing_model_exe;
	string param_opt_model;
	Distribution::Posterior posterior;
	Distribution::Thetha curr_thetha;
	Distribution generator;
	double eps;
	int t;
	int count_iter;
	int count_thread;
	double *error;
	void init_posterior()
	{
		posterior.thetha = new Distribution::Thetha[count_iter];
		posterior.w = new double[count_iter];
	}
};