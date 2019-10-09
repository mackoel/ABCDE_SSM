#pragma once

typedef struct
{
	int N;
	int L;
	double lambda;//take this lambda for exp distribution
}config_deep;

typedef struct
{
	int N;//count iterations
}config_abcde;

void write_deep_config(const char * deep_file, double thetha);

void write_to_file(double * data, const char * file_name);

void read_from_file();
