#include "pch.h"


double * read_real_data(const char * FILE_NAME)
{
	FILE    *fFile = NULL;
	char    szData[20];
	unsigned int valCount = 0;
	double  *dVal = NULL;
	fpos_t filepos;

	fFile = fopen(FILE_NAME, "r");
	fgetpos(fFile, &filepos);
	while (1) {
		if (!fgets(szData, sizeof(szData), fFile))
			break;
		valCount++;
	}

	fsetpos(fFile, &filepos);

	dVal = (double*)malloc(sizeof(double)*valCount);

	for (unsigned int i = 0; i < valCount; ++i) {
		fgets(szData, sizeof(szData), fFile);
		dVal[i] = atof(szData);
		//printf("%f", dVal[i]);
	}
	fclose(fFile);
	return dVal;
}
gchar* model_read_abc(gchar*filename, gsize*size, GError**err)
{
	GFile*gfile;
	char *etag;
	gchar*data = NULL;
	GError *gerror = NULL;
	g_return_val_if_fail(err == NULL || *err == NULL, NULL);
	gfile = g_file_new_for_commandline_arg(filename);
	if (!g_file_load_contents(gfile, (GCancellable*)NULL, &data, size, &etag, &gerror)) {
		g_propagate_error(err, gerror);
		return NULL;
	}
	g_object_unref(gfile);
	return data;
}
int model_load_abc(Config *config, gchar*data, gsize size, GError**err)
{
	GKeyFile*gkf;
	GError *gerror = NULL;
	gchar*str, **strlist;
	gsize length;
	int *ilist, ii, j, jj, k, kk;
	double *dlist, dval;
	int retval = 0;
	g_return_val_if_fail(err == NULL || *err == NULL, 1);
	gkf = g_key_file_new();
	if (!g_key_file_load_from_data(gkf, (const gchar*)data, size, G_KEY_FILE_NONE, &gerror)) {
		g_propagate_error(err, gerror);
		return 1;
	}
	if ((ii = g_key_file_get_integer(gkf, "data", "count_thread", &gerror)) != 0 || gerror == NULL) {
		config->count_thread = ii;
		if (gerror != NULL) g_clear_error(&gerror);
	}
	else {
		g_debug("%s", gerror->message);
		g_clear_error(&gerror);
	}
	if ((str = g_key_file_get_string(gkf, "data", "name_deep_ini_file", &gerror)) != 0 || gerror == NULL) {
		config->name_deep_ini_file = str;
		if (gerror != NULL) g_clear_error(&gerror);
	}
	else {
		g_debug("%s", gerror->message);
		g_clear_error(&gerror);
	}
	if ((str = g_key_file_get_string(gkf, "data", "name_exe_file", &gerror)) != 0 || gerror == NULL) {
		config->name_exe_file = str;
		if (gerror != NULL) g_clear_error(&gerror);
	}
	else {
		g_debug("%s", gerror->message);
		g_clear_error(&gerror);
	}
	if ((dval = g_key_file_get_integer(gkf, "data", "eps", &gerror)) != 0 || gerror == NULL) {
		config->eps = ii;
		if (gerror != NULL) g_clear_error(&gerror);
	}
	else {
		g_debug("%s", gerror->message);
		g_clear_error(&gerror);
	}
	if ((ii = g_key_file_get_double(gkf, "data", "t", &gerror)) != 0 || gerror == NULL) {
		config->t = ii;
		if (gerror != NULL) g_clear_error(&gerror);
	}
	else {
		g_debug("%s", gerror->message);
		g_clear_error(&gerror);
	}
	g_key_file_free(gkf);
	return retval;
}


int read_config_file_abc(Config * config_file, gchar*filename, GError **err)
{
	int ret_val = 0;
	gchar*data = NULL;
	gsize size;
	GError *gerror = NULL;
	g_return_val_if_fail(err == NULL || *err == NULL, 1);
	if ((data = model_read_abc(filename, &size, &gerror)) == NULL) {
		g_propagate_error(err, gerror);
		return 1;
	}
	ret_val = model_load_abc(config_file, data, size, &gerror);
	if (ret_val == 1) {
		g_propagate_error(err, gerror);
		ret_val = 1;
	}
	g_free(data);
}

