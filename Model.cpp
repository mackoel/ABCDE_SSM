#include "pch.h"
/*Only from distibution:
...Ditrubution - return double*
get.. - return double
*/
void initial_init_posterior(Posterior * posterior)
{
	int i = 0;
	posterior->thetha = (Thetha*)malloc(sizeof(Thetha) * N);
	posterior->w = (double*)malloc(sizeof(double) * N);
}

double mean_distance(double * X, double * Y)
{
	int i = 0;
	double mean_X = 0, mean_Y = 0;
	for (i; i < N; i++)
	{
		mean_X += X[i];
		mean_Y += Y[i];
	}
	mean_X /= N;
	mean_Y /= N;
	return abs(mean_X - mean_Y);
}

double distance(double * X, double * Y, const int mode)
{
	if (mode == MEAN_ERROR)
		return mean_distance(X, Y);
}


double sufficient_statistics(const int * X)
{
	return 0.0;
}

double run_model()
{
	return 0.0;
}
double prior_distribution(const int mode, const double param1, const double param2)
{
	if (mode == NORM)
	{
		return getNormalSample();
	}
	if(mode == NORM_WITH_PARAM)
	{
		return getNormalSampleWithParam(param1, param2);//double x = normalRandom()*sigma+Mi;
	}
	if (mode == EXPON)
	{
		return getLrand(param1);
	}
	if (mode == RANDOM)
	{
		return getRandomSample(param1, param2);
	}
	//return ERROR_MODE;
}
double getRandomSample(double param1, double param2)
{
	std::random_device random_device; 
	std::mt19937 generator(random_device()); 

	std::uniform_int_distribution<> distribution(param1, param2); // Равномерное распределение [10, 20]

	double x = distribution(generator); 
	return x;
}
double getNormalSample()
{
	double u = ((double)rand() / (RAND_MAX)) * 2 - 1;
	double v = ((double)rand() / (RAND_MAX)) * 2 - 1;
	double r = u * u + v * v;
	if (r == 0 || r > 1) return getNormalSample();
	double c = sqrt(-2 * log(r) / r);
	return u * c;
}

double getNormalSampleWithParam(double mean, double var)//is this gaussian kernel?
{
	//return var * getNormalSample() + mean;
	std::random_device mch;
	std::default_random_engine gen(mch());
	std::normal_distribution<double> d(mean, var);
	return d(gen);
}

double getLrand(double l)
{
	double u;
	u = rand() / (RAND_MAX + 1.0);
	return -log(1 - u) / l;
}
double* ExponentionalDistribution(double param)//C++ FUNCTION TO CHECK - DELETE AFTER CHECK
{
	/*std::default_random_engine generator;
	std::exponential_distribution<double> distribution(param);*/
	int i = 0;
	double *number = (double*)malloc(sizeof(double) * N);
//	srand((unsigned)time(NULL));
	for (i; i < N; i++)
	{
		//number[i] = distribution(generator);
		number[i] = getLrand(param);
	}
	return number;
	
}

