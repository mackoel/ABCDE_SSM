#pragma once
#include "pch.h"

class Solution
{
public:
	Solution(const Abcde & _main_model, const Deep &_aux_model) {
		main_model = _main_model;
		aux_model = _aux_model;
	}
	Abcde main_model;
	Deep aux_model;
	void run()
	{
		main_model.count_iter = 1;
		for (int i = 0; i < main_model.count_iter; i++)
		{
			do {
				main_model.curr_thetha = main_model.generator.generate_vector_param(Distribution::NORM_WITH_PARAM);
				aux_model.act_with_config_file();
				aux_model.prepare_tmp_deep_ini_file(main_model.curr_thetha, main_model.optimizing_model_exe, main_model.param_opt_model);
				aux_model.run();

			} while (main_model.error[i] < main_model.config.eps);
			main_model.posterior.thetha[i] = main_model.curr_thetha;
			main_model.posterior.w[i] = 1.0 / main_model.count_iter;
		}
	}
};