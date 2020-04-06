#pragma once
#include "pch.h"

class Solution
{
private:
	Abcde main_model;
	Deep aux_model;
	Distribution::Posterior new_posterior;
	Distribution::Thetha curr_thetha;
	double error, alpha;

public:
	Solution(const Abcde& _main_model, const Deep& _aux_model);

	void run();

	void print_log(int iter);

	void copy_posterior(const Distribution::Posterior& posterior_to, const Distribution::Posterior& posterior_from);

};
