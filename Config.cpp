#include "pch.h"

void write_deep_config(const char * deep_file, double thetha)
{
}

void write_to_file(double * data, const char *file_name)
{
	int i = 0;
	FILE *file;
	fopen_s(&file, file_name, "wt");
	//std::cout << file_name << std::endl;
	for (i = 0; i < N; i++) {
	//	std::cout << data[i] << std::endl;
		fprintf(file, "%f\n", data[i]);
	}
	fclose(file);
}

void read_from_file()
{

}
