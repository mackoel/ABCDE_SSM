#pragma once
#include "pch.h"

#define MEAN_ERROR 3
#define LOFFSET log((double)RAND_MAX)
#define MU_N 5
#define MU_L 12
#define MU_LAMBDA 5
#define SIGMA 0.5
#define SSM 7
//parametrs for mutation
#define MU_N_MUT 0.75
#define MU_L_MUT 2.25
#define MU_LAMBDA_MUT 0.75

class Distribution
{
    public:
	    enum TYPE_DISTR
        {
	        NORM,
	        EXPON,
	        NORM_WITH_PARAM,
	        RANDOM
        };

		typedef struct
		{
			int n;
			int l;
			double lambda;
		//	vector<variant<int, double>> param;
		//	vector<boost::any> param;
			vector<double> param;
			double delta;
		}Thetha;

		typedef struct
		{
			Thetha * thetha;
			double * w;
			double* error;
			double delta_one;
		}Posterior;


		double prior_distribution(TYPE_DISTR mode, const double param1 = 0.0, const double param2 = 0.0);

		double getRandomSample(double param1, double param2);

		double getNormalSample();
		
		double getNormalSampleWithParam(double mean, double var);

		double getLrand(double l);
		
		double variancy(const Posterior& posterior, const int mode, const int size);

		double max_weight(double* w, const int size);
		
		Thetha get_prev_iter_with_weight(const Posterior& posterior, const int size);

		double get_new_weight(Distribution::Thetha& prev_thetha, Distribution::Thetha& curr_thetha, const int count_opt_param, vector<double>& mean, vector<double>& std);

		Thetha generate_vector_param(TYPE_DISTR mode, int count_opt_param, vector<double>& mean, vector<double>& std);

		double erf(double x);


		double kernelNormalSampleWithParam(double x, double mean, double var);
		
		double kernel_function(TYPE_DISTR mode, double x, const double param1 = 0.0, const double param2 = 0.0);
};
