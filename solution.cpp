#include "pch.h"


Solution::Solution(const Abcde& _main_model, const Deep& _aux_model) {
	main_model = _main_model;
	aux_model = _aux_model;
	error = 0.0;
	alpha = 0.0;
}
void Solution::copy_posterior(Distribution::Posterior &posterior_to,Distribution::Posterior& posterior_from)
{
	Distribution::Thetha * tmp_thetha = posterior_to.thetha;
	double* tmp_w = posterior_to.w;
	posterior_to.thetha = posterior_from.thetha;
	posterior_from.thetha = tmp_thetha;
	posterior_to.w = posterior_from.w;
	posterior_from.w = tmp_w;
}
void Solution::run()
{
	for (int i = 0; i < main_model.count_iter; i++)
	{
		do {
			main_model.curr_thetha = main_model.generator.generate_vector_param(Distribution::NORM_WITH_PARAM);
			cout << "curr_thetha: ";
			cout << main_model.curr_thetha.n << " ";
			cout << main_model.curr_thetha.l << " ";
			cout << main_model.curr_thetha.lambda << endl;
			aux_model.act_with_config_file();
			aux_model.prepare_tmp_deep_ini_file(main_model.curr_thetha, main_model.optimizing_model_exe, main_model.param_opt_model);
			error = aux_model.run();
			cout << "error = " << error << endl;
		} while (error < main_model.config.eps);
		main_model.posterior.thetha[i] = main_model.curr_thetha;
		main_model.posterior.w[i] = 1.0 / main_model.count_iter;

	}
	print_log(-1);
	//current_values in Soultion, result_value in Abcde
	for (int t = 0; t < main_model.t; t++)
	{
		copy_posterior(main_model.new_posterior, main_model.posterior);
		for (int i = 0; i < main_model.count_iter; i++)
		{
			if (rand() < 0.05)
			{
				main_model.curr_thetha = main_model.mutation(i);
			}
			else
			{
				main_model.curr_thetha = main_model.crossover(i);
			}
			aux_model.act_with_config_file();
			aux_model.prepare_tmp_deep_ini_file(main_model.curr_thetha, main_model.optimizing_model_exe, main_model.param_opt_model);
			error = aux_model.run();
			alpha = main_model.get_statistics(main_model.curr_thetha, error, i);
			if (alpha > 1)
			{
				main_model.new_posterior.thetha[i] = main_model.curr_thetha;
				main_model.new_posterior.w[i] = main_model.generator.get_new_probabilities(main_model.new_posterior, main_model.curr_thetha, main_model.count_iter);//change-make generator private for abcde
			}
		}
		print_log(t);
		copy_posterior(main_model.posterior, main_model.new_posterior);
	}
}

void Solution::print_log(int iter)
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
