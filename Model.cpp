#include "pch.h"
/*Only from distibution:
...Ditrubution - return double*
get.. - return double
*/
void initial_init_posterior(Posterior * posterior)
{
	int i = 0;
	posterior->thetha = (double*)malloc(sizeof(double) * N);//WHY I DO IT DYNAMIC???MAYBE CHANGE IT TO STATIC
	posterior->w = (double*)malloc(sizeof(double) * N);
//	for (i; i < N; i++)
//	{
//		posterior->thetha[i] = getNormalSample();
//		posterior->w[i] = 1 / N;
//	}
}

double distance(double * X, double * Y)
{
	int i = 0;
	double mean_X = 0, mean_Y = 0;
	for (i; i < N; i++)
	{
		mean_X += X[i];
		mean_Y += Y[i];
	}
	mean_X /= N;
	mean_Y /= N;
	return abs(mean_X - mean_Y);
}

double sufficient_statistics(const int * X)
{
	return 0.0;
}

double run_model()
{
	return 0.0;
}
double prior_distribution(const int mode, const double param1 = 0, const double param2 = 0)
{
	if (mode == NORM)
	{
		return getNormalSample();
	}
	if(mode == NORM_WITH_PARAM)
	{
		return getNormalSampleWithParam(param1, param2);//double x = normalRandom()*sigma+Mi;
	}
	if (mode == EXPON)
	{
		return getLrand(param1);
	}
	//return ERROR_MODE;
}
double getNormalSample()
{
	double u = ((double)rand() / (RAND_MAX)) * 2 - 1;
	double v = ((double)rand() / (RAND_MAX)) * 2 - 1;
	double r = u * u + v * v;
	if (r == 0 || r > 1) return getNormalSample();
	double c = sqrt(-2 * log(r) / r);
	return u * c;
}

double getNormalSampleWithParam(double mean, double var)//is this gaussian kernel?
{
	std::default_random_engine generator;
	std::normal_distribution<double> distribution(mean, var);
	return distribution(generator);
}

double getLrand(double l)
{
	double u;
	u = rand() / (RAND_MAX + 1.0);
	return -log(1 - u) / l;
}
double* ExponentionalDistribution(double param)//C++ FUNCTION TO CHECK - DELETE AFTER CHECK
{
	/*std::default_random_engine generator;
	std::exponential_distribution<double> distribution(param);*/
	int i = 0;
	double *number = (double*)malloc(sizeof(double) * N);
	srand((unsigned)time(NULL));
	for (i; i < N; i++)
	{
		//number[i] = distribution(generator);
		number[i] = getLrand(param);
	}
	return number;
	
}
double* Model(const int mode, const double param)
{
	if (mode == EXPON)
	{
		return ExponentionalDistribution(param);
	}
}

double variancy(Posterior *posterior)
{
	int i = 0, j = 0;
	double s = 0;
	for (i; i < N; i++)
	{
		double in_s = 0;
		for (j; j < N; j++)
		{
			in_s += posterior->thetha[j];
		}
		in_s /= N;
		s += posterior->thetha[i] - in_s;
	}
	return 2 * 1 / N * (s * s);
}

double get_new_probabilities(Posterior * posterior, double prev_var)
{
	int j = 0;
	double s = 0;
	for (j; j < N; j++)
	{
		s += posterior->w[j] * getNormalSampleWithParam(posterior->thetha[j], prev_var);
	}
	return getNormalSample() / s;////pi(thetha(i)/s))
}
