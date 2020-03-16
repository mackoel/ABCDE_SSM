// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
#define _CRT_SECURE_NO_WARNINGS
#ifndef PCH_H
#define PCH_H
//#include <glib.h>
//#include <gio/gio.h>
#include <iostream>
//#ifdef GIO_STANDALONE_SOURCE
//#include <gio/gfile.h>
//#else
//#include <gio/gio.h>
//#endif
#include <random>
#include <time.h>
#include <math.h>
#include <string>
#include <iostream>
using namespace std;
//#include <boost/property_tree/ptree.hpp>
//#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <boost/process.hpp>

#include "config.h"
#include "model.h"
#include "distr.h"
#include "abcde.h"
#include "deep.h"
#include "solution.h"


#endif //PCH_H
