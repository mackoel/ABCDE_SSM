#pragma once
#include "pch.h"

class Deep:public Model
{
public:
	Deep(){}
	Deep(const string &param)
	{
		config_file = param;
	}
	
	double run()
	{
		double res;
		namespace bp = boost::process;
		boost::filesystem::path p = bp::search_path("deepmethod.exe");
		string output;
		bp::system(p, "--default-name=" + tmp_config_file, bp::std_out > output);
		parse_result(output);
		return res;
	}
	double parse_result(string output)
	{
		string score_str = "score:";
		string fmean_str = "fmean:";
		int ind = output.find(score_str);
		cout << ind << endl;
		string res = output.substr(ind + score_str.size(), output.find(fmean_str) - ind - score_str.size());
		return stod(res);
	}
	string config_file;
	string tmp_config_file;
	double error;
	void act_with_config_file()
	{
		create_tmp_deep_ini_file();
	}
	void prepare_tmp_deep_ini_file(Distribution::Thetha thetha, string exe_file, string param_exe_file)
	{
		using boost::property_tree::ptree;

		namespace pt = boost::property_tree;

		pt::ptree propTree;
		pt::read_ini(tmp_config_file, propTree);//???

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
		for (int i = 0; i < thetha.n +  thetha.n * 18; i++)
			parms_str += "5;";
		for (auto c : numbers_i)
			parms_str += c;
		propTree.put("default_model.parms", parms_str);

		string dparms_str;
		for (int i = 0; i < thetha.n * thetha.l; i++)
			dparms_str += "200;";
		for (int i = 0; i < thetha.n + thetha.n * 18; i++)
			dparms_str += "5.5;";
		vector<string> numbers_d = { "16.5;", "7.1;", "7.1", "7.1", "7.1", "7.1", "7.1" };
		for (auto c : numbers_d)
			dparms_str += c;
		propTree.put("default_model.dparms", dparms_str);

		string lbounds_str;
		for (int i = 0; i < thetha.n * thetha.l; i++)
			lbounds_str += "0;";
		for (int i = 0; i < thetha.n + thetha.n * 18; i++)
			lbounds_str += "-10;";
		vector<string> numbers_l = { "15;", "0;", "6", "1", "0", "1", "1" };
		for (auto c : numbers_l)
			lbounds_str += c;
		propTree.put("default_model.lbound", lbounds_str);


		string hbounds_str;
		for (int i = 0; i < thetha.n * thetha.l; i++)
			hbounds_str += "1924;";
		for (int i = 0; i < thetha.n + thetha.n * 18; i++)
			hbounds_str += "10;";
		vector<string> numbers_h= { "30;", "15;", "15", "15", "20", "100", "7.5" };
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
		propTree.put("default_model.limited_str", limited_str);

		string scale_str;
		for (int i = 0; i < count_param; i++)
			scale_str += "1;";
		propTree.put("default_model.scale_str", scale_str);

		string partype_str;
		for (int i = 0; i < thetha.n * thetha.l; i++)
			partype_str += "1;";
		for (int i = 0; i < thetha.n + thetha.n * 18; i++)
			partype_str += "0;";
		for (int i = 0; i < 5 + 2; i++)
			partype_str += "0;";
		propTree.put("default_model.partype_str", partype_str);

		write_ini(tmp_config_file, propTree);

	}

	void create_tmp_deep_ini_file()//use boost with VS 2019
	{
		const char *name = tmpnam(NULL);  
		FILE *f = fopen(name, "w"); 
		tmp_config_file = name;
		int i = 0, j = 0;
		char ch;
		FILE * source, *target;
		source = fopen(config_file.c_str(), "r");
		if (source == NULL)
		{
			fclose(f);
		}
		target = fopen(tmp_config_file.c_str(), "w");

		if (target == NULL)
		{
			fclose(source);
		}

		while ((ch = fgetc(source)) != EOF)
			fputc(ch, target);

		fclose(source);
		fclose(target);
		fclose(f);

	}


};
