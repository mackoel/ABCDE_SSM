#include "pch.h"

Deep::Deep() {}

Deep::Deep(const string& param)
{
	config_file = param;
	act_with_config_file();
}

Deep& Deep::operator=(const Deep& other)
{
	propTree = other.propTree;
	config_file = other.config_file;
	keys = other.keys;
	index_in_keys = other.index_in_keys;
	config_file = other.config_file;
	deep_exe = other.deep_exe;
	index_score = other.index_score;
	count_snp = other.count_snp;
	ini_mode = other.ini_mode;
	index_n = other.index_n;
	index_l = other.index_l;
	index_seed = other.index_seed;

	name_deep_opt_params = other.name_deep_opt_params;
	default_deep_opt_params = other.default_deep_opt_params;
	default_deep_opt_dparams = other.default_deep_opt_dparams;
	default_tweak_deep_opt_params = other.default_tweak_deep_opt_params;
	default_lbounds_deep_opt_params = other.default_lbounds_deep_opt_params;
	default_hbounds_deep_opt_params = other.default_hbounds_deep_opt_params;
	return *this;
}



void Deep::act_with_config_file()
{
    boost::property_tree::ini_parser::read_ini(config_file, propTree);
	deep_exe = propTree.get<std::string>("abcde.name_exe_file");
	index_score = stoi(propTree.get<std::string>("abcde.index_score"));
	count_snp = stoi(propTree.get<std::string>("abcde.count_snp"));
	index_n = stoi(propTree.get<std::string>("abcde.index_n"));
	index_l = stoi(propTree.get<std::string>("abcde.index_l"));
	index_seed = stoi(propTree.get<std::string>("abcde.index_seed"));
	ini_mode = stoi(propTree.get<std::string>("abcde.ini_mode"));


	vector<string> str_list;//, str_index;
	string s = propTree.get<std::string>("abcde.keys");
	boost::split(str_list, s, boost::is_any_of(";"));
	for (int i = 0; i < str_list.size(); i++)
	{
		keys.push_back(str_list[i]);
	}
	s = propTree.get<std::string>("abcde.index_in_keys");
	boost::split(str_list, s, boost::is_any_of(";"));
	for (int i = 0; i < str_list.size(); i++)
	{
		index_in_keys.push_back(stoi(str_list[i]));
	}
	s = propTree.get<std::string>("abcde.name_deep_opt_params");
	boost::split(str_list, s, boost::is_any_of(";"));
	for (int i = 0; i < str_list.size(); i++)
	{
		name_deep_opt_params.push_back(str_list[i]+";");
	}
	s = propTree.get<std::string>("abcde.default_deep_opt_params");
	boost::split(str_list, s, boost::is_any_of(";"));
	for (int i = 0; i < str_list.size(); i++)
	{
		default_deep_opt_params.push_back(str_list[i] + ";");
	}
	s = propTree.get<std::string>("abcde.default_deep_opt_dparams");
	boost::split(str_list, s, boost::is_any_of(";"));
	for (int i = 0; i < str_list.size(); i++)
	{
		default_deep_opt_dparams.push_back(str_list[i] + ";");
	}
	s = propTree.get<std::string>("abcde.default_tweak_deep_opt_params");
	boost::split(str_list, s, boost::is_any_of(";"));
	for (int i = 0; i < str_list.size(); i++)
	{
		default_tweak_deep_opt_params.push_back(str_list[i] + ";");
	}
	s = propTree.get<std::string>("abcde.default_lbounds_deep_opt_params");
	boost::split(str_list, s, boost::is_any_of(";"));
	for (int i = 0; i < str_list.size(); i++)
	{
		default_lbounds_deep_opt_params.push_back(str_list[i] + ";");
	}
	s = propTree.get<std::string>("abcde.default_hbounds_deep_opt_params");
	boost::split(str_list, s, boost::is_any_of(";"));
	for (int i = 0; i < str_list.size(); i++)
	{
		default_hbounds_deep_opt_params.push_back(str_list[i] + ";");
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
	cout << bp::search_path(deep_exe).string() + " --default-name=" + tmp_config_file << endl;
	bp::child c(bp::search_path(deep_exe).string() + " --default-name=" + tmp_config_file, bp::std_out > is);
	while (c.running() && std::getline(is, line) && !line.empty())
	{
		cout << line << endl;
		data.push_back(line);
	}
	c.wait();
	
    output = data.back();
	res = parse_result(output);
	std::remove(tmp_config_file.c_str());
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

void Deep::prepare_tmp_deep_ini_file(Distribution::Thetha thetha, vector<int>& dtype, int seed)
{
	vector<string> delimeters = {" ", ";", "," };
	string str;
	vector<string> split_str;
	int index = 0;
	int add_int;
	string delimeter;
	int count_phyl_param = name_deep_opt_params.size() - not_phyl_param_size;
	
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
	//add seed
	string key = "default_model.command";
	str = propTree.get<std::string>(key);
	split_str.clear();
	boost::split(split_str, str, boost::is_any_of(" "));
	split_str[index_seed] = to_string(seed);		
	string output;
	for (int i = 0; i < split_str.size(); i++)
	{
		cout << split_str[i] << endl;
		output += split_str[i];
		if (i < split_str.size() - 1)
			output += ' ';
	}
	propTree.put(key, output);
	//end add seed

	string command = propTree.get<std::string>("default_model.command");
	boost::split(split_str, command, boost::is_any_of(" "));
	int n = stoi(split_str[index_n]);
	int l = stoi(split_str[index_l]);	
	int count_param;
	if (ini_mode == INI_MODE::FUNC_AND_CROPS)
	{	
	/*	int count_param = n * l + n + n * count_snp + 5 +2;//5 - constant(srad, tmax, tmin, rain, dl), 2 - (bmin, cbd)

		string str_parts = "x;" + to_string(count_param) + ";";
		propTree.put("default_model.parts", str_parts);

		string mask_str;
		for (int i = 0; i < count_param; i++)
			mask_str += "0;";
		propTree.put("default_model.mask", mask_str);

		string parms_str;
		for (int i = 0; i < n * l; i++)
			parms_str += "200;";

		vector<string> numbers_i = { "16;", "7;", "7;", "7;", "7;", "7;", "7;" };
		int k = 0;
		for (auto c : numbers_i)
		{
			if (k < count_weather_const + count_added_param)
				parms_str += c;
			k = k + 1;
		}
		propTree.put("default_model.parms", parms_str);

		string dparms_str;
		for (int i = 0; i < n * l; i++)
			dparms_str += "200;";
		for (int i = 0; i < n + n * count_snp; i++)
			dparms_str += "5.5;";
		vector<string> numbers_d = { "16.5;", "7.1;", "7.1;", "7.1;", "7.1;", "7.1;", "7.1;" };
		k = 0;
		for (auto c : numbers_d)
		{
			if (k < count_weather_const + count_added_param)
				dparms_str += c;
			k = k + 1;
		}
		propTree.put("default_model.dparms", dparms_str);

		string lbounds_str;
		for (int i = 0; i < n * l; i++)
			lbounds_str += "0;";
		for (int i = 0; i < n + n * count_snp; i++)
			lbounds_str += "-10;";
		vector<string> numbers_l = { "15;", "0;", "6;", "1;", "0;", "1;", "1;" };
		k = 0;
		for (auto c : numbers_l)
		{
			if (k < count_weather_const + count_added_param)
				lbounds_str += c;
			k = k + 1;
		}

		propTree.put("default_model.lbound", lbounds_str);


		string hbounds_str;
		for (int i = 0; i < n * l; i++)
			hbounds_str += "1924;";
		for (int i = 0; i < n + n * count_snp; i++)
			hbounds_str += "10;";
		vector<string> numbers_h = { "30;", "15;", "15;", "15;", "20;", "100;", "7.5;" };
		k = 0;
		for (auto c : numbers_h)
		{
			if (k < count_weather_const + count_added_param)
				hbounds_str += c;
			k = k + 1;
		}
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
		for (int i = 0; i < n * l; i++)
			partype_str += "1;";
		for (int i = 0; i < n + n * count_snp; i++)
			partype_str += "0;";
		for (int i = 0; i < count_weather_const + count_added_param; i++)
			partype_str += "0;";
		propTree.put("default_model.partype", partype_str);

		write_ini(tmp_config_file, propTree);*/
	}
	else
	{
		

		count_param = n * l + n + n * count_snp +  count_phyl_param;//now weather const and added const in phyl_param - 5 - constant(srad, tmax, tmin, rain, dl) and all cbd and MB - (bmin, cbd)
		string str_parts;
		for (auto& p : name_deep_opt_params)
		{
			if (p == "x;")
				str_parts += p + to_string(n * l) + ";";
			else if (p == "beta;")
				str_parts += p + to_string(n + n * count_snp) + ";";
			else if (p == "MB;")
				str_parts += p + "1;";
			else
				str_parts += p + "1;";


		}
		propTree.put("default_model.parts", str_parts);


		string mask_str;
		for (int i = 0; i < count_param; i++)
			mask_str += "0;";
		propTree.put("default_model.mask", mask_str);

		string parms_str;
		for (int i = 0; i < n * l; i++)
			parms_str += default_deep_opt_params[0];

		for (int i = 0; i < n + n * count_snp; i++)
			parms_str += default_deep_opt_params[1];

		for (int i = 0; i < count_phyl_param; i++)
			parms_str += default_deep_opt_params[2 + i];

		propTree.put("default_model.parms", parms_str);

		string dparms_str;
		for (int i = 0; i < n * l; i++)
			dparms_str += default_deep_opt_dparams[0];

		for (int i = 0; i < n + n * count_snp; i++)
			dparms_str += default_deep_opt_dparams[1];

		for (int i = 0; i < count_phyl_param; i++)
			dparms_str += default_deep_opt_dparams[2 + i];

		propTree.put("default_model.dparms", dparms_str);

		string lbounds_str;
		for (int i = 0; i < n * l; i++)
			lbounds_str += default_lbounds_deep_opt_params[0];

		for (int i = 0; i < n + n * count_snp; i++)
			lbounds_str += default_lbounds_deep_opt_params[1];

		for (int i = 0; i < count_phyl_param; i++)
			lbounds_str += default_lbounds_deep_opt_params[2 + i];

		propTree.put("default_model.lbound", lbounds_str);



		string hbounds_str;
		for (int i = 0; i < n * l; i++)
			hbounds_str += default_hbounds_deep_opt_params[0];

		for (int i = 0; i < n + n * count_snp; i++)
			hbounds_str += default_hbounds_deep_opt_params[1];

		for (int i = 0; i < count_phyl_param-2; i++)
			hbounds_str += default_hbounds_deep_opt_params[2 + i];

		propTree.put("default_model.hbound", hbounds_str);


		string tweak_str;
		for (int i = 0; i < n * l; i++)
			tweak_str += default_tweak_deep_opt_params[0];

		for (int i = 0; i < n + n * count_snp; i++)
			tweak_str += default_tweak_deep_opt_params[1];

		for (int i = 0; i < count_phyl_param; i++)
			tweak_str += default_tweak_deep_opt_params[2 + i];

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
		for (int i = 0; i < n * l; i++)
			partype_str += "1;";
		for (int i = 0; i < n + n * count_snp  + count_phyl_param; i++)
			partype_str += "0;";
		propTree.put("default_model.partype", partype_str);

		write_ini(tmp_config_file, propTree);
	}
}
	
void Deep::create_tmp_deep_ini_file()
{
	const char* name = tmpnam(NULL);
	tmp_config_file = name;
}
