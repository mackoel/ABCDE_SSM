#pragma once
#include "pch.h"
typedef struct
{
	int count_thread;
	char * name_deep_ini_file;
	char * name_exe_file;
	double eps;
	int t;
}Config;

double * read_real_data(const char * FILE_NAME = "real_data.txt");
gchar* model_read_abc(gchar*filename, gsize*size, GError**err);
int model_load_abc(Config *config, gchar*data, gsize size, GError**err);
int read_config_file_abc(Config * config_file, gchar *filename, GError **err);

void prepare_deep_file(Config config, Thetha thetha);
int read_config_file_deep(Config config_file, Thetha thetha, GError **err);
int model_load_deep(Config config, gchar*data, gsize size, Thetha thetha, GError**err);

void create_curr_deep_ini_file(char * name_ini_file, char * default_deep_ini_file, int iter);