#include "pch.h"

Abcde::Abcde() {}

Abcde::Abcde(const string& param)
{
	config_file = param;
	act_with_config_file();
	init_posterior();
	error = new double[count_iter];
}

void Abcde::normalize_weights()
{
	double sum = 0.0;
	for (int i = 0; i < count_iter; i++)
	{
		sum += new_posterior.w[i];
	}
	for (int i = 0; i < count_iter; i++)
	{
		new_posterior.w[i] = new_posterior.w[i] / sum;
	}
}
void  Abcde::act_with_config_file()
{
	boost::property_tree::ptree pt;
	boost::property_tree::ini_parser::read_ini(config_file, pt);
	count_thread = stoi(pt.get<std::string>("abcde.count_thread"));
	deep_exe = pt.get<std::string>("abcde.name_exe_file");
	eps = stod(pt.get<std::string>("abcde.eps"));
	t = stoi(pt.get<std::string>("abcde.t"));
	count_iter = stoi(pt.get<std::string>("abcde.count_iter"));
	start_iter = stoi(pt.get<std::string>("abcde.start_iter"));
	mode_delta = stoi(pt.get<std::string>("abcde.mode_delta"));
	count_opt_param = stoi(pt.get<std::string>("abcde.count_opt_param"));
	vector<string> str_mean, str_std, str_hbound, str_lbound, str_dtype;
	string s = pt.get<std::string>("abcde.mean");
	boost::split(str_mean, s, boost::is_any_of(";"));
	for (int i = 0; i < str_mean.size(); i++)
	{
		mean.push_back(stod(str_mean[i]));
	}
	s = pt.get<std::string>("abcde.std");
	boost::split(str_std, s, boost::is_any_of(";"));
	for (int i = 0; i < str_std.size(); i++)
	{
		std.push_back(stod(str_std[i]));
	}
	s = pt.get<std::string>("abcde.hbound");
	boost::split(str_hbound, s, boost::is_any_of(";"));
	for (int i = 0; i < str_hbound.size(); i++)
	{
		hbound.push_back(stod(str_hbound[i]));

	}
	s = pt.get<std::string>("abcde.lbound");
	boost::split(str_lbound, s, boost::is_any_of(";"));
	for (int i = 0; i < str_lbound.size(); i++)
	{
		lbound.push_back(stod(str_lbound[i]));
	}
	s = pt.get<std::string>("abcde.dtype");
	boost::split(str_dtype, s, boost::is_any_of(";"));
	for (int i = 0; i < str_dtype.size(); i++)
	{
		dtype.push_back(stoi(str_dtype[i]));
	}
}

Distribution::Thetha Abcde::mutation(int index)
{
	Distribution::Thetha _curr_thetha = posterior.thetha[index];
	for (int i = 0; i < count_opt_param; i++)
	{
		_curr_thetha.param[i] = _curr_thetha.param[i] + generator.prior_distribution(Distribution::TYPE_DISTR::NORM, mean[count_opt_param-1], std[count_opt_param - 1]);// another mean for 
	}
	_curr_thetha.delta = _curr_thetha.delta + generator.prior_distribution(Distribution::TYPE_DISTR::EXPON, 0.005);
	return bounds(_curr_thetha);
}
Distribution::Thetha Abcde::bounds(Distribution::Thetha _curr_thetha)
{
	Distribution::Thetha thetha;
	double alpha, beta, delta;
	double q;

	for (int i = 0; i < count_opt_param; i++)
	{
		alpha = (hbound[i] + lbound[i]) / 2.0;
		beta = (hbound[i] - lbound[i]) / 2.0;
		q = alpha + beta * sin(_curr_thetha.param[i]);
		thetha.param.push_back(q);
	}
	alpha = (0.0 + 5.0) / 2.0;
	beta = (5.0 - 0.0) / 2.0;
	q = q = alpha + beta * sin(_curr_thetha.delta);
	thetha.delta = q;
	return thetha;
}
Distribution::Thetha Abcde::crossover(int index)
{
	double si_1 = generator.prior_distribution(Distribution::TYPE_DISTR::NORM_WITH_PARAM, 0.5, 1), si_2 = generator.prior_distribution(Distribution::TYPE_DISTR::NORM_WITH_PARAM, 0.5, 1), b = generator.prior_distribution(Distribution::TYPE_DISTR::NORM_WITH_PARAM, 0.001, 0.001);
	Distribution::Thetha thetha_b, thetha_m, thetha_n, _curr_thetha;
	int m_index, n_index;
	m_index = rand() % (count_iter - 1);
	n_index = rand() % (count_iter - 1);

	while (m_index == index)
	{
		m_index = rand() % (count_iter - 1);
	}
	while (m_index == index || m_index == n_index)
	{
		n_index = rand() % (count_iter - 1);
	}
	thetha_b = generator.get_prev_iter_with_weight(posterior, count_iter);
	thetha_m = posterior.thetha[m_index];
	thetha_n = posterior.thetha[n_index];

	_curr_thetha = posterior.thetha[index];
	for (int i = 0; i < count_opt_param; i++)
	{
		_curr_thetha.param[i] = ((double)_curr_thetha.param[i] + si_1 * ((double)thetha_m.param[i] - (double)thetha_n.param[i]) + si_2 * ((double)thetha_b.param[i] - (double)_curr_thetha.param[i]) + b);
	}
	_curr_thetha.delta = _curr_thetha.delta + si_1 * (thetha_m.delta - thetha_n.delta) + si_2 * (thetha_b.delta - _curr_thetha.delta) + b;
	return bounds(_curr_thetha);
}

double Abcde::get_statistics(Parametrs::MODE _mode, Distribution::Thetha _curr_thetha, double _error, int i)
{
	double psi_curr, psi_prev;

	if (_mode == Parametrs::MODE::INIT)
	{
		psi_curr = generator.kernel_function(Distribution::TYPE_DISTR::NORM_WITH_PARAM, _error, 0.0, _curr_thetha.delta);
	    psi_prev = generator.kernel_function(Distribution::TYPE_DISTR::NORM_WITH_PARAM, posterior.error[i], 0.0, posterior.thetha[i].delta);
		cout << psi_curr << endl;
		cout << psi_prev << endl;
	}
	else
	{
        psi_curr = generator.kernel_function(Distribution::TYPE_DISTR::NORM_WITH_PARAM, _error, 0.0, posterior.delta_one);
	    psi_prev = generator.kernel_function(Distribution::TYPE_DISTR::NORM_WITH_PARAM, posterior.error[i], 0.0, posterior.delta_one);
	}
	double curr_kernel_func = 1.0, prev_kernel_func = 1.0;
	for (int j = 0; j < count_opt_param; j++)
	{
		curr_kernel_func *= generator.kernel_function(Distribution::TYPE_DISTR::NORM_WITH_PARAM, _curr_thetha.param[j], mean[j], std[j]);
		prev_kernel_func *= generator.kernel_function(Distribution::TYPE_DISTR::NORM_WITH_PARAM, posterior.thetha[i].param[j], mean[j], std[j]);
	}
	double curr_alpha = curr_kernel_func * psi_curr;
	double prev_alpha = prev_kernel_func * psi_prev;
	if (prev_alpha == 0.0)
		return 1.0;
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
