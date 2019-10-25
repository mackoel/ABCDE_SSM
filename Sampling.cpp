#include "pch.h"
double qaussian(double x)
{
	double MU = 0.0;
	double SIG = 1.0;
	double M_PI = 3.14;
	return (double)((1.0 / (sqrt(2.0*M_PI*pow(SIG, 2.0)))*(exp(-(pow(x - MU, 2.0))) / (2.0*pow(SIG, 2.0)))));
}

void metropolis_gasstings(const int num_iter, double * thetha)
{
	double r = 0.0;
	double p = qaussian(r);

	srand(time(NULL));

	int i = 0;
	while (i < num_iter) {
		double rn = r + uniform_random(-1.0, 0.0); //random value from -1 to 1
		double pn = qaussian(rn);
		if (pn >= p) {
			p = pn;
			r = rn;
		}
		else {
			double u = uniform_random(0.0, 0.0); //random value from 0 to 1
			if (u < (double)(pn / p)) {
				p = pn;
				r = rn;
			}
		}
		thetha[i] = r;
		i++;
	}
}

double uniform_random(double mm, double nn)
{
	return (double)(mm + rand() / (RAND_MAX / (nn - mm + 1.0) + 1.0));
}

double max(double * value)
{
	double max = 0.0;
	int i = 0;
	for (i = 0; i < N; i++)
	{
		if (value[i] > max)
		{
			max = value[i];
		}
	}
	return max;
}
double fRand(double fmin, double fmax)
{
	double f = (double)rand() / RAND_MAX;
	return fmin + f * (fmax - fmin);
}
Thetha get_prev_iter_with_probabilities(Posterior * posterior)
{
	int i = 0;
	for (i = 0; i < N; i++) {
		double rndNumber = fRand(0, max(posterior->w));
		if (posterior->w[i] >= rndNumber) {
			return posterior->thetha[i];
		}
	}
	return posterior->thetha[0];

}
