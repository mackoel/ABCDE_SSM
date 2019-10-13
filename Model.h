#pragma once
typedef struct
{
	double * thetha;
	double * w;
}Posterior;

void initial_init_posterior(Posterior * posterior);

double distance(double * X, double * Y);

double sufficient_statistics(const int * X);

double run_model();

double getNormalSample();

double* Model(const int mode, const double param);
double getLrand(double l);

double* ExponentionalDistribution(double param);

double getNormalSampleWithParam(double mean, double var);

double variancy(Posterior * posterior);

double get_new_probabilities(Posterior *posterior, double prev_var);

double prior_distribution(const int mode, const double param1 = 0, const double param2 = 0);