#include "pch.h"


double Distribution::prior_distribution(Distribution::TYPE_DISTR mode, const double param1, const double param2)
{
	if (mode == NORM)
	{
		return getNormalSample();
	}
	if (mode == NORM_WITH_PARAM)
	{
		return getNormalSampleWithParam(param1, param2);//double x = normalRandom()*sigma+Mi;
	}
	if (mode == EXPON)
	{
		return getLrand(param1);
	}
	if (mode == RANDOM)
	{
		return getRandomSample(param1, param2);
	}
	return getRandomSample(param1, param2);
}

double Distribution::getRandomSample(double param1, double param2)
{
	std::random_device random_device;
	std::mt19937 generator(random_device());
	std::uniform_real_distribution<double> distribution(param1, param2);
	double x = distribution(generator);
	return x;
}
double Distribution::getNormalSample()
{
	double u = ((double)rand() / (RAND_MAX)) * 2 - 1;
	double v = ((double)rand() / (RAND_MAX)) * 2 - 1;
	double r = u * u + v * v;
	if (r == 0 || r > 1) return getNormalSample();
	double c = sqrt(-2 * log(r) / r);
	return u * c;
}

double Distribution::getNormalSampleWithParam(double mean, double var)
{
	std::random_device mch;
	std::default_random_engine gen(mch());
	std::normal_distribution<double> d(mean, var);
	return d(gen);
}

double Distribution::getLrand(double l)
{
	double u;
	u = rand() / (RAND_MAX + 1.0);
	return -log(1 - u) / l;
}


double Distribution::max_weight(double* w, const int size)
{
	double max_w = -1.0;
	for (int i = 0; i < size; i++)
	{
		if (w[i] > max_w)
			max_w = w[i];
	}
	return max_w;
}

Distribution::Thetha Distribution::get_prev_iter_with_weight(const Distribution::Posterior& posterior, const int size)
{
	double prop;
	for (int i = 0; i < size; i++)
	{
		prop = prior_distribution(RANDOM, 0.0, max_weight(posterior.w, size));
		if (posterior.w[i] >= prop) {
			return posterior.thetha[i];
		}
	}
	return posterior.thetha[0];
}

double Distribution::get_new_weight(Distribution::Posterior& posterior, Distribution::Thetha& curr_thetha, const int count_opt_param, const int count_iter, vector<double>& mean, vector<double>& std)
{
	double phi = 1.0, sum = 0.0, norm;
	for (int i = 0; i < count_opt_param; i++)
	{
		phi *= kernel_function(Distribution::TYPE_DISTR::NORM_WITH_PARAM, curr_thetha.param[i], mean[i], std[i]);
	}
	for (int i = 0; i < count_iter; i++)
	{
		norm = 1.0;
		for (int j = 0; j < count_opt_param; j++)
		{
			norm *= kernel_function(Distribution::TYPE_DISTR::NORM_WITH_PARAM, posterior.thetha[i].param[j], 0.0, curr_thetha.param[j]);
		}
		sum += posterior.w[i] * norm;
	}
	return phi / sum;
}

Distribution::Thetha Distribution::generate_vector_param(Distribution::TYPE_DISTR mode, int count_opt_param, vector<double>& mean, vector<double>& std)//
{
	Thetha thetha;
	for (int i = 0; i < count_opt_param; i++)
	{
    	thetha.param.push_back(prior_distribution(mode, mean[i], std[i]));
	}
	return thetha;
}

double Distribution::kernelNormalSampleWithParam(double x, double mean, double var)
{
	normal norm(mean, var);
	return pdf(norm, x);
}

double Distribution::kernel_function(Distribution::TYPE_DISTR mode, double x, const double param1, const double param2)
{
	if (mode == NORM_WITH_PARAM)
	{
		return kernelNormalSampleWithParam(x, param1, param2);
	}
	return kernelNormalSampleWithParam(x, param1, param2);
}
