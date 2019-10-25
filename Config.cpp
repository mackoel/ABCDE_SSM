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
int model_load_deep(Config *config, gchar*data, gsize size, Thetha thetha,  GError**err)
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

	sprintf(str_lambda, "%f", thetha.lambda);

	printf("%s", str_lambda);
	char* res_string_lambda = new char[strlen(str_lambda) + strlen("penalty;readpenalty; 2; 2;") + 1];
	strcpy(res_string_lambda, "penalty;readpenalty; 2; 2;");
	strcat(res_string_lambda, str_lambda);
	g_key_file_set_string(gkf, "default_target", "l1_pen", res_string_lambda);

	char str_n[1000];
	char str_l[1000];

	sprintf(str_n, "%f", (int)thetha.n);
	sprintf(str_l, "%f", (int)thetha.l);

	printf("%s", str_l);
	printf("%s", str_n);
	char* res_string_n_l = new char[strlen(str_n)+strlen(str_l) + strlen("C:/Qt/Qt5.11.3/5.11.3/msvc2017_64/bin/SSM_improved.exe -P 1 -N 5 -L 12 -Q -R 1 -l 3.453 -D 230 - s0.75 -i 1 -d 60 -w 15 -f 15 -m 2 -r 0.5  -t 900 -q 200 -z 0.13 -x 79 -c 0.5 -b 0.36 -k 0.264 -E 0.13 -j 1 -a 1 -o 3 -U 6 C:/project/SSM/SSM_improved/SSM_improved/crops.ini.src C:/project/SSM/SSM_improved/SSM_improved/chickpea_ssm_snp_autumn_test_1.h5 C:/project/SSM/SSM_improved/SSM_improved/weather_ssm.h5") + 1];//если станет другое число больше символов - ошибка
	strcpy(res_string_n_l, "C:/Qt/Qt5.11.3/5.11.3/msvc2017_64/bin/SSM_improved.exe -P 1 -N ");
	strcat(res_string_n_l, str_n);
	strcat(res_string_n_l, "-L ");
	strcat(res_string_n_l, str_l);
	strcat(res_string_n_l, "-Q -R 1 -l 3.453 -D 230 - s0.75 -i 1 -d 60 -w 15 -f 15 -m 2 -r 0.5  -t 900 -q 200 -z 0.13 -x 79 -c 0.5 -b 0.36 -k 0.264 -E 0.13 -j 1 -a 1 -o 3 -U 6 C:/project/SSM/SSM_improved/SSM_improved/crops.ini.src C:/project/SSM/SSM_improved/SSM_improved/chickpea_ssm_snp_autumn_test_1.h5 C:/project/SSM/SSM_improved/SSM_improved/weather_ssm.h5");
	g_key_file_set_string(gkf, "default_model", "command", res_string_n_l);
	g_key_file_save_to_file(gkf, config->name_deep_ini_file, &gerror);
	g_key_file_free(gkf);
	return retval;
}


int read_config_file_deep(Config * config_file, Thetha thetha, GError **err)
{
	int ret_val = 0;
	gchar*data = NULL;
	gsize size;
	GError *gerror = NULL;
	g_return_val_if_fail(err == NULL || *err == NULL, 1);
	if ((data = model_read_deep(config_file->name_deep_ini_file, &size, &gerror)) == NULL) {
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
	read_config_file_deep(&config, thetha, &gerror);
}