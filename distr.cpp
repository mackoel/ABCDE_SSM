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

	std::uniform_int_distribution<> distribution(param1, param2);

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

double Distribution::variancy(const Distribution::Posterior& posterior, const int mode, const int size)
{
	int i = 0, j = 0;
	int N = size;
	double s = 0.0;
	for (i = 0; i < N; i++)
	{
		if (mode == 0)
			s += posterior.thetha[i].n;
		if (mode == 1)
			s += posterior.thetha[i].l;
		if (mode == 2)
			s += posterior.thetha[i].lambda;
	}
	s = s / (double)N;
	double var = 0.0;
	for (i = 0; i < N; i++)
	{
		if (mode == 0)
			var += (posterior.thetha[i].n - s) * (posterior.thetha[i].n - s);
		if (mode == 1)
			var += (posterior.thetha[i].l - s) * (posterior.thetha[i].l - s);
		if (mode == 2)
			var += (posterior.thetha[i].lambda - s) * (posterior.thetha[i].lambda - s);
	}
	return var;
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

Distribution::Thetha Distribution::get_prev_iter_with_probabilities(const Distribution::Posterior& posterior, const int size)
{
	double prop;
	for (int i = 0; i < size; i++)
	{
		prop = prior_distribution(RANDOM, 0.0, max_weight(posterior.w, size));
		if (posterior.w[i] >= prop) {
			return posterior.thetha[i];
		}
	}
}

double Distribution::get_new_probabilities(const Distribution::Posterior& posterior, Distribution::Thetha thetha, const int size)
{
	int N = size;

	int j = 0;
	double s = 0;
	double mean, var = 0.0;
	var = variancy(posterior, 0, size) * variancy(posterior, 1, size) * variancy(posterior, 2, size);
	for (j = 0; j < N; j++)
	{
		mean = (posterior.thetha[j].l * posterior.thetha[j].n * posterior.thetha[j].lambda) / 3;
		s += posterior.w[j] * getNormalSampleWithParam(mean, var);
	}
	mean = (thetha.l * thetha.n * thetha.lambda) / 3;
	return getNormalSampleWithParam(mean, var) / s;
}

Distribution::Thetha Distribution::generate_vector_param(Distribution::TYPE_DISTR mode)
{
	Thetha thetha;
	thetha.n = (int)prior_distribution(mode, MU_N, SIGMA);
	thetha.l = (int)prior_distribution(mode, MU_L, SIGMA);
	thetha.lambda = prior_distribution(mode, MU_LAMBDA, SIGMA);
	return thetha;
}