gchar* model_read_deep(gchar*filename, gsize*size, GError**err)
{
	GFile*gfile;
	char *etag;
	gchar*data = NULL;
	GError *gerror = NULL;
	g_return_val_if_fail(err == NULL || *err == NULL, NULL);
	gfile = g_file_new_for_commandline_arg(filename);
	if (!g_file_load_contents(gfile, (GCancellable*)NULL, &data, size, &etag, &gerror)) {
		g_propagate_error(err, gerror);
		return NULL;
	}
	g_object_unref(gfile);
	return data;
}
int model_load_deep(Config config, gchar*data, gsize size, Thetha thetha,  GError**err)
{
	GKeyFile*gkf;
	GError *gerror = NULL;
	gchar*str, **strlist;
	gsize length;
	int *ilist, ii, j, jj, k, kk;
	double *dlist, dval;
	int retval = 0;
	g_return_val_if_fail(err == NULL || *err == NULL, 1);
	gkf = g_key_file_new();
	if (!g_key_file_load_from_data(gkf, (const gchar*)data, size, G_KEY_FILE_NONE, &gerror)) {
		g_propagate_error(err, gerror);
		return 1;
	}
	char str_lambda[1000];

	sprintf(str_lambda, "%i", (int)thetha.lambda);

	//printf("%s", str_lambda);
	char* res_string_lambda = new char[strlen(str_lambda) + strlen("penalty;readpenalty;2;2;") + 2];
	strcpy(res_string_lambda, "penalty;readpenalty;2;2;");
	strcat(res_string_lambda, str_lambda);
	strcat(res_string_lambda, ";");
	g_key_file_set_string(gkf, "default_target", "l1_pen", res_string_lambda);

	char str_n[10];
	char str_l[10];

	sprintf(str_n, "%i", thetha.n);
	sprintf(str_l, "%i", thetha.l);

	//printf("%s", str_l);
	//printf("%s", str_n);
	char* res_string_n_l = new char[strlen(str_n)+strlen(str_l) + strlen("C:/project/SSM/SSM_improved/x64/Release/SSM_improved.exe -P 1 -N 5 -L 12 -Q -R 1 -l 3.453 -D 230 -s 0.75 -i 1 -d 60 -w 15 -f 15 -m 2 -r 0.5  -t 900 -q 200 -z 0.13 -x 79 -c 0.5 -b 0.36 -k 0.264 -E 0.13 -j 1 -a 1 -o 3 -U 6 C:/project/SSM/SSM_improved/SSM_improved/crops.ini.src C:/project/SSM/SSM_improved/SSM_improved/chickpea-ssm-snp.h5 C:/project/SSM/SSM_improved/SSM_improved/weather_ssm.h5") + 1];//если станет другое число больше символов - ошибка
	strcpy(res_string_n_l, "C:/project/SSM/SSM_improved/x64/Release/SSM_improved.exe -P 1 -N ");
	strcat(res_string_n_l, str_n);
	strcat(res_string_n_l, " -L ");
	strcat(res_string_n_l, str_l);
	strcat(res_string_n_l, " -Q -R 1 -l 3.453 -D 230 -s 0.75 -i 1 -d 60 -w 15 -f 15 -m 2 -r 0.5  -t 900 -q 200 -z 0.13 -x 79 -c 0.5 -b 0.36 -k 0.264 -E 0.13 -j 1 -a 1 -o 3 -U 6 C:/project/SSM/SSM_improved/SSM_improved/crops.ini.src C:/project/SSM/SSM_improved/SSM_improved/chickpea-ssm-snp.h5 C:/project/SSM/SSM_improved/SSM_improved/weather_ssm.h5");
	g_key_file_set_string(gkf, "default_model", "command", res_string_n_l);


	int count_sym = thetha.n * thetha.l + thetha.n + thetha.n * 18 + 5 + 2;// numbers have more 
	char* default_model_str = new char[(count_sym*4)+count_sym+1];//если станет другое число больше символов - ошибка
	char str_parts[10];
	char parts[10];
	//parts
	sprintf(str_parts, "%i", count_sym);
	strcpy(parts, "");
	strcat(parts, "x;");
	strcat(parts, str_parts);
	strcat(parts, ";");
	g_key_file_set_string(gkf, "default_model", "parts", parts);
	//mask
	strcpy(default_model_str, "");
	for (int i = 0; i < count_sym; i++)
		strcat(default_model_str, "0;");
	g_key_file_set_string(gkf, "default_model", "mask", default_model_str);
	//params
	strcpy(default_model_str, "");
	for (int i = 0; i < thetha.n * thetha.l; i++)
		strcat(default_model_str, "200;");
	for(int i = 0; i < thetha.n + thetha.n * 18; i++)
		strcat(default_model_str, "5;");
	for (int i = 0; i < 5 + 2; i++)
	{
		if(i == 0)
		    strcat(default_model_str, "16;");
		else
			strcat(default_model_str, "7;");
	}
	g_key_file_set_string(gkf, "default_model", "parms", default_model_str);

	//dparams
	strcpy(default_model_str, "");
	for (int i = 0; i < thetha.n * thetha.l; i++)
		strcat(default_model_str, "200;");
	for (int i = 0; i < thetha.n + thetha.n * 18; i++)
		strcat(default_model_str, "5.5;");
	for (int i = 0; i < 5 + 2; i++)
	{
		if (i == 0)
			strcat(default_model_str, "16.5;");
		else
			strcat(default_model_str, "7.1;");
	}
	g_key_file_set_string(gkf, "default_model", "dparms", default_model_str);


	//lbound
	strcpy(default_model_str, "");
	for (int i = 0; i < thetha.n * thetha.l; i++)
		strcat(default_model_str, "0;");
	for (int i = 0; i < thetha.n + thetha.n * 18; i++)
		strcat(default_model_str, "-10;");
	for (int i = 0; i < 5 + 2; i++)
	{
		if (i == 0)
			strcat(default_model_str, "15;");
		if (i == 1)
			strcat(default_model_str, "0;");
		if (i == 2)
			strcat(default_model_str, "6;");
		if (i == 3)
			strcat(default_model_str, "1;");
		if (i == 4)
			strcat(default_model_str, "0;");
		if (i == 5)
			strcat(default_model_str, "1;");
		if (i == 6)
			strcat(default_model_str, "1;");
	}
	g_key_file_set_string(gkf, "default_model", "lbound", default_model_str);

	//hbound
	strcpy(default_model_str, "");
	for (int i = 0; i < thetha.n * thetha.l; i++)
		strcat(default_model_str, "1924;");
	for (int i = 0; i < thetha.n + thetha.n * 18; i++)
		strcat(default_model_str, "10;");
	for (int i = 0; i < 5 + 2; i++)
	{
		if (i == 0)
			strcat(default_model_str, "30;");
		if (i == 1)
			strcat(default_model_str, "15;");
		if (i == 2)
			strcat(default_model_str, "15;");
		if (i == 3)
			strcat(default_model_str, "15;");
		if (i == 4)
			strcat(default_model_str, "20;");
		if (i == 5)
			strcat(default_model_str, "100;");
		if (i == 6)
			strcat(default_model_str, "7.5;");
	}
	g_key_file_set_string(gkf, "default_model", "hbound", default_model_str);
	//tweak
	strcpy(default_model_str, "");
	for (int i = 0; i < count_sym; i++)
		strcat(default_model_str, "1;");
	g_key_file_set_string(gkf, "default_model", "tweak", default_model_str);

	//limited
	strcpy(default_model_str, "");
	for (int i = 0; i < count_sym; i++)
		strcat(default_model_str, "1;");
	g_key_file_set_string(gkf, "default_model", "limited", default_model_str);

	//scale
	strcpy(default_model_str, "");
	for (int i = 0; i < count_sym; i++)
		strcat(default_model_str, "1;");
	g_key_file_set_string(gkf, "default_model", "scale", default_model_str);

	//partype
	strcpy(default_model_str, "");
	for (int i = 0; i < thetha.n * thetha.l; i++)
		strcat(default_model_str, "1;");
	for (int i = 0; i < thetha.n + thetha.n * 18; i++)
		strcat(default_model_str, "0;");
	for (int i = 0; i < 5 + 2; i++)
	{
		strcat(default_model_str, "0;");
	}
	g_key_file_set_string(gkf, "default_model", "partype", default_model_str);
	delete[] res_string_lambda;
	delete[]  res_string_n_l;
	delete[] default_model_str;
	g_key_file_save_to_file(gkf, config.name_deep_ini_file, &gerror);
	g_key_file_free(gkf);
	return retval;
}


