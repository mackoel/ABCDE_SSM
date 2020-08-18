#include "pch.h"


Solution::Solution(const Abcde& _main_model, const Deep& _aux_model, const Parametrs& _param) {
	main_model = _main_model;
	aux_model = _aux_model;
	param = _param;
	alpha = 0.0;
}
inline void Solution::copy_posterior(Distribution::Posterior& posterior_to, Distribution::Posterior& posterior_from)
{
	for (int i = 0; i < main_model.count_iter; i++)
	{
		posterior_to.thetha[i] = posterior_from.thetha[i];
		posterior_to.w[i] = posterior_from.w[i];
		posterior_to.error[i] = posterior_from.error[i];
	}
}

void Solution::run_manager()
{
	manager.read_log_file(main_model.posterior, main_model.new_posterior, main_model.norm_error, main_model.count_iter, main_model.count_opt_param);
	switch (manager.state)
	{
	case Run_manager::STATE::INIT:
		run_init(manager.iter, manager.index_thetha);
		break;
	case Run_manager::STATE::RUN_APPROXIMATE:
		run_approximate(manager.iter, manager.index_thetha);
		break;
	case Run_manager::STATE::RUN:
		run(manager.iter, manager.index_thetha);
		break;
	}
}

void Solution::run_init(int iter, int index_thetha)
{
#ifdef MPIZE
	int tag = 0;
	int rank;
#endif
	int size = 1;
#ifdef MPIZE
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	cout << "This rank is " << rank << endl;
	cout << "This size is " << size << endl;

	MPI_Status status;
	if (rank == 0)
	{
#endif
		ofstream out("log_iteration.txt", std::ios::app);
		out << "INIT" << endl;
		vector<Distribution::Thetha> all_thetha;
		for (int j = 0; j < size; j++)
		{
			vector<vector<double>> _param;
			for (int i = 0; i < main_model.count_iter / size; i++)
			{
				main_model.curr_thetha = main_model.generate_vector_param(Distribution::NORM_WITH_PARAM);
				_param.push_back(main_model.curr_thetha.param);
				all_thetha.push_back(main_model.curr_thetha);
			}
#ifdef MPIZE
			if (j != 0)
			{
				for (int k = 0; k < main_model.count_iter / size; k++)
					MPI_Send(&_param[k].front(), main_model.count_opt_param, MPI_DOUBLE, j, tag, MPI_COMM_WORLD);
			}
#endif
		}
		for (int i = 0; i < main_model.count_iter / size; i++)
		{
			double error;
			main_model.curr_thetha = all_thetha[i];
			aux_model.create_tmp_deep_ini_file();
			int seed = main_model.generator.generate_seed();
			aux_model.prepare_tmp_deep_ini_file(main_model.bounds(main_model.curr_thetha), main_model.dtype, seed);
			error = aux_model.run(0, i);
			if (i == 0)
				main_model.norm_error = error;
			main_model.posterior.thetha[i] = main_model.curr_thetha;
			main_model.posterior.w[i] = 1.0 / main_model.count_iter;
			main_model.posterior.error[i] = error / main_model.norm_error;
			main_model.new_posterior.thetha[i] = main_model.curr_thetha;
			main_model.new_posterior.w[i] = 1.0 / main_model.count_iter;
			main_model.new_posterior.error[i] = error / main_model.norm_error;
			main_model.posterior.thetha[i].delta = main_model.new_posterior.thetha[i].delta = main_model.generator.prior_distribution(Distribution::TYPE_DISTR::EXPON, 0.005);
		}
#ifdef MPIZE
		for (int j = 1; j < size; j++)
		{
			vector<double> error;
			error.resize(main_model.count_iter / size);
			MPI_Recv(&error[0], main_model.count_iter / size, MPI_DOUBLE, j, tag, MPI_COMM_WORLD, &status);
			for (int i = 0; i < main_model.count_iter / size; i++)
			{
				main_model.posterior.thetha[j * main_model.count_iter / (size)+i] = all_thetha[j * main_model.count_iter / (size)+i];
				main_model.posterior.w[j * main_model.count_iter / (size)+i] = 1.0 / main_model.count_iter;
				main_model.posterior.error[j * main_model.count_iter / (size)+i] = error[i] / main_model.norm_error;
				main_model.new_posterior.thetha[j * main_model.count_iter / (size)+i] = all_thetha[j * main_model.count_iter / (size)+i];
				main_model.new_posterior.w[j * main_model.count_iter / (size)+i] = 1.0 / main_model.count_iter;
				main_model.new_posterior.error[j * main_model.count_iter / (size)+i] = error[i] / main_model.norm_error;
				main_model.posterior.thetha[j * main_model.count_iter / (size)+i].delta = main_model.new_posterior.thetha[j * main_model.count_iter / (size)+i].delta = main_model.generator.prior_distribution(Distribution::TYPE_DISTR::EXPON, 0.005);
			}
		}
#endif
		for (int i = 0; i < main_model.count_iter; i++)
			manager.create_log_file(manager.state, main_model.posterior, main_model.new_posterior, main_model.norm_error, -1, i, main_model.count_opt_param);
		main_model.set_sample_dist_param();
		print_log(-1);
		manager.state = Run_manager::STATE::RUN_APPROXIMATE;
		manager.change_state(manager.state);
		out.close();
		run_approximate(0, 0);
#ifdef MPIZE
	}
	else
	{
		vector<vector<double>> _param(main_model.count_iter / size, vector<double>(main_model.count_opt_param));
		vector<double>error;
		for (int k = 0; k < main_model.count_iter / size; k++)
			MPI_Recv(&_param[k][0], main_model.count_opt_param, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
		for (int i = 0; i < main_model.count_iter / size; i++)
		{
			Distribution::Thetha curr_thetha;
			curr_thetha.param = _param[i];
			int seed = main_model.generator.generate_seed();
			aux_model.create_tmp_deep_ini_file();
			aux_model.prepare_tmp_deep_ini_file(main_model.bounds(curr_thetha), main_model.dtype, seed);
			error.push_back(aux_model.run(rank, i));
		}
		MPI_Send(&error[0], main_model.count_iter / size, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
		run_approximate(0, 0);
	}
#endif
}

void Solution::run_approximate(int iter, int index_thetha)
{
#ifdef MPIZE
	int tag = 0;
	int rank;
#endif
	int size = 1;
#ifdef MPIZE
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Status status;
	if (rank == 0)
	{
#endif
		ofstream out("log_iteration.txt", std::ios::app);
		out << "RUN_APPROXIMATE" << endl;
		for (int t = iter; t < main_model.start_iter; t++)
		{
			vector<Distribution::Thetha> all_thetha;
			for (int j = 0; j < size; j++)
			{
				vector<vector<double>> _param;

				for (int i = 0; i < main_model.count_iter / size; i++)
				{
					double choice = main_model.generator.prior_distribution(Distribution::TYPE_DISTR::RANDOM, 0.0, 1.0);
					out << "choice = " <<choice << endl;
					if (choice < 0.05)
					{
						out << "mutation" << endl;
						main_model.curr_thetha = main_model.mutation(i);
						out << "mutation end" << endl;
					}
					else
					{
						out << "crossover" << endl;
						main_model.curr_thetha = main_model.crossover(i);
						out << "crossover end" << endl;
					}
					_param.push_back(main_model.curr_thetha.param);
					all_thetha.push_back(main_model.curr_thetha);
				}
#ifdef MPIZE
				if (j != 0)
				{
					for (int k = 0; k < main_model.count_iter / size; k++)
						MPI_Send(&_param[k].front(), main_model.count_opt_param, MPI_DOUBLE, j, tag, MPI_COMM_WORLD);
				}
#endif
			}
			for (int i = 0; i < main_model.count_iter / (size); i++)
			{
				double error;
				main_model.curr_thetha = all_thetha[i];
				for (int s = 0; s < main_model.count_opt_param; s++)
					out << main_model.curr_thetha.param[s] << endl;
				int seed = main_model.generator.generate_seed();
				aux_model.create_tmp_deep_ini_file();
				aux_model.prepare_tmp_deep_ini_file(main_model.bounds(main_model.curr_thetha), main_model.dtype, seed);
				error = aux_model.run(0, i);
				out << "error ready" << error << endl;
				alpha = main_model.get_statistics(Parametrs::MODE::INIT, error / main_model.norm_error, i);
				out << "original alpha = " << alpha << endl;
				alpha = min(1.0, alpha);
				out << "alpha = " << alpha << endl;
				if (main_model.accept_alpha(alpha))
				{
					out << "accept alpha"<< endl;
					main_model.new_posterior.thetha[i] = main_model.curr_thetha;
					main_model.new_posterior.error[i] = error / main_model.norm_error;
				}
				else
					out << "not accept" << endl;
			}
#ifdef MPIZE
			for (int j = 1; j < size; j++)
			{
				vector<double> error(main_model.count_iter / (size));
				MPI_Recv(&error[0], main_model.count_iter / size, MPI_DOUBLE, j, tag, MPI_COMM_WORLD, &status);
				for (int i = 0; i < main_model.count_iter / size; i++)
				{
					main_model.curr_thetha = all_thetha[j * main_model.count_iter / (size)+i];
					for (int s = 0; s < main_model.count_opt_param; s++)
						out << main_model.curr_thetha.param[s] << endl;
					alpha = main_model.get_statistics(Parametrs::MODE::INIT, error[i] / main_model.norm_error, j * main_model.count_iter / (size)+i);
					out << "original alpha = " << alpha << endl;
					alpha = min(1.0, alpha);
					out << "alpha = " << alpha << endl;
					if (main_model.accept_alpha(alpha))
					{
						out << "accept alpha" << endl;
						main_model.new_posterior.thetha[j * main_model.count_iter / (size)+i] = main_model.curr_thetha;
						main_model.new_posterior.error[j * main_model.count_iter / (size)+i] = error[i] / main_model.norm_error;
					}
					else
						out << "not accept" << endl;
				}
			}
#endif
			main_model.update_posterior();
			copy_posterior(main_model.posterior, main_model.new_posterior);
			for (int i = 0; i < main_model.count_iter; i++)
				manager.create_log_file(manager.state, main_model.posterior, main_model.new_posterior, main_model.norm_error, t, i, main_model.count_opt_param);
			print_log(t);
		}
		double s = 0.0;
		for (int i = 0; i < main_model.count_iter; i++)
		{
			s += main_model.posterior.thetha[i].delta;
		}
		main_model.posterior.delta_one = s / main_model.count_iter;
		main_model.new_posterior.delta_one = main_model.posterior.delta_one;
		manager.state = Run_manager::STATE::RUN;
		manager.change_state(manager.state);
		manager.change_delta(main_model.posterior.delta_one);
		out.close();
		run(0, 0);
#ifdef MPIZE
	}
	else
	{
		for (int t = iter; t < main_model.start_iter; t++)
		{
			vector<vector<double>> _param(main_model.count_iter / size, vector<double>(main_model.count_opt_param));
			vector<double> error;
			for (int k = 0; k < main_model.count_iter / size; k++)
				MPI_Recv(&_param[k][0], main_model.count_opt_param, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
			for (int i = 0; i < main_model.count_iter / (size); i++)
			{
				Distribution::Thetha curr_thetha;
				curr_thetha.param = _param[i];
				int seed = main_model.generator.generate_seed();
				aux_model.create_tmp_deep_ini_file();
				aux_model.prepare_tmp_deep_ini_file(main_model.bounds(curr_thetha), main_model.dtype, seed);
				error.push_back(aux_model.run(rank, i));
			}
			MPI_Send(&error[0], main_model.count_iter / size, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);

		}
		run(0, 0);
	}
#endif
}

void Solution::run(int iter, int index_thetha)
{
#ifdef MPIZE
	int tag = 0;
	int rank;
#endif
	int size = 1;
#ifdef MPIZE
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Status status;
	if (rank == 0)
	{
#endif
		ofstream out("log_iteration.txt", std::ios::app);
		out << "RUN" << endl;
		for (int t = iter; t < main_model.t; t++)
		{
			vector<Distribution::Thetha> all_thetha;
			for (int j = 0; j < size; j++)
			{
				vector<vector<double>> _param;

				for (int i = 0; i < main_model.count_iter / size; i++)
				{
					double choice = main_model.generator.prior_distribution(Distribution::TYPE_DISTR::RANDOM, 0.0, 1.0);
					if (choice < 0.05)
						main_model.curr_thetha = main_model.mutation(i);
					else
						main_model.curr_thetha = main_model.crossover(i);
					_param.push_back(main_model.curr_thetha.param);
					all_thetha.push_back(main_model.curr_thetha);
				}
#ifdef MPIZE
				if (j != 0)
				{
					for (int k = 0; k < main_model.count_iter / size; k++)
						MPI_Send(&_param[k].front(), main_model.count_opt_param, MPI_DOUBLE, j, tag, MPI_COMM_WORLD);
				}
#endif
			}
			for (int i = 0; i < main_model.count_iter / (size); i++)
			{
				double error;
				main_model.curr_thetha = all_thetha[i];
				int seed = main_model.generator.generate_seed();
				aux_model.create_tmp_deep_ini_file();
				aux_model.prepare_tmp_deep_ini_file(main_model.bounds(main_model.curr_thetha), main_model.dtype, seed);
				error = aux_model.run(0, i);
				for (int s = 0; s < main_model.count_opt_param; s++)
					out << main_model.curr_thetha.param[s] << endl;
				alpha = main_model.get_statistics(Parametrs::MODE::INIT, error / main_model.norm_error, i);
				out << "original alpha = " << alpha << endl;
				alpha = min(1.0, alpha);
				out << "alpha = " << alpha << endl;
				if (main_model.accept_alpha(alpha))
				{
					out << "accept alpha" << endl;
					main_model.new_posterior.thetha[i] = main_model.curr_thetha;
					main_model.new_posterior.error[i] = error / main_model.norm_error;
				}
				else
					out << "not accept" << endl;
			}
#ifdef MPIZE
			for (int j = 1; j < size; j++)
			{
				vector<double> error(main_model.count_iter / (size));
				MPI_Recv(&error[0], main_model.count_iter / size, MPI_DOUBLE, j, tag, MPI_COMM_WORLD, &status);
				for (int i = 0; i < main_model.count_iter / size; i++)
				{
					main_model.curr_thetha = all_thetha[j * main_model.count_iter / (size)+i];
					for (int s = 0; s < main_model.count_opt_param; s++)
						out << main_model.curr_thetha.param[s] << endl;
					alpha = main_model.get_statistics(Parametrs::MODE::AUX, error[i] / main_model.norm_error, j * main_model.count_iter / (size)+i);
					out << "original alpha = " << alpha << endl;
					alpha = min(1.0, alpha);
					out << "alpha = " << alpha << endl;
					if (main_model.accept_alpha(alpha))
					{
						out << "accept alpha" << endl;
						main_model.new_posterior.thetha[j * main_model.count_iter / (size)+i] = main_model.curr_thetha;
						main_model.new_posterior.error[j * main_model.count_iter / (size)+i] = error[i] / main_model.norm_error;
					}
					else
						out << "not accept" << endl;
				}
			}
#endif
			main_model.update_posterior();
			copy_posterior(main_model.posterior, main_model.new_posterior);
			for (int i = 0; i < main_model.count_iter; i++)
				manager.create_log_file(manager.state, main_model.posterior, main_model.new_posterior, main_model.norm_error, t, i, main_model.count_opt_param);
			print_log(t);
		}
		manager.state = Run_manager::STATE::END;
		manager.change_state(manager.state);
		out.close();
#ifdef MPIZE
	}
	else
	{
		for (int t = iter; t < main_model.t; t++)
		{
			vector<vector<double>> _param(main_model.count_iter / size, vector<double>(main_model.count_opt_param));
			vector<double> error;
			for (int k = 0; k < main_model.count_iter / size; k++)
				MPI_Recv(&_param[k][0], main_model.count_opt_param, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD, &status);
			for (int i = 0; i < main_model.count_iter / (size); i++)
			{
				Distribution::Thetha curr_thetha;
				curr_thetha.param = _param[i];
				int seed = main_model.generator.generate_seed();
				aux_model.create_tmp_deep_ini_file();
				aux_model.prepare_tmp_deep_ini_file(main_model.bounds(curr_thetha), main_model.dtype, seed);
				error.push_back(aux_model.run(rank, i));
			}
			MPI_Send(&error[0], main_model.count_iter / size, MPI_DOUBLE, 0, tag, MPI_COMM_WORLD);
		}
	}
#endif
}

void Solution::print_log(int iter)
{
	ofstream logfile("log_result.txt", std::ios::app);
	logfile << "iteration = " << iter << endl;
	int print;
	for (int i = 0; i < main_model.count_iter; i++)
	{
		logfile << "element number = " << i << " ";
		for (int j = 0; j < main_model.count_opt_param; j++)
		{
			if (main_model.dtype[j] == 0)
			{
				print = (int)main_model.posterior.thetha[i].param[j];
				logfile << "param[" << j << "] = " << print << " ";

			}
			else
				logfile << "param[" << j << "] = " << main_model.posterior.thetha[i].param[j] << " ";
		}
		logfile << "w = " << main_model.posterior.w[i] << " ";
		logfile << "error = " << main_model.posterior.error[i] << " ";
		logfile << "delta = " << main_model.posterior.thetha[i].delta << " ";
		logfile << endl;
	}
	logfile.close();
}
