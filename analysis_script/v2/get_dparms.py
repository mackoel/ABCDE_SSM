import csv
import math
import os
import numpy as np

import matplotlib.pyplot as plt
from scipy.stats import mannwhitneyu

import errno
import sys

import pandas as pd

def plot_error(error_training, error_valid, brpar = 1, mode = 'err'):
	# print("MAX tr = ", max(error_training))
	# print("MAX va = ", max(error_valid))

	opt_iter = -1

	f = plt.figure(figsize=(20, 10))
	max_max = min(max(max(error_training), max(error_valid)), 200.0)
	min_min = max(min(min(error_training), min(error_valid)), 0.0)
	# print(max_max)
	bins_arr = np.arange(min_min, max_max, brpar)
	plt.hist(error_training, bins=bins_arr, alpha=0.5, label='error_training', rwidth=1)
	plt.hist(error_valid, bins=bins_arr, alpha=0.5, label='error_valid', rwidth=1)
	plt.legend(loc='upper right')

	plt.axvline(np.mean(error_training), color='k', linestyle='dashed', linewidth=1)

	coord_train = np.median(error_training)
	min_ylim, max_ylim = plt.ylim()
	plt.text(coord_train * 1.1, max_ylim * 0.9, 'Median: {:.2f}'.format(coord_train))

	coord_valid = np.median(error_valid)
	plt.axvline(coord_valid, color='g', linestyle='dashed', linewidth=1)

	min_xlim, max_xlim = plt.xlim()
	plt.text(coord_valid * 1.1, max_ylim * 0.8, 'Median: {:.2f}'.format(coord_valid))

	mwu_s, mwu_p = mannwhitneyu(error_training, error_valid)
	if (mwu_p > 0.05):
		plt.text(max_xlim * 0.8, (min_ylim + max_ylim) * 0.5, 'Validated (P={:.5f}'.format(mwu_p) + '>0.05, U={:.2f})'.format(mwu_s))
		if (opt_iter < 0):
			opt_iter = 1
	else:
		plt.text(max_xlim * 0.8, (min_ylim + max_ylim) * 0.5, 'Invalid (P={:.5f}'.format(mwu_p) + '<0.05, U={:.2f})'.format(mwu_s))

	plt.xlabel('Root mean square error')
	plt.ylabel('Frequency')

	filename = "error_dist_" + mode + "_slice_" + str(len(error_valid)) + ".pdf"
	plt.title('Used ' + str(len(error_valid)) + ' accepted from last ' + ' iterations')
	f.savefig(filename)
	return(opt_iter)

def plot_errors(min_df, num_train, num_valid, step = 10):
	iters = min_df['iter'].to_list()
	opt_iter = -1
	for ii in range(step, len(iters) - step, step):
		error_training = min_df.loc[min_df['iter'] >= ii]['target[7]'].to_numpy()
		error_valid = min_df.loc[min_df['iter'] >= ii]['target[21]'].to_numpy()
		error_training = np.sqrt(error_training/num_train)
		error_valid = np.sqrt(error_valid/num_valid)
		if (1 == plot_error(error_training, error_valid, mode = 'err')):
			if (opt_iter < 0):
				opt_iter = ii
	return(opt_iter)

if __name__ == "__main__":
	table_nm = sys.argv[1]
	iter_nm = sys.argv[2]
	dp_nm = sys.argv[3]
	best_nm = sys.argv[4]
	dp_ini_nm = sys.argv[5]
	snp_nm = sys.argv[6]
	dp_list_nm = sys.argv[7]
	num_models = int(sys.argv[8])
	num_train = float(sys.argv[9])
	num_valid = float(sys.argv[10])
	step = int(sys.argv[11])

	fixed_df = pd.read_csv(table_nm, sep=';')

	fixed_df = fixed_df.loc[fixed_df['accept'] != 'not accept']

	idx = fixed_df.groupby(['iter'])['error'].idxmin()

	min_df = fixed_df.loc[idx]

	print(min_df.head())

	min_df.to_csv(best_nm)

	opt_iter = plot_errors(min_df, num_train, num_valid, step)

	print("Optimal iter = " + str(opt_iter))

	if (opt_iter > 0):
		min_df = min_df.loc[min_df['iter'] >= opt_iter]
	else:
		min_df = min_df.loc[min_df['iter'] >= int(iter_nm)]

	min_df = min_df.sort_values(by = 'target[21]').head(num_models)

	print(min_df.head())

	#print(min_df.loc[min_df['iter'] >= int(iter_nm)]['seed'])

	selected_seeds = min_df['seed'].to_list()
	selected_N = min_df['accept_param_bounds[0]'].to_list()
	selected_L = min_df['accept_param_bounds[1]'].to_list()

	# print(selected_N)
	# print(selected_L)

	with open(dp_nm) as f:
		for line in f:
			for j in range(0, len(selected_seeds)):
				sds = selected_seeds[j]
				seed_str = "seed = " + str(sds)
				if (seed_str in line):
					fm_sds = dp_nm + '_' + str(j) + '_' + str(sds) + '.ini.log'
					# j += 1
					with open(fm_sds, "a") as myfile:
						myfile.write(line)
						myfile.write(line)
					#print(line)

	for j in range(0, len(selected_seeds)):
		sds = selected_seeds[j]
		fm_sds = dp_nm + '_' + str(j) + '_' + str(sds) + '.ini.log'
		fm_ini_sds = dp_nm + '_' + str(j) + '_' + str(sds) + '.ini'

		myfile = open(fm_ini_sds, "a")
		with open(dp_ini_nm) as f:
			for line in f:
				if ("partsizes" in line):
					line_p = line.split("=")[1]
					line_tk = line_p.split(";")
					line_tk[0] = str(int(selected_N[j]) * int(selected_L[j]))
					line_tk[1] = str(int(selected_N[j]) * (int(snp_nm) + 1))
					#print(line_tk.join(';'))
					#print(';'.join(line_tk))
					#print('partsizes=' + ';'.join(line_tk))
					myfile.write('partsizes=' + ';'.join(line_tk))
				else:
					myfile.write(line)
		myfile.close()

		cmd_str = 'deepmethod --default-name=' + fm_ini_sds + ' --operation=monitor --monitor=1000'
		# print(cmd_str)
		os.system(cmd_str)
		# j += 1

	with open(dp_list_nm, "w") as f:
		j = 0
		for sds in selected_seeds:
			fm_sds = dp_nm + '_' + str(j) + '_' + str(sds) + '.ini-deep-output' + ',' + str(int(selected_N[j])) + ',' + str(int(selected_L[j])) + '\n'
			f.write(fm_sds)
			j += 1
