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
	Solution(const Abcde& _main_model, const Deep& _aux_model) {
		main_model = _main_model;
		aux_model = _aux_model;
		error = 0.0;
		alpha = 0.0;
	}

	void run()
	{
		double error = 0.0, alpha;
		main_model.count_iter = 1;
		for (int i = 0; i < main_model.count_iter; i++)
		{
			do {
				main_model.curr_thetha = main_model.generator.generate_vector_param(Distribution::NORM_WITH_PARAM);
				aux_model.act_with_config_file();
				aux_model.prepare_tmp_deep_ini_file(main_model.curr_thetha, main_model.optimizing_model_exe, main_model.param_opt_model);
				error = aux_model.run();
			} while (error < main_model.config.eps);
			main_model.posterior.thetha[i] = main_model.curr_thetha;
			main_model.posterior.w[i] = 1.0 / main_model.count_iter;
		}

		for (int t = 0; t < main_model.t; t++)
		{
			new_posterior = main_model.posterior;
			for (int i = 0; i < main_model.count_iter; i++)
			{

				if (rand() < 0.05)
				{
					curr_thetha = main_model.mutation(i);
				}
				else
				{
					curr_thetha = main_model.crossover(i);
				}
				aux_model.act_with_config_file();
				aux_model.prepare_tmp_deep_ini_file(curr_thetha, main_model.optimizing_model_exe, main_model.param_opt_model);
				error = aux_model.run();
				alpha = main_model.get_statistics(curr_thetha, error, i);
				if (alpha > 1)
				{
					new_posterior.thetha[i] = curr_thetha;
					new_posterior.w[i] = main_model.generator.get_new_probabilities(new_posterior, curr_thetha);//change-make generator private for abcde
				}
			}
			print_log(t);
			main_model.posterior = new_posterior;
		}
	}
	void print_log(int iter)
	{
		ofstream logfile("log.txt", std::ios::app);//to end file
		logfile << "iteration = " << iter << " ";
		for (int i = 0; i < main_model.count_iter; i++)
		{
			logfile << "element number = " << i << " ";
			logfile << "n = " << main_model.posterior.thetha[i].n << " ";
			logfile << "l = " << main_model.posterior.thetha[i].l << " ";
			logfile << "lambda = " << main_model.posterior.thetha[i].lambda << " ";
			logfile << "w = " << main_model.posterior.w[i] << " ";
			logfile << endl;
		}
		logfile.close();

	}
};
