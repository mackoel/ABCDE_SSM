#pragma once
#include "pch.h"


class Deep :public Model
{
private:
	pt::ptree propTree;
	enum INI_MODE
	{
		FUNC_AND_CROPS = 0,
		FUNC_WITH_CROPS
	};
	
	/*vector<string> name_opt_param = { "x;", "beta;", "MB;", "phyl;", "PLACON;" , "PLAPOW30;" ,  "SLA;" , "TBRUE;" , "TP1RUE;" , "TP2RUE;" ,  "TCRUE;" ,  "KPAR;" ,  "IRUE1;" ,  "IRUE2;" ,  "FLF1A;" ,  "FLF1B;",  "WTOPL;",  "FLF2;", "FRTRL;",  "GCF;",  "PDHI;",  "WDHI1;", "WDHI2;",  "WDHI3;",  "WDHI4;",  "DEPORT;", "EED;",  "GRTDP;",  "TEC;",  "WSSG;",  "WSSL;",  "WSSD;",  "SLNG;",  "SLNS;",  "SNCG;", "SNCS;",  "GNC;",  "MXNUP;",  "WSSN;",  "TBD;",  "TP1D;", "TP2D;",  "TCD;",  "cpp;",  "ppsen;",  "ttSWEM;",  "ttEMR1;",  "ttR1R3;",  "ttR3R5;", "ttR5R7;",  "ttR7R8;",  "ttBRP;",  "ttTRP;",  "ttWSD;", "ttR1TLM;",  "ttR1TLP;",  "ttRUE;",  "ttBSG;",  "ttTSG;",  "ttBRG;",  "ttTRG;",  "ttBNF;",  "TRESH;", "ttDKill;",  "LtFtsw;",  "LtWdDur;",  "vpd_resp;",  "vpd_cr;", "Pbase;" ,"Sbase;" };
	vector<string> default_opt_params = { "200;", "5;", "5;", "46;", "1;", "2;", "0;", "2;", "14;", "30;", "38;", "0;", "1;", "1;", "0;", "0;", "180;", "0;", "0;", "1;", "0;", "0;", "0;", "450;", "2000;", "200;", "1000;", "17;", "5;", "0;", "0;", "0;", "2;", "0;", "0;", "0;", "0;", "0;", "0;", "2;", "21;", "30;", "40;", "18;", "0;", "5;", "23;", "9;", "3;", "18;", "11;", "5;", "28;", "5;", "9;", "12;", "23;", "40;", "64;", "5;", "40;", "14;", "0;", "40;", "0;", "2;", "1;", "20;", "7;", "40;" };
	vector<string> default_opt_dparams = { "200;", "5.5;", "5;", "46;", "1;","2.158;", "0.021;", "2;", "14;", "30;", "38;", "0.65;", "1.8;", "1.8;","0.53;", "0.3;", "180;", "0.13;", "0.22;", "1;", "0.02;", "0;", "0;", "450;", "2000;", "200;", "1000;", "17;", "5;", "0.3;", "0.4;", "0.4;", "2.3;", "0.78;", "0.025;", "0.0078;", "0.043;", "0.45;", "0.5;", "2;", "21;", "30;", "40;", "18;", "0.1;", "5.0;", "23.0;", "9.0;", "3.0;", "18.0;", "11.0;", "5.0;", "28.0;", "5.0;" ,"9.0;" ,"12.0;", "23.0;", "40.0;", "64.0;" ,"5.0;" ,"40.0;", "14.7;", "0.75;", "40.0;", "0;", "2;", "1;", "20;", " 7.1;" ,"39.9;" };
	vector<string> default_tweak_opt_params = { "1;", "1;", "1;", "0;", "0;",  "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "1;", "0;", "0;", "1;", "1;", "0;", "1;", "1;", "1;", "1;", "1;", "1;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "0;", "1;", "1;" };
	vector<string> default_lbounds_opt_params = { "0;", "-10;", "0.01;", "6;",  "0.1;", "0.158;", "0.001;", "0.2;", "1.4;", "3.0;", "3.8;", "0.05;", "1.0;", "1.0;", "0.03;", "0.03;", "1.80;", "0.03;", "0.02;", "0.1;", "0.002;", "0;", "0;", "45.0;", "200.0;", "20.0;", "100.0;", "1.7;", "0.5;", "0.03;", "0.04;", "0.04;", "0.23;", "0.08;", "0.0025;", "0.0008;", "0.003;", "0.05;", "0.05;", "0.2;", "2.1;", "3.0;", "4.0;", "1.8;", "0.01;", "4.50;", "13;", "5;", "1;", "8;", "5;", "0.50;", "2.80;", "0.50;", "0.90;", "1.20;", "2.30;", "4.00;", "6.40;", "0.50;", "4.00;", "1.47;", "0.05;", "4.00;", "0;", "0.2;", "0.1;", "2.0;", "1;", "1;" };
	vector<string> default_hbounds_opt_params = { "1924;", "10;", "9;",  "60;", "10;", "21.58;", "2.1;", "20;", "40;", "60;", "80;", "6.5;", "18;", "18;", "5.3;", "30;", "800;", "1.3;", "2.2;", "10;", "0.2;", "10;", "10;", "500;", "5000;", "500;", "5000;", "70;", "50;", "3;", "4;", "4;", "3;", "7.8;", "0.25;", "0.78;", "0.43;", "4.5;", "5;", "5;", "51;", "50;", "50;", "80;", "1;", "15.0;", "30.0;", "19;", "30.0;", "80.0;", "50.0;", "50.0;", "80.0;", "50.0;", "90.0;", "20.0;", "30.0;", "80.0;", "164.0;", "15.0;", "140.0;", "54.7;", "7.5;", "80.0;", "10;" ,"12;", "11;", "120;", "100;", "100;" };*/

	vector<string> name_deep_opt_params;
	vector<string> default_deep_opt_params;
	vector<string> default_deep_opt_dparams;
	vector<string> default_tweak_deep_opt_params;
	vector<string> default_lbounds_deep_opt_params;
	vector<string> default_hbounds_deep_opt_params;
public:
public:
	Deep();

	Deep(const string& param);
	const int not_phyl_param_size = 2;//x, beta
	Deep& operator=(const Deep&);

	void act_with_config_file();

	double run();

	double parse_result(string output);

	void prepare_tmp_deep_ini_file(Distribution::Thetha thetha, vector<int>& dtype, int seed);

	void create_tmp_deep_ini_file();

	string config_file;
	string tmp_config_file;
	string deep_exe;
	double error;
	vector<string> keys;// = 46; 46; 32
	vector<int> index_in_keys;// = 5; 7; 5
	int count_snp;
	int ini_mode;
	int index_n, index_l, index_score, index_seed;
};
