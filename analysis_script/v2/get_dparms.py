import csv
import math
import os
import numpy as np

import errno
import sys

import pandas as pd

if __name__ == "__main__":
	table_nm = sys.argv[1]
	iter_nm = sys.argv[2]
	dp_nm = sys.argv[3]
	best_nm = sys.argv[4]
	dp_ini_nm = sys.argv[5]
	snp_nm = sys.argv[6]
	dp_list_nm = sys.argv[7]
	num_models = int(sys.argv[8])

	fixed_df = pd.read_csv(table_nm, sep=';')

	idx = fixed_df.groupby(['iter'])['error'].idxmin()

	min_df = fixed_df.loc[idx]

	print(min_df.head())

	min_df.to_csv(best_nm)

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
		print(cmd_str)
		os.system(cmd_str)
		# j += 1

	with open(dp_list_nm, "w") as f:
		j = 0
		for sds in selected_seeds:
			fm_sds = dp_nm + '_' + str(j) + '_' + str(sds) + '.ini-deep-output' + ',' + str(int(selected_N[j])) + ',' + str(int(selected_L[j])) + '\n'
			f.write(fm_sds)
			j += 1
