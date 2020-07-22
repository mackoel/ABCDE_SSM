#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

#include <random>
#include <time.h>
#include <math.h>
#include <string>
#include <iostream>
using namespace std;

#include <boost/algorithm/string.hpp>
#include<boost/algorithm/cxx11/any_of.hpp>
#include <boost/algorithm/string/join.hpp>
#include "mpi.h"

#endif //PCH_H

int main(int *argc, char* argv[])
{
	int numTask, rank;
	MPI_Init(argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numTask);
	string s = "1;2;3;" + to_string(rand()%5);
	vector<string> str;
	boost::split(str, s, boost::is_any_of(";"));
	for(int i = 0; i < str.size(); i++)
	    cout <<"Output " << numTask << " " << rank << " and number:" << str[i] << endl;
	MPI_Finalize();
}