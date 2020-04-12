#pragma once
#include "pch.h"

class Solution
{
private:
	Abcde main_model;
	Deep aux_model;
	double error, alpha;
	Parametrs param;
public:
	Solution(const Abcde& _main_model, const Deep& _aux_model, const Parametrs& _param);

	void run();

	void print_log(int iter);

	void copy_posterior( Distribution::Posterior& posterior_to,  Distribution::Posterior& posterior_from);

};
