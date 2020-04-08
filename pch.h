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

using boost::property_tree::ptree;
namespace pt = boost::property_tree;

#include "config.h"
#include "model.h"
#include "distr.h"
#include "abcde.h"
#include "deep.h"
#include "solution.h"


#endif //PCH_H
