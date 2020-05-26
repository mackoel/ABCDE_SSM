#include "pch.h"


Solution::Solution(const Abcde& _main_model, const Deep& _aux_model, const Parametrs& _param) {
	main_model = _main_model;
	aux_model = _aux_model;
	param = _param;
	error = 0.0;
	alpha = 0.0;
}
inline void Solution::copy_posterior(Distribution::Posterior &posterior_to,Distribution::Posterior& posterior_from)
{
	Distribution::Thetha * tmp_thetha = posterior_to.thetha;
	double* tmp_w = posterior_to.w;
	posterior_to.thetha = posterior_from.thetha;
	posterior_from.thetha = tmp_thetha;
	posterior_to.w = posterior_from.w;
	posterior_from.w = tmp_w;
}
void Solution::run()//
{
	//ofstream out("log_iteration.txt", std::ios::app);
	cout << "RUN" << endl;
	for (int i = 0; i < main_model.count_iter; i++)
	{
		do {
			main_model.curr_thetha = main_model.bounds(main_model.generator.generate_vector_param(Distribution::NORM_WITH_PARAM, main_model.count_opt_param, main_model.mean, main_model.std));
			aux_model.act_with_config_file();
			aux_model.prepare_tmp_deep_ini_file(main_model.curr_thetha, main_model.optimizing_model_exe, main_model.param_opt_model, main_model.dtype);
			error = aux_model.run();
			cout << "error = " << error << endl;
		} while (error < main_model.config.eps);
		main_model.posterior.thetha[i] = main_model.curr_thetha;
		main_model.posterior.w[i] = 1.0 / main_model.count_iter;
		main_model.posterior.error[i] = error;
		main_model.new_posterior.thetha[i] = main_model.curr_thetha;
		main_model.new_posterior.w[i] = 1.0 / main_model.count_iter;
		main_model.new_posterior.error[i] = error;
		main_model.posterior.thetha[i].delta = main_model.new_posterior.thetha[i].delta = main_model.generator.prior_distribution(Distribution::TYPE_DISTR::EXPON, 0.005);

	}
	print_log(-1);
	for (int t = 0; t < main_model.start_iter; t++)
	{
		for (int i = 0; i < main_model.count_iter; i++)
		{
			double choice = main_model.generator.prior_distribution(Distribution::TYPE_DISTR::RANDOM, 0.0, 1.0);
			cout << "choice var = " << choice << endl;
			if (choice < 0.1)
			{
				cout << "mutation: ";
				main_model.curr_thetha = main_model.mutation(i);
				main_model.curr_thetha.delta = main_model.generator.prior_distribution(Distribution::TYPE_DISTR::EXPON, 0.005);

			}
			else
			{
				cout << "crossover: ";

				main_model.curr_thetha = main_model.crossover(i);
				main_model.curr_thetha.delta = main_model.generator.prior_distribution(Distribution::TYPE_DISTR::EXPON, 0.005);

			}
		//	cout << main_model.curr_thetha.n << " ";
		//	cout << main_model.curr_thetha.l << " ";
		//	cout << main_model.curr_thetha.lambda << endl;

			aux_model.act_with_config_file();
			aux_model.prepare_tmp_deep_ini_file(main_model.curr_thetha, main_model.optimizing_model_exe, main_model.param_opt_model, main_model.dtype);
			error = aux_model.run();

			cout << "error = " << error << endl;
			alpha = main_model.get_statistics(Parametrs::MODE::INIT, main_model.curr_thetha, error, i);

			cout << "alpha original = " << alpha << endl;
			alpha = min(1.0, alpha);
			cout << "alpha after = " << alpha << endl;
			if (main_model.accept_alpha(alpha))
			{
				cout << "accept" << endl;
				main_model.new_posterior.thetha[i] = main_model.curr_thetha;
				main_model.new_posterior.w[i] = main_model.generator.get_new_weight(main_model.posterior.thetha[i], main_model.curr_thetha, main_model.count_opt_param, main_model.mean, main_model.std);//change-make generator private for abcde
				main_model.new_posterior.error[i] = error;
				main_model.normalize_weights();
			}
			else
				cout << "not accept" << endl;
		}
		cout << "begin_copy" << endl;

		copy_posterior(main_model.posterior, main_model.new_posterior);
		cout << "done_copy" << endl;

	}
	if (main_model.mode == Parametrs::DELTA_MODE::MEAN)
	{
		double s = 0.0;
		for (int i = 0; i < main_model.count_iter; i++)
		{
			s += main_model.posterior.thetha[i].delta;
		}
		main_model.posterior.delta_one = s / main_model.count_iter;
	}
	else if (main_model.mode == Parametrs::DELTA_MODE::MED)
	{
		//
	}
	cout << "MAIN ITERATION" << endl;
	for (int t = 0; t < main_model.t; t++)
	{
		if (t != 0)
		    copy_posterior(main_model.new_posterior, main_model.posterior);
		for (int i = 0; i < main_model.count_iter; i++)
		{
			double choice = main_model.generator.prior_distribution(Distribution::TYPE_DISTR::RANDOM, 0.0, 1.0);
			cout << "choice var = " << choice << endl;
			if (choice < 0.1)
			{
				cout << "mutation: ";
				main_model.curr_thetha = main_model.mutation(i);
			}
			else
			{
				cout << "crossover: ";

				main_model.curr_thetha = main_model.crossover(i);
			}
			//cout << main_model.curr_thetha.n << " ";
		//	cout << main_model.curr_thetha.l << " ";
		//	cout << main_model.curr_thetha.lambda << endl;

			aux_model.act_with_config_file();
			aux_model.prepare_tmp_deep_ini_file(main_model.curr_thetha, main_model.optimizing_model_exe, main_model.param_opt_model, main_model.dtype);
			error = aux_model.run();
			cout << "error = " << error << endl;

			alpha = main_model.get_statistics(Parametrs::MODE::AUX, main_model.curr_thetha, error, i);
			cout << "alpha original = " << alpha << endl;
			alpha = min(1.0, alpha);
			cout << "alpha after = " << alpha << endl;
			if (main_model.accept_alpha(alpha))
			{
				cout << "accept" << endl;
				main_model.new_posterior.thetha[i] = main_model.curr_thetha;
				main_model.new_posterior.w[i] = main_model.generator.get_new_weight(main_model.posterior.thetha[i], main_model.curr_thetha, main_model.count_opt_param, main_model.mean, main_model.std);//change-make generator private for abcde
				main_model.new_posterior.error[i] = error;
				main_model.normalize_weights();

			}
			else
				cout << "not accept" << endl;
		}
		print_log(t);
		cout << "begin_copy" << endl;

		copy_posterior(main_model.posterior, main_model.new_posterior);
		cout << "done_copy" << endl;

	}
	//out.close();
}

void Solution::print_log(int iter)
{
	ofstream logfile("log_result.txt", std::ios::app);
	logfile << "iteration = " << iter << " ";
	for (int i = 0; i < main_model.count_iter; i++)
	{
		logfile << "element number = " << i << " ";
		for (int j = 0; j < main_model.count_opt_param; j++)
		{
			logfile << "param[" << j << "] = " << main_model.posterior.thetha[i].param[j] << " ";
		}
		logfile << "w = " << main_model.posterior.w[i] << " ";
		logfile << endl;
	}
	logfile.close();
}
