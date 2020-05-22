#include "pch.h"

Abcde::Abcde() {}

Abcde::Abcde(const string& param)
{
	config_file = param;
	act_with_config_file();
	init_posterior();
	error = new double[count_iter];
}

void  Abcde::act_with_config_file()
{
	boost::property_tree::ptree pt;
	boost::property_tree::ini_parser::read_ini(config_file, pt);
	count_thread = stoi(pt.get<std::string>("data.count_thread"));
	deep_exe = pt.get<std::string>("data.name_exe_file");
	optimizing_model_exe = pt.get<std::string>("data.name_command_exe_file");
	param_opt_model = pt.get<std::string>("data.param_command_exe_file");
	eps = stod(pt.get<std::string>("data.eps"));
	t = stoi(pt.get<std::string>("data.t"));
	count_iter = stoi(pt.get<std::string>("data.count_iter"));
	start_iter = stoi(pt.get<std::string>("data.start_iter"));
	mode = stoi(pt.get<std::string>("data.mode"));
	count_opt_param = stoi(pt.get<std::string>("data.count_opt_param"));
	vector<string> str_mean, str_std;
	boost::split(str_mean, pt.get<std::string>("data.mean"), boost::is_any_of(";"));
	for (int i = 0; i < str_mean.size(); i++)
	{
		mean.push_back(stod(str_mean[i]));
	}
	boost::split(str_std, pt.get<std::string>("data.std"), boost::is_any_of(";"));
	for (int i = 0; i < str_std.size(); i++)
	{
		std.push_back(stod(str_std[i]));
	}

}

Distribution::Thetha  Abcde::mutation(int index)
{
	/*Distribution::Thetha _curr_thetha = posterior.thetha[index];
	_curr_thetha.l = _curr_thetha.l + (int)generator.prior_distribution(Distribution::TYPE_DISTR::NORM, MU_N_MUT, SIGMA);
	_curr_thetha.n = _curr_thetha.n + (int)generator.prior_distribution(Distribution::TYPE_DISTR::NORM, MU_L_MUT, SIGMA);
	_curr_thetha.lambda = _curr_thetha.lambda + generator.prior_distribution(Distribution::TYPE_DISTR::NORM, MU_LAMBDA_MUT, SIGMA);
	_curr_thetha.delta = _curr_thetha.delta + generator.prior_distribution(Distribution::TYPE_DISTR::EXPON, 0.005);
	return _curr_thetha;*/
	Distribution::Thetha _curr_thetha = posterior.thetha[index];
	for (int i = 0; i < count_opt_param; i++)
	{
		_curr_thetha.param[i] = _curr_thetha.param[i] + generator.prior_distribution(Distribution::TYPE_DISTR::NORM, mean[count_opt_param-1], std[count_opt_param - 1]);// another mean for 
	}
	_curr_thetha.delta = _curr_thetha.delta + generator.prior_distribution(Distribution::TYPE_DISTR::EXPON, 0.005);
	return _curr_thetha;
}

Distribution::Thetha Abcde::crossover(int index)
{
	ofstream out("log_crossover_index.txt", std::ios::app);
/*
	double si_1 = generator.prior_distribution(Distribution::TYPE_DISTR::NORM_WITH_PARAM, 0.5, 1), si_2 = generator.prior_distribution(Distribution::TYPE_DISTR::NORM_WITH_PARAM, 0.5, 1), b = generator.prior_distribution(Distribution::TYPE_DISTR::NORM_WITH_PARAM, 0.001, 0.001);
	Distribution::Thetha thetha_b, thetha_m, thetha_n, _curr_thetha;
	int m_index, n_index;
	m_index = rand() % (count_iter - 1);
	n_index = rand() % (count_iter - 1);
	out << "start index:" << index << " " << n_index << " " << m_index << endl;

	while (m_index == index || n_index == index || m_index == n_index)
	{
		m_index = rand() % (count_iter - 1);
		n_index = rand() % (count_iter - 1);
		out << "index:" << index << " " << n_index << " " << m_index << endl;

	}
	
	out << "endindex:" << index << " " << n_index  << " " << m_index << endl;
	thetha_b = generator.get_prev_iter_with_probabilities(posterior, count_iter);
	thetha_m = posterior.thetha[rand() % (count_iter - 1)];
	thetha_n = posterior.thetha[rand() % (count_iter - 1)];

	_curr_thetha = posterior.thetha[index];
	_curr_thetha.n = (int)((double)_curr_thetha.n + si_1 * ((double)thetha_m.n - (double)thetha_n.n) + si_2 * ((double)thetha_b.n - (double)_curr_thetha.n) + b);
	_curr_thetha.l = (int)((double)_curr_thetha.l + si_1 * ((double)thetha_m.l - (double)thetha_n.l) + si_2 * ((double)thetha_b.l - (double)_curr_thetha.l) + b);
	_curr_thetha.lambda = _curr_thetha.lambda + si_1 * (thetha_m.lambda - thetha_n.lambda) + si_2 * (thetha_b.lambda - _curr_thetha.lambda) + b;
	_curr_thetha.delta = _curr_thetha.delta + si_1 * (thetha_m.delta - thetha_n.delta) + si_2 * (thetha_b.delta - _curr_thetha.delta) + b;
	out.close();
	return _curr_thetha;
	*/
//THIS PARAM FOR SI_I NOT FOR USER! PROGRAM CONSTANT FROM ALGORITHM DEFINITION
	double si_1 = generator.prior_distribution(Distribution::TYPE_DISTR::NORM_WITH_PARAM, 0.5, 1), si_2 = generator.prior_distribution(Distribution::TYPE_DISTR::NORM_WITH_PARAM, 0.5, 1), b = generator.prior_distribution(Distribution::TYPE_DISTR::NORM_WITH_PARAM, 0.001, 0.001);
	Distribution::Thetha thetha_b, thetha_m, thetha_n, _curr_thetha;
	int m_index, n_index;
	m_index = rand() % (count_iter - 1);
	n_index = rand() % (count_iter - 1);
	cout << "start index:" << index << " " << n_index << " " << m_index << endl;

	while (m_index == index || n_index == index || m_index == n_index)
	{
		m_index = rand() % (count_iter - 1);
		n_index = rand() % (count_iter - 1);
		cout << "index:" << index << " " << n_index << " " << m_index << endl;

	}

	cout << "endindex:" << index << " " << n_index << " " << m_index << endl;
	thetha_b = generator.get_prev_iter_with_weight(posterior, count_iter);
	thetha_m = posterior.thetha[rand() % (count_iter - 1)];
	thetha_n = posterior.thetha[rand() % (count_iter - 1)];

	_curr_thetha = posterior.thetha[index];
	for (int i = 0; i < count_opt_param; i++)
	{
		_curr_thetha.param[i] = (int)((double)_curr_thetha.param[i] + si_1 * ((double)thetha_m.param[i] - (double)thetha_n.param[i]) + si_2 * ((double)thetha_b.param[i] - (double)_curr_thetha.param[i]) + b);
	}
	_curr_thetha.delta = _curr_thetha.delta + si_1 * (thetha_m.delta - thetha_n.delta) + si_2 * (thetha_b.delta - _curr_thetha.delta) + b;
	return _curr_thetha;
}

