#pragma once
#include "pch.h"

class Abcde : public Model
{
public:
	Abcde();
	Abcde(const string& param);	
	bool accept_alpha(double alpha);
	void init_posterior();
	void act_with_config_file();
	Distribution::Thetha mutation(int index);
	Distribution::Thetha crossover(int index);
	double get_statistics(Parametrs::MODE _mode, Distribution::Thetha curr_thetha, double error, int i);
	string config_file;
	string deep_exe;
	string optimizing_model_exe;
	string param_opt_model;
	Distribution::Posterior posterior;
	Distribution::Posterior new_posterior;
	Distribution::Thetha curr_thetha;
	Distribution generator;
	double eps;
	int t;
	int count_iter;
	int start_iter;
	int count_thread;
	double* error;
	int mode;//0 - mean, 1 - med
	int count_opt_param;
	vector<double> mean, std;

};