gchar ** Model_launch_exe(char * exe_file, char *ini_file)
{
	gchar**result;
	gchar**margv;
	int argcp, i, j;
	GString *command;
	GError*gerror = NULL;
	int flaggs, child_exit_status = 1;
	gchar *standard_output;
	gchar *standard_error;
	gchar*conversion;
	double max_value = G_MAXDOUBLE;
	char* _command = new char[strlen(exe_file) + strlen(ini_file) + strlen("C:/project/SSM/ABCDE_SSM/ABCDE_SSM/") + strlen("--default-name=") +3];//если станет другое число больше символов - ошибка
	strcpy(_command, exe_file);
	strcat(_command, " --default-name=");
	strcat(_command, "C:/project/SSM/ABCDE_SSM/ABCDE_SSM/");
	strcat(_command, ini_file);
	std::cout << _command << std::endl;
	command = g_string_new(_command);
	if (!g_shell_parse_argv(command->str, &argcp, &margv, &gerror)) {
		if (gerror) {
			g_error("g_shell_parse failed for %s\nwith %s", command->str, gerror->message);
		}
	}
	
	g_string_free(command, TRUE);
	flaggs = G_SPAWN_SEARCH_PATH;

	if (!g_spawn_sync(NULL, margv, NULL, (GSpawnFlags)flaggs, NULL, NULL, &standard_output, &standard_error, &child_exit_status, &gerror)) {
		if (gerror) {
			g_error("g_spawn_sync failed for %s\nwith %s", margv[0], gerror->message);
		}
	}

	g_strfreev(margv);
	result = g_strsplit_set(standard_output, "\n", -1);
	std::cout << result << std::endl;
	int result_length = g_strv_length(result);
	int parsing_failed = 0;
	g_strfreev(result);
	g_free(standard_output);
	g_free(standard_error);
	delete[] _command;
	return result;

}
double Model(const int mode, char *exe_file, char *ini_file)
{

	if (mode == SSM)
	{
		gchar** res =  Model_launch_exe(exe_file, ini_file);
		return 0.0;
		//to double
	}
}

double variancy(Posterior *posterior, const int mode)
{
	/*int i = 0, j = 0;
	double s = 0;
	for (i=0; i < N; i++)
	{
		double in_s = 0;
		for (j = 0; j < N; j++)
		{
			in_s += posterior->thetha[j]/(double)N;
		}
		s += posterior->thetha[i] - in_s;
	}
	return (2.0 * (1.0 / (double)N)) * (s * s);*/
	int i = 0, j = 0;
	double s = 0.0;
	for (i = 0; i < N; i++)
	{
		if(mode == 0)
		    s += posterior->thetha[i].n;
		if(mode == 1)
			s += posterior->thetha[i].l;
		if (mode == 2)
			s += posterior->thetha[i].lambda;
	}
	s = s / (double)N;
	double var = 0.0;
	for (i = 0; i < N; i++)
	{
		if (mode == 0)
		    var += (posterior->thetha[i].n - s) * (posterior->thetha[i].n - s);
		if (mode == 1)
			var += (posterior->thetha[i].l - s) * (posterior->thetha[i].l - s);
		if (mode == 2)
			var += (posterior->thetha[i].lambda - s) * (posterior->thetha[i].lambda - s);
	}
	return var;
}

double get_new_probabilities(Posterior * posterior, Thetha thetha)//ИСПРАВИТЬ
{
	int j = 0;
	double s = 0;
	double mean, var;
	var = variancy(posterior, 0) + variancy(posterior, 1) + variancy(posterior, 2);
	for (j = 0; j < N; j++)
	{
		mean = (posterior->thetha[j].l + posterior->thetha[j].n + posterior->thetha[j].lambda) / 3;
		s += posterior->w[j] * getNormalSampleWithParam(mean, var);
	}
	mean = (thetha.l + thetha.n + thetha.lambda) / 3;
	return getNormalSampleWithParam(mean, var) / s;////pi(thetha(i)/s))
}

Thetha generate_vector_param(const int mode, Thetha *prev_thetha)
{
	Thetha thetha;

	if (prev_thetha == NULL)
	{

		thetha.n = (int)prior_distribution(mode, MU_N, SIGMA);
		thetha.l = (int)prior_distribution(mode, MU_L, SIGMA);
		thetha.lambda = prior_distribution(mode, MU_LAMBDA, SIGMA);
		std::cout << "Take this param ";
		std::cout << thetha.n << " ";
		std::cout << thetha.l << std::endl;
	}
	else
	{
		thetha.n = (int)prior_distribution(mode, prev_thetha->n, 0);
		thetha.l = (int)prior_distribution(mode, prev_thetha->l, 1);
		thetha.lambda = prior_distribution(mode, prev_thetha->lambda, 2);
	}
	return thetha;
}