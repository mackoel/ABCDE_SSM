#include "pch.h"

Deep::Deep() {}

Deep::Deep(const string& param)
{
	config_file = param;
	boost::property_tree::ini_parser::read_ini(config_file, propTree);
	deep_exe = propTree.get<std::string>("abcde.name_exe_file");
	index_score = stoi(propTree.get<std::string>("abcde.index_score"));
	count_snp = stoi(propTree.get<std::string>("abcde.count_snp"));
	count_weather_const = stoi(propTree.get<std::string>("abcde.count_weather_const"));
	count_added_param = stoi(propTree.get<std::string>("abcde.count_added_param"));
	index_n = stoi(propTree.get<std::string>("abcde.index_n"));
	index_l = stoi(propTree.get<std::string>("abcde.index_l"));
	ini_mode = stoi(propTree.get<std::string>("abcde.ini_mode"));
	vector<string> str_keys, str_index;
	string s = propTree.get<std::string>("abcde.keys");
	boost::split(str_keys, s, boost::is_any_of(";"));
	for (int i = 0; i < str_keys.size(); i++)
	{
		keys.push_back(str_keys[i]);
	}
	s = propTree.get<std::string>("abcde.index_in_keys");
	boost::split(str_index, s, boost::is_any_of(";"));
	for (int i = 0; i < str_index.size(); i++)
	{
		index_in_keys.push_back(stoi(str_index[i]));
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
	ofstream out("log_deep_work.txt", std::ios::app);

	out << "deep start" << endl;
	out << bp::search_path(deep_exe).string() + " --default-name=" + tmp_config_file << endl;
	bp::child c(bp::search_path(deep_exe).string() + " --default-name=" + tmp_config_file, bp::std_out > is);
	while (c.running() && std::getline(is, line) && !line.empty())
	{
		data.push_back(line);
	}
	c.wait();
	out << "deep end" << endl;

    output = data.back();

	res = parse_result(output);
	out.close();
	return res;
}

double Deep::parse_result(string output)
{
	const char* pattern = ":[-+]?[0-9]*\\.?[0-9]+";
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
	ofstream out("log_tmp_name_file.txt", std::ios::app);
	cout << "name tmp file is: " << tmp_config_file << endl;

	out << "name tmp file is: " << tmp_config_file << endl;
	out.close();
}
void Deep::prepare_tmp_deep_ini_file(Distribution::Thetha thetha, string exe_file, string param_exe_file, vector<int>& dtype)
{
	vector<string> delimeters = {" ", ";", "," };
	string str;
	vector<string> split_str;
	int index = 0;
	int add_int;
	string delimeter;
	for (auto& key : keys)
	{
		str = propTree.get<std::string>(key);
		for (auto& d : delimeters)
		{
			boost::split(split_str, str, boost::is_any_of(d));
			if (split_str.size() > 1)
			{
				delimeter = d;
				break;
			}

		}
		boost::split(split_str, str, boost::is_any_of(delimeter));
		if (dtype[index] == 0)
		{
			add_int = (int)thetha.param[index];
			split_str[index_in_keys[index]] = to_string(add_int);
		}
		else
		    split_str[index_in_keys[index]] = to_string(thetha.param[index]);
		
		index += 1;
		string output;
		for (int i = 0; i < split_str.size(); i++)
		{
			output += split_str[i];
			if(i < split_str.size()-1)
			    output += delimeter;
		}
		propTree.put(key, output);

	}
	string command = propTree.get<std::string>("default_model.command");
	boost::split(split_str, command, boost::is_any_of(" "));
	int n = stoi(split_str[index_n]);
	int l = stoi(split_str[index_l]);	
	int count_param = n * l + n + n * count_snp + count_weather_const + count_added_param;//5 - constant(srad, tmax, tmin, rain, dl), 2 - (bmin, cbd)
	if (ini_mode == INI_MODE::FUNC_AND_CROPS)
	{
		string str_parts = "x;" + to_string(count_param) + ";";
		propTree.put("default_model.parts", str_parts);
	}
	else
	{
		count_param = n * l + n + n * count_snp + count_weather_const + count_added_param;//5 - constant(srad, tmax, tmin, rain, dl), 2 - (bmin, cbd)
		string str_parts;
		for (auto& p : name_opt_param)
		{
			if (p == "x;")
				str_parts += p + to_string(n * l) + ";";
			else if (p == "beta;")
				str_parts += p + to_string(n + n * count_snp) + ";";
			else if (p == "concs;")
				str_parts += p + to_string(count_weather_const) + ";";
			else
				str_parts += p + "1;";


		}
		propTree.put("default_model.parts", str_parts);
	}

	string mask_str;
	for (int i = 0; i < count_param; i++)
		mask_str += "0;";
	propTree.put("default_model.mask", mask_str);

	string parms_str;
	for (int i = 0; i < n * l; i++)
		parms_str += "200;";

	vector<string> numbers_i = { "16;", "7;", "7;", "7;", "7;", "7;", "7;" };
	for (int i = 0; i < n + n * count_snp; i++)
		parms_str += "5;";
	for (auto c : numbers_i)
		parms_str += c;
	propTree.put("default_model.parms", parms_str);

	string dparms_str;
	for (int i = 0; i < n * l; i++)
		dparms_str += "200;";
	for (int i = 0; i < n + n * count_snp; i++)
		dparms_str += "5.5;";
	vector<string> numbers_d = { "16.5;", "7.1;", "7.1;", "7.1;", "7.1;", "7.1;", "7.1;" };
	for (auto c : numbers_d)
		dparms_str += c;
	propTree.put("default_model.dparms", dparms_str);

	string lbounds_str;
	for (int i = 0; i < n * l; i++)
		lbounds_str += "0;";
	for (int i = 0; i < n + n * count_snp; i++)
		lbounds_str += "-10;";
	vector<string> numbers_l = { "15;", "0;", "6;", "1;", "0;", "1;", "1;" };
	for (auto c : numbers_l)
		lbounds_str += c;
	propTree.put("default_model.lbound", lbounds_str);


	string hbounds_str;
	for (int i = 0; i < n * l; i++)
		hbounds_str += "1924;";
	for (int i = 0; i < n + n * count_snp; i++)
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
	for (int i = 0; i <n * l; i++)
		partype_str += "1;";
	for (int i = 0; i <n + n * count_snp; i++)
		partype_str += "0;";
	for (int i = 0; i < count_weather_const + count_added_param; i++)
		partype_str += "0;";
	propTree.put("default_model.partype", partype_str);

	write_ini(tmp_config_file, propTree); 
}
	

/*
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
}*/

void Deep::create_tmp_deep_ini_file()
{
	const char* name = tmpnam(NULL);
	tmp_config_file = name;
}
