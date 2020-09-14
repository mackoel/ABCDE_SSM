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

void Abcde::set_sample_dist_param()
{
	double sum;
	for (int i = 0; i < count_opt_param; i++)
	{
		sum = 0.0;
		for (int j = 0; j < count_iter; j++)
		{
			sum += posterior.thetha[j].param[i];
		}
		sample_mean[i] = sum / count_iter;
		sum = 0.0;
		for (int j = 0; j < count_iter; j++)
		{
			sum += (posterior.thetha[j].param[i] - sample_mean[i]) * (posterior.thetha[j].param[i] - sample_mean[i]);
		}
		sample_std[i] = sum / (count_iter - 1);
	}
}


double Abcde::max_weight(double* w)
{
	double max_w = -1.0;
	for (int i = 0; i < count_iter; i++)
	{
		if (w[i] > max_w)
			max_w = w[i];
	}
	return max_w;
}

Distribution::Thetha Abcde::get_prev_iter_with_weight()
{
	double prop;
	for (int i = 0; i < count_iter; i++)
	{
		prop = generator.prior_distribution(Distribution::TYPE_DISTR::RANDOM, 0.0, max_weight(posterior.w));
		if (posterior.w[i] >= prop) {
			return posterior.thetha[i];
		}
	}
	return posterior.thetha[0];
}

double Abcde::set_new_weight()
{
	double phi = 1.0, sum = 0.0, norm;
	for (int i = 0; i < count_opt_param; i++)
	{
		phi *= generator.kernel_function(Distribution::TYPE_DISTR::NORM_WITH_PARAM, curr_thetha.param[i], sample_mean[i], sample_std[i]);
	}
	for (int i = 0; i < count_iter; i++)
	{
		norm = 1.0;
		for (int j = 0; j < count_opt_param; j++)
		{
			//norm *= generator.kernel_function(Distribution::TYPE_DISTR::NORM_WITH_PARAM, posterior.thetha[i].param[j], curr_thetha.param[j], 2.0 * sample_std[j]);
			norm *= generator.kernel_function(Distribution::TYPE_DISTR::NORM_WITH_PARAM, curr_thetha.param[j], posterior.thetha[i].param[j], 2.0 * sample_std[j]);
		}
		sum += posterior.w[i] * norm;
	}
	return phi / sum;
}

Distribution::Thetha Abcde::generate_vector_param(Distribution::TYPE_DISTR mode)
{
	Distribution::Thetha thetha;
	for (int i = 0; i < count_opt_param; i++)
	{
		thetha.param.push_back(generator.prior_distribution(mode, mean[i], std[i]));
	}
	return thetha;
}


