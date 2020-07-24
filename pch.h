#define _CRT_SECURE_NO_WARNINGS
#ifndef PCH_H
#define PCH_H

#include <iostream>

#include <random>
#include <time.h>
#include <math.h>
#include <string>
#include <iostream>
using namespace std;

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/process.hpp>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include<boost/regex.hpp>
#include<boost/algorithm/cxx11/any_of.hpp>
#include <boost/algorithm/string/join.hpp>
#include <iostream>

using boost::property_tree::ptree;
namespace pt = boost::property_tree;
namespace po = boost::program_options;

#include "config.h"
#include "model.h"
#include "distr.h"
#include "abcde.h"
#include "deep.h"
#include "run_manager.h"
#include "solution.h"
#include "mpi.h"


#endif //PCH_H
