#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
#  mkpermfig.py
#
#  Copyright 2021 kkozlov <kkozlov@wave.bio.spbcas.ru>
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#  MA 02110-1301, USA.

import sys
import matplotlib.pyplot as plt
import numpy as np
from os import walk
import glob
import h5py
import pandas as pd
from math import sqrt

best_correlation = 0.9753158515423843
best_maximum = 60.22727272727272
best_average = 8.15530303030303
best_median = 6.0909090909090935
best_rms = 11.128544097229403

best_score = 0
best_flag = ''

def get_score(flag):
	return {
				flag == 0: ['rms', best_rms],
				flag == 1: ['correlation', best_correlation],
				flag == 2: ['maximum', best_maximum],
				flag == 3: ['average', best_average],
				flag == 4: ['median', best_median]
	}[True]

def get_genotypes(sfilenm):
	f = h5py.File(sfilenm, 'r')
	gr_covar = f['gr_covar'][:]
	gr_names = f['gr_names'][:]
	species = f["species"][:]
	f.close()
	geno_df = pd.DataFrame(data = gr_covar, index = species, columns = gr_names)
	# print(geno_df)
	# geno_df = geno_df.groupby(gr_names.tolist())
	# print(geno_df.size())
	# print(list(geno_df.columns))
	return(geno_df)

def plot_box_fig(datarrs, markers, figfile):
	# mpl.rcParams.update({'font.size': 11})
	# plt.title('Predicate contributions')
	dpi = 80
	fig = plt.figure(dpi = dpi, figsize = (640 / dpi, 480 / dpi) )
# Creating axes instance
	# ax = fig.add_axes([0, 0, 1, 1])
	ax = plt.axes()
	ax.yaxis.grid(True, zorder = 1)
# Creating plot
	bp = ax.boxplot(datarrs)
	plt.xlabel('Factor')
	plt.ylabel('Error difference')
	xs = range(1, len(markers) + 1)
	plt.xticks(xs, markers)
	fig.autofmt_xdate(rotation = 25)
# show plot
	plt.show()
	fig.savefig(figfile)

def get_group_data_from_folder(fl, marker, grnm):
	rmscores = []
	for ii in range(0, 99):
		ff = fl + "/" + marker + '_' + str(ii) + '_stack_output.csv'
		fdf = pd.read_csv(ff, sep=',')
		fdf = fdf.loc[grnm.to_list()]
		# print(fdf)
		y_data = fdf['event_day_R1'].to_numpy()
		y_model = fdf['dtR1'].to_numpy()
		rmscore = sqrt(np.square(y_data - y_model).mean())
		rmscores.append(rmscore - best_score)
	return(np.array(rmscores))

def get_data_from_folder(fl, best_flag):
	fnm = glob.glob(fl + "/*.log")
	rmscores = []
	for ff in fnm:
		with open(ff) as f:
			for line in f:
				if (line.startswith(str(best_flag))):
					rmscores.append(float(line.split("=")[1]) - best_score)
	return(np.array(rmscores))

def main(args):
	# list_of_folers = [v4_with_snp_dl, v4_with_snp_rain, v4_with_snp_snp_v3, v4_with_snp_snp_v4, v4_with_snp_srad, v4_with_snp_tmax, v4_with_snp_tmin]
	mypath = args[1]
	figfile = args[2]
	sfilenm = args[3]
	flagflag = int(args[4])
	flagscore = int(args[5])
	best_ = get_score(flagscore)
	best_flag = best_[0]
	best_score = best_[1]
	print('flag=', best_flag, 'score=', best_score)
	_, list_of_folders, _ = next(walk(mypath), (None, [], None))
	datarrs = list()
	markers = []
	for fl in list_of_folders:
		datarr = get_data_from_folder(fl, best_flag)
		datarrs.append(datarr)
		tt = fl.split('_')
		tt = tt[len(tt) - 1]
		markers.append(tt)
	plot_box_fig(datarrs, markers, figfile)
	if (flagflag == 1):
		grdf = get_genotypes(sfilenm)
		for gr in list(grdf.columns):
			for gra in [-1.0, 0.5, 2.0]:
				# print(grdf[grdf[gr] == gra])
				print(str(gr) + '_' + str(gra))
				grnm = grdf[gr] == gra
				# print(grnm)
				datarrs = list()
				for ii in range(0, len(list_of_folders)):
					fl = list_of_folders[ii]
					datarr = get_group_data_from_folder(fl, markers[ii], grnm)
					datarrs.append(datarr)
				plot_box_fig(datarrs, markers, str(gr) + '_' + str(gra) + '_' + figfile)
	return 0

if __name__ == '__main__':
	sys.exit(main(sys.argv))