int read_config_file_deep(Config config_file, Thetha thetha, GError **err)
{
	int ret_val = 0;
	gchar*data = NULL;
	gsize size;
	GError *gerror = NULL;
	g_return_val_if_fail(err == NULL || *err == NULL, 1);
	if ((data = model_read_deep(config_file.name_deep_ini_file, &size, &gerror)) == NULL) {
		g_propagate_error(err, gerror);
		return 1;
	}
	ret_val = model_load_deep(config_file, data, size, thetha, &gerror);
	if (ret_val == 1) {
		g_propagate_error(err, gerror);
		ret_val = 1;
	}
	g_free(data);
}

void prepare_deep_file(Config config, Thetha thetha)
{
	GError *gerror = NULL;
	read_config_file_deep(config, thetha, &gerror);
}

void create_curr_deep_ini_file(char * name_ini_file, char * default_deep_ini_file, int iter)//default with new data
{
	//char * name_ini_file = (char*)malloc(strlen(default_deep_ini_file) + 4);//2 - for i and _(test_i.ini)//NOT ENOUGH
	
	int i = 0, j = 0;
	char ch;
	FILE * source, *target;
	for (i = 0; i < strlen(default_deep_ini_file) + 3; i++)
	{	
		if (i == 4)
		{
			name_ini_file[i] = iter + '0';
			i++;
		}
		name_ini_file[i] = default_deep_ini_file[j];
		j++;
	}
	name_ini_file[i + 1] = '\0';
	std::cout << "begin" << std::endl;
	std::cout << name_ini_file << std::endl;


	source = fopen(default_deep_ini_file, "r");
	if (source == NULL)
	{
		printf("Press any key to exit...Not default file\n");
		exit(EXIT_FAILURE);
	}
	target = fopen(name_ini_file, "w");

	if (target == NULL)
	{
		fclose(source);
		printf("Press any key to exit...\n");
		exit(EXIT_FAILURE);
	}
	
	while ((ch = fgetc(source)) != EOF)
		fputc(ch, target);

	printf("File copied successfully.\n");

	fclose(source);
	fclose(target);
//	return name_ini_file;
}