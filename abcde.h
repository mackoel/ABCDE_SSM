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
		deep_exe = pt.get<std::string>("data.name_exe_file");
		optimizing_model_exe = pt.get<std::string>("data.name_command_exe_file");
		param_opt_model = pt.get<std::string>("data.param_command_exe_file");
		eps = stod(pt.get<std::string>("data.eps"));
		t = stoi(pt.get<std::string>("data.t"));
		count_iter = stoi(pt.get<std::string>("data.count_iter"));
	}
	Distribution::Thetha mutation(int index)
	{
		Distribution::Thetha curr_thetha = posterior.thetha[index];
		curr_thetha.l = curr_thetha.l + (int)generator.prior_distribution(Distribution::TYPE_DISTR::NORM, MU_N, SIGMA);
		curr_thetha.n = curr_thetha.n + (int)generator.prior_distribution(Distribution::TYPE_DISTR::NORM, MU_L, SIGMA);
		curr_thetha.lambda = curr_thetha.lambda + generator.prior_distribution(Distribution::TYPE_DISTR::NORM, MU_LAMBDA, SIGMA);
		return curr_thetha;
	}
	Distribution::Thetha crossover(int index)
	{
		double si_1 = generator.prior_distribution(Distribution::TYPE_DISTR::NORM_WITH_PARAM, 0.5, 1), si_2 = generator.prior_distribution(Distribution::TYPE_DISTR::NORM_WITH_PARAM, 0.5, 1), b = generator.prior_distribution(Distribution::TYPE_DISTR::NORM_WITH_PARAM, 0.001, 0.001);
		Distribution::Thetha thetha_b, thetha_m, thetha_n, curr_thetha;
	//	thetha_b = generator.get_prev_iter_with_probabilities(posterior);
		thetha_m = posterior.thetha[rand() % (count_iter + 1)];
		thetha_n = posterior.thetha[rand() % (count_iter + 1)];

		curr_thetha = posterior.thetha[index];
		curr_thetha.n = curr_thetha.n + si_1 * (thetha_m.n - thetha_n.n) + si_2 * (thetha_b.n - curr_thetha.n) + b;
		curr_thetha.l = curr_thetha.l + si_1 * (thetha_m.l - thetha_n.l) + si_2 * (thetha_b.l - curr_thetha.l) + b;
		curr_thetha.lambda = curr_thetha.lambda + si_1 * (thetha_m.lambda - thetha_n.lambda) + si_2 * (thetha_b.lambda - curr_thetha.lambda) + b;
		return curr_thetha;
	}
	double get_statistics(Distribution::Thetha curr_thetha, double error, int i)
	{

		double sigma_psi = generator.prior_distribution(Distribution::TYPE_DISTR::EXPON, 0.005);
		double psi = generator.prior_distribution(Distribution::TYPE_DISTR::NORM_WITH_PARAM, error, sigma_psi);
		double alpha = ((generator.prior_distribution(Distribution::TYPE_DISTR::NORM_WITH_PARAM, curr_thetha.n, SIGMA) * generator.prior_distribution(Distribution::TYPE_DISTR::NORM_WITH_PARAM, curr_thetha.l, SIGMA) * generator.prior_distribution(Distribution::TYPE_DISTR::NORM_WITH_PARAM, curr_thetha.lambda, SIGMA)) * psi) / ((generator.prior_distribution(Distribution::TYPE_DISTR::NORM_WITH_PARAM, posterior.thetha[i].n, SIGMA) * generator.prior_distribution(Distribution::TYPE_DISTR::NORM_WITH_PARAM, posterior.thetha[i].l, SIGMA) * generator.prior_distribution(Distribution::TYPE_DISTR::NORM_WITH_PARAM, posterior.thetha[i].lambda, SIGMA)) * psi);
		return alpha;
	}
	string config_file;
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