void Abcde::update_posterior()
{
	set_sample_dist_param();//previous iter posterior
	for(int i = 0; i < count_iter; i++)
	{
		curr_thetha = new_posterior.thetha[i];
		new_posterior.w[i] = set_new_weight();
	}
	normalize_weights();
}
void  Abcde::act_with_config_file()
{
	boost::property_tree::ptree pt;
	boost::property_tree::ini_parser::read_ini(config_file, pt);
	count_thread = stoi(pt.get<std::string>("abcde.count_thread"));
	deep_exe = pt.get<std::string>("abcde.name_exe_file");
	t = stoi(pt.get<std::string>("abcde.t"));
	count_iter = stoi(pt.get<std::string>("abcde.count_iter"));
	start_iter = stoi(pt.get<std::string>("abcde.start_iter"));
	count_opt_param = stoi(pt.get<std::string>("abcde.count_opt_param"));
	bounds_crossing_mode = stoi(pt.get<std::string>("abcde.bounds_crossing_mode"));
	vector<string> str_mean, str_std, str_hbound, str_lbound, str_dtype;
	string s = pt.get<std::string>("abcde.mean");
	boost::split(str_mean, s, boost::is_any_of(";"));
	for (int i = 0; i < str_mean.size(); i++)
	{
		mean.push_back(stod(str_mean[i]));
		sample_mean.push_back(stod(str_mean[i]));
	}
	s = pt.get<std::string>("abcde.std");
	boost::split(str_std, s, boost::is_any_of(";"));
	for (int i = 0; i < str_std.size(); i++)
	{
		std.push_back(stod(str_std[i]));
		sample_std.push_back(stod(str_std[i]));
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
	return _curr_thetha;
}

double Abcde::set_bounds(double x, double _lbound, double _hbound)
{
	double alpha, beta, q;
	alpha = (_hbound + _lbound) / 2.0;
	beta = (_hbound - _lbound) / 2.0;
	if (bounds_crossing_mode == BOUNDS_CROSSING_MODE::SIN)
		q = alpha + beta * sin(x);
	else
		q = alpha + beta * tanh(x);
	return q;
}

Distribution::Thetha Abcde::bounds(Distribution::Thetha _curr_thetha)
{
	Distribution::Thetha thetha;
	for (int i = 0; i < count_opt_param; i++)
	{
		thetha.param.push_back(set_bounds(_curr_thetha.param[i], lbound[i], hbound[i]));
	}
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
	thetha_b = get_prev_iter_with_weight();
	thetha_m = posterior.thetha[m_index];
	thetha_n = posterior.thetha[n_index];

	_curr_thetha = posterior.thetha[index];
	for (int i = 0; i < count_opt_param; i++)
	{
		_curr_thetha.param[i] = ((double)_curr_thetha.param[i] + si_1 * ((double)thetha_m.param[i] - (double)thetha_n.param[i]) + si_2 * ((double)thetha_b.param[i] - (double)_curr_thetha.param[i]) + b);
	}
	_curr_thetha.delta = _curr_thetha.delta + si_1 * (thetha_m.delta - thetha_n.delta) + si_2 * (thetha_b.delta - _curr_thetha.delta) + b;
	return _curr_thetha;
}

double Abcde::get_statistics(Parametrs::MODE _mode,  double _error, int i)
{
	double psi_curr, psi_prev;
	if (_mode == Parametrs::MODE::INIT)
	{
		psi_curr = generator.kernel_function(Distribution::TYPE_DISTR::NORM_WITH_PARAM, _error, 0.0, set_bounds(curr_thetha.delta, 0.0, 5.0));
	    psi_prev = generator.kernel_function(Distribution::TYPE_DISTR::NORM_WITH_PARAM, posterior.error[i], 0.0, set_bounds(posterior.thetha[i].delta, 0.0, 5.0));
	}
	else
	{
        psi_curr = generator.kernel_function(Distribution::TYPE_DISTR::NORM_WITH_PARAM, _error, 0.0, set_bounds(posterior.delta_one, 0.0, 5.0));
	    psi_prev = generator.kernel_function(Distribution::TYPE_DISTR::NORM_WITH_PARAM, posterior.error[i], 0.0, set_bounds(posterior.delta_one, 0.0, 5.0));
	}
	double curr_kernel_func = 1.0, prev_kernel_func = 1.0;
	for (int j = 0; j < count_opt_param; j++)
	{
	//	curr_kernel_func *= generator.kernel_function(Distribution::TYPE_DISTR::NORM_WITH_PARAM, curr_thetha.param[j], sample_mean[j], sample_std[j]);
	//	prev_kernel_func *= generator.kernel_function(Distribution::TYPE_DISTR::NORM_WITH_PARAM, posterior.thetha[i].param[j], sample_mean[j], sample_std[j]);
		curr_kernel_func *= generator.kernel_function(Distribution::TYPE_DISTR::NORM_WITH_PARAM, curr_thetha.param[j], posterior.thetha[i].param[j], sample_std[j]);
		prev_kernel_func *= generator.kernel_function(Distribution::TYPE_DISTR::NORM_WITH_PARAM, posterior.thetha[i].param[j], curr_thetha.param[j], sample_std[j]);
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
