#pragma once
#include "pch.h"
typedef struct
{
	int count_thread;
	char * name_ini_file;
	char * name_exe_file;
	char * name_command_exe_file;
	char * param_command_exe_file;
	char * curr_ini_file;
	double eps;
	int t;
}Config;

gchar* model_read_abc(char * filename, gsize*size, GError**err);
int model_load_abc(Config &config, gchar*data, gsize size, GError**err);
int read_config_file_abc(Config & config,  GError **err);

void prepare_deep_file(Config &config, Thetha &thetha);
int read_config_file_deep(Config &config, Thetha &thetha, GError **err);
int model_load_deep(Config &config, gchar*data, gsize size, Thetha &thetha, GError**err);
void print_param_and_error(const double *error, const Posterior &posterior);
gchar * create_curr_deep_ini_file(Config &config, GError **err);