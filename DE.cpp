#include "pch.h"


double migration(Posterior * posterior)
{
	return 0.0;
}

Thetha crossover(Posterior * posterior, int i)
{
	double si_1 =prior_distribution(NORM_WITH_PARAM, 0.5, 1), si_2 = prior_distribution(NORM_WITH_PARAM, 0.5, 1), b = prior_distribution(NORM_WITH_PARAM, 0.001, 0.001);
	Thetha thetha_b, thetha_m, thetha_n, curr_thetha;
	thetha_b = get_prev_iter_with_probabilities(posterior);
	thetha_m = posterior->thetha[rand() % (N + 1)];
	thetha_n = posterior->thetha[rand() % (N + 1)];

	curr_thetha = posterior->thetha[i];
	curr_thetha.n = curr_thetha.n + si_1 * (thetha_m.n - thetha_n.n) + si_2 * (thetha_b.n - curr_thetha.n) + b;
	curr_thetha.l = curr_thetha.l + si_1 * (thetha_m.l - thetha_n.l) + si_2 * (thetha_b.l - curr_thetha.l) + b;
	curr_thetha.lambda = curr_thetha.lambda + si_1 * (thetha_m.lambda - thetha_n.lambda) + si_2 * (thetha_b.lambda - curr_thetha.lambda) + b;
	return curr_thetha;
}

Thetha mutation(Posterior * posterior, int i)
{
	Thetha curr_thetha = posterior->thetha[i];
	curr_thetha.l = curr_thetha.l + (int)prior_distribution(NORM, MU_N, SIGMA);
	curr_thetha.n = curr_thetha.n + (int)prior_distribution(NORM, MU_L, SIGMA);
	curr_thetha.lambda = curr_thetha.lambda + prior_distribution(NORM, MU_LAMBDA, SIGMA);
	return curr_thetha;
}
