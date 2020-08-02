#pragma once
#include "pch.h"


class Deep:public Model
{
private:
	pt::ptree propTree;
	enum INI_MODE
	{
		FUNC_AND_CROPS = 0,
		FUNC_WITH_CROPS
	};
	vector<string> name_opt_param = { "x;",  "beta;", "concs;",  "CBD", "MB;", "phyl;", "PLACON;" , "PLAPOW30;" ,  "SLA;" , "TBRUE;" , "TP1RUE;" , "TP2RUE;" ,  "TCRUE;" ,  "KPAR;" ,  "IRUE1;" ,  "IRUE2;" ,  "FLF1A;" ,  "FLF1B;",  "WTOPL;",  "FLF2;", "FRTRL;",  "GCF;",  "PDHI;",  "WDHI1;", "WDHI2;",  "WDHI3;",  "WDHI4;",  "DEPORT;", "EED;",  "GRTDP;",  "TEC;",  "WSSG;",  "WSSL;",  "WSSD;",  "SLNG;",  "SLNS;",  "SNCG;", "SNCS;",  "GNC;",  "MXNUP;",  "WSSN;",  "TBD;",  "TP1D;", "TP2D;",  "TCD;",  "cpp;",  "ppsen;",  "ttSWEM;",  "ttEMR1;",  "ttR1R3;",  "ttR3R5;", "ttR5R7;",  "ttR7R8;",  "ttBRP;",  "ttTRP;",  "ttWSD;", "ttR1TLM;",  "ttR1TLP;",  "ttRUE;",  "ttBSG;",  "ttTSG;",  "ttBRG;",  "ttTRG;",  "ttBNF;",  "TRESH;", "ttDKill;",  "LtFtsw;",  "LtWdDur;",  "vpd_resp;",  "vpd_cr;" };
public:
	const int not_phyl_param_size = 5;//x, beta, concs, cbd, mb
	Deep();

	Deep(const string& param);

	double run();

	double parse_result(string output);

	void act_with_config_file();

	void prepare_tmp_deep_ini_file(Distribution::Thetha thetha, string exe_file, string param_exe_file, vector<int>& dtype);

	void create_tmp_deep_ini_file();

	string config_file;
	string tmp_config_file;
	string deep_exe;
	double error;
	int index_score;
	vector<string> keys;// = 46; 46; 32
	vector<int> index_in_keys;// = 5; 7; 5
	int count_snp;
	int count_weather_const;
	int count_added_param;
	int ini_mode;
	int index_n, index_l;
};
