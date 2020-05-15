#include "pch.h"

Deep::Deep() {}

Deep::Deep(const string& param)
{
	config_file = param;
	boost::property_tree::ini_parser::read_ini(config_file, propTree);
	deep_exe = propTree.get<std::string>("data.name_exe_file");
	index_score = stoi(propTree.get<std::string>("data.index_score"));
	vector<string> str_param, str_index;
	boost::split(str_param, propTree.get<std::string>("data.num_string_when_add_param"), boost::is_any_of(";"));
	for (int i = 0; i < str_param.size(); i++)
	{
		num_string_when_add_param.push_back(stoi(str_param[i]));
	}
	boost::split(str_index, propTree.get<std::string>("data.index_opt_param_in_command_line"), boost::is_any_of(";"));
	for (int i = 0; i < str_index.size(); i++)
	{
		index_opt_param_in_command_line.push_back(stoi(str_index[i]));
		index_opt_param_in_vector_param.push_back(i);
	}

	
}


double Deep::run()
{
	double res;
	namespace bp = boost::process;
	bp::ipstream is; 
	string output;
	std::vector<std::string> data;
	std::string line;
	cout << "deep start" << endl;
	cout << bp::search_path(deep_exe).string() + " --default-name=" + tmp_config_file << endl;
	bp::child c(bp::search_path(deep_exe).string() + " --default-name=" + tmp_config_file, bp::std_out > is);
	while (c.running() && std::getline(is, line) && !line.empty())
	{
		data.push_back(line);
	}
	c.wait();
	cout << "deep end" << endl;

    output = data.back();

	res = parse_result(output);
	return res;
}

double Deep::parse_result(string output)
{
	const char* pattern = ":[-+]?[0-9]*\.?[0-9]+";
	boost::regex re(pattern);
	int i = 1;
	boost::sregex_iterator it(output.begin(), output.end(), re);
	boost::sregex_iterator end;

	for (; it != end; ++it)
	{
		if(i == index_score)
		{
			return stod(it->str().erase(0, 1));
		}
		i++;
	}
}

void Deep::act_with_config_file()
{
	create_tmp_deep_ini_file();
	cout << "name tmp file is: " << tmp_config_file << endl;
}
bool if_param_change_ini(int i)
{
	if (i == 5 || i == 7)
	{
		return true;
	}
	return false;
}

bool if_param_not_change_ini(int i)
{
	if (i != 5 && i != 7)
	{
		return true;
	}
	return false;
}
void Deep::prepare_tmp_deep_ini_file(Distribution::Thetha thetha, string exe_file, string param_exe_file)/////надо соотнести важные инедксы(5 и 7) с их позициями в векторе параметров
{
	if (boost::algorithm::any_of(index_opt_param_in_command_line, if_param_change_ini) == true)
	{
		cout << "change" << endl;

	}
	if (boost::algorithm::any_of(index_opt_param_in_command_line, if_param_not_change_ini) == true)
	{
		cout << "and not change" << endl;

	}

	string str_lambda = "penalty;readpenalty;2;2;" + to_string(int(thetha.lambda)) + ";";
	propTree.put("default_target.l1_pen", str_lambda);

	string str_n_l = exe_file + " -P 1 -N " + to_string(int(thetha.n)) + " -L " + to_string(int(thetha.l)) + " " + param_exe_file + "\0";
	propTree.put("default_model.command", str_n_l);

	int count_param = thetha.n * thetha.l + thetha.n + thetha.n * 18 + 5 + 2;
	string str_parts = "x;" + to_string(count_param) + ";";
	propTree.put("default_model.parts", str_parts);

	string mask_str;
	for (int i = 0; i < count_param; i++)
		mask_str += "0;";
	propTree.put("default_model.mask", mask_str);

	string parms_str;
	for (int i = 0; i < thetha.n * thetha.l; i++)
		parms_str += "200;";

	vector<string> numbers_i = { "16;", "7;", "7;", "7;", "7;", "7;", "7;" };
	for (int i = 0; i < thetha.n + thetha.n * 18; i++)
		parms_str += "5;";
	for (auto c : numbers_i)
		parms_str += c;
	propTree.put("default_model.parms", parms_str);

	string dparms_str;
	for (int i = 0; i < thetha.n * thetha.l; i++)
		dparms_str += "200;";
	for (int i = 0; i < thetha.n + thetha.n * 18; i++)
		dparms_str += "5.5;";
	vector<string> numbers_d = { "16.5;", "7.1;", "7.1;", "7.1;", "7.1;", "7.1;", "7.1;" };
	for (auto c : numbers_d)
		dparms_str += c;
	propTree.put("default_model.dparms", dparms_str);

	string lbounds_str;
	for (int i = 0; i < thetha.n * thetha.l; i++)
		lbounds_str += "0;";
	for (int i = 0; i < thetha.n + thetha.n * 18; i++)
		lbounds_str += "-10;";
	vector<string> numbers_l = { "15;", "0;", "6;", "1;", "0;", "1;", "1;" };
	for (auto c : numbers_l)
		lbounds_str += c;
	propTree.put("default_model.lbound", lbounds_str);


	string hbounds_str;
	for (int i = 0; i < thetha.n * thetha.l; i++)
		hbounds_str += "1924;";
	for (int i = 0; i < thetha.n + thetha.n * 18; i++)
		hbounds_str += "10;";
	vector<string> numbers_h = { "30;", "15;", "15;", "15;", "20;", "100;", "7.5;" };
	for (auto c : numbers_h)
		hbounds_str += c;
	propTree.put("default_model.hbound", hbounds_str);


	string tweak_str;
	for (int i = 0; i < count_param; i++)
		tweak_str += "1;";
	propTree.put("default_model.tweak", tweak_str);

	string limited_str;
	for (int i = 0; i < count_param; i++)
		limited_str += "1;";
	propTree.put("default_model.limited", limited_str);

	string scale_str;
	for (int i = 0; i < count_param; i++)
		scale_str += "1;";
	propTree.put("default_model.scale", scale_str);

	string partype_str;
	for (int i = 0; i < thetha.n * thetha.l; i++)
		partype_str += "1;";
	for (int i = 0; i < thetha.n + thetha.n * 18; i++)
		partype_str += "0;";
	for (int i = 0; i < 5 + 2; i++)
		partype_str += "0;";
	propTree.put("default_model.partype", partype_str);

	write_ini(tmp_config_file, propTree);
}

void Deep::create_tmp_deep_ini_file()
{
	const char* name = tmpnam(NULL);
	tmp_config_file = name;
}
