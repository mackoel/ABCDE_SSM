#pragma once
typedef struct
{
	int n;
	int l;
	double lambda;
}Thetha;
typedef struct
{
	Thetha * thetha;
	double * w;
}Posterior;


void initial_init_posterior(Posterior * posterior);

double distance(double * X, double * Y, const int mode);

double mean_distance(double * X, double * Y);


double sufficient_statistics(const int * X);

double run_model();

double getNormalSample();

double Model(const int mode, char *exe_file, char *ini_file);
double getLrand(double l);
double getRandomSample(double param1, double param2);

double* ExponentionalDistribution(double param);

double getNormalSampleWithParam(double mean, double var);

double variancy(Posterior * posterior, const int mode);

double get_new_probabilities(Posterior *posterior, Thetha thetha);

double prior_distribution(const int mode, const double param1 = 0, const double param2 = 0);

Thetha generate_vector_param(const int mode, Thetha *prev_thetha = NULL);