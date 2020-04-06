#pragma once
#include "pch.h"

#define MEAN_ERROR 3
#define LOFFSET log((double)RAND_MAX)
#define MU_N 5
#define MU_L 12
#define MU_LAMBDA 5
#define SIGMA 0.5
#define SSM 7

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
		}Thetha;

		typedef struct
		{
			Thetha * thetha;
			double * w;
		}Posterior;

		double prior_distribution(TYPE_DISTR mode, const double param1 = 0.0, const double param2 = 0.0);

		double getRandomSample(double param1, double param2);

		double getNormalSample();
		
		double getNormalSampleWithParam(double mean, double var);

		double getLrand(double l);
		
		double variancy(const Posterior& posterior, const int mode, const int size);

		double max_weight(double* w, const int size);
		
		Thetha get_prev_iter_with_probabilities(const Posterior& posterior, const int size);

		double get_new_probabilities(const Posterior& posterior, Thetha thetha, const int size);

		Thetha& generate_vector_param(TYPE_DISTR mode);
};