double Abcde::get_statistics(Parametrs::MODE _mode, Distribution::Thetha _curr_thetha, double _error, int i)
{
	double psi_curr, psi_prev;
	if (_mode == Parametrs::MODE::INIT)
	{
		psi_curr = generator.kernel_function(Distribution::TYPE_DISTR::NORM_WITH_PARAM, _error, 0.0, _curr_thetha.delta);
	    psi_prev = generator.kernel_function(Distribution::TYPE_DISTR::NORM_WITH_PARAM, posterior.error[i], 0.0, posterior.thetha[i].delta);
	}
	else
	{
        psi_curr = generator.kernel_function(Distribution::TYPE_DISTR::NORM_WITH_PARAM, _error, 0.0, posterior.delta_one);
	    psi_prev = generator.kernel_function(Distribution::TYPE_DISTR::NORM_WITH_PARAM, posterior.error[i], 0.0, posterior.delta_one);
	}
	double curr_kernel_func = 1, prev_kernel_func = 1;
	for (int j = 0; j < count_opt_param; j++)
	{
		curr_kernel_func *= generator.kernel_function(Distribution::TYPE_DISTR::NORM_WITH_PARAM, _curr_thetha.param[j], mean[j], std[j]);
		prev_kernel_func *= generator.kernel_function(Distribution::TYPE_DISTR::NORM_WITH_PARAM, posterior.thetha[i].param[j], mean[j], std[j]);
	}
	double curr_alpha = curr_kernel_func * psi_curr;
	double prev_alpha = prev_kernel_func * psi_prev;
	//double curr_alpha = ((generator.kernel_function(Distribution::TYPE_DISTR::NORM_WITH_PARAM, _curr_thetha.n, MU_N, SIGMA) * generator.kernel_function(Distribution::TYPE_DISTR::NORM_WITH_PARAM, _curr_thetha.l, MU_L,  SIGMA) * generator.kernel_function(Distribution::TYPE_DISTR::NORM_WITH_PARAM, _curr_thetha.lambda, MU_LAMBDA, SIGMA)) * psi_curr);
	//double prev_alpha = ((generator.kernel_function(Distribution::TYPE_DISTR::NORM_WITH_PARAM, posterior.thetha[i].n, MU_N, SIGMA) * generator.kernel_function(Distribution::TYPE_DISTR::NORM_WITH_PARAM, posterior.thetha[i].l, MU_L, SIGMA) * generator.kernel_function(Distribution::TYPE_DISTR::NORM_WITH_PARAM, posterior.thetha[i].lambda, MU_LAMBDA, SIGMA)) * psi_prev);

	double alpha = curr_alpha / prev_alpha;
	return alpha;
}

bool Abcde::accept_alpha(double alpha)
{
	std::random_device rd;
	std::uniform_real_distribution<> dist(0.0, 1.0);
	std::mt19937 engine(rd());
	double value = dist(engine);
	if (value > 1.0 - alpha)
		return true;
	return false;
}

void Abcde::init_posterior()
{
	posterior.thetha = new Distribution::Thetha[count_iter];
	posterior.w = new double[count_iter];
	posterior.error = new double[count_iter];
	new_posterior.thetha = new Distribution::Thetha[count_iter];
	new_posterior.w = new double[count_iter];
	new_posterior.error = new double[count_iter];
}
